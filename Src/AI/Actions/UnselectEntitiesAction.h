/**
@file UnselectEntitiesAction.h

Action that set if some type of entity can or cannot be clicked by the button especified.

@see UnselectEntitiesAction

@author Ivan Leon
@date May 2015
*/

#pragma once

#ifndef __AI_UnselectEntitiesAction_H
#define __AI_UnselectEntitiesAction_H

#include "AI/ActionNode.h"


namespace AI {
	

	class CUnselectEntitiesAction: public IActionNode
	{
	public:
		CUnselectEntitiesAction():IActionNode("CUnselectEntitiesAction"){};

		virtual ~CUnselectEntitiesAction(){};

		virtual IActionStatus onStart();
	};


} // namespace AI

#endif __AI_UnselectEntitiesActionManager_H
