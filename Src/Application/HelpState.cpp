#include "HelpState.h"
#include "LoadingGameState.h"

#include "GUI/Server.h"
#include <CEGUI/CEGUI.h>

#include "Logic/Server.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"
#include "Logic/Entity/Entity.h"
#include "Logic\Entity\Components\CameraMenu.h"

#include "Physics/SceneManager.h"
#include "Physics/Server.h"

#define TIME_FOR_SHOWING 750

namespace Application {

	CHelpState::~CHelpState() {}

	//--------------------------------------------------------

	bool CHelpState::init() 
	{
		CApplicationState::init();

		// Cargamos la ventana que muestra el menú
		_helpWindow = CEGUI::WindowManager::getSingletonPtr()->loadLayoutFromFile("Help.layout");
		
		// Asociamos los botones del menú con las funciones que se deben ejecutar.
		_helpWindow->getChildElement("Back")->
			subscribeEvent(CEGUI::PushButton::EventClicked, 
				CEGUI::SubscriberSlot(&CHelpState::backReleased, this));

		_helpWindow->getChildElement("Back")->
			subscribeEvent(CEGUI::PushButton::EventMouseEntersSurface, 
			CEGUI::SubscriberSlot(&CHelpState::buttonHover, this));

		return true;

	}

	//--------------------------------------------------------

	void CHelpState::release() 
	{
		CApplicationState::release();
	}

	//--------------------------------------------------------

	void CHelpState::activate() 
	{
		CApplicationState::activate();

		// Activamos la ventana que nos muestra el menú y activamos el ratón.
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().setRootWindow(_helpWindow);
		_helpWindow->setVisible(true);
		_helpWindow->activate();
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().show();
		_helpShowing = true;
	}

	//--------------------------------------------------------

	void CHelpState::deactivate() 
	{		
		// Desactivamos la ventana GUI con el menú y el ratón.
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().hide();
		_helpWindow->deactivate();
		_helpWindow->setVisible(false);

		CApplicationState::deactivate();

	}

	//--------------------------------------------------------

	void CHelpState::tick(unsigned int msecs) 
	{
		CApplicationState::tick(msecs);

		
		if(_helpShowing)
		{
			_helpHiding = false;
			float posX = _helpWindow->getPosition().d_x.d_scale;
			_helpWindow->setPosition(CEGUI::UVector2( CEGUI::UDim(posX + (1.0f/TIME_FOR_SHOWING) * msecs, 0.0f ), CEGUI::UDim(0.0f, 0.0f ) ));

			if(_helpWindow->getPosition().d_x.d_scale > 0.0f)
			{
				_helpWindow->setPosition(CEGUI::UVector2( CEGUI::UDim(0.0f, 0.0f ), CEGUI::UDim(0.0f, 0.0f ) ));
				_helpShowing = false;
			}
		}

		if(_helpHiding)
		{
			_helpShowing = false;
			float posX = _helpWindow->getPosition().d_x.d_scale;
			_helpWindow->setPosition(CEGUI::UVector2( CEGUI::UDim(posX - (1.0f/TIME_FOR_SHOWING) * msecs, 0.0f ), CEGUI::UDim(0.0f, 0.0f ) ));

			if(_helpWindow->getPosition().d_x.d_scale < -1.0f)
			{
				_helpWindow->setPosition(CEGUI::UVector2( CEGUI::UDim(-1.0f, 0.0f ), CEGUI::UDim(0.0f, 0.0f ) ));
				_helpHiding = false;
				_app->unstackState();
			}
		}

		// Simulación física
		Physics::CServer::getSingletonPtr()->tick(msecs);

		// Actualizamos la lógica de juego.
		Logic::CServer::getSingletonPtr()->tick(msecs);

		_time += msecs;

	}

	//--------------------------------------------------------

	bool CHelpState::keyPressed(GUI::TKey key)
	{
		return false;
	}

	//--------------------------------------------------------

	bool CHelpState::keyReleased(GUI::TKey key)
	{
		switch(key.keyId)
		{
		case GUI::Key::ESCAPE:
			CApplicationState::buttonClickedSound->play();
			((Logic::CCameraMenu*)Logic::CServer::getSingletonPtr()->getMap()->getEntityByName("CameraMenu")->getComponent("CCameraMenu"))->goToMenu();
			_helpHiding = true;
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

	}

	//--------------------------------------------------------
	
	bool CHelpState::mouseMoved(const GUI::CMouseState &mouseState)
	{
		return false;
	}

	//--------------------------------------------------------
		
	bool CHelpState::mousePressed(const GUI::CMouseState &mouseState)
	{
		return false;
	}

	//--------------------------------------------------------


	bool CHelpState::mouseReleased(const GUI::CMouseState &mouseState)
	{
		return false;
	}
			
	//--------------------------------------------------------

	bool CHelpState::backReleased(const CEGUI::EventArgs& e)
	{
		CApplicationState::buttonClickedSound->play();
		((Logic::CCameraMenu*)Logic::CServer::getSingletonPtr()->getMap()->getEntityByName("CameraMenu")->getComponent("CCameraMenu"))->goToMenu();
		_helpHiding = true;
		
		return true;

	}

	void CHelpState::reloadGUI()
	{
		this->deactivate();
		this->release();
		this->init();
		this->activate();
	}


	//--------------------------------------------------------
		
	bool CHelpState::buttonHover(const CEGUI::EventArgs& e)
	{
		buttonHoveredSound->play();
		return true;
	}

	//--------------------------------------------------------
		
	bool CHelpState::buttonClicked(const CEGUI::EventArgs& e)
	{
		buttonClickedSound->play();
		return true;
	}
}
