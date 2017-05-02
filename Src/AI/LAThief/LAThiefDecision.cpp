#include "LAThiefDecision.h"
#include "AI\Server.h"

#include "Logic\Entity\Entity.h"
#include "Logic/Entity/Components/InteractSOState.h"
#include "Logic/Entity/Components/TransportedComponent.h"
#include "Logic\Entity\Components\ChaseCarriedEntityState.h"
#include "Logic/SmartObjectMessages.h"
#include "Logic/MovementMessages.h"
#include "Logic/EnemyMessages.h"
#include "Logic/CarrierMessages.h"
#include "Logic/RouteCalculationMessages.h"
#include "AI\Server.h"
#include "AI\GameManager.h"
#include <cassert>

#define RESOURCE_DISTANCE 5.0f

namespace AI 
{
	bool CLAThiefDecision::acceptN(const std::shared_ptr<NMessage> &message)
	{
		return (message->type.compare("AvatarMoveToEntity") == 0);
	}
	
	void CLAThiefDecision::processN(const std::shared_ptr<NMessage> &message)
	{
		if(message->type.compare("AvatarMoveToEntity") == 0 )
		{
			std::shared_ptr<AvatarMoveToEntity> m = std::static_pointer_cast<AvatarMoveToEntity>(message);
			decisionEntity = m->target;
		}
	}

	CLatentAction::LAStatus CLAThiefDecision::OnStart()
	{	
		if(decisionEntity==nullptr)
			return FAIL;
		// Si la entidad es un smart object.
		if(decisionEntity->getType() == "Chest" || decisionEntity->getType() == "ChestTrap")
		{
			//std::cout<<_entity->getName()<<" decision chest=";

			Logic::IComponent *iComponent = _entity->getComponent("CInteractSOState");
			assert(iComponent != nullptr);
			interactComponent = static_cast<Logic::CInteractSOState*>(iComponent);
			
			if(interactComponent->getSmartEntity()!=nullptr && interactComponent->getSmartPosition()!=Vector3::NEGATIVE_UNIT_Y)
				AI::CServer::getSingletonPtr()->getGameManager()->freeSmartPosition(interactComponent->getSmartEntity(),interactComponent->getSmartPosition(),_entity);


			if (decisionEntity->getType() == "Chest")
			{
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

					goNearestResource(0);

					return SUSPENDED;

				}
			}
			//std::cout<<"interact chest"<<std::endl;

			//start interaction with smart object.
			interactComponent->setSmartPosition(Vector3::NEGATIVE_UNIT_Y);

			// Se apunta el smart object en uso en el compoente de estado.
			interactComponent->setSmartEntity(decisionEntity);

			auto interactMessage = std::make_shared<InteractWithSmartObject>();
			_entity->emitMessageN(interactMessage);

			goNearestResource(RESOURCE_DISTANCE);	
		}else if(decisionEntity->getType() == "SpawnPoint")
		{
			auto interactMessage = std::make_shared<ThiefEscape>();
			interactMessage->escapeEntity = decisionEntity;
			_entity->emitMessageN(interactMessage);

			goNearestResource(0);	
			
		}

		
		return SUSPENDED;
	}

	void CLAThiefDecision::goNearestResource(float distance)
	{
		// Se calcula una posición cercana al smart object fuera desde la caja de colision
			//para evitar que el rayo del RouteTo component choca con el arbol
			Vector3 direction =  _entity->getPosition()-decisionEntity->getPosition();
			direction.normalise();
			Vector3 position = decisionEntity->getPosition() + direction*distance;

//			std::cout<<_entity->getName()<<" decision go to entity "<<decisionEntity->getName()<<" d="<<distance<<std::endl;
			// Se pide a la entidad que mueva hacia el smart object.
			auto routeMessage = std::make_shared<AvatarRouteTo>();
			routeMessage->target = position;
			routeMessage->sender = _entity;
			_entity->emitMessageN(routeMessage);
	}
} //namespace AI