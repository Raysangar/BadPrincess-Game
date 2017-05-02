/**
@file SetClickIsEnabledAction.h

Action that set if some type of entity can or cannot be clicked by the button especified.

@see SetClickIsEnabledAction

@author Ivan Leon
@date May 2015
*/

#pragma once

#ifndef __AI_SetClickIsEnabledAction_H
#define __AI_SetClickIsEnabledAction_H

#include "AI/ActionNode.h"


namespace AI {
	

	class CSetClickIsEnabledAction: public IActionNode
	{
	public:
		CSetClickIsEnabledAction(int button, std::string type, bool clickable):IActionNode("CSetClickIsEnabledAction"),_button(button),_type(type),_clickable(clickable){};

		virtual ~CSetClickIsEnabledAction(){};

		virtual IActionStatus onStart();

	private:

		std::string _type;
		int _button;
		bool _clickable;
	};


} // namespace AI

#endif __AI_SetClickIsEnabledActionManager_H
