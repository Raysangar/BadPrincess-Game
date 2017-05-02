/**
@file InteractSOState.cpp
*/

#include "InteractSOState.h"

#include "Logic/EntityMessages.h"
#include "Logic/MovementMessages.h"
#include "Logic/RouteCalculationMessages.h"
#include "Logic\SmartObjectMessages.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Components/WorkmanState.h"
#include "Logic/Entity/Components/ThiefEnemyState.h"

#include "AI/Server.h"
#include "AI/EntityManager.h"
#include "AI/GameManager.h"

#define RADIUS_OVERLAPS 30
#define RESOURCE_GET_AWAY_DISTANCE 30.0f
#define RESOURCE_GET_AWAY_ANGLE 120

namespace Logic 
{
	IMP_FACTORY(CInteractSOState);

	//---------------------------------------------------------

	CInteractSOState::~CInteractSOState() 
	{

	} 

	//---------------------------------------------------------

	bool CInteractSOState::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		return true;

	} 

	void CInteractSOState::awake()
	{

		_smartEntity=nullptr;
		_smartPosition = Vector3::NEGATIVE_UNIT_Y;
		_isInteracting = false;
	}
	void CInteractSOState::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);
	}

	//---------------------------------------------------------

	bool CInteractSOState::acceptN(const std::shared_ptr<NMessage> &message)
	{
		return message->type.compare("EntityDestroying") == 0 || message->type.compare("SmartDataPositionReceiver") == 0 || message->type.compare("SmartObjectDisabled") == 0 || message->type.compare("AttemptSmartObjectLimitReached")==0;
	}

	//---------------------------------------------------------

	void CInteractSOState::processN(const std::shared_ptr<NMessage> &message)
	{
		if(message->type.compare("EntityDestroying") == 0)
		{
			std::shared_ptr<EntityDestroying> m = std::static_pointer_cast<EntityDestroying>(message);
			if(m->entity==_smartEntity)
			{
				Logic::IComponent* iComponent = _entity->getComponent("CWorkmanState");
				if(iComponent != nullptr)
				{
					Logic::CWorkmanState* ws = static_cast<Logic::CWorkmanState*>(iComponent);
					ws->manageSmartEntityDestroyed(_smartEntity);
				}else
				{
					iComponent = _entity->getComponent("CThiefEnemyState");
					if(iComponent != nullptr)
					{
						Logic::CThiefEnemyState* ts = static_cast<Logic::CThiefEnemyState*>(iComponent);
						ts->manageSmartEntityDestroyed(_smartEntity);
					}
				}
				_smartEntity=nullptr;
				_smartPosition = Vector3::NEGATIVE_UNIT_Y;
			}
		}else if(message->type.compare("SmartDataPositionReceiver") == 0)
		{
			std::shared_ptr<SmartDataPositionReceiver> m = std::static_pointer_cast<SmartDataPositionReceiver>(message);
		
			setSmartPosition(m->position);

			auto moveMessage = std::make_shared<AvatarRouteTo>();
			moveMessage->calculateNewRoute = true;
			moveMessage->target = m->position;
			moveMessage->sender = _entity;
			_entity->emitMessageN(moveMessage);
		}
		else if(message->type.compare("SmartObjectDisabled") == 0)
		{
//			std::cout<<_entity->getName()<<" interactSOState recived SamrtObjectDisabled"<<std::endl;
			std::shared_ptr<SmartObjectDisabled> m = std::static_pointer_cast<SmartObjectDisabled>(message);
			setSmartEntity(nullptr);		
			setSmartPosition(Vector3::NEGATIVE_UNIT_Y);

			//auto moveMessage = std::make_shared<AvatarInterruptRoute>();
			//moveMessage->sender = _entity;
			//_entity->emitMessageN(moveMessage);
		}else if( message->type.compare("AttemptSmartObjectLimitReached")==0)
		{
			std::shared_ptr<AttemptSmartObjectLimitReached> m = std::static_pointer_cast<AttemptSmartObjectLimitReached>(message);
			AI::CServer::getSingletonPtr()->getGameManager()->freeSmartPosition(m->entity,getSmartPosition(),_entity);
		}
	} 

	void CInteractSOState::setSmartEntity(CEntity *entity)
	{
		_smartEntity = entity;
	}

	CEntity* CInteractSOState::getSmartEntity()
	{
		return _smartEntity;
	}

	void CInteractSOState::setSmartPosition(const Vector3 &p)
	{
		_smartPosition = p;
	}

	const Vector3& CInteractSOState::getSmartPosition()
	{
		return _smartPosition;
	}

}

