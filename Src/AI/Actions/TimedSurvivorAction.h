/**
@file TimedSurvivorAction.h

Action that lauch waves for certain time without relax time

@see TimedSurvivorAction

@author Guidi Giacomo
@date March 2015

*/

#pragma once

#ifndef __AI_TimedSurvivorAction_H
#define __AI_TimedSurvivorAction_H

#include "AI/ActionNode.h"
namespace ScriptManager{
	class CScriptManager;
}
namespace AI {
	
	class CTimerAction;

	class CTimedSurvivorAction: public IActionNode
	{
	public:
		CTimedSurvivorAction(int battleTimeForEveryUnit,int enemyFrequencySpawnTime,int initialKnapsackSpace,int incrementKnapsackSpace,int timeToSurvivor);

		virtual ~CTimedSurvivorAction();

		virtual bool onCheck(unsigned int msecs);

		virtual IActionStatus onStart();


	private:
		
		int _battleTimeForEveryEnemy;
		int _enemyFrequencySpawnTime;
		int _timeToSurvivor;
		int _incrementKnapsackSpace;
		int _initialKnapsackSpace;

		CTimerAction* _timerAction;
		IActionNode* _currentAction;
		std::string *_spArray;
		int _spLength;
		ScriptManager::CScriptManager *_sm;
		void startWave();

	};


} // namespace AI

#endif __AI_TimedSurvivorAction_H
