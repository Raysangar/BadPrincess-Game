#include "LAWaiting.h"

#include "Logic\RouteCalculationMessages.h"
#include "Logic\Entity\Entity.h"

namespace AI 
{


//////////////////////////////
//	Implementación de CLAWaiting
//////////////////////////////

	//-------------------------------------------------------------------------

	CLatentAction::LAStatus CLAWaiting::OnStart()
	{	
//		std::cout <<_entity->getName()<< " Waiting" << std::endl;

		sendAnimationMessage("Idle");
		_currentWaitTime=0;
		//interrupt movement
		auto moveMessage = std::make_shared<AvatarInterruptRoute>();
		moveMessage->sender = _entity;
		_entity->emitMessageN(moveMessage);
		return RUNNING;
	}

	//-------------------------------------------------------------------------

	void CLAWaiting::OnStop()
	{
	}

	//-------------------------------------------------------------------------

	CLatentAction::LAStatus CLAWaiting::OnRun(unsigned int msecs) 
	{
		_currentWaitTime+=msecs;
		if(_currentWaitTime>=_waitTime)
			return SUCCESS;
		return RUNNING;
	}

	//-------------------------------------------------------------------------

	CLatentAction::LAStatus CLAWaiting::OnAbort() 
	{
		// Cuando se aborta se queda en estado terminado con fallo
		return FAIL;
	}
	
	//-------------------------------------------------------------------------

	bool CLAWaiting::acceptN(const std::shared_ptr<NMessage> &message)
	{
		// La acción no acepta mensajes
		return false;
	}

	//-------------------------------------------------------------------------

	void CLAWaiting::processN(const std::shared_ptr<NMessage> &message)
	{		
		// La acción no procesa mensajes
	}


} //namespace AI 
