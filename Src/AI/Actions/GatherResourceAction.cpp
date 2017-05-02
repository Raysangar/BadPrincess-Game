/**
@file ActionNode.cpp

Clase istancia de una oleada.

@see ActionNode

@author Guidi Giacomo 
@date March 2015

*/
#include "GatherResourceAction.h"


namespace AI {
	CGatherResourceAction::CGatherResourceAction(std::string resourceType,int qta):IActionNode("GatherResourceAction"),_gatherQuantity(qta),_startQuantity(0){
		if(resourceType.compare("Wood")==0)
			_resourceType=AI::ResourceType::Wood;
		else if(resourceType.compare("Stone")==0)
			_resourceType=AI::ResourceType::Stone;
		else if(resourceType.compare("Iron")==0)
			_resourceType=AI::ResourceType::Iron;

	}

	IActionStatus CGatherResourceAction::onStart(){
		_startQuantity = AI::ResourceServer::getInstance()->getResource(_resourceType);
		return IActionStatus::OnStart;
	};

	bool CGatherResourceAction::onCheck(unsigned int msecs){
		return  (AI::ResourceServer::getInstance()->getResource(_resourceType)-_startQuantity)>=_gatherQuantity;
	};

}