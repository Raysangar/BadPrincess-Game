/**
@file ActiveUnitController.h

Contiene la declaración de la clase CActiveUnitController. Se encarga de gestionar los
eventos en la parte de la interfaz dedicada a la unidad activa entre las seleccionadas.

@author Ivan Leon
@date Mayo, 2015
*/

#include "ActiveUnitController.h"

#include <iostream>
#include <cassert>

#include "InputManager.h"
#include "BaseSubsystems/Server.h"

#include "AI/Server.h"
#include "AI/ResourceServer.h"

#include "Logic\Server.h"
#include "Logic\Entity\Entity.h"
#include "Logic\AllyUnitMessages.h"
#include "Logic\GraphicsMessages.h"
#include "Logic\Entity\Components\EnemyDetectionManager.h"
#include "Logic\Entity\Components\GUIManager.h"
#include "Logic\Entity\Components\BuildComponent.h"
#include "Logic\Entity\Components\Life.h"
#include "Logic\Entity\Components\WorkmanState.h"
#include "Logic\Entity\Components\PrincessManager.h"
#include "Logic\Maps\Map.h"

#include "GUI\Server.h"
#include "GUI\PlayerController.h"
#include "GUI\SelectedUnitsController.h"

#include "CEGUI\CEGUI.h"


namespace GUI {

	CActiveUnitController::CActiveUnitController():_activeUnit(0),_activeUnitPortrait(0),_activeUnitHealthBar(0),_abilityQ(0),_abilityW(0),_abilityE(0),_abilityR(0),
		_aggressiveMode(false), _enemyDetectionComponent(0),_GUIManagerComponent(0),_lifeComponent(0),clickAction(false),_unitsBox(0),_activeUnitFrame(0), _aggressiveModeLabel(0),
		_abilityQlabel(0),_abilityWlabel(0),_abilityElabel(0),_abilityRlabel(0),_abilityQtooltip(0),_abilityWtooltip(0),_abilityEtooltip(0),_abilityRtooltip(0),_aggressiveModeTooltip(0),
		_abilityUsabilityChanged(false),_changeCursor(false),_costLabel(0),_lastEntityUnderMouse(0),_currentModeLabel(0)
	{
	}


	//--------------------------------------------------------
	
	CActiveUnitController::~CActiveUnitController()
	{
		deactivate();
	} // ~CActiveUnitController

	//--------------------------------------------------------

	void CActiveUnitController::awake()
	{		
		_activeUnit=nullptr;
		_cursorsPerType["Rock"] = "BadPrincessHud/GatherStoneIronCursor";
		_cursorsPerType["Iron"] = "BadPrincessHud/GatherStoneIronCursor";
		_cursorsPerType["Tree"] = "BadPrincessHud/GatherWoodCursor";
		_cursorsPerType["WellBuilding"] = "BadPrincessHud/RepairCursor";
		_cursorsPerType["PotBuilding"] = "BadPrincessHud/RepairCursor";
		_cursorsPerType["Well"] = "BadPrincessHud/InsideWellCursor";
		_currentCursor = "BadPrincessHud/PointCursor";

		_spendingAbilitiesPerType["WorkmanGoblin"]["Q"] = "Well";

		_spendingAbilitiesPerType["RangerGoblin"]["Q"] = "DamageTrap";
		_spendingAbilitiesPerType["RangerGoblin"]["W"] = "MudTrap";
		_spendingAbilitiesPerType["RangerGoblin"]["E"] = "ChestTrap";

		_spendingAbilitiesPerType["Well"]["Q"] = "WorkmanGoblin";
		_spendingAbilitiesPerType["Well"]["W"] = "WarriorGoblin";
		_spendingAbilitiesPerType["Well"]["E"] = "RangerGoblin";

		initAbilitiesUsability();

	}

	//--------------------------------------------------------

	void CActiveUnitController::activate()
	{		
		CInputManager::getSingletonPtr()->addKeyListener(this);
		CInputManager::getSingletonPtr()->addMouseListener(this);		

		_unitsBox = CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChild("Units");
		uninteractiveElements.push_back(_unitsBox);

		_activeUnitFrame = CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChild("ActiveUnitFrame");
		uninteractiveElements.push_back(_activeUnitFrame);

		_activeUnitPortrait = CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChild("ActiveUnitPortrait");
		uninteractiveElements.push_back(_activeUnitPortrait);

		_activeUnitType = CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChild("ActiveUnitType");
		uninteractiveElements.push_back(_activeUnitType);

		_activeUnitHealthBar = (CEGUI::ProgressBar*)CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChild("ActiveUnitHealthBar");
		uninteractiveElements.push_back(_activeUnitHealthBar);  

		_activeUnitHealth = _activeUnitHealthBar->getChild("ActiveUnitHealth");
		uninteractiveElements.push_back(_activeUnitHealth);

		
		_abilityQlabel = CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChild("AbilityQLabel");
		uninteractiveElements.push_back(_abilityQlabel);
		_abilityQ = (CEGUI::PushButton*)CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChild("AbilityQ");
		interactiveElements.push_back(_abilityQ);

		_abilityWlabel = CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChild("AbilityWLabel");
		uninteractiveElements.push_back(_abilityWlabel);
		_abilityW = (CEGUI::PushButton*)CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChild("AbilityW");
		interactiveElements.push_back(_abilityW);

		_abilityElabel = CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChild("AbilityELabel");
		uninteractiveElements.push_back(_abilityElabel);
		_abilityE = (CEGUI::PushButton*)CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChild("AbilityE");		
		interactiveElements.push_back(_abilityE);

		_abilityRlabel = CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChild("AbilityRLabel");
		uninteractiveElements.push_back(_abilityRlabel);
		_abilityR = (CEGUI::PushButton*)CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChild("AbilityR");
		interactiveElements.push_back(_abilityR);

		_aggressiveModeLabel = CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChild("AggressiveModeLabel");
		uninteractiveElements.push_back(_aggressiveModeLabel);
		_currentModeLabel = CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChild("CurrentModeLabel");
		uninteractiveElements.push_back(_currentModeLabel);
		_aggressiveMode = (CEGUI::PushButton*)CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChild("AggressiveButton");
		interactiveElements.push_back(_aggressiveMode);
		_passiveMode = (CEGUI::PushButton*)CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChild("PassiveButton");
		interactiveElements.push_back(_passiveMode);

		_aggressiveModeTooltip = CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChild("AggressiveModeTooltip");
		_passiveModeTooltip = CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChild("PassiveModeTooltip");

		for(CEGUI::Window* element : uninteractiveElements)
		{
			element->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::SubscriberSlot(&CActiveUnitController::handleMouseClick, this));		
			element->subscribeEvent(CEGUI::Window::EventMouseDoubleClick, CEGUI::SubscriberSlot(&CActiveUnitController::handleMouseClick, this));		
			element->subscribeEvent(CEGUI::Window::EventMouseTripleClick, CEGUI::SubscriberSlot(&CActiveUnitController::handleMouseClick, this));		
		}

		_abilityQ->subscribeEvent(CEGUI::PushButton::EventMouseEntersArea, CEGUI::SubscriberSlot(&CActiveUnitController::showTooltipQ, this));		
		_abilityQ->subscribeEvent(CEGUI::PushButton::EventMouseLeavesArea, CEGUI::SubscriberSlot(&CActiveUnitController::hideTooltipQ, this));		
		_abilityQ->subscribeEvent(CEGUI::PushButton::EventMouseButtonDown, CEGUI::SubscriberSlot(&CActiveUnitController::useAbilityQ, this));		
		_abilityQ->subscribeEvent(CEGUI::PushButton::EventMouseDoubleClick, CEGUI::SubscriberSlot(&CActiveUnitController::useAbilityQ, this));
		_abilityQ->subscribeEvent(CEGUI::PushButton::EventMouseTripleClick, CEGUI::SubscriberSlot(&CActiveUnitController::useAbilityQ, this));

		_abilityW->subscribeEvent(CEGUI::PushButton::EventMouseEntersArea, CEGUI::SubscriberSlot(&CActiveUnitController::showTooltipW, this));		
		_abilityW->subscribeEvent(CEGUI::PushButton::EventMouseLeavesArea, CEGUI::SubscriberSlot(&CActiveUnitController::hideTooltipW, this));
		_abilityW->subscribeEvent(CEGUI::PushButton::EventMouseButtonDown, CEGUI::SubscriberSlot(&CActiveUnitController::useAbilityW, this));		
		_abilityW->subscribeEvent(CEGUI::PushButton::EventMouseDoubleClick, CEGUI::SubscriberSlot(&CActiveUnitController::useAbilityW, this));
		_abilityW->subscribeEvent(CEGUI::PushButton::EventMouseTripleClick, CEGUI::SubscriberSlot(&CActiveUnitController::useAbilityW, this));

		_abilityE->subscribeEvent(CEGUI::PushButton::EventMouseEntersArea, CEGUI::SubscriberSlot(&CActiveUnitController::showTooltipE, this));		
		_abilityE->subscribeEvent(CEGUI::PushButton::EventMouseLeavesArea, CEGUI::SubscriberSlot(&CActiveUnitController::hideTooltipE, this));
		_abilityE->subscribeEvent(CEGUI::PushButton::EventMouseButtonDown, CEGUI::SubscriberSlot(&CActiveUnitController::useAbilityE, this));		
		_abilityE->subscribeEvent(CEGUI::PushButton::EventMouseDoubleClick, CEGUI::SubscriberSlot(&CActiveUnitController::useAbilityE, this));
		_abilityE->subscribeEvent(CEGUI::PushButton::EventMouseTripleClick, CEGUI::SubscriberSlot(&CActiveUnitController::useAbilityE, this));

		_abilityR->subscribeEvent(CEGUI::PushButton::EventMouseEntersArea, CEGUI::SubscriberSlot(&CActiveUnitController::showTooltipR, this));		
		_abilityR->subscribeEvent(CEGUI::PushButton::EventMouseLeavesArea, CEGUI::SubscriberSlot(&CActiveUnitController::hideTooltipR, this));
		_abilityR->subscribeEvent(CEGUI::PushButton::EventMouseButtonDown, CEGUI::SubscriberSlot(&CActiveUnitController::useAbilityR, this));		
		_abilityR->subscribeEvent(CEGUI::PushButton::EventMouseDoubleClick, CEGUI::SubscriberSlot(&CActiveUnitController::useAbilityR, this));
		_abilityR->subscribeEvent(CEGUI::PushButton::EventMouseTripleClick, CEGUI::SubscriberSlot(&CActiveUnitController::useAbilityR, this));

		_aggressiveMode->subscribeEvent(CEGUI::PushButton::EventMouseEntersArea, CEGUI::SubscriberSlot(&CActiveUnitController::showAgressiveModeTooltip, this));		
		_aggressiveMode->subscribeEvent(CEGUI::PushButton::EventMouseLeavesArea, CEGUI::SubscriberSlot(&CActiveUnitController::hideAgressiveModeTooltip, this));
		_passiveMode->subscribeEvent(CEGUI::PushButton::EventMouseEntersArea, CEGUI::SubscriberSlot(&CActiveUnitController::showPassiveModeTooltip, this));		
		_passiveMode->subscribeEvent(CEGUI::PushButton::EventMouseLeavesArea, CEGUI::SubscriberSlot(&CActiveUnitController::hidePassiveModeTooltip, this));
		_aggressiveMode->subscribeEvent(CEGUI::PushButton::EventMouseButtonDown, CEGUI::SubscriberSlot(&CActiveUnitController::setAggressiveMode, this));		
		_aggressiveMode->subscribeEvent(CEGUI::PushButton::EventMouseDoubleClick, CEGUI::SubscriberSlot(&CActiveUnitController::setAggressiveMode, this));	
		_aggressiveMode->subscribeEvent(CEGUI::PushButton::EventMouseTripleClick, CEGUI::SubscriberSlot(&CActiveUnitController::setAggressiveMode, this));	
		_passiveMode->subscribeEvent(CEGUI::PushButton::EventMouseButtonDown, CEGUI::SubscriberSlot(&CActiveUnitController::setPassiveMode, this));		
		_passiveMode->subscribeEvent(CEGUI::PushButton::EventMouseDoubleClick, CEGUI::SubscriberSlot(&CActiveUnitController::setPassiveMode, this));	
		_passiveMode->subscribeEvent(CEGUI::PushButton::EventMouseTripleClick, CEGUI::SubscriberSlot(&CActiveUnitController::setPassiveMode, this));	

		if(_activeUnit)
			activateElements();
		else
			deactivateElements();

	} // activate

	//--------------------------------------------------------

	void CActiveUnitController::deactivate()
	{
		CInputManager::getSingletonPtr()->removeKeyListener(this);
		CInputManager::getSingletonPtr()->removeMouseListener(this);
		uninteractiveElements.clear();
		interactiveElements.clear();
	} // deactivate

	void CActiveUnitController::reset()
	{
		initAbilitiesUsability();
		_currentCursor = "BadPrincessHud/PointCursor";
	}
	//--------------------------------------------------------

	void CActiveUnitController::tick() 
	{
		setMovingMouseCursor(_mouseState);
		if(_activeUnit)
		{		

			

			

			if(_lifeComponent)
			{
				int currentLife = (int)_lifeComponent->getCurrentLife() > 0 ? (int)_lifeComponent->getCurrentLife() : 0;
				std::string health =  std::to_string(currentLife) + " / " + std::to_string((int)_lifeComponent->getMaxLife());
				_activeUnitHealth->setText(health);
				_activeUnitHealthBar->setProgress(currentLife / _lifeComponent->getMaxLife());
			}
			if(_abilityUsabilityChanged)
			{

				checkAbilityQEnabled();

				checkAbilityWEnabled();

				checkAbilityEEnabled();

				checkAbilityREnabled();
			
				_abilityUsabilityChanged = false;
			}
			
			checkResourceAviability("Q")?_abilityQ->enable() : _abilityQ->disable();
			checkResourceAviability("W")?_abilityW->enable() : _abilityW->disable();
			checkResourceAviability("E")?_abilityE->enable() : _abilityE->disable();
			checkResourceAviability("R")?_abilityR->enable() : _abilityR->disable();

		}
		else
			restoreMouseCursor();
	}
	//--------------------------------------------------------

	bool CActiveUnitController::keyPressed(TKey key){
		
		bool pressed = false;
		if (_activeUnit)
		{
			switch (key.keyId)
			{
				case GUI::Key::Q:			
					if(!_abilityQ->isDisabled())
					{
						//if(_activeUnit->getType().compare("WorkmanGoblin")==0)
						//{
						//	if(!AI::ResourceServer::getInstance()->checkResourceAvailability("WellHelper"))
						//		break;
						//}
						sendUseAbilityMessage("Q");				
						_abilityQ->setPushedState(true);
						pressed = true;
					}		
					else
					{
						AI::ResourceServer::getInstance()->checkResourceAvailability(_spendingAbilitiesPerType[_activeUnit->getType()]["Q"],1.0f,true);
					}
					break;
				case GUI::Key::W:
					if (!_abilityW->isDisabled())
					{
						sendUseAbilityMessage("W");		
						_abilityW->setPushedState(true);
						pressed = true;
					}
					else
					{
						AI::ResourceServer::getInstance()->checkResourceAvailability(_spendingAbilitiesPerType[_activeUnit->getType()]["W"],1.0f,true);
					}
					break;
				case GUI::Key::E:
					if (!_abilityE->isDisabled())
					{
						sendUseAbilityMessage("E");		
						_abilityE->setPushedState(true);
						pressed = true;
					}
					else
					{
						AI::ResourceServer::getInstance()->checkResourceAvailability(_spendingAbilitiesPerType[_activeUnit->getType()]["E"],1.0f,true);
					}
					break;

				case GUI::Key::R:
					if (!_abilityR->isDisabled())
					{
						sendUseAbilityMessage("R");		
						_abilityR->setPushedState(true);
						pressed = true;
					}
					else
					{
						AI::ResourceServer::getInstance()->checkResourceAvailability(_spendingAbilitiesPerType[_activeUnit->getType()]["R"],1.0f,true);
					}
					break;
				default:
					pressed = false;
					break;
			}
		}

		return pressed;

	} // keyPressed

	//--------------------------------------------------------

	bool CActiveUnitController::keyReleased(TKey key){

		bool released = false;
		switch (key.keyId)
		{
			case GUI::Key::Q:			
				if (_activeUnit)
				{
					released = true;
					_abilityQ->setPushedState(false);
				}
				break;
			case GUI::Key::W:
				if (_activeUnit)
				{
					released = true;
					_abilityW->setPushedState(false);
				}
				break;
			case GUI::Key::E:
				if (_activeUnit)
				{
					released = true;
					_abilityE->setPushedState(false);
				}
				break;

			case GUI::Key::R:
				if (_activeUnit)
				{
					released = true;
					_abilityR->setPushedState(false);
				}
				break;
			default:
				released = false;
				break;
		}


		return released;

	} // keyReleased

	//--------------------------------------------------------

	void CActiveUnitController::activateElements()
	{
		_enemyDetectionComponent = static_cast<Logic::CEnemyDetectionManager*>(_activeUnit->getComponent("CEnemyDetectionManager"));	
		_lifeComponent = static_cast<Logic::CLife*>(_activeUnit->getComponent("CLife"));
		_GUIManagerComponent = static_cast<Logic::CGUIManager*>(_activeUnit->getComponent("CGUIManager"));

		_activeUnitPortrait->setProperty("Image", _GUIManagerComponent->getPortraitName());

		std::string type = _activeUnit->getType();
		std::string subs = "Goblin";		
		std::string::size_type i = type.find(subs);

		if (i != std::string::npos)
		   type.erase(i, subs.length());

		_activeUnitType->setText(type);

		std::string health =  std::to_string((int)_lifeComponent->getCurrentLife()) + " / " + std::to_string((int)_lifeComponent->getMaxLife());
		_activeUnitHealth->setText(health);
		_activeUnitHealthBar->setProgress( _lifeComponent->getCurrentLife() / _lifeComponent->getMaxLife());
		CEGUI::Colour c(_lifeComponent->getLifebarColorR(), _lifeComponent->getLifebarColorG(), _lifeComponent->getLifebarColorB(), 1.0);   // R,G,B,A...
		CEGUI::ColourRect colorHP(c);         // Set colors for all 4 corners of rectangle...
		_activeUnitHealthBar->setProperty("CustomRectColor", CEGUI::PropertyHelper<CEGUI::ColourRect>::toString(colorHP));

		activateElement(_activeUnitPortrait);
		activateElement(_activeUnitType);
		activateElement(_activeUnitHealth);
		activateElement(_activeUnitHealthBar);

		if(_GUIManagerComponent->getAbilityQName() != "")
		{
			activateElement(_abilityQlabel);
			setNameAbilityQ( _GUIManagerComponent->getAbilityQName());		

			activateElement(_abilityQ);

			setTooltipAbilityQ(_GUIManagerComponent->getAbilityQTooltip(),_GUIManagerComponent->getAbilityQLuaField());
			
			checkAbilityQEnabled();

		}
		else
		{
			deactivateElement(_abilityQlabel);
			deactivateElement(_abilityQ);
		}

		if(_GUIManagerComponent->getAbilityWName() != "")
		{
			activateElement(_abilityWlabel);
			setNameAbilityW( _GUIManagerComponent->getAbilityWName());
			activateElement(_abilityW);

			setTooltipAbilityW(_GUIManagerComponent->getAbilityWTooltip(),_GUIManagerComponent->getAbilityWLuaField());

			checkAbilityWEnabled();
		}		
		else
		{
			deactivateElement(_abilityWlabel);
			deactivateElement(_abilityW);
		}

		if(_GUIManagerComponent->getAbilityEName() != "")
		{
			activateElement(_abilityElabel);
			setNameAbilityE( _GUIManagerComponent->getAbilityEName());
			activateElement(_abilityE);

			setTooltipAbilityE(_GUIManagerComponent->getAbilityETooltip(),_GUIManagerComponent->getAbilityELuaField());

			checkAbilityEEnabled();
		}		
		else
		{
			deactivateElement(_abilityElabel);
			deactivateElement(_abilityE);
		};

		if(_GUIManagerComponent->getAbilityRName() != "")
		{
			activateElement(_abilityRlabel);
			setNameAbilityR( _GUIManagerComponent->getAbilityRName());	
			
			activateElement(_abilityR);
			
			setTooltipAbilityR(_GUIManagerComponent->getAbilityRTooltip(),_GUIManagerComponent->getAbilityRLuaField());

			checkAbilityREnabled();
		}
		else
		{
			deactivateElement(_abilityRlabel);
			deactivateElement(_abilityR);
		}

		if(_enemyDetectionComponent)
		{
			activateElement(_aggressiveModeLabel);
			activateElement(_aggressiveMode);
			activateElement(_passiveMode);
			if(_enemyDetectionComponent->getIsAggressive())
			{
				_aggressiveMode->setProperty("NormalImage", "BadPrincessHud2/AggressiveModeDown");
				_aggressiveMode->setProperty("AlwaysOnTop", "true");
				_currentModeLabel->setProperty("Text", "Aggressive Stand");	
				_passiveMode->setProperty("NormalImage", "BadPrincessHud/PassiveMode");				
				
			}
			else
			{
				_aggressiveMode->setProperty("NormalImage", "BadPrincessHud/AggressiveMode");
				_aggressiveMode->setProperty("AlwaysOnTop", "true");
				_currentModeLabel->setProperty("Text", "Passive Stand");	
				_passiveMode->setProperty("NormalImage", "BadPrincessHud2/PassiveModeDown");				
									
			}
			
		}
		else
		{
			deactivateElement(_aggressiveModeLabel);
			deactivateElement(_aggressiveMode);
			deactivateElement(_passiveMode);
		}
	}
	
	//--------------------------------------------------------

	void CActiveUnitController::setNameAbilityQ(std::string name)
	{
		setNameAbility(_abilityQ,name);
	}

	//--------------------------------------------------------

	void CActiveUnitController::setTooltipAbilityQ(std::string name, std::string luaField)
	{
		if(name!="")
		{

			//to mantain the consistance of enter/leave button
			bool oldIsVisible = false;
			if(_abilityQtooltip!=nullptr)
			{
				oldIsVisible = _abilityQtooltip->isVisible();
				_abilityQtooltip->setVisible(false);
			}
			_abilityQtooltip = CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChild(name);

			if(_activeUnit->getType() == "Princess")
			{
				int wrath = ((Logic::CPrincessManager*)(_activeUnit->getComponent("CPrincessManager")))->getAoeStunCost();
				_costLabel =_abilityQtooltip->getChild("WrathCostLabel");
				_costLabel->setText(std::to_string(wrath));
			}
			else if(luaField != "")
			{
				int wood, stone, iron = 0;

				wood = AI::ResourceServer::getInstance()->getRequiredQuantity(luaField.c_str(),"Wood");
				stone = AI::ResourceServer::getInstance()->getRequiredQuantity(luaField.c_str(),"Stone");
				iron = AI::ResourceServer::getInstance()->getRequiredQuantity(luaField.c_str(),"Iron");

				if(wood > 0)
				{
					_costLabel =_abilityQtooltip->getChild("WoodCostLabel");
					_costLabel->setText(std::to_string(wood));
				}

				if(stone > 0)
				{
					_costLabel =_abilityQtooltip->getChild("StoneCostLabel");
					_costLabel->setText(std::to_string(stone));
				}

				if(iron > 0)
				{
					_costLabel =_abilityQtooltip->getChild("IronCostLabel");
					_costLabel->setText(std::to_string(iron));
				}

			}

			_abilityQtooltip->setVisible(oldIsVisible);
			_abilityQtooltip->activate();

		}else
		{
			if(_abilityQtooltip!=nullptr)
			{
				_abilityQtooltip->setVisible(false);
				_abilityQtooltip = nullptr;
			}
		}
	}

	//--------------------------------------------------------

	void CActiveUnitController::setNameAbilityW(std::string name)
	{
		setNameAbility(_abilityW,name);
	}
	
	//--------------------------------------------------------

	void CActiveUnitController::setTooltipAbilityW(std::string name, std::string luaField)
	{
		if(name!="")
		{
			
			bool oldIsVisible = false;
			if(_abilityWtooltip!=nullptr)
			{
				oldIsVisible = _abilityWtooltip->isVisible();
				_abilityWtooltip->setVisible(false);
			}
			_abilityWtooltip = CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChild(name);

			if(_activeUnit->getType() == "Princess")
			{
				int wrath = ((Logic::CPrincessManager*)(_activeUnit->getComponent("CPrincessManager")))->getAoeBuffCost();
				_costLabel =_abilityWtooltip->getChild("WrathCostLabel");
				_costLabel->setText(std::to_string(wrath));
			}
			else if(luaField != "")
			{
				int wood, stone, iron = 0;

				wood = AI::ResourceServer::getInstance()->getRequiredQuantity(luaField.c_str(),"Wood");
				stone = AI::ResourceServer::getInstance()->getRequiredQuantity(luaField.c_str(),"Stone");
				iron = AI::ResourceServer::getInstance()->getRequiredQuantity(luaField.c_str(),"Iron");

				if(wood > 0)
				{
					_costLabel =_abilityWtooltip->getChild("WoodCostLabel");
					_costLabel->setText(std::to_string(wood));
				}

				if(stone > 0)
				{
					_costLabel =_abilityWtooltip->getChild("StoneCostLabel");
					_costLabel->setText(std::to_string(stone));
				}

				if(iron > 0)
				{
					_costLabel =_abilityWtooltip->getChild("IronCostLabel");
					_costLabel->setText(std::to_string(iron));
				}

			}

			_abilityWtooltip->setVisible(oldIsVisible);
			_abilityWtooltip->activate();

		}else
		{
			if(_abilityWtooltip!=nullptr)
			{
				_abilityWtooltip->setVisible(false);
				_abilityWtooltip = nullptr;
			}
		}
	}
	//--------------------------------------------------------

	void CActiveUnitController::setNameAbilityE(std::string name)
	{
		setNameAbility(_abilityE,name);
	}

	//--------------------------------------------------------

	void CActiveUnitController::setTooltipAbilityE(std::string name, std::string luaField)
	{
		if(name!="")
		{
			bool oldIsVisible = false;
			if(_abilityEtooltip!=nullptr)
			{
				oldIsVisible = _abilityEtooltip->isVisible();
				_abilityEtooltip->setVisible(false);
			}
			_abilityEtooltip = CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChild(name);

			if(_activeUnit->getType() == "Princess")
			{
				int wrath = ((Logic::CPrincessManager*)(_activeUnit->getComponent("CPrincessManager")))->getAoeHealCost();
				_costLabel =_abilityEtooltip->getChild("WrathCostLabel");
				_costLabel->setText(std::to_string(wrath));
			}
			else if(luaField != "")
			{
				int wood, stone, iron = 0;

				wood = AI::ResourceServer::getInstance()->getRequiredQuantity(luaField.c_str(),"Wood");
				stone = AI::ResourceServer::getInstance()->getRequiredQuantity(luaField.c_str(),"Stone");
				iron = AI::ResourceServer::getInstance()->getRequiredQuantity(luaField.c_str(),"Iron");

				if(wood > 0)
				{
					_costLabel =_abilityEtooltip->getChild("WoodCostLabel");
					_costLabel->setText(std::to_string(wood));
				}

				if(stone > 0)
				{
					_costLabel =_abilityEtooltip->getChild("StoneCostLabel");
					_costLabel->setText(std::to_string(stone));
				}

				if(iron > 0)
				{
					_costLabel =_abilityEtooltip->getChild("IronCostLabel");
					_costLabel->setText(std::to_string(iron));
				}

			}

			_abilityEtooltip->setVisible(oldIsVisible);
			_abilityEtooltip->activate();
		}else
		{
			if(_abilityEtooltip!=nullptr)
			{
				_abilityEtooltip->setVisible(false);
				_abilityEtooltip = nullptr;
			}
		}
	}
	//--------------------------------------------------------

	void CActiveUnitController::setNameAbilityR(std::string name)
	{
		setNameAbility(_abilityR,name);
	}

	//--------------------------------------------------------

	void CActiveUnitController::setTooltipAbilityR(std::string name, std::string luaField)
	{
		if(name!="")
		{
			bool oldIsVisible = false;
			if(_abilityRtooltip!=nullptr)
			{
				oldIsVisible = _abilityRtooltip->isVisible();
				_abilityRtooltip->setVisible(false);
			}
			_abilityRtooltip = CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChild(name);

			if(_activeUnit->getType() == "Princess")
			{
				int wrath = ((Logic::CPrincessManager*)(_activeUnit->getComponent("CPrincessManager")))->getBerserkerModeCost();
				_costLabel =_abilityRtooltip->getChild("WrathCostLabel");
				_costLabel->setText(std::to_string(wrath));
			}
			else if(luaField != "")
			{
				int wood, stone, iron = 0;

				wood = AI::ResourceServer::getInstance()->getRequiredQuantity(luaField.c_str(),"Wood");
				stone = AI::ResourceServer::getInstance()->getRequiredQuantity(luaField.c_str(),"Stone");
				iron = AI::ResourceServer::getInstance()->getRequiredQuantity(luaField.c_str(),"Iron");

				if(wood > 0)
				{
					_costLabel =_abilityRtooltip->getChild("WoodCostLabel");
					_costLabel->setText(std::to_string(wood));
				}

				if(stone > 0)
				{
					_costLabel =_abilityRtooltip->getChild("StoneCostLabel");
					_costLabel->setText(std::to_string(stone));
				}

				if(iron > 0)
				{
					_costLabel =_abilityRtooltip->getChild("IronCostLabel");
					_costLabel->setText(std::to_string(iron));
				}

			}
			

			_abilityRtooltip->setVisible(oldIsVisible);
			_abilityRtooltip->activate();
		}else
		{
			if(_abilityRtooltip!=nullptr)
			{
				_abilityRtooltip->setVisible(false);
				_abilityRtooltip = nullptr;
			}
		}
	}

	//--------------------------------------------------------

	void CActiveUnitController::setNameAbility(CEGUI::PushButton* button, std::string name)
	{
		button->setProperty("NormalImage", name);
		button->setProperty("PushedImage", name + "Down");
		button->setProperty("DisabledImage", name + "Down");
		button->setProperty("HoverImage", name + "Hover");
		button->enable();
	}


	//--------------------------------------------------------

	void CActiveUnitController::activateElement(CEGUI::Window* element)
	{
		if(!element->isVisible())
			element->setVisible(true);

		if(!element->isActive())
			element->activate();			
	}

	//--------------------------------------------------------

	void CActiveUnitController::deactivateElements()
	{		
		_enemyDetectionComponent = NULL;
		_GUIManagerComponent = NULL;
		_lifeComponent = NULL;

		deactivateElement(_activeUnitPortrait);
		deactivateElement(_activeUnitType);
		deactivateElement(_activeUnitHealth);
		deactivateElement(_activeUnitHealthBar);
		deactivateElement(_abilityQlabel);
		deactivateElement(_abilityQ);
		deactivateElement(_abilityQtooltip);
		deactivateElement(_abilityWlabel);
		deactivateElement(_abilityW);
		deactivateElement(_abilityWtooltip);
		deactivateElement(_abilityElabel);
		deactivateElement(_abilityE);
		deactivateElement(_abilityEtooltip);
		deactivateElement(_abilityRlabel);
		deactivateElement(_abilityR);
		deactivateElement(_abilityRtooltip);
		deactivateElement(_aggressiveModeLabel);
		deactivateElement(_aggressiveMode);
		deactivateElement(_passiveMode);
		deactivateElement(_currentModeLabel);
	}
	
	//--------------------------------------------------------

	void CActiveUnitController::deactivateElement(CEGUI::Window* element)
	{
		if(element)
		{
			if(element->isVisible())
				element->setVisible(false);
			if(element->isActive())
				element->deactivate();		
		}
	}


	//--------------------------------------------------------

	bool CActiveUnitController::useAbilityQ(const CEGUI::EventArgs &e)
	{	
		if(!clickAction)
		{
			clickAction=true;
			sendUseAbilityMessage("Q");
		}
		return true;
	}

	//--------------------------------------------------------

	bool CActiveUnitController::showTooltipQ(const CEGUI::EventArgs &e)
	{	

			_abilityQtooltip->setVisible(true);
		return true;
	}

	//--------------------------------------------------------

	bool CActiveUnitController::hideTooltipQ(const CEGUI::EventArgs &e)
	{	

			_abilityQtooltip->setVisible(false);
		return true;
	}

	//--------------------------------------------------------
	
	bool CActiveUnitController::useAbilityW(const CEGUI::EventArgs &e)
	{		
		if(!clickAction)
		{
			clickAction=true;
			sendUseAbilityMessage("W");	
		}
		return true;
	}

	//--------------------------------------------------------

	bool CActiveUnitController::showTooltipW(const CEGUI::EventArgs &e)
	{	

			_abilityWtooltip->setVisible(true);

		return true;
	}
	
	//--------------------------------------------------------

	bool CActiveUnitController::hideTooltipW(const CEGUI::EventArgs &e)
	{	

			_abilityWtooltip->setVisible(false);
		return true;
	}

	//--------------------------------------------------------
	
	bool CActiveUnitController::useAbilityE(const CEGUI::EventArgs &e)
	{		
		if(!clickAction)
		{
			clickAction=true;
			sendUseAbilityMessage("E");
		}
		return true;
	}

	//--------------------------------------------------------

	bool CActiveUnitController::showTooltipE(const CEGUI::EventArgs &e)
	{	

			_abilityEtooltip->setVisible(true);

		return true;
	}
	
	//--------------------------------------------------------

	bool CActiveUnitController::hideTooltipE(const CEGUI::EventArgs &e)
	{	

			_abilityEtooltip->setVisible(false);
		return true;
	}

	//--------------------------------------------------------
	
	bool CActiveUnitController::useAbilityR(const CEGUI::EventArgs &e)
	{		
		if(!clickAction)
		{
			clickAction=true;
			sendUseAbilityMessage("R");
		}
		return true;
	}

	//--------------------------------------------------------

	bool CActiveUnitController::showTooltipR(const CEGUI::EventArgs &e)
	{	

			_abilityRtooltip->setVisible(true);

		return true;
	}
	
	//--------------------------------------------------------

	bool CActiveUnitController::hideTooltipR(const CEGUI::EventArgs &e)
	{	

			_abilityRtooltip->setVisible(false);
		return true;
	}

	//--------------------------------------------------------

	void CActiveUnitController::sendUseAbilityMessage(std::string ability)
	{		
		if(existsInAbilitiesUsability(_activeUnit->getType(),ability) && _abilitiesUsability[_activeUnit->getType()][ability])
		{
			auto message = std::make_shared<CastAbility>();
			message->ability = ability;
			_activeUnit->emitMessageN(message);

			Logic::CServer::getSingletonPtr()->getMap()->getEntityByName("GameManager")->emitMessageN(message);
		}
	}

	//--------------------------------------------------------
	
	bool CActiveUnitController::setAggressiveMode(const CEGUI::EventArgs &e)
	{				
		if(!_enemyDetectionComponent->getIsAggressive())
		{
			_enemyDetectionComponent->setIsAggressive(true);
			_aggressiveMode->setProperty("NormalImage", "BadPrincessHud2/AggressiveModeDown");
			_passiveMode->setProperty("NormalImage", "BadPrincessHud/PassiveMode");
			_currentModeLabel->setProperty("Text", "Aggressive Stand");				
		}
		GUI::CServer::getSingletonPtr()->getSelectedUnitsController()->setSelectedUnitsStandMode(true);

		clickAction=true;
		return true;
	}

		//--------------------------------------------------------
	
	bool CActiveUnitController::setPassiveMode(const CEGUI::EventArgs &e)
	{				
		if(_enemyDetectionComponent->getIsAggressive())
		{
			_enemyDetectionComponent->setIsAggressive(false);
			_passiveMode->setProperty("NormalImage", "BadPrincessHud2/PassiveModeDown");
			_aggressiveMode->setProperty("NormalImage", "BadPrincessHud/AggressiveMode");			
			_currentModeLabel->setProperty("Text", "Passive Stand");				
		}

		GUI::CServer::getSingletonPtr()->getSelectedUnitsController()->setSelectedUnitsStandMode(false);
		clickAction=true;
		return true;
	}

	//--------------------------------------------------------

	bool CActiveUnitController::showAgressiveModeTooltip(const CEGUI::EventArgs &e)
	{	

			_aggressiveModeTooltip->setVisible(true);

		return true;
	}

	//--------------------------------------------------------

	bool CActiveUnitController::hideAgressiveModeTooltip(const CEGUI::EventArgs &e)
	{	

			_aggressiveModeTooltip->setVisible(false);
		return true;
	}
	//--------------------------------------------------------
	
	bool CActiveUnitController::showPassiveModeTooltip(const CEGUI::EventArgs &e)
	{	

			_passiveModeTooltip->setVisible(true);

		return true;
	}

	//--------------------------------------------------------

	bool CActiveUnitController::hidePassiveModeTooltip(const CEGUI::EventArgs &e)
	{	

			_passiveModeTooltip->setVisible(false);
		return true;
	}
	//--------------------------------------------------------
	
	bool CActiveUnitController::mouseMoved(const CMouseState &mouseState,const bool alreadyManaged)
	{	
		_mouseState = mouseState;

		setMouseCursor(_currentCursor);

		return false;		

	} // mouseMoved

	//--------------------------------------------------------
		
	bool CActiveUnitController::mousePressed(const CMouseState &mouseState,const bool alreadyManaged)
	{

		CEGUI::Vector2f mousePos = CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().getPosition();

		bool isHit = false;
		for(CEGUI::Window *element : interactiveElements)
		{
			if(element->isHit(mousePos,true))
			{
				isHit = true;

				if(element == _abilityQ)
				{
					if(!checkResourceAviability("Q"))
						AI::ResourceServer::getInstance()->checkResourceAvailability(_spendingAbilitiesPerType[_activeUnit->getType()]["Q"],1.0f,true);
				}
				else if(element == _abilityW)
				{
					if(!checkResourceAviability("W"))
						AI::ResourceServer::getInstance()->checkResourceAvailability(_spendingAbilitiesPerType[_activeUnit->getType()]["W"],1.0f,true);
				}
				else if(element == _abilityE)
				{
					if(!checkResourceAviability("E"))
						AI::ResourceServer::getInstance()->checkResourceAvailability(_spendingAbilitiesPerType[_activeUnit->getType()]["E"],1.0f,true);
				}
				else if(element == _abilityR)
				{
					if(!checkResourceAviability("R"))
						AI::ResourceServer::getInstance()->checkResourceAvailability(_spendingAbilitiesPerType[_activeUnit->getType()]["R"],1.0f,true);
				}
				break;
			}
		}


		if(!isHit)
		{
			for(CEGUI::Window *element : uninteractiveElements)
			{
				if(element->isHit(mousePos))
				{
					isHit = true;
					break;
				}
			}
		}

		return isHit;
	} // mousePressed

	//--------------------------------------------------------

	bool CActiveUnitController::mouseReleased(const CMouseState &mouseState,const bool alreadyManaged)
	{
		if(clickAction)
		{
			clickAction=false;
			return true;
		}else
			return false;
	} // mouseReleased

	//--------------------------------------------------------

	bool CActiveUnitController::handleMouseClick(const CEGUI::EventArgs &e)
	{
		if(!clickAction)
		{
			clickAction=true;
		}
		return true;
	}

	//--------------------------------------------------------

	void CActiveUnitController::initAbilitiesUsability()
	{
		_abilitiesUsability["Princess"]["Q"] = true;
		_abilitiesUsability["Princess"]["W"] = true;
		_abilitiesUsability["Princess"]["E"] = true;
		_abilitiesUsability["Princess"]["R"] = true;

		_abilitiesUsability["WorkmanGoblin"]["Q"] = true;
		_abilitiesUsability["WorkmanGoblin"]["W"] = true;
		_abilitiesUsability["WorkmanGoblin"]["E"] = true;
		
		_abilitiesUsability["RangerGoblin"]["Q"] = true;
		_abilitiesUsability["RangerGoblin"]["W"] = true;
		_abilitiesUsability["RangerGoblin"]["E"] = true;

		_abilitiesUsability["Well"]["Q"] = true;
		_abilitiesUsability["Well"]["W"] = true;
		_abilitiesUsability["Well"]["E"] = true;
		_abilitiesUsability["Well"]["R"] = true;
	}

	//--------------------------------------------------------

	void CActiveUnitController::setAbilityUsability(std::string type, std::string ability, bool usable)
	{
		if(existsInAbilitiesUsability(type,ability))
		{
			_abilitiesUsability[type][ability] = usable;
			_abilityUsabilityChanged = true;
		}
	}

	//--------------------------------------------------------

	bool CActiveUnitController::existsInAbilitiesUsability(std::string type, std::string ability)
	{
		return _abilitiesUsability.find(type) != _abilitiesUsability.end() && 
			_abilitiesUsability[type].find(ability) != _abilitiesUsability[type].end();
	}
	
	//--------------------------------------------------------

	bool CActiveUnitController::checkAbilityUsability(std::string name)
	{
		return (existsInAbilitiesUsability(_activeUnit->getType(),name) && _abilitiesUsability[_activeUnit->getType()][name] ) ;
	}
	
	//--------------------------------------------------------

	void CActiveUnitController::checkAbilityQEnabled()
	{
		(existsInAbilitiesUsability(_activeUnit->getType(),"Q") && _abilitiesUsability[_activeUnit->getType()]["Q"] ) ? _abilityQ->enable() : _abilityQ->disable();
	}
	void CActiveUnitController::checkAbilityWEnabled()
	{
		(existsInAbilitiesUsability(_activeUnit->getType(),"W") && _abilitiesUsability[_activeUnit->getType()]["W"] ) ? _abilityW->enable() : _abilityW->disable();
	}
	void CActiveUnitController::checkAbilityEEnabled()
	{
		(existsInAbilitiesUsability(_activeUnit->getType(),"E") && _abilitiesUsability[_activeUnit->getType()]["E"] ) ? _abilityE->enable() : _abilityE->disable();
	}
	void CActiveUnitController::checkAbilityREnabled()
	{
		(existsInAbilitiesUsability(_activeUnit->getType(),"R") && _abilitiesUsability[_activeUnit->getType()]["R"] ) ? _abilityR->enable() : _abilityR->disable();
	}

	//--------------------------------------------------------

	void CActiveUnitController::setMovingMouseCursor(const CMouseState &mouseState)
	{	
		Logic::CEntity* getEntity= GUI::CServer::getSingletonPtr()->getPlayerController()->getEntityUnderMouse(mouseState);

		if(getEntity)
		{
			if(_activeUnit)
			{
				if(getEntity->isEnemy())
				{

					_lastEntityUnderMouse = getEntity;
					auto m = std::make_shared<ToggleGlow>();	
					m->activate = true;
					_lastEntityUnderMouse->emitMessageN(m);

					GUI::CServer::getSingletonPtr()->setMouseCursor("BadPrincessHud/AttackCursor");
					_changeCursor = true;
				}
				//if the entity pointed by the mouse is a well and the unit  selected ara a goblin (only not a well and princess )
				else if(getEntity->getType() == "Well" && (_activeUnit->getType() != "Well" && _activeUnit->getType() != "Princess"))
				{
					Logic::CBuildComponent* buildComponent = static_cast<Logic::CBuildComponent*>(getEntity->getComponent("CBuildComponent"));	
					//if the selected entity is a goblin we have to check if it have to repair/build the well
					if (_activeUnit->getType() == "WorkmanGoblin")
					{
				
						Logic::CWorkmanState* workmanComponent = static_cast<Logic::CWorkmanState*>(_activeUnit->getComponent("CWorkmanState"));	
						if(!buildComponent->isBuilded() || workmanComponent->isRepairBuildingAbilityActive())
						{
							if(_lastEntityUnderMouse != getEntity)
							{
								_lastEntityUnderMouse = getEntity;
								auto m = std::make_shared<ToggleGlow>();	
								m->activate = true;
								_lastEntityUnderMouse->emitMessageN(m);
							}
							GUI::CServer::getSingletonPtr()->setMouseCursor(_cursorsPerType[getEntity->getType()+"Building"]);
							_changeCursor = true;
						}else
						{
							if(_lastEntityUnderMouse != getEntity)
							{
								_lastEntityUnderMouse = getEntity;
								auto m = std::make_shared<ToggleGlow>();
								m->activate = true;
								_lastEntityUnderMouse->emitMessageN(m);
							}

							GUI::CServer::getSingletonPtr()->setMouseCursor(_cursorsPerType[getEntity->getType()]);
							_changeCursor = true;
						}

					}else if(buildComponent->isBuilded())// all other entities setup the mouse pointer with jump into well if
						//the well is builded
					{
						if(_lastEntityUnderMouse != getEntity)
						{
							_lastEntityUnderMouse = getEntity;
							auto m = std::make_shared<ToggleGlow>();	
							m->activate = true;
							_lastEntityUnderMouse->emitMessageN(m);
						}

						GUI::CServer::getSingletonPtr()->setMouseCursor(_cursorsPerType[getEntity->getType()]);
						_changeCursor = true;
					}
				}			//if the entity pointed by the mouse is a kitchen and the unit  selected are a workman goblin
				else if(getEntity->getType() == "Pot" && _activeUnit->getType() == "WorkmanGoblin")
				{
					Logic::CBuildComponent* buildComponent = static_cast<Logic::CBuildComponent*>(getEntity->getComponent("CBuildComponent"));	
					if(!buildComponent->isBuilded())
					{
						if(_lastEntityUnderMouse != getEntity)
						{
							_lastEntityUnderMouse = getEntity;
							auto m = std::make_shared<ToggleGlow>();				
							m->activate = true;
							_lastEntityUnderMouse->emitMessageN(m);
						}
						GUI::CServer::getSingletonPtr()->setMouseCursor(_cursorsPerType[getEntity->getType()+"Building"]);
						_changeCursor = true;
					}

				}
				//if the pointed entity it isn't a well and the unit selected is a workmann change a pointer with the correct 
				//icon (here the remaining entity types pointed from cursor could be only resources and chest )
				else if (getEntity->getType() != "Well" && _activeUnit->getType() == "WorkmanGoblin")
				{
					if (_cursorsPerType.find(getEntity->getType()) != _cursorsPerType.end())
					{
						if(_lastEntityUnderMouse != getEntity)
						{
							_lastEntityUnderMouse = getEntity;
							auto m = std::make_shared<ToggleGlow>();	
							m->activate = true;
							_lastEntityUnderMouse->emitMessageN(m);
						}

						GUI::CServer::getSingletonPtr()->setMouseCursor(_cursorsPerType[getEntity->getType()]);
						_changeCursor = true;
					}
				}
			}
			if(_lastEntityUnderMouse != getEntity)
			{
				if(_lastEntityUnderMouse != NULL)
				{
					auto m = std::make_shared<ToggleGlow>();
					m->activate = false;
					_lastEntityUnderMouse->emitMessageN(m);
				}
				if (getEntity->isPlayer() || getEntity->getType() == "Well")
				{
					_lastEntityUnderMouse = getEntity;
					auto m = std::make_shared<ToggleGlow>();
					m->activate = true;
					_lastEntityUnderMouse->emitMessageN(m);
				}
			}
			
		}
		else
		{
			if(_changeCursor)
			{
				GUI::CServer::getSingletonPtr()->setMouseCursor("BadPrincessHud/PointCursor");
				_changeCursor = false;
			}
			if(_lastEntityUnderMouse)
			{
				auto m = std::make_shared<ToggleGlow>();
				m->activate = false;
				_lastEntityUnderMouse->emitMessageN(m);
				_lastEntityUnderMouse = NULL;
			}
		}
		//}
	}

	//--------------------------------------------------------

	void CActiveUnitController::setMouseCursor(std::string cursor)
	{
		_currentCursor = cursor;
		GUI::CServer::getSingletonPtr()->setMouseCursor(cursor);
	}

	//-------------------------------------------------------

	void CActiveUnitController::restoreMouseCursor()
	{
		GUI::CServer::getSingletonPtr()->setMouseCursor("BadPrincessHud/PointCursor");
		_currentCursor = "BadPrincessHud/PointCursor";
	}

	//-------------------------------------------------------

	bool CActiveUnitController::checkResourceAviability(std::string abilityName)
	{
		if(existsInAbilitiesUsability(_activeUnit->getType(),abilityName) && _abilitiesUsability[_activeUnit->getType()][abilityName] )
		{
			if(_spendingAbilitiesPerType.find(_activeUnit->getType()) != _spendingAbilitiesPerType.end() 
				&& _spendingAbilitiesPerType[_activeUnit->getType()].find(abilityName) != _spendingAbilitiesPerType[_activeUnit->getType()].end())
			{			
				return AI::ResourceServer::getInstance()->checkResourceAvailability(_spendingAbilitiesPerType[_activeUnit->getType()][abilityName],1.0f);
			}
			return true;
		}
		return false;
	}

	//--------------------------------------------------


} // namespace GUI
