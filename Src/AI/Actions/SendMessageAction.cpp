/**
@file ActionNode.cpp

Clase istancia de una oleada.

@see ActionNode

@author Guidi Giacomo 
@date March 2015

*/
#include "SendMessageAction.h"
#include <memory>

#include "Logic\Entity\Entity.h"
#include "Logic/Server.h"
#include "Logic/Maps/Map.h"
#include "Logic/NMessage.h"

namespace AI {
	IActionStatus CSendMessageAction::onStart(){
		if(_reciver.compare("All")==0)
			Logic::CServer::getSingletonPtr()->getMap()->sendMessageToAll(_message);
		else
		{
			Logic::CEntity *e = Logic::CServer::getSingletonPtr()->getMap()->getEntityByName(_reciver);
			if(e!=nullptr)
				e->emitMessageN(_message);
		}
		return IActionStatus::OnCheck;
	};

}