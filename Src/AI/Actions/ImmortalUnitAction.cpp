/**
@file ImmortalUnitAction.cpp
*/
#include "ImmortalUnitAction.h"

#include "Logic/Entity/Components/Life.h"
#include "Logic\Entity\Entity.h"
#include "Logic/Server.h"
#include "Logic/Maps/Map.h"


namespace AI {
	IActionStatus CImmortalUnitAction::onStart(){

		Logic::CEntity *e = Logic::CServer::getSingletonPtr()->getMap()->getEntityByName(_entityName);

		if(e!=nullptr)
		{
			static_cast<Logic::CLife*>(e->getComponent("CLife"))->setInvincible(_immortal);
			return IActionStatus::OnStart;
		}else
			return IActionStatus::OnEnd;
	};

}