/**
@file UnselectEntitiesAction.cpp

Action that unselect any selected unit.

@see UnselectEntitiesAction

@author Ivan Leon
@date May 2015

*/
#include "UnselectEntitiesAction.h"
#include "GUI/Server.h"
#include "GUI/PlayerController.h"

#include "Logic\Entity\Entity.h"
#include "Logic/Server.h"
#include "Logic/Maps/Map.h"
#include "Logic\CameraMessages.h"


namespace AI {
	IActionStatus CUnselectEntitiesAction::onStart(){

		GUI::CServer::getSingletonPtr()->getPlayerController()->unselectUnits();

		return IActionStatus::OnStart;
	};

}