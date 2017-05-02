/**
@file SetClickIsEnabledAction.cpp

Action that set if some type of entity can or cannot be clicked by the button especified.

@see SetClickIsEnabledAction

@author Ivan Leon
@date May 2015

*/
#include "SetClickIsEnabledAction.h"
#include "GUI/Server.h"
#include "GUI/PlayerController.h"

#include "Logic\Entity\Entity.h"
#include "Logic/Server.h"
#include "Logic/Maps/Map.h"
#include "Logic\CameraMessages.h"


namespace AI {
	IActionStatus CSetClickIsEnabledAction::onStart(){

		GUI::CServer::getSingletonPtr()->getPlayerController()->setTypeIsClickable(_button,_type,_clickable);

		return IActionStatus::OnStart;
	};

}