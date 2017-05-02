/**
@file CreateEntityAction.cpp

Action that set if certain ability of some type of unit is enabled or disabled

@see CreateEntityAction

@author Ivan Leon
@date May 2015

*/
#include "CreateEntityAction.h"
#include "GUI/Server.h"
#include "GUI/ActiveUnitController.h"
#include "GUI/StatisticsController.h"

#include "Logic\Entity\Entity.h"
#include "Logic/Server.h"
#include "Logic/Maps/Map.h"
#include "Logic/Entity/Components/SpawnPointComponent.h"

namespace AI {
	IActionStatus CCreateEntityAction::onStart(){

		Logic::CEntity *e = Logic::CServer::getSingletonPtr()->getMap()->getEntityByName(_entityName);
		Logic::CEntity *sp = Logic::CServer::getSingletonPtr()->getMap()->getEntityByName(_spawnPointName);
		if(sp && e)
		{
			Logic::CSpawnPointComponent *c=static_cast<Logic::CSpawnPointComponent*>( sp->getComponent("CSpawnPointComponent"));
			if(c!=nullptr)
				c->createEntity(e);
			if(e->isEnemy())
				GUI::CServer::getSingletonPtr()->getStatisticsController()->addEntityCounter(e->getType(),1);
		}
		return IActionStatus::OnStart;
	};

}