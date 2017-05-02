/**
@file DefendToWaveAction.cpp

Action that check if the player destroy all enemies of a wave

@see ActionNode

@author Guidi Giacomo 
@date March 2015

*/
#include "DefendToWaveAction.h"
#include "EntityAction.h"
#include "AI/Server.h"
#include "AI/WaveManager.h"
#include "AI/GameManager.h"
#include "AI/Wave.h"
#include "Logic/GameControlMessages.h"
#include "Logic/EntityMessages.h"
#include "Logic/Server.h"
#include "Logic/Maps/Map.h"
#include "Logic/Entity/Entity.h"
#include "Logic/AudioMessages.h"

namespace AI {

	CDefendToWaveAction::CDefendToWaveAction(int waveId,long maxTime):IActionNode("CDefendToWaveAction"),_waveId(waveId),_maxTime(maxTime),_currentTime(0){}

	CDefendToWaveAction::~CDefendToWaveAction(){
	}

	bool CDefendToWaveAction::onCheck(unsigned int msecs){
		//check if max time available is ended to avoid that player not kill the last enemy to gain time
		if(_maxTime!=-1)
		{
			_currentTime+=msecs;
			if(_currentTime>=_maxTime)
			{
				std::cout<<"CDefendToWaveAction ended for time exceeded "<<_currentTime<<"/"<<_maxTime<<std::endl;
				return true;
			}
		}

		bool res=true;
		//check if all entity node action are ended
		for(IActionNode *action:_runtimeCreatedAction)
		{
			if(action->getStatus()!=IActionStatus::Ended)
			{
				res=false;
				break;
			}
		}

		return res;
	};
	IActionStatus CDefendToWaveAction::onStart(){

		std::cout<<"start CDefendToWaveAction wid="<<_waveId<<" maxtime="<<_maxTime<<std::endl;
		CWave* wave = AI::CServer::getSingletonPtr()->getWaveManager()->getWaveFromId(_waveId);
		std::map<std::string,int> enemyNumber;
		std::deque<CWaveNode> nodeList = wave->getWaveNodeList();
		//get all wave node to sum all the entity created into the wave
		for(CWaveNode node: nodeList)
		{
			if(enemyNumber.find(node.getEnemyType())!=enemyNumber.end())
				enemyNumber[node.getEnemyType()]+=node.getEnemyNumber();
			else
				enemyNumber[node.getEnemyType()]=node.getEnemyNumber();			
		}
		//create a CEntityAction to check if all entity create into the wave are died
		for(auto e:enemyNumber)
		{
			CEntityAction * action=new CEntityAction(e.first,-e.second);
			_runtimeCreatedAction.push_back(action);
			//launch the action
			AI::CServer::getSingletonPtr()->getGameManager()->executeAction(action);
		}

		Logic::CEntity* gameManager = Logic::CServer::getSingletonPtr()->getMap()->getEntityByName("GameManager");
		auto playBackground = std::make_shared<SetParameterValue>();
		playBackground->eventName = "default";
		playBackground->parameterName = "trans";
		playBackground->value = 4;
		gameManager->emitMessageN(playBackground);

		//send start wave message
		auto m=std::make_shared<StartWave>();
		m->id=_waveId;
		Logic::CServer::getSingletonPtr()->getMap()->sendMessageToAll(m);
		return IActionStatus::OnStart;
	};

	IActionStatus CDefendToWaveAction::onEnd()
	{

		Logic::CEntity* gameManager = Logic::CServer::getSingletonPtr()->getMap()->getEntityByName("GameManager");
		auto playBackground = std::make_shared<SetParameterValue>();
		playBackground->eventName = "default";
		playBackground->parameterName = "trans";
		playBackground->value = 6;
		gameManager->emitMessageN(playBackground);

		auto endWave = std::make_shared<EndWave>();
		endWave->waveId = _waveId;
		gameManager->emitMessageN(endWave);


		return IActionNode::onEnd();

	}
}