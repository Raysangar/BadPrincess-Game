#include "LAStunned.h"

#include "Logic\RouteCalculationMessages.h"
#include "Logic\Entity\Entity.h"
#include "Logic\PrincessMessages.h"
#include "Logic/MovementMessages.h"
#include "Logic/GraphicsMessages.h"
#include "Logic\Entity\Components\RouteTo.h"

namespace AI 
{


//////////////////////////////
//	Implementación de CLAStunned
//////////////////////////////

#define SPEED 3.0f

	//-------------------------------------------------------------------------

	CLatentAction::LAStatus CLAStunned::OnStart()
	{	
		auto stopMovementMessage = std::make_shared<AvatarInterruptMovement>();
		_entity->emitMessageN(stopMovementMessage);
		sendAnimationMessage("Stunned"); // Explotará aquí si se intenta aturdir a un Saqueador. Controlado.
		_stunned = false;
		_currentWaitTime = 0;
	
		return RUNNING;
	}

	//-------------------------------------------------------------------------

	void CLAStunned::OnStop()
	{
	}

	//-------------------------------------------------------------------------

	CLatentAction::LAStatus CLAStunned::OnRun(unsigned int msecs) 
	{
		if(_stunned == true)
		{
			_currentWaitTime += msecs;

			if(_currentWaitTime >= _waitTime)
			{
				auto stopParticleMessage = std::make_shared<StopParticle>();
				stopParticleMessage->particleName = "stunned";
				_entity->emitMessageN(stopParticleMessage);
				return SUCCESS;
			}
		}
		else if(_entity->getPosition().distance(_origin) <= _distance)
		{
			auto moveMessage = std::make_shared<AvatarMove>();
			moveMessage->movement = SPEED * _direction;
			_entity->emitMessageN(moveMessage);
		}
		else
		{
			_stunned = true;

			auto startParticleMessage = std::make_shared<PlayParticle>();
			startParticleMessage->particleName = "stunned";
			_entity->emitMessageN(startParticleMessage);
		}
		
		return RUNNING;
	}

	//-------------------------------------------------------------------------

	CLatentAction::LAStatus CLAStunned::OnAbort() 
	{
		// Cuando se aborta se queda en estado terminado con fallo
		return FAIL;
	}
	
	//-------------------------------------------------------------------------

	bool CLAStunned::acceptN(const std::shared_ptr<NMessage> &message)
	{
		return (message->type.compare("StunHability") == 0);
	}

	//-------------------------------------------------------------------------

	void CLAStunned::processN(const std::shared_ptr<NMessage> &message)
	{		
		if(message->type.compare("StunHability") == 0 )
		{
			std::shared_ptr<StunHability> m = std::static_pointer_cast<StunHability>(message);

			_direction = _entity->getPosition() - m->origin;
			_direction.normalise();
			_distance = m->distance;
			_waitTime = (unsigned int) m->time;
			_origin = m->origin;

			if(_stunned == true)
				_stunned = false;

			std::cout << _entity->getName() << " Stunned" << std::endl;

			//this->resume();
		}
	}


} //namespace AI 
