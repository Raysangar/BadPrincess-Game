/**
@file SurvivorAction.cpp

Action that check if the player destroy all enemies of a wave

@see ActionNode

@author Guidi Giacomo 
@date March 2015

*/
#include "SurvivorAction.h"
#include "DefendToWaveAction.h"
#include "TimerAction.h"

#include "AI/Server.h"
#include "AI/EntityManager.h"
#include "AI/GameManager.h"

#include "GUI/Server.h"
#include "GUI/StatisticsController.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Server.h"
#include "Logic/Maps/Map.h"


#include "BaseSubsystems\ScriptManager.h"

#define TIMER_COLOR "FFFAFA00"
namespace AI {

	CSurvivorAction::CSurvivorAction(int battleTimeForEveryUnit,int enemyFrequencySpawnTime, int incrementKnapsackSpace,long minTimeBeetweenWave):IActionNode("CSurvivorAction"),_battleTimeForEveryEnemy(battleTimeForEveryUnit),_enemyFrequencySpawnTime(enemyFrequencySpawnTime),_incrementKnapsackSpace(incrementKnapsackSpace),_minTimeBetweenWave(minTimeBeetweenWave),_spArray(nullptr),_currentAction(nullptr),_sm(nullptr),_currentTimer(nullptr),_spLength(0){}

	CSurvivorAction::~CSurvivorAction(){
		if(_spArray!=nullptr)
			delete[] _spArray;
	}

	bool CSurvivorAction::onCheck(unsigned int msecs){
		if(_currentAction==nullptr)
		{
			//if the timer it isn't started
			if(_currentTimer==nullptr)
			{
				//create a timerAction
				_currentTimer =new AI::CTimerAction(TIMER_COLOR,_minTimeBetweenWave);
				_runtimeCreatedAction.push_back(_currentTimer);
				//launch the action
				AI::CServer::getSingletonPtr()->getGameManager()->executeAction(_currentTimer);
				

			}else if(_currentTimer->getStatus()==AI::IActionStatus::Ended)//check if the time to waiting for the new wave is finish
			{
			//_currentTime+=msecs;
			//if(_currentTime>=_minTimeBetweenWave)
			//{
				startWave();
				_currentTimer = nullptr;
			//	_currentTime=0;
			//}
			}
		}else if(_currentAction->getStatus()==AI::IActionStatus::Ended)
		{
			_currentAction= nullptr;
		}
		return false;
	};

	void CSurvivorAction::startWave()
	{
		AI::CServer::getSingletonPtr()->getGameManager()->addWavesCounter(1);
		GUI::CServer::getSingletonPtr()->getStatisticsController()->setWavesCounterText(std::to_string(AI::CServer::getSingletonPtr()->getGameManager()->getWavesCounter()));

		int idWave;
		int knapsackSpace = _incrementKnapsackSpace*AI::CServer::getSingletonPtr()->getGameManager()->getWavesCounter();
		_sm->executeFunction("createWave",knapsackSpace,_spArray,_spLength,_enemyFrequencySpawnTime,idWave);

		_currentAction =new CDefendToWaveAction(idWave,knapsackSpace*_battleTimeForEveryEnemy);
		_runtimeCreatedAction.push_back(_currentAction);
		//launch the action
		AI::CServer::getSingletonPtr()->getGameManager()->executeAction(_currentAction);
	}


	IActionStatus CSurvivorAction::onStart(){
//		GUI::CServer::getSingletonPtr()->getStatisticsController()->showWavesCounter();
		GUI::CServer::getSingletonPtr()->getStatisticsController()->setWavesCounterText(std::to_string(AI::CServer::getSingletonPtr()->getGameManager()->getWavesCounter()));

		_sm = ScriptManager::CScriptManager::GetPtrSingleton();
		_sm->loadScript("media/lua/waveGenerator.lua");

		std::set<Logic::CEntity*> *spList= AI::CServer::getSingletonPtr()->getEntityManager()->getSpawnPointList();
		_spLength = spList->size();
			_spArray = new std::string[_spLength];
		std::set<Logic::CEntity*>::iterator it= spList->begin();
		for (int i=0; it != spList->end(); ++it,++i)
			_spArray[i]=(*it)->getName();

		_currentAction = nullptr;

		return IActionStatus::OnStart;
	};


}