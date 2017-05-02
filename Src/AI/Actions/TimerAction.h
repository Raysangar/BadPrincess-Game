/**
@file TimerAction.h

Action that set if certain ability of some type of unit is enabled or disabled

@see TimerAction

@author Ivan Leon
@date May 2015

*/

#pragma once

#ifndef __AI_TimerAction_H
#define __AI_TimerAction_H

#include "AI/ActionNode.h"


namespace AI {
	

	class CTimerAction: public IActionNode
	{
	public:
		CTimerAction(std::string color, int time):IActionNode("CTimerAction"),_maxTime(time),_lastTimeWritten(""),_color(color){};

		virtual ~CTimerAction(){};

		virtual IActionStatus onStart();

		virtual bool onCheck(unsigned int msecs);

		virtual IActionStatus onEnd();
	private:

		std::string convertTimeToString(int msecs);

		int _maxTime;

		std::string _lastTimeWritten;

		std::string _color;
	};


} // namespace AI

#endif __AI_TimerActionManager_H
