/**
@file SendMessageAction.h

Action that send a message from lua to all entities or a specific entity

@see SendMessageAction

@author Guidi Giacomo
@date March 2015

*/

#pragma once

#ifndef __AI_SendMessageAction_H
#define __AI_SendMessageAction_H

#include "AI/ActionNode.h"

namespace AI {


	class CSendMessageAction:public IActionNode
	{
	public:
		CSendMessageAction(std::string entity,std::shared_ptr<NMessage> mess):IActionNode("CSendMessageAction"), _reciver(entity),_message(mess){};


		virtual ~CSendMessageAction(){};

		 virtual IActionStatus onStart();

	private:
		std::shared_ptr<NMessage> _message;
		std::string _reciver;
	};
} // namespace AI

#endif __AI_SendMessageActionManager_H
