//---------------------------------------------------------------------------
// LoadingGameState.cpp
//---------------------------------------------------------------------------

/**
@file LoadingGameState.cpp

Contiene la implementación del estado de menú.

@see Application::CApplicationState
@see Application::CLoadingGameState

@author Guidi Giacomo
@date Mayo, 2015
*/

#include "LoadingGameState.h"

#include "GameState.h"
#include "MenuState.h"

#include "Logic/Server.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"

#include "GUI/Server.h"

#include "Graphics\Server.h"

#include "OgreRoot.h"
#include <CEGUI/CEGUI.h>



namespace Application {
	
	
	CLoadingGameState::~CLoadingGameState() 
	{
	} // ~CLoadingGameState

	//--------------------------------------------------------

	bool CLoadingGameState::init() 
	{
		CApplicationState::init();

		// Cargamos la ventana que muestra el menú
		_menuWindow = CEGUI::WindowManager::getSingletonPtr()->loadLayoutFromFile("Loading.layout");

		_progressBar = static_cast<CEGUI::ProgressBar*>( _menuWindow->getChildElement("LoadingGame/ProgressBar"));
	
		if(_gameName == "")
		{
			_gameName = "Menu";
			_loaded = false;
		}

		return true;

	} // init

	//--------------------------------------------------------

	void CLoadingGameState::release() 
	{
		CApplicationState::release();

	} // release

	//--------------------------------------------------------

	void CLoadingGameState::activate() 
	{
		CApplicationState::activate();

		// Activamos la ventana que nos muestra el menú y activamos el ratón.
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().setRootWindow(_menuWindow);
		_menuWindow->setVisible(true);
		_menuWindow->activate();
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().hide();


	} // activate

	//--------------------------------------------------------

	void CLoadingGameState::deactivate() 
	{		

		// Desactivamos la ventana GUI con el menú y el ratón.
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().hide();
		_menuWindow->deactivate();
		_menuWindow->setVisible(false);
		
		CApplicationState::deactivate();

	} // deactivate

	//--------------------------------------------------------

	void CLoadingGameState::tick(unsigned int msecs) 
	{
		CApplicationState::tick(msecs);
		if(!_loaded)
		{
			bool init=true;
	
			setupUI(0);

			if(_app->findState("game")!=nullptr)
			{
				_app->releaseState("game");
			}

			

			// Cargamos el archivo con las definiciones de las entidades del nivel.
			if (!Logic::CEntityFactory::getSingletonPtr()->loadBluePrints(_gameName+"Blueprints.txt"))
				init = false;

			setupUI(1);

			// Cargamos el nivel a partir del nombre del mapa. 
			if ( init && !Logic::CServer::getSingletonPtr()->loadLevel(_gameName+"Map.txt"))
				init = false;

			setupUI(2);

			if(_gameName != "Menu")
			{
				if(init && !_app->addState("game", new CGameState(_app,_gameName)))
				init=false;
			}

			setupUI(3);

			if(init)
			{
						// Activamos el mapa que ha sido cargado para la partida.
				Logic::CServer::getSingletonPtr()->activateMap();

				setupUI(4);

				Logic::CServer::getSingletonPtr()->awakeMap();

				setupUI(5);

				if(_gameName == "Menu")
					_app->setState("menu");
				else
					_app->setState("game");


				_loaded=true;
			}

		}


	} // tick

	//--------------------------------------------------------

	bool CLoadingGameState::keyPressed(GUI::TKey key)
	{
		return false;

	} // keyPressed

	//--------------------------------------------------------

	bool CLoadingGameState::keyReleased(GUI::TKey key)
	{
		//switch(key.keyId)
		//{
		//
		//case GUI::Key::F5:
		//	reloadGUI();
		//	break;
		//default:
		//	return false;
		//}
		return false;

	} // keyReleased

	//--------------------------------------------------------
	
	bool CLoadingGameState::mouseMoved(const GUI::CMouseState &mouseState)
	{

		return false;

	} // mouseMoved

	//--------------------------------------------------------
		
	bool CLoadingGameState::mousePressed(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mousePressed

	//--------------------------------------------------------




	bool CLoadingGameState::mouseReleased(const GUI::CMouseState &mouseState)
	{
		
		return false;

	} // mouseReleased
			

	//--------------------------------------------------------

	void CLoadingGameState::reloadGUI()
	{
		this->deactivate();
		this->release();
		this->init();
		this->activate();
	}
	void CLoadingGameState::loadGame(std::string name)
	{
		_gameName = name;
		_loaded=false;

	}

	void CLoadingGameState::initGraphicsResource()
	{
		if(_gameName=="Menu")
		{
			_menuWindow->getChildElement("LoadingGame")->setProperty("Visible","false");
			_menuWindow->getChildElement("LoadingMenu")->setProperty("Visible","true");
			if(_isFirstLaunch)
			{
				_isFirstLaunch = false;
				_menuWindow->getChildElement("LoadingMenu/First")->setProperty("Visible","true");
				_menuWindow->getChildElement("LoadingMenu/Normal")->setProperty("Visible","false");
			}else
			{
				_menuWindow->getChildElement("LoadingMenu/First")->setProperty("Visible","false");
				_menuWindow->getChildElement("LoadingMenu/Normal")->setProperty("Visible","true");
			}
			_menuWindow->getChildElement("LoadingMenu/Loading")->setProperty("Text","loading");

		}
		else
		{
			_progressBar->setProgress(0.0f);
			_menuWindow->getChildElement("LoadingGame")->setProperty("Visible","true");
			_menuWindow->getChildElement("LoadingGame/PrincessIcon")->setProperty("Visible","false");
			_menuWindow->getChildElement("LoadingGame/WarriorIcon")->setProperty("Visible","false");
			_menuWindow->getChildElement("LoadingGame/RangerIcon")->setProperty("Visible","false");
			_menuWindow->getChildElement("LoadingGame/WorkmanIcon")->setProperty("Visible","false");

			_menuWindow->getChildElement("LoadingMenu")->setProperty("Visible","false");

		}
	}
	void CLoadingGameState::setupUI(int step)
	{
		switch (step)
		{
		case 0:
			initGraphicsResource();
			break;
		case 1:
			if(_gameName!="Menu")
			{
				_progressBar->setProgress(0.2f);
				_menuWindow->getChildElement("LoadingGame/WorkmanIcon")->setProperty("Visible","true");
			}else
			{
				_menuWindow->getChildElement("LoadingMenu/Loading")->setProperty("Text","loading.");
			}
			break;
		case 2:
			if(_gameName!="Menu")
			{
				_progressBar->setProgress(0.4f);
				_menuWindow->getChildElement("LoadingGame/RangerIcon")->setProperty("Visible","true");
			}else
			{
				_menuWindow->getChildElement("LoadingMenu/Loading")->setProperty("Text","loading..");
			}
			break;
		case 3:
			if(_gameName!="Menu")
			{
				_progressBar->setProgress(0.6f);
				_menuWindow->getChildElement("LoadingGame/WarriorIcon")->setProperty("Visible","true");
			}else
			{
				_menuWindow->getChildElement("LoadingMenu/Loading")->setProperty("Text","loading...");
			}
			break;
		case 4:
			if(_gameName!="Menu")
			{
				_progressBar->setProgress(0.8f);
				_menuWindow->getChildElement("LoadingGame/PrincessIcon")->setProperty("Visible","true");
			}else
			{
				_menuWindow->getChildElement("LoadingMenu/Loading")->setProperty("Text","loading....");
			}
			break;
		case 5:
			if(_gameName!="Menu")
				_progressBar->setProgress(1.0f);
			else
			{
				_menuWindow->getChildElement("LoadingMenu/Loading")->setProperty("Text","loading.....");
			}
			break;
		}

		Graphics::CServer::getSingletonPtr()->getOgreRoot()->renderOneFrame();
	}

} // namespace Application
