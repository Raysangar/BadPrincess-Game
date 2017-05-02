/**
@file ThiefEnemyState.cpp
*/

#include "ThiefEnemyState.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"
#include "Logic/Maps/EntityFactory.h"
#include "AI\Server.h"
#include "AI\EntityManager.h"
#include "Logic/EntityMessages.h"
#include "Logic/MovementMessages.h"
#include "Logic/CarrierMessages.h"
#include "Logic/SmartObjectMessages.h"
#include "Logic/Entity/Components/TransportedComponent.h"
#include "Logic\EnemyMessages.h"
#include "Logic\AllyUnitMessages.h"
#include "Logic/Entity/Components/InteractSOState.h"

#define ESCAPE_DISTANCE_TOLLERANCE 5
#define MAX_SQUARED_DIST_TO_GO_TO_TRAP_CHEST 2500
#define SMARTOBJECT_FULL_RADIUS_OVERLAPS 30.0f

namespace Logic 
{
	IMP_FACTORY(CThiefEnemyState);

	//---------------------------------------------------------

	CThiefEnemyState::~CThiefEnemyState() 
	{

	} 

	//---------------------------------------------------------

	bool CThiefEnemyState::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;
		assert(entityInfo->hasAttribute("thief_units"));
		_thiefUnit = entityInfo->getIntAttribute("thief_units");

		assert(entityInfo->hasAttribute("thief_time"));
		_thiefTime = entityInfo->getIntAttribute("thief_time");

		assert(entityInfo->hasAttribute("thief_animation"));
		_thiefAnimation = entityInfo->getStringAttribute("thief_animation");

		return true;

	} 

	void CThiefEnemyState::awake()
	{
		 _resType=AI::ResourceType::none;
		 _currentThiefQuantity=0;
		 _escapeSpawnPoint=nullptr;		
		
		auto m = std::make_shared<ThiefAttack>();
		_entity->emitMessageN(m);
		
	}

	void CThiefEnemyState::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);
	}

	//---------------------------------------------------------

	bool CThiefEnemyState::acceptN(const std::shared_ptr<NMessage> &message)
	{
		return message->type.compare("EntityDying") == 0 || message->type.compare("ThiefAttack") == 0 
			|| message->type.compare("CarriedEntityReached") == 0 ||  message->type.compare("SmartObjectDisabled")==0 
			||  message->type.compare("AttemptSmartObjectLimitReached")==0 || message->type.compare("ThiefEscape")==0 
			|| message->type.compare("AvatarFinishedRoute")==0 || message->type.compare("ChestTrapCreated")==0;
	}

	//---------------------------------------------------------

	void CThiefEnemyState::processN(const std::shared_ptr<NMessage> &message)
	{
		if(message->type.compare("EntityDying")==0)
		{
			auto dieMessage = std::static_pointer_cast<EntityDying>(message);
			if(_resType!= AI::ResourceType::none)
			{
				//std::cout<<"restituito qta ="<<getCurrentThiefQuantity()<<std::endl;
				AI::ResourceServer::getInstance()->addResource(getResourceType(),getCurrentThiefQuantity());
				setCurrentThiefQuantity(0);
			}
		}
		else if(message->type.compare("ThiefAttack")==0)
		{
			attackChest();
		}
		else if( message->type.compare("CarriedEntityReached")==0)
		{
			//when i recive this message i reach the transported chest
			auto reachCarriedMessage = std::static_pointer_cast<CarriedEntityReached>(message);
			Logic::IComponent *component=reachCarriedMessage->entity->getComponent("CTransportedComponent");
			assert(component);
			Logic::CTransportedComponent * transportComponent = static_cast<Logic::CTransportedComponent*>(component);
			if(transportComponent->isTransported())
			{
				//send message to interrupt the transport 
				auto interruptTransport = std::make_shared<InterruptTransport>();
				transportComponent->getTransporterEntity()->emitMessageN(interruptTransport);
			}
			//send a message to go to chest in order to start stole
			auto move = std::make_shared<AvatarMoveToEntity>();
			move->target = reachCarriedMessage->entity;
			_entity->emitMessageN(move);
		}
		else if( message->type.compare("SmartObjectDisabled")==0)
		{
			//send a message to go to chest in order to go to chase chest state
			std::shared_ptr<SmartObjectDisabled> m=std::static_pointer_cast<SmartObjectDisabled>(message);
			//std::cout<<_entity->getName()<<" smartobject disabled ="<<std::endl;
			auto move = std::make_shared<AvatarMoveToEntity>();
			move->target = m->entity;
			_entity->emitMessageN(move);


		}else if( message->type.compare("AttemptSmartObjectLimitReached")==0)
		{
			std::shared_ptr<AttemptSmartObjectLimitReached> m=std::static_pointer_cast<AttemptSmartObjectLimitReached>(message);
			if(m->entity!=nullptr)
				attackChest(m->entity);
			else
				escape();
		}else if(message->type.compare("ThiefEscape")==0)
		{
			std::shared_ptr<ThiefEscape> m=std::static_pointer_cast<ThiefEscape>(message);
			_escapeSpawnPoint = m->escapeEntity;

		}else if(message->type.compare("AvatarFinishedRoute")==0)
		{
			//if thief is escaping  and it finished the movement 
			if(_escapeSpawnPoint != nullptr)
			{
				//check if it out of world visible area

				//in this case it finish his job and we destroy it but without send the entityDying message to
				//this component in order to avoid the loot restitution
				auto dieMessage = std::make_shared<EntityDying>();
				dieMessage->entity = _entity;
				_entity->emitMessageN(dieMessage, this);

				auto destroyMessage = std::make_shared<EntityDestroying>();
				destroyMessage->entity = _entity;
				_entity->getMap()->sendMessageToAll(destroyMessage);
				
			}
		}else if(message->type.compare("ChestTrapCreated")==0)
		{
			std::shared_ptr<ChestTrapCreated> m=std::static_pointer_cast<ChestTrapCreated>(message);
			std::cout<<_entity->getName()<<" chestTrapCreated"<<std::endl;
			//if it doesn't already stole or it doesn't already go to a trapchest
			if(_escapeSpawnPoint==nullptr)
			{
				Logic::IComponent *iComponent = _entity->getComponent("CInteractSOState");
				assert(iComponent != nullptr);
				Logic::CInteractSOState* interactComponent = static_cast<Logic::CInteractSOState*>(iComponent);
				if(interactComponent->getSmartEntity()==nullptr)
				{
					//std::cout<<_entity->getName()<<" interact smartENeity null move to trap"<<std::endl;
					auto moveMessage = std::make_shared<AvatarMoveToEntity>();
					moveMessage->target = m->trap;
					_entity->emitMessageN(moveMessage);
				}
				//if it doesn't interact with a chest
				else 
				{
					std::set<Logic::CEntity*>::const_iterator it= AI::CServer::getSingletonPtr()->getEntityManager()->getTrapChestList()->find(interactComponent->getSmartEntity());
					if(it==AI::CServer::getSingletonPtr()->getEntityManager()->getTrapChestList()->end() && !interactComponent->isInteracting())
					{
						//compute the distance from the
						float smartEntitySquaredDist=_entity->getPosition().squaredDistance(interactComponent->getSmartEntity()->getPosition());
						float trapSquaredDist=_entity->getPosition().squaredDistance(m->trap->getPosition());
						float trapSmartEntitySquaredDist=interactComponent->getSmartEntity()->getPosition().squaredDistance(m->trap->getPosition());
						//std::cout<<_entity->getName()<<" interact not started smd="<<smartEntitySquaredDist<<" td="<<trapSquaredDist<<" smtd="<<trapSmartEntitySquaredDist<<std::endl;
						if(smartEntitySquaredDist>=MAX_SQUARED_DIST_TO_GO_TO_TRAP_CHEST 
							|| (smartEntitySquaredDist<MAX_SQUARED_DIST_TO_GO_TO_TRAP_CHEST && trapSquaredDist<MAX_SQUARED_DIST_TO_GO_TO_TRAP_CHEST)
							|| (smartEntitySquaredDist<MAX_SQUARED_DIST_TO_GO_TO_TRAP_CHEST && trapSquaredDist>MAX_SQUARED_DIST_TO_GO_TO_TRAP_CHEST && trapSmartEntitySquaredDist<MAX_SQUARED_DIST_TO_GO_TO_TRAP_CHEST))
						{
							//std::cout<<_entity->getName()<<"move to trap"<<std::endl;
							auto moveMessage = std::make_shared<AvatarMoveToEntity>();
							moveMessage->target = m->trap;
							_entity->emitMessageN(moveMessage);
						}
					}
				}
			}
		}else if( message->type.compare("SmartObjectFull") == 0)
		{
			std::shared_ptr<SmartObjectFull> m=std::static_pointer_cast<SmartObjectFull>(message);
			Logic::CEntity * chest = AI::CServer::getSingletonPtr()->getEntityManager()->getClosestEntity(AI::CEntityManager::EntityType::TrapChest,_entity->getPosition(),m->sender);
			if(chest == nullptr || (chest!=nullptr && chest->getPosition().distance(_entity->getPosition())>SMARTOBJECT_FULL_RADIUS_OVERLAPS))
				chest = AI::CServer::getSingletonPtr()->getEntityManager()->getClosestEntity(AI::CEntityManager::EntityType::Chest,_entity->getPosition(),m->sender);

			if(chest!=nullptr && chest->getPosition().distance(_entity->getPosition())<=SMARTOBJECT_FULL_RADIUS_OVERLAPS)
			{
				//std::cout<<_entity->getName()<<" smartObject full go to "<<chest->getName()<<std::endl;
			//move to that resource
				auto moveTNext = std::make_shared<AvatarMoveToEntity>();
				moveTNext->target = chest;
				_entity->emitMessageN(moveTNext);
			}
			//else
			//	std::cout<<_entity->getName()<<" smartObject full next chest null "<<std::endl;
		}
	} 
	void CThiefEnemyState::attackChest(Logic::CEntity* entityToAvoid)
	{
		Logic::CEntity * chest = AI::CServer::getSingletonPtr()->getEntityManager()->getClosestEntity(AI::CEntityManager::EntityType::TrapChest,_entity->getPosition(),entityToAvoid);
		if(chest == nullptr)
			chest = AI::CServer::getSingletonPtr()->getEntityManager()->getClosestEntity(AI::CEntityManager::EntityType::Chest,_entity->getPosition(),entityToAvoid);
			
		auto moveMessage = std::make_shared<AvatarMoveToEntity>();
		moveMessage->target = chest;
		_entity->emitMessageN(moveMessage);
	}

	void CThiefEnemyState::escape()
	{
		Logic::CEntity* spawnPoint =  AI::CServer::getSingletonPtr()->getEntityManager()->getClosestEntity(AI::CEntityManager::EntityType::SpawnPoint, _entity->getPosition());

		auto m2 = std::make_shared<AvatarMoveToEntity>();		
		m2->target = spawnPoint;
		_entity->emitMessageN(m2);
	}

	void CThiefEnemyState::manageSmartEntityDestroyed(Logic::CEntity *smartEntity)
	{
//		std::cout<<_entity->getName()<<" manageSmartEntityDestroyed"<<std::endl;
		attackChest();
	}
			
}

