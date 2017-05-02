/**
@file AssassinState.cpp
*/

#include <algorithm>

#include "AssassinState.h"

#include "AI/Server.h"
#include "AI/EntityManager.h"

#include "Logic/CombatMessages.h"
#include "Logic/EntityMessages.h"
#include "Logic/PrincessMessages.h"
#include "Logic/EnemyMessages.h"
#include "Logic/UnitMessages.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Components/CombatStats.h"
#include "Logic/Entity/Components/AssassinCamouflage.h"
#include "Logic/Entity/Components/Life.h"
#include "Logic/Maps/Map.h"

#include "Map/MapEntity.h"
#include "Logic/Server.h"

namespace Logic 
{
	IMP_FACTORY(CAssassinState);

	//---------------------------------------------------------	

	bool CAssassinState::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		return true;

	} 

	//---------------------------------------------------------

	CAssassinState::~CAssassinState()
	{
	}

	void CAssassinState::awake()
	{
		combatComponent = static_cast<CCombatStats*>(_entity->getComponent("CCombatStats"));
		camouflageComponent = static_cast<CAssassinCamouflage*>(_entity->getComponent("CAssassinCamouflage"));
		princess = CServer::getSingletonPtr()->getMap()->getEntityByName("Princess");
		enemyList = AI::CServer::getSingletonPtr()->getEntityManager()->getAlliesList();
		assassinIsActive = true;
	}

	//---------------------------------------------------------

	void CAssassinState::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);
		if (assassinIsActive == false) return;
		if(objetive == nullptr || combatComponent->getCurrentEnemy() == nullptr)
			selectNewObjective();
	}

	//---------------------------------------------------------

	bool CAssassinState::acceptN(const std::shared_ptr<NMessage> &message)
	{
		return (message->type == "EntityDestroying") || (message->type == "PrincessHasFallen")
			|| (message->type == "ActivateAssassin") || (message->type == "DamageMessage") 
			|| (message->type == "PrincessIsFree") || (message->type == "Attacking");
	}

	//---------------------------------------------------------

	void CAssassinState::processN(const std::shared_ptr<NMessage> &message)
	{		
		if (message->type == "ActivateAssassin")
			assassinIsActive = true;
		else if (message->type == "DamageMessage")
		{
			if (!camouflageComponent->assassinIsAttackin())
				attack(message->sender);
		}
		else if (message->type == "Attacking")
			attacking = std::static_pointer_cast<Attacking>(message)->attacking;
		else if (message->type == "PrincessIsFree" && !attacking)
			attack(princess);
		else
		{
			CEntityPtr fallenEntity = (message->type == "PrincessHasFallen") ? princess : 
				std::static_pointer_cast<EntityDestroying>(message)->entity;

			if(fallenEntity == objetive)
			{
				lastVictim = objetive;
				objetive = nullptr;
				attacking = false;
				camouflageComponent->setAssassinIsAttackin(false);
			}
		}
		
	}

	//---------------------------------------------------------

	CEntity* CAssassinState::getCurrentEnemy()
	{
		if (princessHasFallen())
			return getClosestSuitableEnemy();
		else
			return princess;	
	}

	//---------------------------------------------------------

	bool CAssassinState::princessHasFallen()
	{
		return  ((CLife*) princess->getComponent("CLife"))->getCurrentLife() <= 0;
	}

	//---------------------------------------------------------

	CEntityPtr CAssassinState::getClosestSuitableEnemy()
	{
		CEntityPtr newObjective = nullptr;
		float minDistance = std::numeric_limits<float>::max();
		for (std::list<std::string>::iterator it = priorityEnemyTypeList.begin(); 
			it != priorityEnemyTypeList.end() && newObjective == nullptr; ++it)
		{
			for (CEntityPtr enemy : *enemyList)
				if (enemy != lastVictim && (*it) == enemy->getType() && _entity->getPosition().distance(enemy->getPosition()) < minDistance)
					newObjective = enemy;
		}
		return newObjective;
	}

	//---------------------------------------------------------

	void CAssassinState::selectNewObjective()
	{
		attack(getCurrentEnemy());
	}

	//---------------------------------------------------------

	void CAssassinState::attack(CEntityPtr newObjective)
	{
		objetive = newObjective;
		combatComponent->setCurrentEnemy(objetive);
		if (objetive != nullptr) 
			_entity->emitMessageN(std::make_shared<NewTarget>());
		else
			_entity->emitMessageN(std::make_shared<MissionComplete>());
	}
}