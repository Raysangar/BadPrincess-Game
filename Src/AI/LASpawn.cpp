#include "LASpawn.h"

#include "Logic/Entity/Entity.h"
#include "Logic\Entity\Components\AnimatedGraphics.h"

#include "Graphics\AnimatedEntity.h"


namespace AI 
{


//////////////////////////////
//	Implementación de CLASpawn
//////////////////////////////

	//-------------------------------------------------------------------------

	CLatentAction::LAStatus CLASpawn::OnStart()
	{	
		
		_animatedGraphicsComponent = static_cast<Logic::CAnimatedGraphics*>(_entity->getComponent("CAnimatedGraphics"));
		sendAnimationMessage("JumpOutFromWell",false,false);
		return RUNNING;
	}

	//-------------------------------------------------------------------------

	void CLASpawn::OnStop()
	{

	}

	//-------------------------------------------------------------------------

	CLatentAction::LAStatus CLASpawn::OnRun(unsigned int msecs) 
	{		
		if(_animatedGraphicsComponent->getAnimatedGraphicsEntity()->animationIsFinished())
		{
			return SUCCESS;
		}

		return RUNNING;
	}

	//-------------------------------------------------------------------------

	CLatentAction::LAStatus CLASpawn::OnAbort() 
	{
		// Cuando se aborta se queda en estado terminado con fallo
		return FAIL;
	}

	//-------------------------------------------------------------------------

	bool CLASpawn::acceptN(const std::shared_ptr<NMessage> &message)
	{
		// La acción no acepta mensajes
		return false; //cambiar por AvatarFinishedRoute cuando esté implementado
	}

	//-------------------------------------------------------------------------

	void CLASpawn::processN(const std::shared_ptr<NMessage> &message)
	{	
		

	}


} //namespace AI 
