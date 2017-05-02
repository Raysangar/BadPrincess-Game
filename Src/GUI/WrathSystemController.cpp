/**
@file WrathSystemController.h

Contiene la declaración de la clase CWrathSystemController. Se encarga de gestionar los
eventos en la parte de la interfaz dedicada a la unidad activa entre las seleccionadas.

@author Ivan Leon
@date Mayo, 2015
*/

#include "WrathSystemController.h"

#include <iostream>
#include <cassert>

#include "GUI\Server.h"
#include "GUI\ActiveUnitController.h"

#include "InputManager.h"
#include "BaseSubsystems/Server.h"

#include "Logic\Server.h"
#include "Logic\Entity\Entity.h"
#include "Logic\Maps\Map.h"


#include "Logic\Entity\Components\PrincessManager.h"

#include "CEGUI\CEGUI.h"
#include "CEGUI\Quaternion.h"

namespace GUI {

	//--------------------------------------------------------
	
	CWrathSystemController::~CWrathSystemController()
	{
		deactivate();
	} // ~CWrathSystemController

	//--------------------------------------------------------

	void CWrathSystemController::awake()
	{
		_princessComponent = static_cast<Logic::CPrincessManager*>(Logic::CServer::getSingletonPtr()->getMap()->getEntityByType("Princess")->getComponent("CPrincessManager"));
		assert(_princessComponent);
	}

	//--------------------------------------------------------

	void CWrathSystemController::activate()
	{		
		CInputManager::getSingletonPtr()->addKeyListener(this);
		CInputManager::getSingletonPtr()->addMouseListener(this);		

		_princessPortrait = CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChild("PrincessPortrait");
		elements.push_back(_princessPortrait);

		_wrathBarIn = CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChild("WrathMeterIn");
		elements.push_back(_wrathBarIn);

		_wrathBarOut = (CEGUI::ProgressBar*)CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChild("WrathMeterOut");
		elements.push_back(_wrathBarOut);  

		_abilityQ = (CEGUI::PushButton*)CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChild("AbilityQ");
		elements.push_back(_abilityQ);

		_abilityW = (CEGUI::PushButton*)CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChild("AbilityW");
		elements.push_back(_abilityW);

		_abilityE = (CEGUI::PushButton*)CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChild("AbilityE");		
		elements.push_back(_abilityE);

		_abilityR = (CEGUI::PushButton*)CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChild("AbilityR");
		elements.push_back(_abilityR);

		_princessPortrait->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::SubscriberSlot(&CWrathSystemController::handleMouseClick, this));				
		_wrathBarIn->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::SubscriberSlot(&CWrathSystemController::handleMouseClick, this));		
		_wrathBarOut->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::SubscriberSlot(&CWrathSystemController::handleMouseClick, this));		

		_princessPortrait->subscribeEvent(CEGUI::Window::EventMouseDoubleClick, CEGUI::SubscriberSlot(&CWrathSystemController::handleMouseClick, this));				
		_wrathBarIn->subscribeEvent(CEGUI::Window::EventMouseDoubleClick, CEGUI::SubscriberSlot(&CWrathSystemController::handleMouseClick, this));		
		_wrathBarOut->subscribeEvent(CEGUI::Window::EventMouseDoubleClick, CEGUI::SubscriberSlot(&CWrathSystemController::handleMouseClick, this));	

		_princessPortrait->subscribeEvent(CEGUI::Window::EventMouseTripleClick, CEGUI::SubscriberSlot(&CWrathSystemController::handleMouseClick, this));				
		_wrathBarIn->subscribeEvent(CEGUI::Window::EventMouseTripleClick, CEGUI::SubscriberSlot(&CWrathSystemController::handleMouseClick, this));		
		_wrathBarOut->subscribeEvent(CEGUI::Window::EventMouseTripleClick, CEGUI::SubscriberSlot(&CWrathSystemController::handleMouseClick, this));	


	} // activate

	//--------------------------------------------------------

	void CWrathSystemController::deactivate()
	{
		CInputManager::getSingletonPtr()->removeKeyListener(this);
		CInputManager::getSingletonPtr()->removeMouseListener(this);
		elements.clear();
	} // deactivate
	void CWrathSystemController::reset()
	{
		_activeUnit = nullptr;
		_princessPortrait= nullptr;
		_wrathBarIn= nullptr;
		_wrathBarOut= nullptr;
		_abilityQ= nullptr;
		_abilityW= nullptr;
		_abilityE= nullptr;
		_abilityR= nullptr;
		_currentWrath= 0;
		clickAction=false;
	} // deactivate
	//--------------------------------------------------------

	void CWrathSystemController::tick() 
	{
		
		_activeUnit = GUI::CServer::getSingletonPtr()->getActiveUnitController()->getActiveUnit();
		if(_activeUnit && _activeUnit->getType() == "Princess")			
		{
			if(_currentWrath < _princessComponent->getAoeStunCost() || !GUI::CServer::getSingletonPtr()->getActiveUnitController()->checkAbilityUsability("Q"))
				_abilityQ->disable();		
			else
				_abilityQ->enable();		

			if(_currentWrath < _princessComponent->getAoeBuffCost() || !GUI::CServer::getSingletonPtr()->getActiveUnitController()->checkAbilityUsability("W"))
				_abilityW->disable();
			else
				_abilityW->enable();	

			if(_currentWrath < _princessComponent->getAoeHealCost() || !GUI::CServer::getSingletonPtr()->getActiveUnitController()->checkAbilityUsability("E"))
				_abilityE->disable();
			else
				_abilityE->enable();	

			if(_currentWrath < _princessComponent->getBerserkerModeCost() || !GUI::CServer::getSingletonPtr()->getActiveUnitController()->checkAbilityUsability("R"))
				_abilityR->disable();
			else
				_abilityR->enable();	
		}
		CEGUI::Quaternion q = CEGUI::Quaternion::eulerAnglesDegrees(0,0,75 - _currentWrath * 65 / 100.0f);

		_wrathBarIn->setRotation(q);
		

	}
	//--------------------------------------------------------

	bool CWrathSystemController::keyPressed(TKey key){

		return false;

	} // keyPressed

	//--------------------------------------------------------

	bool CWrathSystemController::keyReleased(TKey key)
	{

		return false;

	} // keyReleased


	//--------------------------------------------------------
	
	bool CWrathSystemController::mouseMoved(const CMouseState &mouseState,const bool alreadyManaged)
	{	
		return clickAction;
		

	} // mouseMoved

	//--------------------------------------------------------
		
	bool CWrathSystemController::mousePressed(const CMouseState &mouseState,const bool alreadyManaged)
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

	bool CWrathSystemController::mouseReleased(const CMouseState &mouseState,const bool alreadyManaged)
	{
		if(clickAction)
		{
			clickAction=false;
			return true;
		}else
			return false;
	} // mouseReleased

	//--------------------------------------------------------

	bool CWrathSystemController::handleMouseClick(const CEGUI::EventArgs &e)
	{
		if(!clickAction)
		{
			clickAction=true;
		}
		return true;
	}

	

} // namespace GUI
