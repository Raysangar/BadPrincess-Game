#include "LAMoveTo.h"


namespace AI 
{


//////////////////////////////
//	Implementaci�n de CLAMoveTo
//////////////////////////////

	//-------------------------------------------------------------------------

	CLatentAction::LAStatus CLAMoveTo::OnStart()
	{	
		//std::cout << "STATE: MOVING" << std::endl;
		sendAnimationMessage("Walk");
		return SUSPENDED;
	}

	//-------------------------------------------------------------------------

	void CLAMoveTo::OnStop()
	{

	}

	//-------------------------------------------------------------------------

	CLatentAction::LAStatus CLAMoveTo::OnRun(unsigned int msecs) 
	{
		return SUSPENDED;
	}

	//-------------------------------------------------------------------------

	CLatentAction::LAStatus CLAMoveTo::OnAbort() 
	{
		// Cuando se aborta se queda en estado terminado con fallo
		return FAIL;
	}

	//-------------------------------------------------------------------------

	bool CLAMoveTo::acceptN(const std::shared_ptr<NMessage> &message)
	{
		// La acci�n no acepta mensajes
		return false; //cambiar por AvatarFinishedRoute cuando est� implementado
	}

	//-------------------------------------------------------------------------

	void CLAMoveTo::processN(const std::shared_ptr<NMessage> &message)
	{	
		

	}


} //namespace AI 
