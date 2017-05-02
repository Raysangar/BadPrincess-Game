//---------------------------------------------------------------------------
// GameState.cpp
//---------------------------------------------------------------------------

/**
@file GameState.cpp

Contiene la implementación del estado de juego.

@see Application::CApplicationState
@see Application::CGameState

@author David Llansó
@date Agosto, 2010
*/

#include "GameState.h"

#include "BaseSubsystems\Server.h"

#include "Logic/Server.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"

#include "GUI/Server.h"

#include "Audio\Server.h"

#include "Graphics\Camera.h"
#include "Graphics/Scene.h"
#include "Graphics\FogOfWar.h"

#include "Physics/SceneManager.h"
#include "Physics/Server.h"

#include "AI/Server.h"

#include <CEGUI/System.h>
#include <CEGUI/WindowManager.h>
#include <CEGUI/Window.h>
#include <CEGUI\CEGUI.h>

namespace Application {

	bool CGameState::init() 
	{
		CApplicationState::init();

		// Crear la escena física.
		//Physics::CServer::getSingletonPtr()->setGroupCollisions(1,1,false);
		//Physics::CServer::getSingletonPtr()->getSceneManager()->createScene();

			_window = CEGUI::WindowManager::getSingletonPtr()->loadLayoutFromFile("BadPrincess-16-9.layout");

		return true;

	} // init

	//--------------------------------------------------------

	void CGameState::release() 
	{
		Logic::CServer::getSingletonPtr()->unLoadLevel();

		Logic::CEntityFactory::getSingletonPtr()->unloadBluePrints();

		AI::CServer::getSingletonPtr()->resetManagers();
		//Physics::CServer::getSingletonPtr()->getSceneManager()->destroyScene();

		Audio::CServer::getSingletonPtr()->reset();
		

		//liberamos los controladores de la interfaz que son relativa a una partida
		GUI::CServer::getSingletonPtr()->resetGameController();

		//TODO reload base subsystema ogre resource
		BaseSubsystems::CServer::getSingletonPtr()->reloadOgreResource();

		

		CApplicationState::release();

	} // release

	//--------------------------------------------------------

	void CGameState::activate() 
	{
		CApplicationState::activate();
		// Activamos la GUI del juego.
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().setRootWindow(_window);
		_window->setVisible(true);
		_window->activate();

		_window->getChildElement("MenuButton")->
			subscribeEvent(CEGUI::PushButton::EventClicked, 
				CEGUI::SubscriberSlot(&CGameState::pauseReleased, this));

		//_window->getChildElement("MenuButton")->
		//	subscribeEvent(CEGUI::PushButton::EventMouseEntersSurface, 
		//		CEGUI::SubscriberSlot(&CGameState::buttonHover, this));

		//decimos a la interfaz que se activa para la primera vez 
		GUI::CServer::getSingletonPtr()->awakeGameController();

		// Queremos que el GUI maneje la interfaz.
		GUI::CServer::getSingletonPtr()->activate();		 
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().show();
	} // activate

	//--------------------------------------------------------

	void CGameState::deactivate() 
	{
		// Desactivamos la ventana de tiempo.
		_window->deactivate();
		_window->setVisible(false);

		// Desactivamos la clase que procesa eventos de interfaz
		GUI::CServer::getSingletonPtr()->deactivate();

		// Desactivamos el mapa de la partida.
		Logic::CServer::getSingletonPtr()->deactivateMap();
		CApplicationState::deactivate();
		Graphics::CServer::getSingletonPtr()->getFogOfWarController()->restart();

	}

	//--------------------------------------------------------

	void CGameState::pause() 
	{

		Logic::CServer::getSingletonPtr()->getMap()->getScene()->getCamera()->setCompositor("SimpleBlur");
		Logic::CServer::getSingletonPtr()->getMap()->getScene()->getCamera()->enableCompositor("SimpleBlur");
		
		// Desactivamos la ventana de tiempo.
		_window->deactivate();
		_window->setVisible(false);

		// Desactivamos la clase que procesa eventos de interfaz
		GUI::CServer::getSingletonPtr()->deactivate();

		Logic::CServer::getSingletonPtr()->pauseMap();
	}

	//--------------------------------------------------------

	void CGameState::resume() 
	{
		// Activamos la GUI del juego.
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().setRootWindow(_window);
		_window->setVisible(true);
		_window->activate();

		Logic::CServer::getSingletonPtr()->restartMap();
		GUI::CServer::getSingletonPtr()->activate();
		Logic::CServer::getSingletonPtr()->getMap()->getScene()->getCamera()->disableCompositor("SimpleBlur");
	}

	//--------------------------------------------------------

	void CGameState::tick(unsigned int msecs) 
	{
		// Simulación física
		Physics::CServer::getSingletonPtr()->tick(msecs);

		// Actualizamos la lógica de juego.
		Logic::CServer::getSingletonPtr()->tick(msecs);
				
		GUI::CServer::getSingletonPtr()->tick(msecs);

		AI::CServer::getSingletonPtr()->tick(msecs);

		_time += msecs;
		
		CApplicationState::tick(msecs);

	} // tick

	//--------------------------------------------------------

	bool CGameState::keyPressed(GUI::TKey key)
	{
		return false;

	} // keyPressed

	//--------------------------------------------------------

	bool CGameState::keyReleased(GUI::TKey key)
	{
		switch(key.keyId)
		{
		case GUI::Key::ESCAPE:
			_app->stackState("pause");
			break;
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
	
	bool CGameState::mouseMoved(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mouseMoved

	//--------------------------------------------------------
		
	bool CGameState::mousePressed(const GUI::CMouseState &mouseState)
	{
		
		return false;

	} // mousePressed

	//--------------------------------------------------------


	bool CGameState::mouseReleased(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mouseReleased

	//--------------------------------------------------------

	void CGameState::reloadGUI()
	{
		// Desactivamos la ventana de tiempo.
		_window->deactivate();
		_window->setVisible(false);
		// Desactivamos la clase que procesa eventos de interfaz
		GUI::CServer::getSingletonPtr()->deactivate();

		float originalAspectRatio = 4.0f/3.0f;
		// Cargamos la ventana que muestra el tiempo de juego transcurrido.
		if (originalAspectRatio == Logic::CServer::getSingletonPtr()->getMap()->getScene()->getCamera()->getAspectRatio())
			_window = CEGUI::WindowManager::getSingletonPtr()->loadLayoutFromFile("BadPrincess-4-3.layout");
		else
			_window = CEGUI::WindowManager::getSingletonPtr()->loadLayoutFromFile("BadPrincess-16-9.layout");
		
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().setRootWindow(_window);
		_window->setVisible(true);
		_window->activate();

				
		// Queremos que el GUI maneje la interfaz.
		GUI::CServer::getSingletonPtr()->activate();	
	}
	
	//--------------------------------------------------------

	void CGameState::restartLevel()
	{
		this->deactivate();
		this->release();
		this->init();
		this->activate();
	}

	//--------------------------------------------------------

	bool CGameState::pauseReleased(const CEGUI::EventArgs& e)
	{
		buttonClickedSound->play();
		_app->stackState("pause");
		return true;
	}

	//--------------------------------------------------------

	bool CGameState::buttonHover(const CEGUI::EventArgs& e)
	{
		buttonHoveredSound->play();
		return true;
	}
} // namespace Application
