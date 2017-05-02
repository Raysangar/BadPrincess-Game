#include "LAIdle.h"


namespace AI 
{


//////////////////////////////
//	Implementaci�n de CLAIdle
//////////////////////////////

	//-------------------------------------------------------------------------

	CLatentAction::LAStatus CLAIdle::OnStart()
	{	
		//std::cout << "IDLELING" << std::endl;

		sendAnimationMessage("Idle");

		return SUSPENDED;
	}

	//-------------------------------------------------------------------------

	void CLAIdle::OnStop()
	{
	}

	//-------------------------------------------------------------------------

	CLatentAction::LAStatus CLAIdle::OnRun(unsigned int msecs) 
	{
		return SUSPENDED;
	}

	//-------------------------------------------------------------------------

	CLatentAction::LAStatus CLAIdle::OnAbort() 
	{
		// Cuando se aborta se queda en estado terminado con fallo
		return FAIL;
	}
	
	//-------------------------------------------------------------------------

	bool CLAIdle::acceptN(const std::shared_ptr<NMessage> &message)
	{
		// La acci�n no acepta mensajes
		return false;
	}

	//-------------------------------------------------------------------------

	void CLAIdle::processN(const std::shared_ptr<NMessage> &message)
	{		
		// La acci�n no procesa mensajes
	}


} //namespace AI 
