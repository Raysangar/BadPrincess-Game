/**
@file TimedSurvivorAction.cpp

Action that lauch waves for certain time without relax time

@see ActionNode

@author Guidi Giacomo 
@date March 2015

*/
#include "TimedSurvivorAction.h"
#include "DefendToWaveAction.h"
#include "TimerAction.h"

#include "AI/Server.h"
#include "AI/EntityManager.h"
#include "AI/GameManager.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Server.h"
#include "Logic/Maps/Map.h"


#include "BaseSubsystems\ScriptManager.h"

#define TIMER_COLOR "FFFAFA00"
namespace AI {

	CTimedSurvivorAction::CTimedSurvivorAction(int battleTimeForEveryUnit,int enemyFrequencySpawnTime,int initialKnapsackSpace, int incrementKnapsackSpace,int timeToSurvivor):IActionNode("CTimedSurvivorAction"),_battleTimeForEveryEnemy(battleTimeForEveryUnit),_enemyFrequencySpawnTime(enemyFrequencySpawnTime),_initialKnapsackSpace(initialKnapsackSpace),_incrementKnapsackSpace(incrementKnapsackSpace),_timeToSurvivor(timeToSurvivor),_spArray(nullptr),_currentAction(nullptr),_sm(nullptr),_timerAction(nullptr),_spLength(0){}

	CTimedSurvivorAction::~CTimedSurvivorAction(){
		if(_spArray!=nullptr)
			delete[] _spArray;
	}

	bool CTimedSurvivorAction::onCheck(unsigned int msecs){
		if(_timerAction->getStatus()!=AI::IActionStatus::Ended)
		{
			if(_currentAction == nullptr || (_currentAction != nullptr && _currentAction->getStatus()==AI::IActionStatus::Ended))
			{
				std::cout<<"launch new wave"<<std::endl;
				startWave();
			}
		}else if(_currentAction->getStatus()==AI::IActionStatus::Ended)
		{
			std::cout<<"time finished and last wave finished"<<std::endl;
			return true;
		}
		return false;
	};

	void CTimedSurvivorAction::startWave()
	{
		AI::CServer::getSingletonPtr()->getGameManager()->addWavesCounter(1);
//		GUI::CServer::getSingletonPtr()->getStatisticsController()->setWavesCounterText(std::to_string(AI::CServer::getSingletonPtr()->getGameManager()->getWavesCounter()));

		int idWave;
		int knapsackSpace = _initialKnapsackSpace+_incrementKnapsackSpace*AI::CServer::getSingletonPtr()->getGameManager()->getWavesCounter();
		_sm->executeFunction("createWave",knapsackSpace,_spArray,_spLength,_enemyFrequencySpawnTime,idWave);

		_currentAction =new CDefendToWaveAction(idWave,knapsackSpace*_battleTimeForEveryEnemy);
		_runtimeCreatedAction.push_back(_currentAction);
		//launch the action
		AI::CServer::getSingletonPtr()->getGameManager()->executeAction(_currentAction);
	}


	IActionStatus CTimedSurvivorAction::onStart(){
//		GUI::CServer::getSingletonPtr()->getStatisticsController()->setWavesCounterText(std::to_string(AI::CServer::getSingletonPtr()->getGameManager()->getWavesCounter()));

		_sm = ScriptManager::CScriptManager::GetPtrSingleton();
		_sm->loadScript("media/lua/waveGenerator.lua");

		std::set<Logic::CEntity*> *spList= AI::CServer::getSingletonPtr()->getEntityManager()->getSpawnPointList();
		_spLength = spList->size();
		_spArray = new std::string[_spLength];
		std::set<Logic::CEntity*>::iterator it= spList->begin();
		for (int i=0; it != spList->end(); ++it,++i)
			_spArray[i]=(*it)->getName();

		_currentAction = nullptr;

		//create a timerAction
		_timerAction =new AI::CTimerAction(TIMER_COLOR,_timeToSurvivor);
		_runtimeCreatedAction.push_back(_timerAction);
		//launch the action
		AI::CServer::getSingletonPtr()->getGameManager()->executeAction(_timerAction);


		return IActionStatus::OnStart;
	};


}