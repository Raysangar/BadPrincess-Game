#include "LAIdleMenu.h"
#include "Logic/RouteCalculationMessages.h"
#include "Logic/EntityMessages.h"
#include "Logic/Entity/Entity.h"

#define MOVING_DISTANCE 30
namespace AI 
{


//////////////////////////////
//	Implementación de CLAIdleMenu
//////////////////////////////

	//-------------------------------------------------------------------------

	CLatentAction::LAStatus CLAIdleMenu::OnStart()
	{	
		//std::cout << "IDLELING" << std::endl;
		std::cout << _entity->getPosition() << std::endl;
		sendAnimationMessage("Idle", _entity->getType() == "PrincessMenu"? true : false );
		_idleNum = 1;

		return RUNNING;
	}

	//-------------------------------------------------------------------------

	void CLAIdleMenu::OnStop()
	{
	}

	//-------------------------------------------------------------------------

	CLatentAction::LAStatus CLAIdleMenu::OnRun(unsigned int msecs) 
	{


		return RUNNING;
	}

	//-------------------------------------------------------------------------

	CLatentAction::LAStatus CLAIdleMenu::OnAbort() 
	{
		// Cuando se aborta se queda en estado terminado con fallo
		return FAIL;
	}
	
	//-------------------------------------------------------------------------

	bool CLAIdleMenu::acceptN(const std::shared_ptr<NMessage> &message)
	{
		
		return (message->type == "AnimationFinished");
	}

	//-------------------------------------------------------------------------

	void CLAIdleMenu::processN(const std::shared_ptr<NMessage> &message)
	{		
		if  (message->type == "AnimationFinished")
		{
			if(_idleNum < 3)
			{
				++_idleNum;
				sendAnimationMessage("Idle" + std::to_string(_idleNum),false);
			}
			else
			{
				Ogre::Vector3 direction = Math::getDirection(_entity->getOrientation());
				direction.normalise();
				direction = Math::getPerpendicular(direction);
				Ogre::Vector3 pos = _entity->getPosition() + direction * MOVING_DISTANCE;

				auto m=std::make_shared<AvatarRouteTo>();
				m->calculateNewRoute = true;
				m->target = pos;
				_entity->emitMessageN(m);

			}
		}
	}


} //namespace AI 
