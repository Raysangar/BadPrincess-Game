/**
@file KillAllEnemiesAction.cpp

Action that check if the player destroy all enemies of a wave

@see ActionNode

@author Guidi Giacomo 
@date March 2015

*/
#include "KillAllEnemiesAction.h"
#include "EntityAction.h"

#include "AI/Server.h"
#include "AI/EntityManager.h"
#include "AI/GameManager.h"

#include "Logic/EntityMessages.h"
#include "Logic/Server.h"
#include "Logic/Maps/Map.h"
#include "Logic/Entity/Entity.h"
#include "Logic/AudioMessages.h"

namespace AI {

	CKillAllEnemiesAction::CKillAllEnemiesAction():IActionNode("CKillAllEnemiesAction"){}

	CKillAllEnemiesAction::~CKillAllEnemiesAction(){
	}

	bool CKillAllEnemiesAction::onCheck(unsigned int msecs){

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

	IActionStatus CKillAllEnemiesAction::onEnd()
	{
		Logic::CEntity* gameManager = Logic::CServer::getSingletonPtr()->getMap()->getEntityByName("GameManager");
		auto playBackground = std::make_shared<SetParameterValue>();
		playBackground->eventName = "default";
		playBackground->parameterName = "trans";
		playBackground->value = 6;
		gameManager->emitMessageN(playBackground);

		return IActionNode::onEnd();

	}


	IActionStatus CKillAllEnemiesAction::onStart(){
		//get all entities on the map and launch an action for each one to check if the entities are killed
		int knightNumber = AI::CServer::getSingletonPtr()->getEntityManager()->getEntitiesNumber(AI::CEntityManager::Enemy,"Knight");
		startEntityAction("Knight",-knightNumber);
		int assassinNumber = AI::CServer::getSingletonPtr()->getEntityManager()->getEntitiesNumber(AI::CEntityManager::Enemy,"Assassin");
		startEntityAction("Assassin",-assassinNumber);
		int thiefNumber = AI::CServer::getSingletonPtr()->getEntityManager()->getEntitiesNumber(AI::CEntityManager::Enemy,"ThiefEnemy");
		startEntityAction("ThiefEnemy",-thiefNumber);
//		std::cout<<"CKillAllEnemiesAction onStart"<<std::endl;

		Logic::CEntity* gameManager = Logic::CServer::getSingletonPtr()->getMap()->getEntityByName("GameManager");
		auto playBackground = std::make_shared<SetParameterValue>();
		playBackground->eventName = "default";
		playBackground->parameterName = "trans";
		playBackground->value = 4;
		gameManager->emitMessageN(playBackground);

		return IActionStatus::OnStart;
	};

	void CKillAllEnemiesAction::startEntityAction(std::string type,int num)
	{
		CEntityAction * action=new CEntityAction(type,num);
		_runtimeCreatedAction.push_back(action);
		//launch the action
		AI::CServer::getSingletonPtr()->getGameManager()->executeAction(action);

	}

}