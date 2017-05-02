/**
@file ChangeAttackAction.cpp
*/
#include "ChangeAttackAction.h"

#include "Logic/Entity/Components/CombatStats.h"
#include "Logic\Entity\Entity.h"
#include "Logic/Server.h"
#include "Logic/Maps/Map.h"


namespace AI {
	IActionStatus CChangeAttackAction::onStart(){

		Logic::CEntity *e = Logic::CServer::getSingletonPtr()->getMap()->getEntityByName(_entityName);

		if(e!=nullptr)
		{
			static_cast<Logic::CCombatStats*>(e->getComponent("CCombatStats"))->setAttackDamage((int)_attack);
			return IActionStatus::OnStart;
		}else
			return IActionStatus::OnEnd;
	};

}