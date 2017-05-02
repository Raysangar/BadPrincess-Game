/**
@file SelectedUnitsController.h

Contiene la declaración de la clase CSelectedUnitsController. Se encarga de gestionar los
eventos en la parte de la interfaz dedicada a la unidad activa entre las seleccionadas.

@author Ivan Leon
@date Mayo, 2015
*/

#include "SelectedUnitsController.h"
#include "PlayerController.h"
#include "ActiveUnitController.h"
#include "Server.h"

#include <iostream>
#include <cassert>

#include "InputManager.h"
#include "BaseSubsystems/Server.h"

#include "AI/Server.h"
#include "AI/ResourceServer.h"

#include "Logic\Server.h"
#include "Logic\Entity\Entity.h"
#include "Logic\Entity\Components\EnemyDetectionManager.h"

#include "Logic\Entity\Components\GUIManager.h"
#include "Logic\Entity\Components\Life.h"

#include "CEGUI\CEGUI.h"


#define SELECTION_MAX_UNITS 9


namespace GUI {

	//--------------------------------------------------------
	
	CSelectedUnitsController::~CSelectedUnitsController()
	{
		deactivate();
	} // ~CSelectedUnitsController

	//--------------------------------------------------------

	void CSelectedUnitsController::awake()
	{
		_selectedUnits = GUI::CServer::getSingletonPtr()->getPlayerController()->getSelectedUnits();
		
	}

	//--------------------------------------------------------

	void CSelectedUnitsController::activate()
	{		
		CInputManager::getSingletonPtr()->addKeyListener(this);
		CInputManager::getSingletonPtr()->addMouseListener(this);		

		CEGUI::Window* portrait;
		CEGUI::ProgressBar* lifebar;

		for(int i = 1; i <= SELECTION_MAX_UNITS; ++i)
		{
			portrait = CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChild("SelectedUnit" + std::to_string(i));
			_portraits.push_back(portrait);
			portrait->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::SubscriberSlot(&CSelectedUnitsController::changeActiveUnit, this));		
			portrait->subscribeEvent(CEGUI::Window::EventMouseDoubleClick, CEGUI::SubscriberSlot(&CSelectedUnitsController::changeActiveUnit, this));
			portrait->subscribeEvent(CEGUI::Window::EventMouseTripleClick, CEGUI::SubscriberSlot(&CSelectedUnitsController::changeActiveUnit, this));	
			deactivateElement(portrait);

			lifebar = (CEGUI::ProgressBar*)CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChild("SelectedUnit" + std::to_string(i) + "HB");
			_lifebars.push_back(lifebar);
			lifebar->subscribeEvent(CEGUI::ProgressBar::EventMouseButtonDown, CEGUI::SubscriberSlot(&CSelectedUnitsController::changeActiveUnit, this));		
			lifebar->subscribeEvent(CEGUI::ProgressBar::EventMouseDoubleClick, CEGUI::SubscriberSlot(&CSelectedUnitsController::changeActiveUnit, this));	
			lifebar->subscribeEvent(CEGUI::ProgressBar::EventMouseTripleClick, CEGUI::SubscriberSlot(&CSelectedUnitsController::changeActiveUnit, this));	
			deactivateElement(lifebar);
		}

		_activeUnitFrame  = CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChild("ActiveUnitMiniFrame");
		_activeUnitFrame->setVisible(false);
		_activeUnitFrame->deactivate();

		_selectedUnitsFrame  = CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChild("SelectedUnits");
		_selectedUnitsFrame->deactivate();
		_selectedUnitsFrame->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::SubscriberSlot(&CSelectedUnitsController::handleMouseClick, this));		
		_selectedUnitsFrame->subscribeEvent(CEGUI::Window::EventMouseDoubleClick, CEGUI::SubscriberSlot(&CSelectedUnitsController::handleMouseClick, this));						
		_selectedUnitsFrame->subscribeEvent(CEGUI::Window::EventMouseTripleClick, CEGUI::SubscriberSlot(&CSelectedUnitsController::handleMouseClick, this));		

		if(_selectedUnits && _selectedUnits->size() > 0)
			activateElements();

	} // activate

	//--------------------------------------------------------

	void CSelectedUnitsController::deactivate()
	{
		CInputManager::getSingletonPtr()->removeKeyListener(this);
		CInputManager::getSingletonPtr()->removeMouseListener(this);
		_elementsActivated = false;
		_portraits.clear();
		_lifebars.clear();
	} // deactivate

	void CSelectedUnitsController::reset()
	{
		_selectedUnits=nullptr;
	}
	//--------------------------------------------------------

	void CSelectedUnitsController::tick() 
	{
		if(_elementsActivated)
		{	
			int i = 0;
			_activeUnit = GUI::CServer::getSingletonPtr()->getActiveUnitController()->getActiveUnit();
			if(!_activeUnit)
				_activeUnitFrame->setVisible(false);
			for(Logic::CEntity* unit : (*_selectedUnits))
			{				
				_lifeComponent = static_cast<Logic::CLife*>(unit->getComponent("CLife"));
				int currentLife = (int)_lifeComponent->getCurrentLife() > 0 ? (int)_lifeComponent->getCurrentLife() : 0;
				_lifebars.at(i)->setProgress(currentLife / _lifeComponent->getMaxLife());				

				if(unit == _activeUnit)
				{
					_activeUnitFrame->setArea(_portraits.at(i)->getArea());
					_activeUnitFrame->setVisible(true);
				}

				i++;				
			}

			if(i < SELECTION_MAX_UNITS)
			{
				for(; i < SELECTION_MAX_UNITS; ++i)
				{
					deactivateElement(_portraits.at(i));
					deactivateElement(_lifebars.at(i));
				}
			}

		}
	}
	//--------------------------------------------------------

	bool CSelectedUnitsController::keyPressed(TKey key)
	{
		//TEMPORAL: Para las pruebas de los waypoints, cuando este arreglado quitar
	/*	switch(key.keyId)
		{
			case GUI::Key::G:
				{
					_activeUnit = GUI::CServer::getSingletonPtr()->getActiveUnitController()->getActiveUnit();
					std::cout << "Posicion: " << _activeUnit->getPosition() << std::endl;
				}
				break;
		}*/
		
		return false;
	} // keyPressed

	//--------------------------------------------------------

	bool CSelectedUnitsController::keyReleased(TKey key)
	{ 
		return false;

	} // keyReleased

	//--------------------------------------------------------
	
	void CSelectedUnitsController::activateElements()
	{
		if(!_elementsActivated)
		{
			int i = 0;
			_activeUnit = GUI::CServer::getSingletonPtr()->getActiveUnitController()->getActiveUnit();
			for(Logic::CEntity* unit : (*_selectedUnits))
			{	
					
				_lifeComponent = static_cast<Logic::CLife*>(unit->getComponent("CLife"));
				_GUIManagerComponent = static_cast<Logic::CGUIManager*>(unit->getComponent("CGUIManager"));	

				if(_GUIManagerComponent)
					_portraits.at(i)->setProperty("Image", _GUIManagerComponent->getPortraitName());

				if(_lifeComponent)
				{
					int currentLife = (int)_lifeComponent->getCurrentLife() > 0 ? (int)_lifeComponent->getCurrentLife() : 0;
					_lifebars.at(i)->setProgress(currentLife / _lifeComponent->getMaxLife());
					CEGUI::Colour c(_lifeComponent->getLifebarColorR(), _lifeComponent->getLifebarColorG(), _lifeComponent->getLifebarColorB(), 1.0);   // R,G,B,A...
					CEGUI::ColourRect colorHP(c);         // Set colors for all 4 corners of rectangle...
					_lifebars.at(i)->setProperty("CustomRectColor", CEGUI::PropertyHelper<CEGUI::ColourRect>::toString(colorHP));

				}

				activateElement(_portraits.at(i));
				activateElement(_lifebars.at(i));				

				if(unit == _activeUnit)
				{
					_activeUnitFrame->setArea(_portraits.at(i)->getArea());
					_activeUnitFrame->setVisible(true);
				}

				i++;
				
			}

			if(i < SELECTION_MAX_UNITS)
			{
				for(; i < SELECTION_MAX_UNITS; ++i)
				{
					deactivateElement(_portraits.at(i));
					deactivateElement(_lifebars.at(i));
				}
			}
		}
		else
		{	
			deactivateElements();
			_activeUnitFrame->setVisible(false);
		}
		_elementsActivated = true;

	}

	//--------------------------------------------------------

	void CSelectedUnitsController::activateElement(CEGUI::Window* element)
	{
		if(!element->isVisible())
			element->setVisible(true);

		if(!element->isActive())
			element->activate();		
	}

	//--------------------------------------------------------

	void CSelectedUnitsController::deactivateElements()
	{
		if(_elementsActivated)
		{
			_elementsActivated = false;
			for(int i = 0; i < SELECTION_MAX_UNITS; ++i)
			{
				deactivateElement(_portraits.at(i));
				deactivateElement(_lifebars.at(i));
			}
			_activeUnitFrame->setVisible(false);
		}

	}
	
	//--------------------------------------------------------

	void CSelectedUnitsController::deactivateElement(CEGUI::Window* element)
	{
		if(element->isVisible())
			element->setVisible(false);
		if(element->isActive())
			element->deactivate();		
	}

	//--------------------------------------------------------
	
	bool CSelectedUnitsController::mouseMoved(const CMouseState &mouseState,const bool alreadyManaged)
	{	
		return clickAction;

	} // mouseMoved

	//--------------------------------------------------------
		
	bool CSelectedUnitsController::mousePressed(const CMouseState &mouseState,const bool alreadyManaged)
	{
		
		CEGUI::Vector2f mousePos = CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().getPosition();
		bool isHit = false;

		if(_selectedUnitsFrame->isHit(mousePos))
			isHit = true;

		if(!isHit)
		{
			for(unsigned int i = 0; i < _portraits.size(); ++i)
			{
				if(_portraits.at(i)->isHit(mousePos) || _lifebars.at(i)->isHit(mousePos) )
				{
					isHit = true;				
					break;
				}
			}
		}

		return isHit;
	} // mousePressed

	//--------------------------------------------------------

	bool CSelectedUnitsController::mouseReleased(const CMouseState &mouseState,const bool alreadyManaged)
	{
		if(clickAction)
		{
			clickAction=false;
			return true;
		}else
			return false;
	} // mouseReleased

	//-------------------------------------------------------
	
	bool CSelectedUnitsController::changeActiveUnit(const CEGUI::EventArgs &e)
	{
		if(!clickAction)
		{
			clickAction = true;
			_activeUnit = GUI::CServer::getSingletonPtr()->getActiveUnitController()->getActiveUnit();
			CEGUI::Vector2f mousePos = CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().getPosition();

			int i = 0;
			bool isHit = false;
			for(CEGUI::Window *element : _portraits)
			{
				if(element->isHit(mousePos) && element->isActive())
				{
					std::list<Logic::CEntity*>::iterator pos = _selectedUnits->begin();
					std::advance(pos,i);
					isHit = true;
					GUI::CServer::getSingletonPtr()->getPlayerController()->changeActiveUnit(*pos);					
					break;
				}
				i++;
			}

			if(!isHit)
			{
				i = 0;
				for(CEGUI::Window *element : _lifebars)
				{
					if(element->isHit(mousePos) && element->isActive())
					{
						isHit = true;
						std::list<Logic::CEntity*>::iterator pos = _selectedUnits->begin();
						std::advance(pos,i);
						GUI::CServer::getSingletonPtr()->getPlayerController()->changeActiveUnit(*pos);					
						break;
					}
					i++;
				}
			}
		}

		return true;
	}

	//-------------------------------------------------------

	bool CSelectedUnitsController::handleMouseClick(const CEGUI::EventArgs &e)
	{
		if(!clickAction)
		{
			clickAction=true;
		}
		return true;
	}

	//-------------------------------------------------------

	void CSelectedUnitsController::setSelectedUnitsStandMode(bool areAggressive)
	{
		Logic::CEnemyDetectionManager*_enemyDetectionComponent;
		
		for(Logic::CEntity* unit : (*_selectedUnits))
		{				
			_enemyDetectionComponent = static_cast<Logic::CEnemyDetectionManager*>(unit->getComponent("CEnemyDetectionManager"));
			if(_enemyDetectionComponent)
				_enemyDetectionComponent->setIsAggressive(areAggressive);
		}

	}

} // namespace GUI
