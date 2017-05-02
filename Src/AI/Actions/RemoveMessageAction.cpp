/**
@file ActionNode.cpp

Clase istancia de una oleada.

@see ActionNode

@author Guidi Giacomo 
@date March 2015

*/
#include "RemoveMessageAction.h"
#include "GUI/Server.h"
#include "GUI/MessagesController.h"

namespace AI {
	IActionStatus CRemoveMessageAction::onStart(){
		GUI::CServer::getSingletonPtr()->getMessagescontroller()->removeMessage(_id);
		return IActionStatus::OnStart;
	};

}