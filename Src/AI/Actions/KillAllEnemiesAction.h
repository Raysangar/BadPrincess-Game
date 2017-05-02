/**
@file KillAllEnemiesAction.h

Action that check if all enemies into the map are killed

@see KillAllEnemiesAction

@author Guidi Giacomo
@date March 2015

*/

#pragma once

#ifndef __AI_KillAllEnemiesAction_H
#define __AI_KillAllEnemiesAction_H

#include "AI/ActionNode.h"

namespace AI {
	

	class CKillAllEnemiesAction: public IActionNode
	{
	public:
		CKillAllEnemiesAction();

		virtual ~CKillAllEnemiesAction();

		virtual bool onCheck(unsigned int msecs);

		virtual IActionStatus onStart();

		virtual IActionStatus onEnd();

	private:		
		void startEntityAction(std::string type,int num);
	};


} // namespace AI

#endif __AI_KillAllEnemiesAction_H
