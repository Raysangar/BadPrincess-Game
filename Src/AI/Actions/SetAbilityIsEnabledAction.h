/**
@file SetAbilityIsEnabledAction.h

Action that set if certain ability of some type of unit is enabled or disabled

@see SetAbilityIsEnabledAction

@author Ivan Leon
@date May 2015

*/

#pragma once

#ifndef __AI_SetAbilityIsEnabledAction_H
#define __AI_SetAbilityIsEnabledAction_H

#include "AI/ActionNode.h"


namespace AI {
	

	class CSetAbilityIsEnabledAction: public IActionNode
	{
	public:
		CSetAbilityIsEnabledAction(std::string type, std::string ability, bool enabled):IActionNode("CSetAbilityIsEnabledAction"),_type(type),_ability(ability),_enabled(enabled){};

		virtual ~CSetAbilityIsEnabledAction(){};

		virtual IActionStatus onStart();

	private:

		std::string _type;
		std::string _ability;
		bool _enabled;
	};


} // namespace AI

#endif __AI_SetAbilityIsEnabledActionManager_H
