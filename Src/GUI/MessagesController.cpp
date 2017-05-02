/**

*/

#include "MessagesController.h"

#include <iostream>
#include <cassert>
#include "BaseSubsystems/Server.h"
#include "GUIMessage.h"

namespace GUI {
	CMessagesController::CMessagesController(): _eventMessagesCount(0)
	{
		
	}
	// CMessagesController

	//--------------------------------------------------------
	
	CMessagesController::~CMessagesController()
	{
		deactivate();
		clearScreen();
	} // ~CMessagesController

	//--------------------------------------------------------

	void CMessagesController::activate()
	{		

	} // activate

	
	//--------------------------------------------------------
	void CMessagesController::deactivate()
	{
		
	} // deactivate
	void CMessagesController::reset()
	{
		clearScreen();	
	} // deactivate

	//--------------------------------------------------------
	void CMessagesController::tick(unsigned int msecs)
	{
		CMessagesMap::iterator it = _messagesMap.begin();

		for (; it != _messagesMap.end();)// ++it)
		{
			if(it->second->getDurationTime() > 0)
			{
				
				int time = it->second->getCurrentTime() + msecs; 

				if(time >= it->second->getDurationTime())
				{
					delete it->second;
					it->second = nullptr;

					if(it->first.substr(0,4).compare("event"))
					{
						--_eventMessagesCount;
						recalculatePositions();
					}
					
					it = _messagesMap.erase(it);	
				}
				else
				{
					it->second->setCurrentTime(time);
					++it;
				}
			}
			else
			 ++it;
		}
		
	} // tick



	//--------------------------------------------------------
	void CMessagesController::showMessage(std::string id, std::string text, float pos_x, float pos_y, std::string portraitName)
	{
		_messagesMap.insert(std::make_pair(id,new CGUIMessage("regular",id,text,pos_x,pos_y,portraitName,0)));
	} // showMessage

	//--------------------------------------------------------
	void CMessagesController::showTemporalMessage(std::string id, std::string text, float pos_x, float pos_y, std::string portraitName,int duration)
	{
		if(_messagesMap.find(id) == _messagesMap.end())
		{
			if(id.substr(0,4).compare("event"))
				++_eventMessagesCount;

			_messagesMap.insert(std::make_pair(id,new CGUIMessage("regular",id,text,pos_x,pos_y,portraitName,duration)));
		}
	} // showTemporalMessage
	//--------------------------------------------------------
	void CMessagesController::showObjectiveMessage(std::string id, std::string text, float pos_x, float pos_y, std::string portraitName,std::string color)
	{
		_messagesMap.insert(std::make_pair(id,new CGUIMessage("objective",id,text,pos_x,pos_y,portraitName,0,color)));
	} // showErrorMessage
	//--------------------------------------------------------
	void CMessagesController::showErrorMessage(std::string id, std::string text, float pos_x, float pos_y, std::string portraitName)
	{
		_messagesMap.insert(std::make_pair(id,new CGUIMessage("error",id,text,pos_x,pos_y,portraitName,0)));
	} // showErrorMessage

	//--------------------------------------------------------
	void CMessagesController::showTemporalErrorMessage(std::string id, std::string text, float pos_x, float pos_y, std::string portraitName, int duration)
	{
		if(_messagesMap.find(id) == _messagesMap.end())
		{
			if(id.substr(0,4).compare("event"))
				++_eventMessagesCount;

			_messagesMap.insert(std::make_pair(id,new CGUIMessage("error",id,text,pos_x,pos_y,portraitName,duration)));	
		}
	} // showErrorMessage

	//--------------------------------------------------------
	void CMessagesController::clearScreen()
	{
		//std::cout << "CLS - messages count: " << _messageList.size() << std::endl;
		CMessagesMap::iterator it = _messagesMap.begin();

		for (; it != _messagesMap.end(); ++it)
		{
			if(it->first.substr(0,4).compare("event"))
				--_eventMessagesCount;

			if(it->second != nullptr)
				delete it->second;

		}

		_messagesMap.clear();
	} // clearScreen

	//--------------------------------------------------------
	void CMessagesController::clearRegularMessages()
	{
		//std::cout << "CLS - messages count: " << _messageList.size() << std::endl;
		CMessagesMap::iterator it = _messagesMap.begin();

		for ( ;it != _messagesMap.end(); )
		{
			if(it->second != NULL && it->second->getType() == "regular")
			{		
				if(it->first.substr(0,4).compare("event"))
					--_eventMessagesCount;

				delete it->second;
				it = _messagesMap.erase(it);
			}
			else
			{
				++it;
			}
		}
	} // clearRegularMessages

	//--------------------------------------------------------
	void CMessagesController::clearErrorMessages()
	{
		//std::cout << "CLS - messages count: " << _messageList.size() << std::endl;
		CMessagesMap::iterator it = _messagesMap.begin();

		for ( ; it != _messagesMap.end(); )
		{
			if(it->second != NULL && it->second->getType() == "error")
			{			
				if(it->first.substr(0,4).compare("event"))
					--_eventMessagesCount;

				delete it->second;
				it = _messagesMap.erase(it);
			}
			else
			{
				++it;
			}
		}
	} // clearRegularMessages

	//--------------------------------------------------------
	void CMessagesController::removeMessage(std::string id)
	{
		CMessagesMap::iterator it = _messagesMap.find(id);
		if(it->second != NULL)
		{
			if(it->first.substr(0,4).compare("event"))
				--_eventMessagesCount;

			delete it->second;
			_messagesMap.erase(it);			
		}
	}

	void CMessagesController::recalculatePositions()
	{
		CMessagesMap::iterator it = _messagesMap.begin();

		for (; it != _messagesMap.end(); ++it)
		{
			if(it->second != nullptr && it->first.substr(0,4).compare("event"))
			{
				float positionY =  it->second->getPosition().second + 0.08f;
				it->second->setPositionY(positionY);
			}
		}
	}
} // namespace GUI
