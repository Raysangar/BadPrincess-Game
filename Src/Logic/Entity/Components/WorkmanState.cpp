/**
@file WorkmanState.cpp
*/

#include "WorkmanState.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Server.h"
#include "Logic/Maps/Map.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/AllyUnitMessages.h"
#include "Logic/RouteCalculationMessages.h"
#include "Logic/MovementMessages.h"
#include "Logic/WorkmanMessages.h"
#include "Logic/EntityMessages.h"
#include "Logic/SmartObjectMessages.h"
#include "Logic/Entity/Components/TransportedComponent.h"
#include "Logic\Entity\Components\BuildComponent.h"

#include "GUI\Server.h"
#include "GUI\PlayerController.h"
#include "GUI\ActiveUnitController.h"

#include "Map/MapEntity.h"

#include "AI/Server.h"
#include "AI/EntityManager.h"
#include "AI/GameManager.h"


#define RADIUS_OVERLAPS 50
#define RESOURCE_GET_AWAY_DISTANCE 30.0f
#define RESOURCE_GET_AWAY_ANGLE 120

namespace Logic 
{
	IMP_FACTORY(CWorkmanState);

	//---------------------------------------------------------

	CWorkmanState::~CWorkmanState() 
	{

	} 

	//---------------------------------------------------------

	bool CWorkmanState::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;
		assert(entityInfo->hasAttribute("max_to_gather"));
		maxToGather = entityInfo->getIntAttribute("max_to_gather");

		assert(entityInfo->hasAttribute("gather_unit"));
		gatherUnit = entityInfo->getIntAttribute("gather_unit");

		assert(entityInfo->hasAttribute("time_between_gather"));
		timeBetweenGather = entityInfo->getIntAttribute("time_between_gather");

		assert(entityInfo->hasAttribute("speed"));
		speed = entityInfo->getFloatAttribute("speed");

		assert(entityInfo->hasAttribute("buildAbilityChar"));
		buildAbilityChar = entityInfo->getStringAttribute("buildAbilityChar");

		assert(entityInfo->hasAttribute("repairBuildingAbilityChar"));
		repairBuildingAbilityChar = entityInfo->getStringAttribute("repairBuildingAbilityChar");

		return true;

	} 

	void CWorkmanState::awake()
	{
		resType=AI::ResourceType::none;
		currentGatherQuantity=0;
		deltaTime=0;
		buildingEntity=nullptr;
		lastResource=nullptr;
		repairBuildingAbilityActive=false;
		action=none;
	}
	void CWorkmanState::tick(unsigned int msecs)
	{
		deltaTime = msecs;
		IComponent::tick(msecs);
	}

	//---------------------------------------------------------

	bool CWorkmanState::acceptN(const std::shared_ptr<NMessage> &message)
	{
		return message->type.compare("InstantiateAbilityObject")==0 || message->type.compare("CastAbility")==0 || 
			message->type.compare("SmartObjectFull")==0 ||  message->type.compare("EntityDestroying") == 0 ||  
			message->type.compare("SmartObjectDisabled") == 0 || (message->type.compare("AvatarActivation") == 0) ||
			message->type.compare("AvatarRouteTo") == 0 || (message->type.compare("AvatarMoveToEntity") == 0) ||
			message->type.compare("DisableRepairAbility") == 0;
	}

	//---------------------------------------------------------

	void CWorkmanState::processN(const std::shared_ptr<NMessage> &message)
	{
		if(message->type.compare("CastAbility") == 0)
		{
			std::shared_ptr<CastAbility> m=std::static_pointer_cast<CastAbility>(message);			
			if(m->ability.compare(buildAbilityChar)==0 && !GUI::CServer::getSingletonPtr()->getPlayerController()->hasAttachedEntity())
			{
				CEntityPtr well = Logic::CEntityFactory::getSingletonPtr()->instantiate(Logic::CServer::getSingletonPtr()->getMap()->getEntityByName("WellHelper"), "WellHelper",
				GUI::CServer::getSingletonPtr()->getPlayerController()->getActualMouseWorldPosition());

				GUI::CServer::getSingletonPtr()->getPlayerController()->attachEntityToMouse(well);
			}else if(m->ability.compare(repairBuildingAbilityChar)==0)
			{
				repairBuildingAbilityActive  = !repairBuildingAbilityActive;

				if(repairBuildingAbilityActive)
				{
					GUI::CServer::getSingletonPtr()->getActiveUnitController()->setMouseCursor("BadPrincessHud/RepairCursor");
				}
				else
				{
					GUI::CServer::getSingletonPtr()->getActiveUnitController()->restoreMouseCursor();
				}
			}
		}
		else if(message->type.compare("InstantiateAbilityObject") == 0)
		{
			std::shared_ptr<InstantiateAbilityObject> m=std::static_pointer_cast<InstantiateAbilityObject>(message);			
			Logic::CEntity *well = Logic::CEntityFactory::getSingletonPtr()->instantiate(Logic::CServer::getSingletonPtr()->getMap()->getEntityByName("Well"), "Well", m->position);			
			auto moveToWell = std::make_shared<AvatarMoveToEntity>();
			moveToWell->target = well;
			_entity->emitMessageN(moveToWell);
//			sendMoveToWellMessage(smartEntity);
				
		}
		else if( message->type.compare("EntityDestroying") == 0)
		{
			//if the entity destroyed is the last resource, find the closest resource
			std::shared_ptr<EntityDestroying> m=std::static_pointer_cast<EntityDestroying>(message);
			if(m->entity==lastResource)
				lastResource = AI::CServer::getSingletonPtr()->getEntityManager()->getClosestEntityTypeWithOverlaps(lastResource,RADIUS_OVERLAPS,_entity->getPosition());

		}else if( message->type.compare("SmartObjectFull") == 0)
		{
			std::shared_ptr<SmartObjectFull> m=std::static_pointer_cast<SmartObjectFull>(message);
//			std::cout<<_entity->getName()<<" smartobject full next=";
			findNextResourceClosest(m->sender);
		}else if( message->type.compare("SmartObjectDisabled")==0)
		{
			std::shared_ptr<SmartObjectDisabled> m=std::static_pointer_cast<SmartObjectDisabled>(message);
//			std::cout<<_entity->getName()<<" smartobject disabled next=";
			Logic::IComponent *component=m->entity->getComponent("CTransportedComponent");
			if(component!=nullptr)
			{
				//if the entity is transported 
				Logic::CTransportedComponent* transportComponet= static_cast<CTransportedComponent*>(component);
				if(transportComponet->isTransported())
				{
					//send a message to go to them in order to go to chase state
					auto move = std::make_shared<AvatarMoveToEntity>();
					move->target = m->entity;
					_entity->emitMessageN(move);
				}else
					findNextResourceClosest(m->entity);
			}else
				findNextResourceClosest(m->entity);
		}		
		else if (message->type.compare("AvatarActivation") == 0)
		{
			std::shared_ptr<AvatarActivation> m = std::static_pointer_cast<AvatarActivation>(message);

			if(!m->actived)
			{
				repairBuildingAbilityActive = false;
				GUI::CServer::getSingletonPtr()->getActiveUnitController()->restoreMouseCursor();
			}
		}
		else if (message->type.compare("AvatarRouteTo") == 0)
		{
			auto m = std::static_pointer_cast<AvatarRouteTo>(message);
			//if the player send command to go to a specific position reset the behaviour
			if(m->sender != _entity)
			{
				repairBuildingAbilityActive = false;
				GUI::CServer::getSingletonPtr()->getActiveUnitController()->restoreMouseCursor();
			}
		}else if(message->type.compare("AvatarMoveToEntity") == 0)
		{
			auto m = std::static_pointer_cast<AvatarMoveToEntity>(message);
			Logic::CEntity* decisionEntity = m->target;

			if(decisionEntity->getType().compare("Pot")==0)
			{
				//go to the kitchen
				auto moveMessage = std::make_shared<AvatarRouteTo>();
				moveMessage->sender = _entity;
				moveMessage->target = m->target->getPosition();
				moveMessage->calculateNewRoute = true;
				_entity->emitMessageN(moveMessage);

				Logic::IComponent *iComponent = decisionEntity->getComponent("CBuildComponent");
				assert(iComponent);
				Logic::CBuildComponent *buildComponent = static_cast<Logic::CBuildComponent*>(iComponent);

				//if i have to build  the kitchen 
				if(!buildComponent->isBuilded())
				{
					setBuildingEntity(decisionEntity);
					
					auto buildMessage = std::make_shared<BuildKitchen>();
					_entity->emitMessageN(buildMessage);
				}
			}else if(decisionEntity->getType().compare("Well")==0)
			{
				Logic::IComponent *iComponent = decisionEntity->getComponent("CBuildComponent");
				assert(iComponent);
				Logic::CBuildComponent *buildComponent = static_cast<Logic::CBuildComponent*>(iComponent);

				//if i have to build  the well or the repair ability are selected
				if(!buildComponent->isBuilded() || (buildComponent->isBuilded() && isRepairBuildingAbilityActive()))
				{

					setBuildingEntity(decisionEntity);

					// Se pide a la entidad que mueva hacia el pozo.
					auto routeMessage = std::make_shared<AvatarRouteTo>();
					routeMessage->target = decisionEntity->getPosition();
					routeMessage->sender = _entity;
					_entity->emitMessageN(routeMessage);

					auto buildMessage = std::make_shared<BuildWell>();
					_entity->emitMessageN(buildMessage);
//					std::cout<<"workman state buildwell"<<std::endl;
				}

			}else // i click into some other entities and reset building entity and repair ability
			{
				setBuildingEntity(nullptr);

				removeRepairAbility(decisionEntity);
			}

		}else if(message->type.compare("DisableRepairAbility") == 0)
		{
//			std::cout<<"workman state disable repair well"<<std::endl;
			disableRepairBuildingAbility();
		}
	} 

	void CWorkmanState::removeRepairAbility(Logic::CEntity* decisionEntity)
	{

		if(isRepairBuildingAbilityActive())
		{
			disableRepairBuildingAbility();			
		}

	}
	void CWorkmanState::setBuildingEntity(CEntity *entity)
	{
		buildingEntity = entity;
	}

	CEntity* CWorkmanState::getBuildingEntity()
	{
		return buildingEntity;
	}

	bool CWorkmanState::isRepairBuildingAbilityActive()
	{
		return repairBuildingAbilityActive;
	}
	void CWorkmanState::disableRepairBuildingAbility()
	{
		repairBuildingAbilityActive=false;		
	}


	void CWorkmanState::setLastResource(CEntity *tree)
	{
		lastResource = tree;
	}

	CEntity* CWorkmanState::getLastResource()
	{
		return lastResource;
	}

	void CWorkmanState::setPosition(const Vector3 &p)
	{
		position = p;
	}

	const Vector3& CWorkmanState::getPosition()
	{
		return position;
	}

	void CWorkmanState::setAction(WorkmanAction a)
	{
		action = a;
	}

	CWorkmanState::WorkmanAction CWorkmanState::getAction()
	{
		return action;
	}

	unsigned int CWorkmanState::getTime()
	{
		return deltaTime;
	}

	// maxToGather methods.

	unsigned int CWorkmanState::getGatherMax()
	{
		return maxToGather;
	}

	// gatherUnit methods.

	unsigned int CWorkmanState::getGatherUnit()
	{
		return gatherUnit;
	}

	// timeBetweenGather methods.

	unsigned int CWorkmanState::getGatherTime()
	{
		return timeBetweenGather;
	}

	// currentGatherQuantity methods.

	unsigned int CWorkmanState::getCurrentCharge()
	{
		return currentGatherQuantity;
	}

	void CWorkmanState::setCurrentCharge(unsigned int q)
	{
		currentGatherQuantity = q;
	}

	// resType methods.

	AI::ResourceType CWorkmanState::getResourceType()
	{
		return resType;
	}

	void CWorkmanState::setResourceType(AI::ResourceType type)
	{
		resType = type;
	}
	void CWorkmanState::manageSmartEntityDestroyed(Logic::CEntity *smartEntity)
	{
//		std::cout<<_entity->getName()<<" entity died";
		Logic::CEntity* next= AI::CServer::getSingletonPtr()->getEntityManager()->getClosestEntityTypeWithOverlaps(smartEntity,RADIUS_OVERLAPS,_entity->getPosition());
		if(next!=nullptr)
		{
//			std::cout<<" move to "<<next->getName()<<std::endl;
			auto m = std::make_shared<AvatarMoveToEntity>();
			m->target = next;
			_entity->emitMessageN(m);
		}else
		{
			//if the entity which i 'm interacting are destroyed and there isn't another resource closest,
			//if i have some resource i will go to the chest
			if(currentGatherQuantity>0)
			{
				Logic::CEntity* closestChest = AI::CServer::getSingletonPtr()->getEntityManager()->getClosestEntity(AI::CEntityManager::EntityType::Chest,_entity->getPosition());
				// Se pide a la entidad que mueva hacia el cofre.
				auto moveMessage = std::make_shared<AvatarMoveToEntity>();
				moveMessage->target = closestChest;
				_entity->emitMessageN(moveMessage);	

			}else
			{
				//move the entity little far from chest when they stop near it
//				std::cout<<" go far from "<<smartEntity->getName()<<std::endl;
				auto routeMessage = std::make_shared<AvatarRouteTo>();
				routeMessage->target = AI::CServer::getSingletonPtr()->getGameManager()->getPositionFarToEntity(smartEntity,_entity,RESOURCE_GET_AWAY_DISTANCE,RESOURCE_GET_AWAY_ANGLE);
				_entity->emitMessageN(routeMessage);
			}

		}
	}

	void CWorkmanState::findNextResourceClosest(Logic::CEntity* entity)
	{
			Logic::CEntity* next = AI::CServer::getSingletonPtr()->getEntityManager()->getClosestEntityTypeWithOverlaps(entity,RADIUS_OVERLAPS,_entity->getPosition());
//			std::cout<<next<<std::endl;
			//if there is a resource closest
			if(next!=nullptr)
			{
				//move to that resource
				auto moveTNext = std::make_shared<AvatarMoveToEntity>();
				moveTNext->target = next;
				_entity->emitMessageN(moveTNext);
			}else //go little far from entity
			{
				auto routeMessage = std::make_shared<AvatarRouteTo>();
				routeMessage->target = AI::CServer::getSingletonPtr()->getGameManager()->getPositionFarToEntity(entity,_entity,RESOURCE_GET_AWAY_DISTANCE,RESOURCE_GET_AWAY_ANGLE);
				_entity->emitMessageN(routeMessage);

			}
	}

}

