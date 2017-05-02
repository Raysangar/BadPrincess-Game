#include "LAWaitCarriedEntity.h"

#include "Logic\Entity\Entity.h"
#include "Logic\Entity\Components\ChaseCarriedEntityState.h"
#include "Logic\Entity\Components\TransportedComponent.h"
#include "Logic\Entity\Components\SteeringMovement.h"
#include "Logic/MovementMessages.h"

namespace AI 
{


//////////////////////////////
//	Implementación de CLAWaitCarriedEntity
//////////////////////////////

	//---------------------------------------------------------------
	
	CLatentAction::LAStatus CLAWaitCarriedEntity::OnStart()
	{	
//		std::cout<<_entity->getName() << "START WAITING.." << std::endl;


		Logic::IComponent* component=_entity->getComponent("CChaseCarriedEntityState");
		assert(component);
		Logic::CChaseCarriedEntityState * chaseState = static_cast<Logic::CChaseCarriedEntityState*>(component);

		if(chaseState->getCarriedEntity()==nullptr)
			return EXIT_SM_FAIL;

		sendAnimationMessage("Idle2");
		component=chaseState->getCarriedEntity()->getComponent("CTransportedComponent");
		assert(component);
		_transportedComponent = static_cast<Logic::CTransportedComponent*>(component);	
		//check if the chest it isn't released
		if(!_transportedComponent->isTransported())
		{
			auto m = std::make_shared<AvatarMoveToEntity>();
			m->target = _transportedComponent->getEntity();	
			_entity->emitMessageN(m);
			return SUSPENDED;
		}
		component= _transportedComponent->getTransporterEntity()->getComponent("CSteeringMovement");
		assert(component);
		_carrierSteeringComponent = static_cast<Logic::CSteeringMovement*>(component);
		return RUNNING;
	}

	//---------------------------------------------------------------

	CLatentAction::LAStatus CLAWaitCarriedEntity::OnRun(unsigned int msecs) 
	{		
		//first check if the carried entity revert to be available
		if(!_transportedComponent->isTransported())
		{
			auto m = std::make_shared<AvatarMoveToEntity>();
			m->target = _transportedComponent->getEntity();	
			_entity->emitMessageN(m);
			return SUSPENDED;
		}else if(_carrierSteeringComponent->getCurrentSpeed()!=Vector3::ZERO)
		{
			//if the carrier restart to move go back to chase state
			return FAIL;
		}
		
		return RUNNING;
	}



	//---------------------------------------------------------------

	void CLAWaitCarriedEntity::OnStop()
	{

	}

	//---------------------------------------------------------------

	CLatentAction::LAStatus CLAWaitCarriedEntity::OnAbort() 
	{
		// Cuando se aborta se queda en estado terminado con fallo
		
		return FAIL;
	}

	//---------------------------------------------------------------

	bool CLAWaitCarriedEntity::acceptN(const std::shared_ptr<NMessage> &message)
	{		
		return (message->type.compare("AvatarRouteTo") == 0);
	}

	//---------------------------------------------------------------

	void CLAWaitCarriedEntity::processN(const std::shared_ptr<NMessage> &message)
	{		
		if(message->type.compare("AvatarRouteTo") == 0)
		{
			if(message->sender==nullptr)
				this->exitSM(false);
			else
				this->finish(false);
		}
	}


} //namespace AI 
