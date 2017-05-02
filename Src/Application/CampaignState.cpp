#include "CampaignState.h"

#include "BaseSubsystems/ScriptManager.h"
#include "BaseSubsystems/Server.h"

#include "GUI/Server.h"
#include <CEGUI/CEGUI.h>

#include "LoadingGameState.h"

#include "Logic/Server.h"
#include "Logic/Maps/Map.h"
#include "Logic/Entity/Entity.h"
#include "Logic\Entity\Components\CameraMenu.h"

#include "Physics/SceneManager.h"
#include "Physics/Server.h"

#define TIME_FOR_SHOWING 750

namespace Application {

	CCampaignState::~CCampaignState() {}

	//--------------------------------------------------------

	bool CCampaignState::init() 
	{
		CApplicationState::init();

		// Cargamos la ventana que muestra el menú
		_campaignWindow = CEGUI::WindowManager::getSingletonPtr()->loadLayoutFromFile("Campaign.layout");
		
		// Asociamos los botones del menú con las funciones que se deben ejecutar.
		_campaignWindow->getChildElement("Level1")->
			subscribeEvent(CEGUI::PushButton::EventClicked, 
				CEGUI::SubscriberSlot(&CCampaignState::level1Released, this));

		_campaignWindow->getChildElement("Level1")->
			subscribeEvent(CEGUI::PushButton::EventMouseEntersSurface, 
			CEGUI::SubscriberSlot(&CCampaignState::buttonHover, this));

		_campaignWindow->getChildElement("Level2")->
			subscribeEvent(CEGUI::PushButton::EventClicked, 
				CEGUI::SubscriberSlot(&CCampaignState::level2Released, this));

		_campaignWindow->getChildElement("Level2")->
			subscribeEvent(CEGUI::PushButton::EventMouseEntersSurface, 
			CEGUI::SubscriberSlot(&CCampaignState::buttonHover, this));

		_campaignWindow->getChildElement("Level3")->
			subscribeEvent(CEGUI::PushButton::EventClicked, 
				CEGUI::SubscriberSlot(&CCampaignState::level3Released, this));

		_campaignWindow->getChildElement("Level3")->
			subscribeEvent(CEGUI::PushButton::EventMouseEntersSurface, 
			CEGUI::SubscriberSlot(&CCampaignState::buttonHover, this));

		_campaignWindow->getChildElement("Level4")->
			subscribeEvent(CEGUI::PushButton::EventClicked, 
				CEGUI::SubscriberSlot(&CCampaignState::level4Released, this));

		_campaignWindow->getChildElement("Level4")->
			subscribeEvent(CEGUI::PushButton::EventMouseEntersSurface, 
			CEGUI::SubscriberSlot(&CCampaignState::buttonHover, this));

		_campaignWindow->getChildElement("Back")->
			subscribeEvent(CEGUI::PushButton::EventClicked, 
				CEGUI::SubscriberSlot(&CCampaignState::backReleased, this));

		_campaignWindow->getChildElement("Back")->
			subscribeEvent(CEGUI::PushButton::EventMouseEntersSurface, 
			CEGUI::SubscriberSlot(&CCampaignState::buttonHover, this));

		return true;
	}

	void CCampaignState::release() 
	{
		CApplicationState::release();

	} // release

	//--------------------------------------------------------

	void CCampaignState::activate() 
	{
		CApplicationState::activate();

		// Activamos la ventana que nos muestra el menú y activamos el ratón.
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().setRootWindow(_campaignWindow);
		_campaignWindow->setVisible(true);
		_campaignWindow->activate();
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().show();
		_campaignShowing = true;

		// Se desbloquean los niveles
		int record = ScriptManager::CScriptManager::GetPtrSingleton()->getField("UserProfile", "campaignLevel", 0);

		// Para evitar accesos a a elementos que no existen
		if(record >(int) (_campaignWindow->getChildCount() - 2))
			record = _campaignWindow->getChildCount() - 2;

		for(int i = 0; i<=record; ++i)
		{
			int num =  i+1;
			std::string level = "Level" + std::to_string(num);

			_campaignWindow->getChildElement(level)->setProperty("Disabled", "False");
		}

	} // activate

	//--------------------------------------------------------

	void CCampaignState::deactivate() 
	{		
		CApplicationState::deactivate();

		// Desactivamos la ventana GUI con el menú y el ratón.
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().hide();
		_campaignWindow->deactivate();
		_campaignWindow->setVisible(false);
	} // deactivate

	//--------------------------------------------------------

	void CCampaignState::tick(unsigned int msecs) 
	{
		CApplicationState::tick(msecs);

		if(_campaignShowing)
		{
			_campaignHiding = false;
			float posX = _campaignWindow->getPosition().d_x.d_scale;
			_campaignWindow->setPosition(CEGUI::UVector2( CEGUI::UDim(posX + (1.0f/TIME_FOR_SHOWING) * msecs, 0.0f ), CEGUI::UDim(0.0f, 0.0f ) ));

			if(_campaignWindow->getPosition().d_x.d_scale > 0.0f)
			{
				_campaignWindow->setPosition(CEGUI::UVector2( CEGUI::UDim(0.0f, 0.0f ), CEGUI::UDim(0.0f, 0.0f ) ));
				_campaignShowing = false;
			}
		}

		if(_campaignHiding)
		{
			_campaignShowing = false;
			float posX = _campaignWindow->getPosition().d_x.d_scale;
			_campaignWindow->setPosition(CEGUI::UVector2( CEGUI::UDim(posX - (1.0f/TIME_FOR_SHOWING) * msecs, 0.0f ), CEGUI::UDim(0.0f, 0.0f ) ));

			if(_campaignWindow->getPosition().d_x.d_scale < -1.0f)
			{
				_campaignWindow->setPosition(CEGUI::UVector2( CEGUI::UDim(-1.0f, 0.0f ), CEGUI::UDim(0.0f, 0.0f ) ));
				_campaignHiding = false;
				_app->unstackState();
			}
		}

		// Simulación física
		Physics::CServer::getSingletonPtr()->tick(msecs);

		// Actualizamos la lógica de juego.
		Logic::CServer::getSingletonPtr()->tick(msecs);

		_time += msecs;

	} // tick

	//--------------------------------------------------------

	bool CCampaignState::keyPressed(GUI::TKey key)
	{

		return false;

	} // keyPressed

	//--------------------------------------------------------

	bool CCampaignState::keyReleased(GUI::TKey key)
	{
		switch(key.keyId)
		{
		case GUI::Key::ESCAPE:
			CApplicationState::buttonClickedSound->play();
			((Logic::CCameraMenu*)Logic::CServer::getSingletonPtr()->getMap()->getEntityByName("CameraMenu")->getComponent("CCameraMenu"))->goToMenu();
			_campaignHiding = true;
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
	
	bool CCampaignState::mouseMoved(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mouseMoved

	//--------------------------------------------------------
		
	bool CCampaignState::mousePressed(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mousePressed

	//--------------------------------------------------------


	bool CCampaignState::mouseReleased(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mouseReleased

	//--------------------------------------------------------

	bool CCampaignState::level1Released(const CEGUI::EventArgs& e)
	{
		startGame("level1");
		return true;
	}

	//--------------------------------------------------------

	bool CCampaignState::level2Released(const CEGUI::EventArgs& e)
	{
		startGame("Level2");
		return true;
	}

	//--------------------------------------------------------

	bool CCampaignState::level3Released(const CEGUI::EventArgs& e)
	{
		startGame("Level3");
		return true;
	}
	
	//--------------------------------------------------------

	bool CCampaignState::level4Released(const CEGUI::EventArgs& e)
	{
		startGame("Level4");
		return true;
	}
		
	//--------------------------------------------------------

	bool CCampaignState::level5Released(const CEGUI::EventArgs& e)
	{
		startGame("Level5");
		return true;
	}
	
	//--------------------------------------------------------

	bool CCampaignState::backReleased(const CEGUI::EventArgs& e)
	{
		CApplicationState::buttonClickedSound->play();
		((Logic::CCameraMenu*)Logic::CServer::getSingletonPtr()->getMap()->getEntityByName("CameraMenu")->getComponent("CCameraMenu"))->goToMenu();
		_campaignHiding = true;
		
		return true;

	} //backReleased

	//--------------------------------------------------------

	bool CCampaignState::buttonHover(const CEGUI::EventArgs& e)
	{
		buttonHoveredSound->play();
		return true;
	} // buttonhover

	//--------------------------------------------------------

	bool CCampaignState::buttonClicked(const CEGUI::EventArgs& e)
	{
		buttonClickedSound->play();
		return true;
	} //buttonClicked

	//--------------------------------------------------------

	void CCampaignState::reloadGUI()
	{
		this->deactivate();
		this->release();
		this->init();
		this->activate();
	} //reloadGUI

	//--------------------------------------------------------

	void CCampaignState::startGame(std::string gameName)
	{
		buttonHoveredSound->stop();
		CLoadingGameState *loading= static_cast<CLoadingGameState*>( _app->findState("loading"));
		if(_app->setState("loading"))
		{
			_app->releaseState("campaign");
			loading->loadGame(gameName);
		}
	}

}