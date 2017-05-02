/**
@file ActionNode.cpp

Clase istancia de una oleada.

@see ActionNode

@author Guidi Giacomo 
@date March 2015

*/
#include "IncrementsWavesCounter.h"

#include "AI/Server.h"
#include "AI/GameManager.h"

#include "GUI/Server.h"
#include "GUI/StatisticsController.h"

namespace AI {
	IActionStatus CIncrementsWavesCounter::onStart(){
		AI::CServer::getSingletonPtr()->getGameManager()->addWavesCounter(_incrementWavesNumber);
		GUI::CServer::getSingletonPtr()->getStatisticsController()->setWavesCounterText(std::to_string(AI::CServer::getSingletonPtr()->getGameManager()->getWavesCounter())+" / "+std::to_string(_maxWaves));
		return IActionStatus::OnStart;
	};

}