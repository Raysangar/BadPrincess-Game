/**
@file KnightState.cpp
*/

#include <algorithm>

#include "KnightState.h"

#include "AI/Server.h"
#include "AI/GameManager.h"
#include "AI/EntityManager.h"

#include "Logic/CombatMessages.h"
#include "Logic/EntityMessages.h"
#include "Logic/PrincessMessages.h"
#include "Logic/EnemyMessages.h"
#include "Logic/UnitMessages.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Components/CombatStats.h"
#include "Logic/Maps/Map.h"

#include "Map/MapEntity.h"

namespace Logic 
{
	IMP_FACTORY(CKnightState);

	//---------------------------------------------------------	

	bool CKnightState::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		return true;

	} 

	//---------------------------------------------------------

	CKnightState::~CKnightState()
	{
	}

	void CKnightState::awake()
	{
		_combatComponent = static_cast<Logic::CCombatStats*>(_entity->getComponent("CCombatStats"));

		_objetive = nullptr;

		_princess = nullptr;

		_knight = AI::CServer::getSingletonPtr()->getGameManager()->getKnight();

		_enemyList.clear();

		_enemiesAttacking.clear();

		_wellList.clear();
	}

	//---------------------------------------------------------

	void CKnightState::deactivate()
	{
	}

	//---------------------------------------------------------

	void CKnightState::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);

		if(_objetive == nullptr)
		{
			_objetive = getCurrentEnemy();
			_combatComponent->setCurrentEnemy(_objetive);
		}
	}

	//---------------------------------------------------------

	bool CKnightState::acceptN(const std::shared_ptr<NMessage> &message)
	{
		return (message->type.compare("DamageMessage") == 0) || (message->type.compare("EntityDestroying") == 0) || (message->type.compare("EntityDying") == 0)
			|| (message->type.compare("PrincessTaken") == 0) || (message->type.compare("PrincessKidnapped") == 0) || (message->type.compare("GoAttack") == 0)
			|| (message->type.compare("PlayerSeen") == 0);
	}

	//---------------------------------------------------------

	void CKnightState::processN(const std::shared_ptr<NMessage> &message)
	{		
		if (message->type.compare("DamageMessage") == 0) {
			auto damageMessage = std::static_pointer_cast<DamageMessage>(message);
			CEntity *ent = damageMessage->sender;
			bool found = (std::find(_enemyList.begin(), _enemyList.end(), ent) != _enemyList.end());
			if(ent && ent->getType().compare("Princess") != 0 && !found)
				addEnemy(ent);
			else if(_objetive->getType() == "Well" && ent->getType().compare("Princess") == 0)
			{
				_objetive = ent;
				_combatComponent->setCurrentEnemy(_objetive);
			}

			if(_knight != nullptr)
			{
				_objetive = getCurrentEnemy();
				_combatComponent->setCurrentEnemy(_objetive);
				AI::CServer::getSingletonPtr()->getGameManager()->removeKnight(_entity);

				auto mMS = std::make_shared<RestoreStat>();
				mMS->modID = "KidnapPrincess";
				_entity->emitMessageN(mMS);

				auto m = std::make_shared<KnightActivate>();
				_entity->emitMessageN(m);
			}
			if(_princess != nullptr)
			{
				bool found = (std::find(_enemiesAttacking.begin(), _enemiesAttacking.end(), ent) != _enemiesAttacking.end());
				if(!found)
				{
					if(AI::CServer::getSingletonPtr()->getGameManager()->protectKnight(ent))
						_enemiesAttacking.push_back(ent);
				}
			}
		}
		else if(message->type.compare("PrincessTaken") == 0 && _knight == nullptr)
		{
			auto m = std::static_pointer_cast<PrincessTaken>(message);

			_princess = m->princess;

			_enemyList.clear();

			_enemiesAttacking.clear();

			_wellList.clear();
		}
		else if(message->type.compare("PrincessKidnapped") == 0 && _knight == nullptr)
		{
			auto m = std::static_pointer_cast<PrincessKidnapped>(message);
			if(m->sender != _entity && _princess == nullptr)
			{
				_knight = m->sender;

				_enemyList.clear();
				_enemiesAttacking.clear();
				_wellList.clear();

				_objetive = _knight;
				_combatComponent->setCurrentEnemy(_knight);
			}
		}
		else if(message->type.compare("EntityDying") == 0)
		{
			if(_princess != nullptr)
			{
				auto mPL = std::make_shared<PrincessLeft>();
				_princess->emitMessageN(mPL);
				_princess = nullptr;
				AI::CServer::getSingletonPtr()->getGameManager()->unregisterPrincessKidnapped();
			}
			if(_knight != nullptr)
			{
				AI::CServer::getSingletonPtr()->getGameManager()->removeKnight(_entity);
			}
		}
		else if (message->type.compare("EntityDestroying") == 0)
		{
			auto dieMessage = std::static_pointer_cast<EntityDestroying>(message);
			CEntity *ent = dieMessage->entity;
			if(ent->isPlayer() || ent->getType() == "Well")
			{
				bool found = (std::find(_enemyList.begin(), _enemyList.end(), ent) != _enemyList.end());
				if(found)
					removeEnemy(ent);

				found = (std::find(_wellList.begin(), _wellList.end(), ent) != _wellList.end());
				if(found)
					removeEnemy(ent);

				found = (std::find(_enemiesAttacking.begin(), _enemiesAttacking.end(), ent) != _enemiesAttacking.end());
				if(found)
					_enemiesAttacking.remove(ent);
			
			}
			if(ent == _knight)
			{
				_knight = nullptr;
				_objetive = getCurrentEnemy();
				_combatComponent->setCurrentEnemy(_objetive);
				auto m = std::make_shared<KnightActivate>();
				_entity->emitMessageN(m);
			}
			
		}
		else if (message->type.compare("GoAttack") == 0)
		{
			auto damageMessage = std::static_pointer_cast<GoAttack>(message);
			CEntity *ent = damageMessage->ent;
			bool found = (std::find(_enemyList.begin(), _enemyList.end(), ent) != _enemyList.end());
			if(ent->getType().compare("Princess") != 0 && !found)
				addEnemy(ent);

			auto mMS = std::make_shared<RestoreStat>();
			mMS->modID = "KidnapPrincess";
			_entity->emitMessageN(mMS);

			_objetive = getCurrentEnemy();
			_combatComponent->setCurrentEnemy(_objetive);
			auto m = std::make_shared<KnightActivate>();
			_entity->emitMessageN(m);
		}
		else if (message->type.compare("PlayerSeen") == 0)
		{
			if(_knight == nullptr)
			{
				auto message2 = std::static_pointer_cast<PlayerSeen>(message);
				CEntity *ent = message2->ent;
				bool found = (std::find(_wellList.begin(), _wellList.end(), ent) != _wellList.end());
				if(ent->getType().compare("Princess") != 0 && !found)
					addEnemy(ent);
			}
			
		}
	}

	//---------------------------------------------------------

	void CKnightState::addEnemy(CEntity* enemy)
	{
		if(enemy->getType() == "Well")
			_wellList.push_back(enemy);
		else
			_enemyList.push_back(enemy);

		if(getCurrentEnemy() != _combatComponent->getCurrentEnemy())
		{
			_objetive = getCurrentEnemy();
			_combatComponent->setCurrentEnemy(_objetive);
			
		}

	}

	//---------------------------------------------------------

	void CKnightState::removeEnemy(CEntity* enemy)
	{
		if(enemy->getType() == "Well")
			_wellList.remove(enemy);
		else
			_enemyList.remove(enemy);

		if(getCurrentEnemy() != _combatComponent->getCurrentEnemy())
		{
			_objetive = getCurrentEnemy();
			_combatComponent->setCurrentEnemy(_objetive);
		}
	}

	//---------------------------------------------------------

	CEntity* CKnightState::getCurrentEnemy()
	{
		if(_enemyList.size() > 0)
			return _enemyList.front();
		else if (_wellList.size() > 0)
			return _wellList.front();
		else
		{
			if(_knight != nullptr)
				return _knight;
			else
				return AI::CServer::getSingletonPtr()->getEntityManager()
					->getClosestEntity(AI::CEntityManager::EntityType::Princess, _entity->getPosition());
		}
			
	}
}