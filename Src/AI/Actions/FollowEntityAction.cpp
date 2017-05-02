/**
@file FollowEntityAction.cpp
*/
#include "FollowEntityAction.h"

#include "Logic/Entity/Components/Camera.h"
#include "Logic\Entity\Entity.h"
#include "Logic/Server.h"
#include "Logic/Maps/Map.h"


namespace AI {
	IActionStatus CFollowEntityAction::onStart(){

		Logic::CEntity *e = Logic::CServer::getSingletonPtr()->getMap()->getEntityByName(_entityName);
		Logic::CEntity *camera = Logic::CServer::getSingletonPtr()->getMap()->getEntityByName("Camera");

		if(camera!=nullptr)
		{
			static_cast<Logic::CCamera*>(camera->getComponent("CCamera"))->setEntityFollowed(e);
			return IActionStatus::OnStart;
		}else
			return IActionStatus::OnEnd;
	};

}