/**
@file SurvivorAction.h

Action that lauch a wave and check if the wave finish

@see SurvivorAction

@author Guidi Giacomo
@date March 2015

*/

#pragma once

#ifndef __AI_SurvivorAction_H
#define __AI_SurvivorAction_H

#include "AI/ActionNode.h"
namespace ScriptManager{
	class CScriptManager;
}
namespace AI {
	
	class CTimerAction;

	class CSurvivorAction: public IActionNode
	{
	public:
		CSurvivorAction(int battleTimeForEveryEnemy,int enemyFrequencySpawnTime,int incrementKnapsackSpace,long minTimeBetweenWave);

		virtual ~CSurvivorAction();

		virtual bool onCheck(unsigned int msecs);

		virtual IActionStatus onStart();


	private:
		
		int _battleTimeForEveryEnemy;
		int _enemyFrequencySpawnTime;
		long _minTimeBetweenWave;
		int _incrementKnapsackSpace;

		CTimerAction* _currentTimer;
		IActionNode* _currentAction;
		std::string *_spArray;
		int _spLength;

		ScriptManager::CScriptManager *_sm;
		void startWave();

	};


} // namespace AI

#endif __AI_SurvivorAction_H
