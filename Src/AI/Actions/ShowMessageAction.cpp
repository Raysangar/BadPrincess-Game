/**
@file ActionNode.cpp

Clase istancia de una oleada.

@see ActionNode

@author Guidi Giacomo 
@date March 2015

*/
#include "ShowMessageAction.h"
#include "GUI/Server.h"
#include "GUI/MessagesController.h"
#include "Logic/GameControlMessages.h"

namespace AI {
	IActionStatus CShowMessageAction::onStart(){
		if(_type == "")
			GUI::CServer::getSingletonPtr()->getMessagescontroller()->showMessage(_id,_text,_posX,_posY,_portraitName);
		else if (_type == "objective")
			GUI::CServer::getSingletonPtr()->getMessagescontroller()->showObjectiveMessage(_id,_text,_posX,_posY,_portraitName);
		return IActionStatus::OnStart;
	};

	bool CShowMessageAction::onCheck(unsigned int msecs){
		if(_appearTime > 0)
		{
			_currentAppearTime+=msecs;
			return _currentAppearTime>=_appearTime;
		}
		return true;
	};


	IActionStatus CShowMessageAction::onEnd(){
		if(_appearTime > 0)
			GUI::CServer::getSingletonPtr()->getMessagescontroller()->removeMessage(_id);	
		return IActionStatus::Ended;
	};

	bool CShowMessageAction::acceptN(const std::shared_ptr<NMessage> &message)
	{
		return message->type == "SkipText";
	}
		
	void CShowMessageAction::processN(const std::shared_ptr<NMessage> &message)
	{
		if(message->type == "SkipText")
			_currentAppearTime=_appearTime;
	}
}