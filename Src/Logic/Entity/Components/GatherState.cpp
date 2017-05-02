/**
@file GatherState.cpp
*/

#include "GatherState.h"

#include "AI/Server.h"

#include "Logic\Entity\Entity.h"
#include "Logic/Entity/Components/InteractSOState.h"
#include "Logic/Entity/Components/TransportedComponent.h"
#include "Logic\Entity\Components\CarrierState.h"
#include "Logic\Entity\Components\WorkmanState.h"
#include "Logic\Entity\Components\ChaseCarriedEntityState.h"
#include "Logic\Entity\Components\SmartObject.h"
#include "Logic/SmartObjectMessages.h"
#include "Logic/MovementMessages.h"
#include "Logic/RouteCalculationMessages.h"
#include "Logic/WorkmanMessages.h"
#include "Logic/CarrierMessages.h"
#include "Logic/CombatMessages.h"
#include "Logic/AllyUnitMessages.h"

#include "AI\Server.h"
#include "AI\EntityManager.h"
#include "AI\GameManager.h"


#define RESOURCE_DISTANCE 10.0f
#define RADIUS_OVERLAPS 20

namespace Logic 
{
	IMP_FACTORY(CGatherState);

	//---------------------------------------------------------

	CGatherState::~CGatherState() 
	{

	} 

	//---------------------------------------------------------

	bool CGatherState::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		return true;

	} 

	void CGatherState::awake()
	{


	}
	void CGatherState::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);
	}

	//---------------------------------------------------------

	bool CGatherState::acceptN(const std::shared_ptr<NMessage> &message)
	{
		return (message->type.compare("AvatarMoveToEntity") == 0);
	}

	//---------------------------------------------------------

	void CGatherState::processN(const std::shared_ptr<NMessage> &message)
	{
		if(message->type.compare("AvatarMoveToEntity") == 0 )
		{
			std::shared_ptr<AvatarMoveToEntity> m = std::static_pointer_cast<AvatarMoveToEntity>(message);
			Logic::CEntity* decisionEntity = m->target;
			if(decisionEntity==nullptr)
				return ;
			// Si la entidad es un smart object.
			if(decisionEntity->getType() == "Tree" || decisionEntity->getType() == "Iron" || decisionEntity->getType() == "Rock" || decisionEntity->getType() == "Chest")
			{
				Logic::IComponent *iComponent = _entity->getComponent("CInteractSOState");
				assert(iComponent);
				Logic::CInteractSOState* interactComponent = static_cast<Logic::CInteractSOState*>(iComponent);

				if(interactComponent->getSmartEntity()!=nullptr && interactComponent->getSmartPosition()!=Vector3::NEGATIVE_UNIT_Y)
					AI::CServer::getSingletonPtr()->getGameManager()->freeSmartPosition(interactComponent->getSmartEntity(),interactComponent->getSmartPosition(),_entity);

				iComponent=_entity->getComponent("CCarrierState");
				assert(iComponent);
				Logic::CCarrierState* carrierComponent = static_cast<Logic::CCarrierState*>(iComponent);


				//check if is a chest and i pulse the transport ability
				if((decisionEntity->getType().compare("Chest")==0))
				{
					//if chest is already disabled
					iComponent=decisionEntity->getComponent("CTransportedComponent");
					assert(iComponent);
					Logic::CTransportedComponent* transportComponent = static_cast<Logic::CTransportedComponent*>(iComponent);
					//if chest it isn't accessible someone still transport it and chase that
					if(transportComponent->isTransported())
					{
						//std::cout<<"chase chest"<<std::endl;
						iComponent=_entity->getComponent("CChaseCarriedEntityState");
						assert(iComponent);
						Logic::CChaseCarriedEntityState* chaseComponent = static_cast<Logic::CChaseCarriedEntityState*>(iComponent);
						chaseComponent->setCarriedEntity(decisionEntity);

						auto m = std::make_shared<ChaseCarriedEntity>();
						_entity->emitMessageN(m);

						auto routeMessage = std::make_shared<AvatarRouteTo>();
						routeMessage->target = decisionEntity->getPosition();
						routeMessage->sender = _entity;
						_entity->emitMessageN(routeMessage);

						return ;
					
					}else
					{
						//if the transport ability is activate
						if(carrierComponent->isCarrierAbilityActive())
						{
							//std::cout<<"transport chest"<<std::endl;
							carrierComponent->setCarriedEntity(decisionEntity);
							//go to transport state
							auto transportMessage = std::make_shared<TransportEntity>();
							_entity->emitMessageN(transportMessage);
						
							auto routeMessage = std::make_shared<AvatarRouteTo>();
							routeMessage->target = decisionEntity->getPosition();
							routeMessage->sender = _entity;
							_entity->emitMessageN(routeMessage);

							return ;
						}
						//else intract with chest normally
					}

				}else{
					//if the transport ability is activate send a message to disable it 
					//in order to avoid that when the workman is full of resource and go to chest not try to transportChest
					removeTransportAbility(decisionEntity,carrierComponent);
				}


				interactComponent->setSmartPosition(Vector3::NEGATIVE_UNIT_Y);

				// Se apunta el smart object en uso en el compoente de estado.
				interactComponent->setSmartEntity(decisionEntity);

				auto interactMessage = std::make_shared<InteractWithSmartObject>();
				_entity->emitMessageN(interactMessage);

				goNearestSmartObjectResource(decisionEntity);

			
			}else{
	//			std::cout<<_entity->getName()<<" decision entity not managed "<<decisionEntity->getName()<<std::endl;
				//if we don't react with this entity move up to him
				//auto routeMessage = std::make_shared<AvatarRouteTo>();
				//routeMessage->target = decisionEntity->getPosition();
				//routeMessage->sender = _entity;
				//_entity->emitMessageN(routeMessage);

				//check if the carrier ability is enabled and disable it if necessary
				removeTransportAbility(decisionEntity);
				return ;
			}
		}
	} 

	void CGatherState::goNearestSmartObjectResource(Logic::CEntity* decisionEntity)
	{
		Logic::IComponent *iComponent = decisionEntity->getComponent("CSmartObject");
			assert(iComponent);
			Logic::CSmartObject *smartObjectComponet = static_cast<Logic::CSmartObject*>(iComponent);
			Vector3 position = smartObjectComponet->getNearestSmartPositionAvailable(_entity->getPosition());
			if(position==Vector3::ZERO)
			{
//				std::cout<<"compute the distance"<<std::endl;
			// Se calcula una posición cercana al smart object fuera desde la caja de colision
				//para evitar que el rayo del RouteTo component choca con el arbol
				Vector3 direction =  _entity->getPosition() - decisionEntity->getPosition();
				direction.normalise();
				position = decisionEntity->getPosition() + direction*RESOURCE_DISTANCE;
			}
//			std::cout<<_entity->getName()<<" decision go to entity "<<decisionEntity->getName()<<" entity pos="<<decisionEntity->getPosition()<<" dest pos="<<position<<std::endl;
			// Se pide a la entidad que mueva hacia el smart object.
			auto routeMessage = std::make_shared<AvatarRouteTo>();
			routeMessage->target = position;
			routeMessage->sender = _entity;
			_entity->emitMessageN(routeMessage);

	}
	void CGatherState::removeTransportAbility(Logic::CEntity* decisionEntity,Logic::CCarrierState* carrierComponent)
	{
		if(carrierComponent==nullptr)
		{
			Logic::IComponent *iComponent=_entity->getComponent("CCarrierState");
			assert(iComponent);
			carrierComponent = static_cast<Logic::CCarrierState*>(iComponent);
		}

		if(carrierComponent->isCarrierAbilityActive())
		{
			auto abilityMessage=std::make_shared<CastAbility>();
			abilityMessage->ability = carrierComponent->getCarrierAbilityCharKey();
			_entity->emitMessageN(abilityMessage);			
		}

	}

}

