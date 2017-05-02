/**
@file LockMouseAction.h

Action that set if the mouse is locked for any click

@see LockMouseAction

@author Ivan Leon
@date May 2015

*/

#pragma once

#ifndef __AI_LockMouseAction_H
#define __AI_LockMouseAction_H

#include "AI/ActionNode.h"


namespace AI {
	

	class CLockMouseAction: public IActionNode
	{
	public:
		CLockMouseAction(bool lock):IActionNode("CLockMouseAction"),_lock(lock){};

		virtual ~CLockMouseAction(){};

		virtual IActionStatus onStart();

	private:

		bool _lock;
	};


} // namespace AI

#endif __AI_LockMouseActionManager_H
