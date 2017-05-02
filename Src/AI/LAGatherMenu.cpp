#include "LAGatherMenu.h"
#include "Logic/RouteCalculationMessages.h"
#include "Logic/EntityMessages.h"
#include "Logic/Entity/Entity.h"

#define MOVING_DISTANCE 30
namespace AI 
{


//////////////////////////////
//	Implementación de CLAGatherMenu
//////////////////////////////

	//-------------------------------------------------------------------------

	CLatentAction::LAStatus CLAGatherMenu::OnStart()
	{			
		sendAnimationMessage("Gather");

		return SUSPENDED;
	}

	//-------------------------------------------------------------------------

	void CLAGatherMenu::OnStop()
	{
	}

	//-------------------------------------------------------------------------

	CLatentAction::LAStatus CLAGatherMenu::OnRun(unsigned int msecs) 
	{


		return RUNNING;
	}

	//-------------------------------------------------------------------------

	CLatentAction::LAStatus CLAGatherMenu::OnAbort() 
	{
		// Cuando se aborta se queda en estado terminado con fallo
		return FAIL;
	}
	
	//-------------------------------------------------------------------------


		

	bool CLAGatherMenu::acceptN(const std::shared_ptr<NMessage> &message)
	{
		
		return false;
	}

	//-------------------------------------------------------------------------

	void CLAGatherMenu::processN(const std::shared_ptr<NMessage> &message)
	{		

	}

} //namespace AI 
