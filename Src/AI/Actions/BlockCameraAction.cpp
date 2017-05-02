/**
@file BlockCameraAction.cpp
*/
#include "BlockCameraAction.h"

#include "Logic/Entity/Components/Camera.h"
#include "Logic\Entity\Entity.h"
#include "Logic/Server.h"
#include "Logic/Maps/Map.h"
#include "Logic\CameraMessages.h"


namespace AI {
	IActionStatus CBlockCameraAction::onStart(){
		Logic::CEntity *camera = Logic::CServer::getSingletonPtr()->getMap()->getEntityByName("Camera");
		if(camera!=nullptr)
		{
			static_cast<Logic::CCamera*>(camera->getComponent("CCamera"))->setCameraBlock(_cameraBlocked);
			return IActionStatus::OnStart;
		}else
			return IActionStatus::OnEnd;
	};

}