/**
@file TimerAction.cpp

Action that set if certain ability of some type of unit is enabled or disabled

@see TimerAction

@author Ivan Leon
@date May 2015

*/
#include "TimerAction.h"
#include "GUI/Server.h"
#include "GUI/StatisticsController.h"

namespace AI {
	IActionStatus CTimerAction::onStart(){

		_lastTimeWritten = convertTimeToString(_maxTime);
		GUI::CServer::getSingletonPtr()->getStatisticsController()->setTimerTextColor(_color);
		GUI::CServer::getSingletonPtr()->getStatisticsController()->setTimerText(_lastTimeWritten);
		GUI::CServer::getSingletonPtr()->getStatisticsController()->setTimerVisibility(true);

		return IActionStatus::OnStart;
	}

	bool CTimerAction::onCheck(unsigned int msecs)
	{
		_maxTime-=msecs;
		if(_maxTime>0)
		{
			std::string currentTime = convertTimeToString(_maxTime);
			if(_lastTimeWritten.compare(currentTime)!=0)
			{
				_lastTimeWritten = currentTime;
				GUI::CServer::getSingletonPtr()->getStatisticsController()->setTimerText(_lastTimeWritten);
			}
			return false;
		}else
			return true;
	}

	std::string CTimerAction::convertTimeToString(int millisecond)
	{
		unsigned int secs = (unsigned int) millisecond/1000;
		unsigned int min = (unsigned int) secs / 60;
		secs -= min*60;
		std::string zero = secs >= 10 ? "" : "0";
		return std::to_string(min)+" : " + zero + std::to_string(secs);
	}

	IActionStatus CTimerAction::onEnd(){
		GUI::CServer::getSingletonPtr()->getStatisticsController()->setTimerVisibility(false);
		return IActionStatus::Ended;
	};
}