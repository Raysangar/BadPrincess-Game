#include "LAProtectKnight.h"

#include "AI/Server.h"
#include "GameManager.h"

#include "Logic\Entity\Entity.h"
#include "Logic/Entity/Components/CombatStats.h"
#include "Logic/Entity/Components/KnightState.h"
#include "Logic/Entity/Components/StatsModifier.h"

#include "Logic/RouteCalculationMessages.h"
#include "Logic/UnitMessages.h"

#include "EntityManager.h"

#define DISTANCE 50


namespace AI
{
	CLatentAction::LAStatus CLAProtectKnight::OnStart()
	{	
		
		_currentEnemy = (static_cast<Logic::CCombatStats*>(_entity->getComponent("CCombatStats")))->getCurrentEnemy();
		_positioned = false;
		_positionAssigned = false;
		_relativePos =  Vector3(0.0f, 0.0f, 0.0f);

		sendAnimationMessage("Walk");

		if(!_currentEnemy->isEnemy())
			return FAIL;

		sendRouteMessage( _currentEnemy->getPosition(), true);


		return RUNNING;
	}

	CLatentAction::LAStatus CLAProtectKnight::OnRun(unsigned int msecs) 
	{
		if(_positionAssigned)
		{
			sendRouteMessage( _currentEnemy->getPosition() + _relativePos, true);
		}
		else if(!_positioned)
		{
			if(_entity->getPosition().distance(_currentEnemy->getPosition()) > DISTANCE)
			{
				sendRouteMessage( _currentEnemy->getPosition(), true);
			}
			else
			{
				_relativePos = AI::CServer::getSingletonPtr()->getGameManager()->calculatePosition(_entity);
				_positionAssigned = true;
			}
		}
		
		return RUNNING;
	}

	void CLAProtectKnight::OnStop()
	{
	}

	//---------------------------------------------------------------

	CLatentAction::LAStatus CLAProtectKnight::OnAbort() 
	{
		// Cuando se aborta se queda en estado terminado con fallo
		return FAIL;
	}

	//---------------------------------------------------------------

	bool CLAProtectKnight::acceptN(const std::shared_ptr<NMessage> &message)
	{		
		return (message->type.compare("AvatarFinishedRoute") == 0);
	}

	//---------------------------------------------------------------

	void CLAProtectKnight::processN(const std::shared_ptr<NMessage> &message)
	{		

		if(message->type.compare("AvatarFinishedRoute") == 0)
		{
			if(!_positioned && _positionAssigned)
			{
				_positioned = true;
				_positionAssigned = false;

				auto mMS = std::make_shared<ModifyStat>();
				mMS->modID = "KidnapPrincess";
				mMS->modType = Logic::ModificationType::speedMod;
				mMS->percentage = -50;
				mMS->durationMsecs = 0;
				_entity->emitMessageN(mMS);

				_spawnPoint = AI::CServer::getSingletonPtr()->getEntityManager()->
					getClosestEntity(CEntityManager::EntityType::SpawnPoint, _entity->getPosition())->getPosition();

				sendRouteMessage( _spawnPoint + _relativePos, true);
			}
			else
			{
				this->finish(true);
			}
		}
	}

	//---------------------------------------------------------------
	
	void CLAProtectKnight::sendRouteMessage(Vector3 target,bool recalculateRoute)
	{		
		auto m = std::make_shared<AvatarRouteTo>();
		m->target = target;
		m->calculateNewRoute = recalculateRoute;
		_entity->emitMessageN(m);
//		std::cout<<"CHASE ENEMY send message AvatarRouteTo"<<std::endl;
	}
	

} //namespace AI