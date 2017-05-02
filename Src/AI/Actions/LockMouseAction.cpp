/**
@file LockMouseAction.cpp

Action that set if the mouse is locked for any click

@see LockMouseAction

@author Ivan Leon
@date May 2015

*/
#include "LockMouseAction.h"
#include "GUI/Server.h"
#include "GUI/PlayerController.h"

#include "Logic\Entity\Entity.h"
#include "Logic/Server.h"
#include "Logic/Maps/Map.h"
#include "Logic\CameraMessages.h"


namespace AI {
	IActionStatus CLockMouseAction::onStart(){

		GUI::CServer::getSingletonPtr()->getPlayerController()->setMouseIsLocked(_lock);

		return IActionStatus::OnStart;
	};

}