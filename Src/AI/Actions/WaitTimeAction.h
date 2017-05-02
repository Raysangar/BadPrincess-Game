/**
@file WaitTimeAction.h

Action that wait a fixed time (could be used as Corutine)

@see WaitTimeAction

@author Guidi Giacomo
@date March 2015

*/

#pragma once

#ifndef __AI_WaitTimeAction_H
#define __AI_WaitTimeAction_H

#include "AI/ActionNode.h"

namespace AI {


	class CWaitTimeAction:public IActionNode
	{
	public:
		CWaitTimeAction(unsigned int time):IActionNode("CWaitTimeAction"),_waitTime(time),_currentWaitTime(0){};

		virtual ~CWaitTimeAction(){};

		 virtual IActionStatus onStart();

		 virtual bool onCheck(unsigned int msecs);
	private:
		unsigned int _waitTime;
		unsigned int _currentWaitTime;
	};
} // namespace AI

#endif __AI_WaitTimeActionManager_H
