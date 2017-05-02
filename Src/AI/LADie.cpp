#include "LADie.h"

#include "Logic\Entity\Entity.h"
#include "Logic/AnimationMessages.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/EntityMessages.h"
#include "Logic/RouteCalculationMessages.h"


namespace AI 
{


//////////////////////////////
//	Implementación de CLADie
//////////////////////////////

	//-------------------------------------------------------------------------

	CLatentAction::LAStatus CLADie::OnStart()
	{	
		sendAnimationMessage("Death",false,false);

		auto m2 = std::make_shared<AvatarInterruptRoute>();
		_entity->emitMessageN(m2);

		return SUSPENDED;
	}

	//-------------------------------------------------------------------------

	void CLADie::OnStop()
	{

	}

	//-------------------------------------------------------------------------

	CLatentAction::LAStatus CLADie::OnRun(unsigned int msecs) 
	{
	
			return EXIT_SM_SUCCESS;
	}

	//-------------------------------------------------------------------------

	CLatentAction::LAStatus CLADie::OnAbort() 
	{
		// Cuando se aborta se queda en estado terminado con fallo
		return FAIL;
	}

	//-------------------------------------------------------------------------

	bool CLADie::acceptN(const std::shared_ptr<NMessage> &message)
	{
		return message->type.compare("AnimationFinished") == 0; 
	}

	//-------------------------------------------------------------------------

	void CLADie::processN(const std::shared_ptr<NMessage> &message)
	{	
		if (message->type.compare("AnimationFinished") == 0)
		{
			std::shared_ptr<AnimationFinished> m = std::static_pointer_cast<AnimationFinished>(message);
//			std::cout<<_entity->getName()<<"death AnimationFinished "<<std::endl;
			if(m->animationName=="Death")
			{	
				this->resume();
				Logic::CEntityFactory::getSingletonPtr()->deferredDeleteEntity(_entity);
			}
		}

	}


} //namespace AI 
