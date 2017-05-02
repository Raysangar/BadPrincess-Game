/**
@file MoveCameraAction.h

Action that move a camera to a position defined to an entity

@see MoveCameraAction

@author Guidi Giacomo
@date March 2015

*/

#pragma once

#ifndef __AI_MoveCameraAction_H
#define __AI_MoveCameraAction_H

#include "AI/ActionNode.h"


namespace AI {
	

	class CMoveCameraAction: public IActionNode
	{
	public:
		CMoveCameraAction(std::string name):IActionNode("CMoveCameraAction"),_entityName(name){};

		virtual ~CMoveCameraAction(){};

		virtual IActionStatus onStart();


	private:
		std::string _entityName;
	};


} // namespace AI

#endif __AI_MoveCameraActionManager_H
