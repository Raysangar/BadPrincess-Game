/**
@file RemoveMessageAction.h

Action that show a message during a fixed time (NOT FINAL VERSION, WAIT TO BE ABLE TO SHOW CEGUI MESSAGE)

@see RemoveMessageAction

@author Guidi Giacomo
@date March 2015

*/

#pragma once

#ifndef __AI_RemoveMessageAction_H
#define __AI_RemoveMessageAction_H

#include "AI/ActionNode.h"

namespace AI {


	class CRemoveMessageAction:public IActionNode
	{
	public:
		CRemoveMessageAction(std::string id):IActionNode("CRemoveMessageAction"),
			_id(id){};

		virtual ~CRemoveMessageAction(){};

		 virtual IActionStatus onStart();

	private:
		std::string _id;
	};
} // namespace AI

#endif __AI_RemoveMessageActionManager_H
