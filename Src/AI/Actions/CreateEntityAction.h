/**
@file CreateEntityAction.h

Action that set if certain ability of some type of unit is enabled or disabled

@see CreateEntityAction

@author Ivan Leon
@date May 2015

*/

#pragma once

#ifndef __AI_CreateEntityAction_H
#define __AI_CreateEntityAction_H

#include "AI/ActionNode.h"


namespace AI {
	

	class CCreateEntityAction: public IActionNode
	{
	public:
		CCreateEntityAction(std::string entityName, std::string spawnPointName):IActionNode("CCreateEntityAction"),_spawnPointName(spawnPointName),_entityName(entityName){};

		virtual ~CCreateEntityAction(){};

		virtual IActionStatus onStart();

	private:

		std::string _entityName;
		std::string _spawnPointName;		
	};


} // namespace AI

#endif __AI_CreateEntityActionManager_H
