/**
@file ResourceCountController.cpp

Contiene la implementación de la clase CResourceCountController. Se encarga de
recibir eventos del teclado y del ratón y de interpretarlos para
mover al jugador.

@see GUI::CResourceCountController

@author David Llansó
@date Agosto, 2010
*/

#include "ResourceCountController.h"

#include <iostream>
#include <cassert>

#include "InputManager.h"
#include "BaseSubsystems/Server.h"

#include "CEGUI\CEGUI.h"


namespace GUI {
	CResourceCountController::CResourceCountController():clickAction(false)
	{
	}
	// CResourceCountController

	//--------------------------------------------------------
	
	CResourceCountController::~CResourceCountController()
	{
		deactivate();
	} // ~CResourceCountController

	//--------------------------------------------------------

	void CResourceCountController::awake()
	{
		_stoneQuantity = 0;
		_ironQuantity = 0;
		_woodQuantity = 0;
	}
	void CResourceCountController::reset()
	{
		_stoneQuantity = 0;
		_ironQuantity = 0;
		_woodQuantity = 0;
		clickAction = false;
	}
	//--------------------------------------------------------


	void CResourceCountController::activate()
	{		
		CInputManager::getSingletonPtr()->addKeyListener(this);
		CInputManager::getSingletonPtr()->addMouseListener(this);
//		BaseSubsystems::CServer::getSingletonPtr()->getWindowExtents(_windowWidth,_windowHeight);
//		_listboxWindow =(CEGUI::Listbox *)CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChild("MessageListBox");
//
//		_listboxWindow->setVisible(false);
////		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChild("BtnOpenMessageList")->setVisible(false);
//
//		_listboxWindow->subscribeEvent(CEGUI::Listbox::EventMouseMove, 
//			CEGUI::SubscriberSlot(&CResourceCountController::logWindowMouseMove, this));     
//		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChild("BtnOpenMessageList")->subscribeEvent(CEGUI::PushButton::EventMouseButtonDown, 
//			CEGUI::SubscriberSlot(&CResourceCountController::showLogList, this));     
//		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChild("BtnCloseMessageList")->subscribeEvent(CEGUI::PushButton::EventMouseButtonDown, 
//			CEGUI::SubscriberSlot(&CResourceCountController::closeLogList, this));
//		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChild("BtnCloseMessageList")->setVisible(false);

		ironQuantityText = CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChild("IronQuantity");
		
		woodQuantityText = CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChild("WoodQuantity");
		
		stoneQuantityText = CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChild("StoneQuantity");

		ironIcon = CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChild("IronCounter");
		elements.push_back(ironIcon);
		woodIcon= CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChild("WoodCounter");
		elements.push_back(woodIcon);
		stoneIcon = CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChild("StoneCounter");
		elements.push_back(stoneIcon);
				
		ironIcon->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::SubscriberSlot(&CResourceCountController::handleMouseClick, this));
		ironIcon->subscribeEvent(CEGUI::Window::EventMouseDoubleClick, CEGUI::SubscriberSlot(&CResourceCountController::handleMouseClick, this));	
		ironIcon->subscribeEvent(CEGUI::Window::EventMouseTripleClick, CEGUI::SubscriberSlot(&CResourceCountController::handleMouseClick, this));	
				
		woodIcon->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::SubscriberSlot(&CResourceCountController::handleMouseClick, this));
		woodIcon->subscribeEvent(CEGUI::Window::EventMouseDoubleClick, CEGUI::SubscriberSlot(&CResourceCountController::handleMouseClick, this));	
		woodIcon->subscribeEvent(CEGUI::Window::EventMouseTripleClick, CEGUI::SubscriberSlot(&CResourceCountController::handleMouseClick, this));	
				
		stoneIcon->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::SubscriberSlot(&CResourceCountController::handleMouseClick, this));
		stoneIcon->subscribeEvent(CEGUI::Window::EventMouseDoubleClick, CEGUI::SubscriberSlot(&CResourceCountController::handleMouseClick, this));	
		stoneIcon->subscribeEvent(CEGUI::Window::EventMouseTripleClick, CEGUI::SubscriberSlot(&CResourceCountController::handleMouseClick, this));	

		woodQuantityText->setText("x " + std::to_string(_woodQuantity));
		stoneQuantityText->setText("x " + std::to_string(_stoneQuantity));
		ironQuantityText->setText("x " + std::to_string(_ironQuantity));

	} // activate

	//--------------------------------------------------------

	void CResourceCountController::subscribeAllElements()
	{
		for(CEGUI::Window *element : elements)
		{
			element->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::SubscriberSlot(&CResourceCountController::handleMouseClick, this));
			element->subscribeEvent(CEGUI::Window::EventMouseDoubleClick, CEGUI::SubscriberSlot(&CResourceCountController::handleMouseClick, this));	
		}
	}

	//--------------------------------------------------------
	void CResourceCountController::deactivate()
	{
		CInputManager::getSingletonPtr()->removeKeyListener(this);
		CInputManager::getSingletonPtr()->removeMouseListener(this);
		elements.clear();
		
	} // deactivate

	//--------------------------------------------------------

	bool CResourceCountController::keyPressed(TKey key){
		return false;

	} // keyPressed

	//--------------------------------------------------------

	bool CResourceCountController::keyReleased(TKey key){

		return false;

	} // keyReleased

	//--------------------------------------------------------
	
	bool CResourceCountController::mouseMoved(const CMouseState &mouseState,const bool alreadyManaged)
	{	
			
		return clickAction;
		
	} // mouseMoved

	//--------------------------------------------------------
		
	bool CResourceCountController::mousePressed(const CMouseState &mouseState,const bool alreadyManaged)
	{
		CEGUI::Vector2f mousePos = CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().getPosition();

		bool isHit = false;
		for(CEGUI::Window *element : elements)
		{
			if(element->isHit(mousePos))
			{
				isHit = true;
				break;
			}
		}

		return isHit;
	} // mousePressed

	//--------------------------------------------------------

	bool CResourceCountController::mouseReleased(const CMouseState &mouseState,const bool alreadyManaged)
	{
		if(clickAction)
		{
			clickAction=false;
			return true;
		}else
			return false;
	} // mouseReleased

	//--------------------------------------------------------

	//bool CResourceCountController::showLogList(const CEGUI::EventArgs &e)
	//{		
	//	_listboxWindow->setVisible(true);
	//	CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChild("BtnCloseMessageList")->setVisible(true);
	//	CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChild("BtnOpenMessageList")->setVisible(false);
	//	clickAction=true;
	//	return true;
	//}

	////--------------------------------------------------------

	//bool CResourceCountController::closeLogList(const CEGUI::EventArgs &e)
	//{
	//	CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChild("BtnCloseMessageList")->setVisible(false);		
	//	_listboxWindow->setVisible(false);
	//	CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChild("BtnOpenMessageList")->setVisible(true);
	//	clickAction=true;
	//	return true;
	//}

	//--------------------------------------------------------

	//bool CResourceCountController::logWindowMouseMove(const CEGUI::EventArgs &e){		
	//	clickAction=true;
	//	return true;
	//};

	//--------------------------------------------------------

	//void CResourceCountController::printMessageToLog(const std::string message){
	//	CEGUI::ListboxTextItem* newItem = new CEGUI::ListboxTextItem(message);
	//		newItem->setTextColours(CEGUI::Colour(0.0f,0.0f,0.0f)); // Set the text color
	//		_listboxWindow->addItem(newItem);
	//};

	//--------------------------------------------------------

	void CResourceCountController::setWoodQuantity(int quantity)
	{
		_woodQuantity = quantity;
		woodQuantityText->setText("x " + std::to_string(_woodQuantity));
	}

	//--------------------------------------------------------

	void CResourceCountController::setIronQuantity(int quantity)
	{
		_ironQuantity = quantity;
		ironQuantityText->setText("x " + std::to_string(_ironQuantity));
	}

	//--------------------------------------------------------

	void CResourceCountController::setStoneQuantity(int quantity)
	{
		_stoneQuantity = quantity;
		stoneQuantityText->setText("x " + std::to_string(_stoneQuantity));
	}
	
	//--------------------------------------------------------
		
	bool CResourceCountController::handleMouseClick(const CEGUI::EventArgs &e)
	{
		if(!clickAction)
		{
			clickAction=true;
		}
		return true;
	}
} // namespace GUI
