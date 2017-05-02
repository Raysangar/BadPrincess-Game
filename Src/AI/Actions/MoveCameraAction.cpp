/**
@file ActionNode.cpp

Clase istancia de una oleada.

@see ActionNode

@author Guidi Giacomo 
@date March 2015

*/
#include "MoveCameraAction.h"

#include "Logic\Entity\Entity.h"
#include "Logic/Server.h"
#include "Logic/Maps/Map.h"
#include "Logic\CameraMessages.h"


namespace AI {
	IActionStatus CMoveCameraAction::onStart(){
		Logic::CEntity *e = Logic::CServer::getSingletonPtr()->getMap()->getEntityByName(_entityName);
		Logic::CEntity *camera = Logic::CServer::getSingletonPtr()->getMap()->getEntityByName("Camera");
		if(e!=nullptr && camera!=nullptr)
		{
			auto m = std::make_shared<CameraMoveTo>();
			m->position = e->getPosition();
			m->position.y = camera->getPosition().y;
			camera->emitMessageN(m);
			return IActionStatus::OnStart;
		}else
			return IActionStatus::OnEnd;
	};

}