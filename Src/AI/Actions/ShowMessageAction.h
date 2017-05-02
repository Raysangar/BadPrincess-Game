/**
@file ShowMessageAction.h

Action that show a message during a fixed time (NOT FINAL VERSION, WAIT TO BE ABLE TO SHOW CEGUI MESSAGE)

@see ShowMessageAction

@author Guidi Giacomo
@date March 2015

*/

#pragma once

#ifndef __AI_ShowMessageAction_H
#define __AI_ShowMessageAction_H

#include "AI/ActionNode.h"

namespace AI {


	class CShowMessageAction:public IActionNode
	{
	public:
		CShowMessageAction(std::string id, std::string text,float posX, float posY, unsigned int appearTime,std::string portraitName = "", std::string type = ""):IActionNode("CShowMessageAction"),
			_id(id),_text(text),_appearTime(appearTime),_currentAppearTime(0),_posX(posX),_posY(posY),_portraitName(portraitName),_type(type){};

		virtual ~CShowMessageAction(){};

		 virtual IActionStatus onStart();

		 virtual bool onCheck(unsigned int msecs);

		virtual bool acceptN(const std::shared_ptr<NMessage> &message);
		
		virtual void processN(const std::shared_ptr<NMessage> &message);

		 virtual IActionStatus onEnd();
	private:
		std::string _id;
		std::string _text;
		std::string _portraitName;
		std::string _type;
		float _posX;
		float _posY;
		unsigned int _appearTime;
		unsigned int _currentAppearTime;
	};
} // namespace AI

#endif __AI_ShowMessageActionManager_H
