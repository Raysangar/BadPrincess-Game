#include "LAKidnappingPrincess.h"

#include "AI/Server.h"

#include "Logic/Entity/Components/CombatStats.h"
#include "Logic/Entity/Components/StatsModifier.h"

#include "Logic/RouteCalculationMessages.h"
#include "Logic/EnemyMessages.h"
#include "Logic/PrincessMessages.h"
#include "Logic/UnitMessages.h"

#include "Logic/Entity/Entity.h"
#include "EntityManager.h"
#include "GameManager.h"

namespace AI
{
	CLatentAction::LAStatus CLAKidnappingPrincess::OnStart()
	{	
		_currentEnemy = AI::CServer::getSingletonPtr()->getGameManager()->getPrincess();

		if(_currentEnemy == nullptr)
		{
			return FAIL;
		}

		auto mPT = std::make_shared<PrincessTaken>();
		mPT->raptor = _entity;
		mPT->princess = _currentEnemy;
		_currentEnemy->emitMessageN(mPT);
		_entity->emitMessageN(mPT);

		auto mPK = std::make_shared<PrincessKidnapped>();
		std::set<Logic::CEntity*> *_enemyUnitList = AI::CServer::getSingletonPtr()->getEntityManager()->getEnemiesList();
		_enemyUnitList->begin();
		for(std::set<Logic::CEntity*>::iterator it = _enemyUnitList->begin();it != _enemyUnitList->end(); it++)
		{
			if(*it != _entity)
				(*it)->emitMessageN(mPK, _entity->getComponent("CLife"));
		}
			

		auto mMS = std::make_shared<ModifyStat>();
		mMS->modID = "KidnapPrincess";
		mMS->modType = Logic::ModificationType::speedMod;
		mMS->percentage = -50;
		mMS->durationMsecs = 0;
		_entity->emitMessageN(mMS);

		Vector3 spawnPoint = AI::CServer::getSingletonPtr()->getEntityManager()->
			getClosestEntity(CEntityManager::EntityType::SpawnPoint, _entity->getPosition())->getPosition();

		auto mRT = std::make_shared<AvatarRouteTo>();
		mRT->target = spawnPoint;
		_entity->emitMessageN(mRT);

		return RUNNING;
	}

	CLatentAction::LAStatus CLAKidnappingPrincess::OnRun(unsigned int msecs) 
	{
		return RUNNING;
	}

	void CLAKidnappingPrincess::OnStop()
	{
	}

	//---------------------------------------------------------------

	CLatentAction::LAStatus CLAKidnappingPrincess::OnAbort() 
	{
		// Cuando se aborta se queda en estado terminado con fallo
		return FAIL;
	}

	//---------------------------------------------------------------

	bool CLAKidnappingPrincess::acceptN(const std::shared_ptr<NMessage> &message)
	{		
		return true;
	}

	//---------------------------------------------------------------

	void CLAKidnappingPrincess::processN(const std::shared_ptr<NMessage> &message)
	{		
		
	}
} //namespace AI