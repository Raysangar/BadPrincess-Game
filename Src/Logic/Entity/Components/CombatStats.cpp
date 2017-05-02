/**
@file CombatStats.cpp
*/
#include "CombatStats.h"
#include "SteeringMovement.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include "Logic/CombatMessages.h"
#include "Logic/MovementMessages.h"
#include "Logic/EntityMessages.h"

namespace Logic 
{
	IMP_FACTORY(CCombatStats);

	//---------------------------------------------------------	

	bool CCombatStats::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		assert(entityInfo->hasAttribute("attack_range"));
		_attackRange = entityInfo->getFloatAttribute("attack_range");

		assert(entityInfo->hasAttribute("attack_damage"));
		_attackDamage = entityInfo->getIntAttribute("attack_damage");

		assert(entityInfo->hasAttribute("time_between_attacks"));
		_timeBetweenAttacks = entityInfo->getIntAttribute("time_between_attacks");

		assert(entityInfo->hasAttribute("attack_animation"));
		_attackAnimation = entityInfo->getStringAttribute("attack_animation");		

		return true;
	} 

	//---------------------------------------------------------

	void CCombatStats::awake()
	{
		_steeringComponent = static_cast<Logic::CSteeringMovement*>(_entity->getComponent("CSteeringMovement"));
		_currentEnemy=nullptr;
	}

	//---------------------------------------------------------

	bool CCombatStats::acceptN(const std::shared_ptr<NMessage> &message)
	{
		// La acción no acepta mensajes
		return (message->type.compare("AvatarMoveToEntity") == 0) || (message->type.compare("DamageMessage") == 0) ;
	}

	//---------------------------------------------------------

	void CCombatStats::processN(const std::shared_ptr<NMessage> &message)
	{		
		if(message->type.compare("AvatarMoveToEntity") == 0)
		{			
			auto m = std::static_pointer_cast<AvatarMoveToEntity>(message);
			if((_entity->isPlayer() && m->target->isEnemy()) || (_entity->isEnemy() && m->target->isPlayer()) )
			{
				_currentEnemy = m->target;

				auto m2 = std::make_shared<AttackEnemy>();
				_entity->emitMessageN(m2);
			}
		}
		else if(message->type.compare("DamageMessage") == 0)
		{		
			if( _currentEnemy == NULL && (_entity->isEnemy() || _steeringComponent->getCurrentSpeed() == Vector3::ZERO))
			{
				auto m = std::static_pointer_cast<DamageMessage>(message);
				_currentEnemy = m->sender;

				auto m2 = std::make_shared<AttackEnemy>();
				_entity->emitMessageN(m2);
			}
			
		}
	}	

	
}


