/**
@file DeleteEntityAction.cpp
*/
#include "DeleteEntityAction.h"

#include "Logic/Entity/Components/Life.h"
#include "Logic\Entity\Entity.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Server.h"
#include "Logic/Maps/Map.h"


namespace AI {
	IActionStatus CDeleteEntityAction::onStart(){

		Logic::CEntity *e = Logic::CServer::getSingletonPtr()->getMap()->getEntityByName(_entityName);

		Logic::CEntityFactory::getSingletonPtr()->deferredDeleteEntity(e);

		return IActionStatus::OnStart;
	};

}