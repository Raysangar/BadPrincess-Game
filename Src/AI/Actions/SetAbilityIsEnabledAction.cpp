/**
@file SetAbilityIsEnabledAction.cpp

Action that set if certain ability of some type of unit is enabled or disabled

@see SetAbilityIsEnabledAction

@author Ivan Leon
@date May 2015

*/
#include "SetAbilityIsEnabledAction.h"
#include "GUI/Server.h"
#include "GUI/ActiveUnitController.h"

#include "Logic\Entity\Entity.h"
#include "Logic/Server.h"
#include "Logic/Maps/Map.h"
#include "Logic\CameraMessages.h"


namespace AI {
	IActionStatus CSetAbilityIsEnabledAction::onStart(){

		GUI::CServer::getSingletonPtr()->getActiveUnitController()->setAbilityUsability(_type,_ability,_enabled);

		return IActionStatus::OnStart;
	};

}