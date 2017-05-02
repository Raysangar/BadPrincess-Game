//---------------------------------------------------------------------------
// PauseState.cpp
//---------------------------------------------------------------------------

/**
@file PauseState.cpp

Contiene la implementación del estado de menú.

@see Application::CApplicationState
@see Application::CPauseState

@author David Llansó
@date Agosto, 2010
*/

#include "PauseState.h"
#include "GameState.h"
#include "LoadingGameState.h"

#include "Logic/Server.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Component.h"
#include "Logic/Entity/Components/Camera.h"


#include "Graphics\Scene.h"
#include "Graphics\Camera.h"

#include "GUI/Server.h"

#include <CEGUI/CEGUI.h>

namespace Application {

	CPauseState::~CPauseState() 
	{
	} // ~CPauseState

	//--------------------------------------------------------

	bool CPauseState::init() 
	{
		CApplicationState::init();

		// Cargamos la ventana que muestra el menú
		_menuWindow = CEGUI::WindowManager::getSingletonPtr()->loadLayoutFromFile("Pause.layout");
		
		// Asociamos los botones del menú con las funciones que se deben ejecutar.
		_menuWindow->getChildElement("Menu")->
			subscribeEvent(CEGUI::PushButton::EventClicked, 
				CEGUI::SubscriberSlot(&CPauseState::menuReleased, this));

		_menuWindow->getChildElement("Menu")->
			subscribeEvent(CEGUI::PushButton::EventMouseEntersSurface, 
				CEGUI::SubscriberSlot(&CPauseState::buttonHover, this));
		
		_menuWindow->getChildElement("Resume")->
			subscribeEvent(CEGUI::PushButton::EventClicked, 
				CEGUI::SubscriberSlot(&CPauseState::resumeReleased, this));

		_menuWindow->getChildElement("Resume")->
			subscribeEvent(CEGUI::PushButton::EventMouseEntersSurface, 
				CEGUI::SubscriberSlot(&CPauseState::buttonHover, this));

		_menuWindow->getChildElement("Restart")->
			subscribeEvent(CEGUI::PushButton::EventClicked, 
				CEGUI::SubscriberSlot(&CPauseState::restartReleased, this));

		_menuWindow->getChildElement("Restart")->
			subscribeEvent(CEGUI::PushButton::EventMouseEntersSurface, 
				CEGUI::SubscriberSlot(&CPauseState::buttonHover, this));
	
		return true;

	} // init

	//--------------------------------------------------------

	void CPauseState::release() 
	{
		CApplicationState::release();

	} // release

	//--------------------------------------------------------

	void CPauseState::activate() 
	{
		CApplicationState::activate();

		// Activamos la ventana que nos muestra el menú y activamos el ratón.
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().setRootWindow(_menuWindow);
		_menuWindow->setVisible(true);
		_menuWindow->activate();
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().show();
		Graphics::CServer::getSingletonPtr()->getActiveScene()->getCamera()->setCompositor("SimpleBlur");

	} // activate

	//--------------------------------------------------------

	void CPauseState::deactivate() 
	{		
		// Desactivamos la ventana GUI con el menú y el ratón.		
		_menuWindow->deactivate();
		_menuWindow->setVisible(false);
		Graphics::CServer::getSingletonPtr()->getActiveScene()->getCamera()->disableCompositor("SimpleBlur");
		CApplicationState::deactivate();

	} // deactivate

	//--------------------------------------------------------

	void CPauseState::tick(unsigned int msecs) 
	{
		CApplicationState::tick(msecs);

	} // tick

	//--------------------------------------------------------

	bool CPauseState::keyPressed(GUI::TKey key)
	{
		return false;

	} // keyPressed

	//--------------------------------------------------------

	bool CPauseState::keyReleased(GUI::TKey key)
	{
		switch(key.keyId)
		{
		case GUI::Key::ESCAPE:
			_app->unstackState();
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
	
	bool CPauseState::mouseMoved(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mouseMoved

	//--------------------------------------------------------
		
	bool CPauseState::mousePressed(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mousePressed

	//--------------------------------------------------------


	bool CPauseState::mouseReleased(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mouseReleased
			
	//--------------------------------------------------------
		
	bool CPauseState::menuReleased(const CEGUI::EventArgs& e)
	{
		buttonHoveredSound->stop();		
		CLoadingGameState *loading= static_cast<CLoadingGameState*>( _app->findState("loading"));
		if(_app->setState("loading"))
		{			
			loading->loadGame("Menu");
			return false;
		}
		return true;

	} // startReleased
			
	//--------------------------------------------------------

	bool CPauseState::resumeReleased(const CEGUI::EventArgs& e)
	{
		buttonClickedSound->play();
		_app->unstackState();
		return true;

	} // exitReleased

	//--------------------------------------------------------

	bool CPauseState::restartReleased(const CEGUI::EventArgs& e)
	{		
		if(_app->setState("loading"))
		{
			buttonHoveredSound->stop();	
			std::string name= static_cast<CGameState*>( _app->findState("game"))->getName();
			CLoadingGameState *loading= static_cast<CLoadingGameState*>( _app->findState("loading"));
			loading->loadGame(name);

		return true;
		}
		return false;

	} // exitReleased

	//--------------------------------------------------------

	bool CPauseState::buttonHover(const CEGUI::EventArgs& e)
	{
		buttonHoveredSound->play();
		return true;
	}

	//--------------------------------------------------------

	void CPauseState::reloadGUI()
	{
		this->deactivate();
		this->release();
		this->init();
		this->activate();
	}

} // namespace Application
