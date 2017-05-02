#include "LAChaseCarriedEntity.h"

#include "Logic\Entity\Entity.h"
#include "Logic\Entity\Components\TransportedComponent.h"
#include "Logic\Entity\Components\SteeringMovement.h"
#include "Logic\Entity\Components\StatsModifier.h"
#include "Logic\Entity\Components\ChaseCarriedEntityState.h"
#include "Logic/RouteCalculationMessages.h"
#include "Logic/CarrierMessages.h"
#include "Logic/UnitMessages.h"
#include "Logic/MovementMessages.h"

#include "AI\Server.h"
#include "AI\EntityManager.h"

#define MAX_RANGE_DISTANCE 12
#define CLOSEST_OVERLAPS_RADIUS 30

namespace AI 
{


//////////////////////////////
//	Implementación de CLAChaseCarriedEntity
//////////////////////////////

	//---------------------------------------------------------------
	
	CLatentAction::LAStatus CLAChaseCarriedEntity::OnStart()
	{	
//		std::cout <<_entity->getName()<< "START CHASING.." << std::endl;

		sendAnimationMessage("Walk");

		Logic::IComponent* component=_entity->getComponent("CChaseCarriedEntityState");
		assert(component);
		_chaseState = static_cast<Logic::CChaseCarriedEntityState*>(component);	

		if(_chaseState->getCarriedEntity()==nullptr)
			return EXIT_SM_FAIL;
		component=_chaseState->getCarriedEntity()->getComponent("CTransportedComponent");
		assert(component);
		Logic::CTransportedComponent * transportComponent = static_cast<Logic::CTransportedComponent*>(component);	

		//if it not contains transporter means that someone interrupt the transport 
		if(!transportComponent->isTransported())
		{
			//try to interact with this entity
			auto m = std::make_shared<AvatarMoveToEntity>();
			m->target = _chaseState->getCarriedEntity();	
			_entity->emitMessageN(m);
			return SUSPENDED;
		}
		else
			_carrierEntity = transportComponent->getTransporterEntity();
		component=_carrierEntity->getComponent("CSteeringMovement");
		assert(component);
		_carrierSteering = static_cast<Logic::CSteeringMovement*>(component);

		//sendRouteMessage(_chaseState->getCarriedEntity()->getPosition());

		_slowSpeed= false;
		return RUNNING;
	}

	//---------------------------------------------------------------

	CLatentAction::LAStatus CLAChaseCarriedEntity::OnRun(unsigned int msecs) 
	{		
		//compute the distance to the carrier entity
		float dist=_entity->getPosition().distance(_chaseState->getCarriedEntity()->getPosition());
		//if i'm closest and i walk at max speed 
		if(!_slowSpeed && dist<MAX_RANGE_DISTANCE)
		{
			//go slowly
			auto mMS = std::make_shared<ModifyStat>();
			mMS->modID = "ChaseCarriedEntity";
			mMS->modType = Logic::ModificationType::speedMod;
			mMS->percentage = -50;
			mMS->durationMsecs = 0;
			_entity->emitMessageN(mMS);

			//send message that i reach a enity (used for thief)
			auto reachedEntity = std::make_shared<CarriedEntityReached>();
			reachedEntity->entity = _chaseState->getCarriedEntity();
			_entity->emitMessageN(reachedEntity);

			_slowSpeed=true;
		}
		
		//if the carrier it isn't stopped
		Vector3 carrierSpeed = _carrierSteering->getCurrentSpeed();
		if(carrierSpeed!=Vector3::ZERO)
		{
			//check if there is another entity of the same type closest and active
			if(!checkClosestEntity())
				sendRouteMessage(_chaseState->getCarriedEntity()->getPosition());//if not continue to chase this entity			
		}else
		{
			//check if i'm stopped and closest
			Logic::IComponent* component=_entity->getComponent("CSteeringMovement");
			assert(component);
			Logic::CSteeringMovement* mySteering = static_cast<Logic::CSteeringMovement*>(component);
			//if i'm stopped
			if(mySteering->getCurrentSpeed()==Vector3::ZERO)
			{
				//and closest
				if(dist<MAX_RANGE_DISTANCE)
					return SUCCESS;//go to waitChase
				else
					sendRouteMessage(_chaseState->getCarriedEntity()->getPosition());//else try to reach this entity			
			}
		}
		return RUNNING;
	}

	bool CLAChaseCarriedEntity::checkClosestEntity()
	{
		Logic::CEntity*closest= AI::CServer::getSingletonPtr()->getEntityManager()->getClosestEntityTypeWithOverlaps(_chaseState->getCarriedEntity(),CLOSEST_OVERLAPS_RADIUS,_entity->getPosition());
		if(closest!=nullptr)
		{
			//check if the new entity is available
			Logic::IComponent* component=closest->getComponent("CTransportedComponent");
			assert(component);
			Logic::CTransportedComponent* transportComponent = static_cast<Logic::CTransportedComponent*>(component);	
			if(!transportComponent->isTransported())
			{
				//if is available move to it
				auto m = std::make_shared<AvatarMoveToEntity>();
				m->target = closest;	
				_entity->emitMessageN(m);

				return true;
			}
		}
		return false;
	}

	//---------------------------------------------------------------

	void CLAChaseCarriedEntity::OnStop()
	{
		if(_slowSpeed)
		{
			auto mMS = std::make_shared<RestoreStat>();
			mMS->modID = "ChaseCarriedEntity";
			_entity->emitMessageN(mMS);
		}
	}

	//---------------------------------------------------------------

	CLatentAction::LAStatus CLAChaseCarriedEntity::OnAbort() 
	{
		// Cuando se aborta se queda en estado terminado con fallo
		if(_slowSpeed)
		{
			auto mMS = std::make_shared<RestoreStat>();
			mMS->modID = "ChaseCarriedEntity";
			_entity->emitMessageN(mMS);
		}
		return FAIL;
	}

	//---------------------------------------------------------------

	bool CLAChaseCarriedEntity::acceptN(const std::shared_ptr<NMessage> &message)
	{		
		return (message->type.compare("AvatarRouteTo") == 0);
	}

	//---------------------------------------------------------------

	void CLAChaseCarriedEntity::processN(const std::shared_ptr<NMessage> &message)
	{		
		if(message->type.compare("AvatarRouteTo") == 0)
		{
			if(message->sender==nullptr)
				this->exitSM(false);
		}

	}

	void CLAChaseCarriedEntity::sendRouteMessage(Vector3 target)
	{		
		auto m = std::make_shared<AvatarRouteTo>();
		m->sender = _entity;
		m->target = target;
		m->calculateNewRoute = true;
		_entity->emitMessageN(m);
//		std::cout<<"CHASE ENTITY send message AvatarRouteTo"<<std::endl;
	}

} //namespace AI 
