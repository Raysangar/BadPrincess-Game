#include "LAGameOver.h"

#include "AI/Server.h"
#include "AI/GameManager.h"

#include "Logic/Entity/Components/CombatStats.h"
#include "Logic/Entity/Entity.h"

#include "Logic/Entity/Entity.h"
#include "EntityManager.h"

#include "Logic/RouteCalculationMessages.h"

namespace AI
{
	CLatentAction::LAStatus CLAGameOver::OnStart()
	{	
		//Para que no salga el gameover si se bloquea por A*
		Vector3 spawnPoint = AI::CServer::getSingletonPtr()->getEntityManager()->
			getClosestEntity(CEntityManager::EntityType::SpawnPoint, _entity->getPosition())->getPosition();

		if(_entity->getPosition().distance(spawnPoint) < 20)
			AI::CServer::getSingletonPtr()->getGameManager()->endGame(false);
		else
		{
			auto mRT = std::make_shared<AvatarRouteTo>();
			mRT->target = spawnPoint;
			mRT->calculateNewRoute = true;
			_entity->emitMessageN(mRT);
		}

		return SUCCESS;
	}

	CLatentAction::LAStatus CLAGameOver::OnRun(unsigned int msecs) 
	{
		return RUNNING;
	}

	void CLAGameOver::OnStop()
	{
	}

	//---------------------------------------------------------------

	CLatentAction::LAStatus CLAGameOver::OnAbort() 
	{
		// Cuando se aborta se queda en estado terminado con fallo
		return FAIL;
	}

	//---------------------------------------------------------------

	bool CLAGameOver::acceptN(const std::shared_ptr<NMessage> &message)
	{		
		return true;
	}

	//---------------------------------------------------------------

	void CLAGameOver::processN(const std::shared_ptr<NMessage> &message)
	{		
		
	}
} //namespace AI