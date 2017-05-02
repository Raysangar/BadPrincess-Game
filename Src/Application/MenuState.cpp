//---------------------------------------------------------------------------
// MenuState.cpp
//---------------------------------------------------------------------------

/**
@file MenuState.cpp

Contiene la implementación del estado de menú.

@see Application::CApplicationState
@see Application::CMenuState

@author David Llansó
@date Agosto, 2010
*/

#include "MenuState.h"
#include "LoadingGameState.h"


#include "BaseSubsystems\Server.h"

#include "Logic/Server.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"
#include "Logic/Entity/Entity.h"
#include "Logic\Entity\Components\CameraMenu.h"

#include "Graphics\Camera.h"
#include "Graphics/Scene.h"
#include "Graphics\FogOfWar.h"

#include "Physics/SceneManager.h"
#include "Physics/Server.h"

#include "GUI/Server.h"
#include <CEGUI/CEGUI.h>

#define TIME_FOR_SHOWING 1000

namespace Application {

	CMenuState::~CMenuState() 
	{
	} // ~CMenuState

	//--------------------------------------------------------

	bool CMenuState::init() 
	{
		CApplicationState::init();

		// Cargamos la ventana que muestra el menú
		_menuWindow = CEGUI::WindowManager::getSingletonPtr()->loadLayoutFromFile("Menu.layout");
		
		// Asociamos los botones del menú con las funciones que se deben ejecutar.
		initGUI();

		menuMusic = new Audio::CEvent("event:/Music/MainMenu");
		menuMusic->load();

		return true;

	} // init

	//--------------------------------------------------------

	void CMenuState::initGUI()
	{
				// Asociamos los botones del menú con las funciones que se deben ejecutar.
		_menuWindow->getChildElement("Tutorial")->
			subscribeEvent(CEGUI::PushButton::EventClicked, 
			CEGUI::SubscriberSlot(&CMenuState::startTutorialReleased, this));

		_menuWindow->getChildElement("Tutorial")->
			subscribeEvent(CEGUI::PushButton::EventMouseEntersSurface, 
			CEGUI::SubscriberSlot(&CMenuState::buttonHover, this));

		_menuWindow->getChildElement("Campaign")->
			subscribeEvent(CEGUI::PushButton::EventClicked, 
			CEGUI::SubscriberSlot(&CMenuState::campaignReleased, this));

		_menuWindow->getChildElement("Campaign")->
			subscribeEvent(CEGUI::PushButton::EventMouseEntersSurface, 
			CEGUI::SubscriberSlot(&CMenuState::buttonHover, this));

		_menuWindow->getChildElement("Survivor")->
			subscribeEvent(CEGUI::PushButton::EventClicked, 
			CEGUI::SubscriberSlot(&CMenuState::startSurvivorReleased, this));
		
		_menuWindow->getChildElement("Survivor")->
			subscribeEvent(CEGUI::PushButton::EventMouseEntersSurface, 
			CEGUI::SubscriberSlot(&CMenuState::buttonHover, this));

		_menuWindow->getChildElement("Options")->
			subscribeEvent(CEGUI::PushButton::EventClicked, 
				CEGUI::SubscriberSlot(&CMenuState::optionsReleased, this));

		_menuWindow->getChildElement("Options")->
			subscribeEvent(CEGUI::PushButton::EventMouseEntersSurface, 
			CEGUI::SubscriberSlot(&CMenuState::buttonHover, this));
_menuWindow->getChildElement("Credits")->
			subscribeEvent(CEGUI::PushButton::EventClicked, 
				CEGUI::SubscriberSlot(&CMenuState::creditsReleased, this));

		_menuWindow->getChildElement("Credits")->
			subscribeEvent(CEGUI::PushButton::EventMouseEntersSurface, 
			CEGUI::SubscriberSlot(&CMenuState::buttonHover, this));

_menuWindow->getChildElement("Help")->
			subscribeEvent(CEGUI::PushButton::EventClicked, 
			CEGUI::SubscriberSlot(&CMenuState::helpReleased, this));

		_menuWindow->getChildElement("Help")->
			subscribeEvent(CEGUI::PushButton::EventMouseEntersSurface, 
			CEGUI::SubscriberSlot(&CMenuState::buttonHover, this));		_menuWindow->getChildElement("Exit")->
			subscribeEvent(CEGUI::PushButton::EventClicked, 
				CEGUI::SubscriberSlot(&CMenuState::exitReleased, this));

		_menuWindow->getChildElement("Exit")->
			subscribeEvent(CEGUI::PushButton::EventMouseEntersSurface, 
			CEGUI::SubscriberSlot(&CMenuState::buttonHover, this));
	}

	//-----------------------------------------------------

	void CMenuState::release() 
	{

		Logic::CServer::getSingletonPtr()->unLoadLevel();

		Logic::CEntityFactory::getSingletonPtr()->unloadBluePrints();

		menuMusic->unload();
		delete menuMusic;
				
		//TODO reload base subsystema ogre resource
		BaseSubsystems::CServer::getSingletonPtr()->reloadOgreResource();

		
		CApplicationState::release();

	} // release

	//--------------------------------------------------------

	void CMenuState::activate() 
	{
		CApplicationState::activate();

		// Activamos la ventana que nos muestra el menú y activamos el ratón.
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().setRootWindow(_menuWindow);
		_menuWindow->setVisible(true);
		_menuWindow->activate();
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().show();

		// Al no hacer releaseState desde campaña del menu, mantiene su configuracion de oculto
		if(!_firstTick)
			_menuShowing = true;
		
		menuMusic->play();

	} // activate

	//--------------------------------------------------------

	void CMenuState::deactivate() 
	{		
		// Desactivamos la ventana GUI con el menú y el ratón.
		_menuWindow->deactivate();
		_menuWindow->setVisible(false);
		
		menuMusic->stop();

		Logic::CServer::getSingletonPtr()->deactivateMap();
		CApplicationState::deactivate();

	} // deactivate

	//--------------------------------------------------------

	void CMenuState::resume() 
	{
		CApplicationState::resume();

		// Activamos la ventana que nos muestra el menú y activamos el ratón.
		_menuShowing = true;
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().setRootWindow(_menuWindow);
		_menuWindow->setVisible(true);
		_menuWindow->activate();
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().show();

	} // activate

	//--------------------------------------------------------

	void CMenuState::pause() 
	{		
		// Desactivamos la ventana GUI con el menú y el ratón.
		_menuWindow->deactivate();
		_menuWindow->setVisible(false);

		CApplicationState::pause();

	}

	//--------------------------------------------------------
	

	void CMenuState::tick(unsigned int msecs) 
	{
		
		CApplicationState::tick(msecs);

		if(_menuShowing)
		{
			float posX = _menuWindow->getPosition().d_x.d_scale;
			_menuWindow->setPosition(CEGUI::UVector2( CEGUI::UDim(posX + (1.0f/TIME_FOR_SHOWING) * msecs, 0.0f ), CEGUI::UDim(0.0f, 0.0f ) ));

			if(_menuWindow->getPosition().d_x.d_scale > 0.0f)
			{
				_menuWindow->setPosition(CEGUI::UVector2( CEGUI::UDim(0.0f, 0.0f ), CEGUI::UDim(0.0f, 0.0f ) ));
				_menuShowing = false;
			}
		}

		if(_menuHiding)
		{
			float posX = _menuWindow->getPosition().d_x.d_scale;
			_menuWindow->setPosition(CEGUI::UVector2( CEGUI::UDim(posX - (1.0f/TIME_FOR_SHOWING) * msecs, 0.0f ), CEGUI::UDim(0.0f, 0.0f ) ));

			if(_menuWindow->getPosition().d_x.d_scale < -1.0f)
			{
				_menuWindow->setPosition(CEGUI::UVector2( CEGUI::UDim(-1.0f, 0.0f ), CEGUI::UDim(0.0f, 0.0f ) ));
				_menuHiding = false;
				_app->stackState(_nextState);
			}			
		}

		if(_firstTick)
		{
			_firstTick = false;
			_menuShowing = true;
		}

		// Simulación física
		Physics::CServer::getSingletonPtr()->tick(msecs);

		// Actualizamos la lógica de juego.
		Logic::CServer::getSingletonPtr()->tick(msecs);

		_time += msecs;
		
		

	}

	//--------------------------------------------------------

	bool CMenuState::keyPressed(GUI::TKey key)
	{
		return false;

	} // keyPressed

	//--------------------------------------------------------

	bool CMenuState::keyReleased(GUI::TKey key)
	{
		switch(key.keyId)
		{
		case GUI::Key::ESCAPE:
			_app->exitRequest();
			break;
		//case GUI::Key::RETURN:
		//	startGame();
		//	break;
		#ifdef _DEBUG
		case GUI::Key::F5:
			this->reloadGUI();
			break;
		#endif
		default:
			return false;
		}
		return true;

	} // keyReleased

	//--------------------------------------------------------
	
	bool CMenuState::mouseMoved(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mouseMoved

	//--------------------------------------------------------
		
	bool CMenuState::mousePressed(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mousePressed

	//--------------------------------------------------------


	bool CMenuState::mouseReleased(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mouseReleased
			
	//--------------------------------------------------------
		
	bool CMenuState::startTutorialReleased(const CEGUI::EventArgs& e)
	{
		startGame("tutorial");
		return true;

	} // startReleased

	//--------------------------------------------------------

	bool CMenuState::campaignReleased(const CEGUI::EventArgs& e)
	{
		CApplicationState::buttonClickedSound->play();
		((Logic::CCameraMenu*)Logic::CServer::getSingletonPtr()->getMap()->getEntityByName("CameraMenu")->getComponent("CCameraMenu"))->goToCastle();
		_menuHiding = true;

		_nextState = "campaign";
		return true;
	}

	bool CMenuState::startSurvivorReleased(const CEGUI::EventArgs& e)
	{		
		startGame("survivor");
		return true;

	} // startReleased
			
	//--------------------------------------------------------

	bool CMenuState::exitReleased(const CEGUI::EventArgs& e)
	{
		CApplicationState::buttonHoveredSound->stop();
		_app->exitRequest();
		return true;

	} // exitReleased

	//--------------------------------------------------------

	bool CMenuState::optionsReleased(const CEGUI::EventArgs& e)
	{
		CApplicationState::buttonClickedSound->play();
		((Logic::CCameraMenu*)Logic::CServer::getSingletonPtr()->getMap()->getEntityByName("CameraMenu")->getComponent("CCameraMenu"))->goToOptions();
		_menuHiding = true;

		_nextState = "options";
		return true;
	}
	bool CMenuState::helpReleased(const CEGUI::EventArgs& e)
	{
		CApplicationState::buttonClickedSound->play();
		((Logic::CCameraMenu*)Logic::CServer::getSingletonPtr()->getMap()->getEntityByName("CameraMenu")->getComponent("CCameraMenu"))->goToHelp();
		_menuHiding = true;

		_nextState = "help";
		return true;
	}
	//--------------------------------------------------------

	bool CMenuState::creditsReleased(const CEGUI::EventArgs& e)
	{
		CApplicationState::buttonClickedSound->play();
		((Logic::CCameraMenu*)Logic::CServer::getSingletonPtr()->getMap()->getEntityByName("CameraMenu")->getComponent("CCameraMenu"))->goToCredits();
		_menuHiding = true;

		_nextState = "credits";
		return true;
	}

	//--------------------------------------------------------

	void CMenuState::reloadGUI()
	{
		_menuWindow->deactivate();
		_menuWindow->setVisible(false);

		_menuWindow = CEGUI::WindowManager::getSingletonPtr()->loadLayoutFromFile("Menu.layout");

		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().setRootWindow(_menuWindow);
		_menuWindow->setVisible(true);
		_menuWindow->activate();
		initGUI();
		_menuShowing = true;

	}

	void CMenuState::startGame(std::string gameName)
	{
		buttonHoveredSound->stop();
		CLoadingGameState *loading= static_cast<CLoadingGameState*>( _app->findState("loading"));
		if(_app->setState("loading"))
		{
			_app->releaseState("menu");
			loading->loadGame(gameName);
		}
	}

	//--------------------------------------------------------
		
	bool CMenuState::buttonHover(const CEGUI::EventArgs& e)
	{
		buttonHoveredSound->play();
		return true;
	}
} // namespace Application
