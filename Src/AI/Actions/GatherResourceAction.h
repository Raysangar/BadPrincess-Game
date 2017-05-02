/**
@file GatherResourceAction.h

Action that wait for gathering a certain quantity of resource

@see GatherResourceAction

@author Guidi Giacomo
@date March 2015

*/

#pragma once

#ifndef __AI_GatherResourceAction_H
#define __AI_GatherResourceAction_H

#include "AI/ActionNode.h"
#include "AI/ResourceServer.h"

namespace AI {
	

	class CGatherResourceAction: public IActionNode
	{
	public:
		CGatherResourceAction(std::string resourceType,int qta);
		virtual ~CGatherResourceAction(){};

		virtual IActionStatus onStart();

		virtual bool onCheck(unsigned int msecs);

	private:
		AI::ResourceType _resourceType;
		int _gatherQuantity;
		int _startQuantity;
	};


} // namespace AI

#endif __AI_GatherResourceActionManager_H
