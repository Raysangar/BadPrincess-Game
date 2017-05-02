#include "CreditsState.h"
#include "LoadingGameState.h"

#include "GUI/Server.h"
#include <CEGUI/CEGUI.h>

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

#define TIME_FOR_SHOWING 750
#define TIME_FOR_HIDING_CREDITS 10000
#define TIME_HOLDING_CREDITS 3000

namespace Application {

	CCreditsState::~CCreditsState() {}

	//--------------------------------------------------------

	bool CCreditsState::init() 
	{
		CApplicationState::init();

		// Cargamos la ventana que muestra el menú
		_creditsWindow = CEGUI::WindowManager::getSingletonPtr()->loadLayoutFromFile("Credits.layout");
		
		
		std::ifstream configFile("media/Credits.txt");
		std::string line;
		std::vector<std::string> creditsList;
		creditsList.push_back("");
		int lineCount = 0;
		while (std::getline(configFile, line))
		{
			++lineCount;
			if (lineCount%23 == 0)
				creditsList.push_back("");
			creditsList.at(creditsList.size()-1) += line + "\n";	
		}
		while (lineCount%23 != 0)
		{
			creditsList.at(creditsList.size()-1) += "\n";
			++lineCount;
		}
		numberOfPanels = creditsList.size();
		for (unsigned int i = 0; i < creditsList.size(); ++i)
			_creditsWindow->getChild("CreditsText" + std::to_string(i))->setText(creditsList.at(i));

		return true;
	}

	//--------------------------------------------------------

	void CCreditsState::release() 
	{
		CApplicationState::release();
	}

	//--------------------------------------------------------

	void CCreditsState::activate() 
	{
		CApplicationState::activate();

		// Activamos la ventana que nos muestra el menú y activamos el ratón.
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().setRootWindow(_creditsWindow);
		_creditsWindow->setVisible(true);
		_creditsWindow->activate();
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().show();
		_creditsShowing = true;
	}

	//--------------------------------------------------------

	void CCreditsState::deactivate() 
	{		
		// Desactivamos la ventana GUI con el menú y el ratón.
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().hide();
		_creditsWindow->deactivate();
		_creditsWindow->setVisible(false);

		_startCredits = false;
		_creditsFinished = false;
		_timeHoldingCredits = 0;
		for (int i = 0; i < numberOfPanels; ++i)
			_creditsWindow->getChildElement("CreditsText" + std::to_string(i))->
				setPosition(CEGUI::UVector2( CEGUI::UDim(0.0f, 0.0f ), CEGUI::UDim(1.0f+i, 0.0f ) ));

		CApplicationState::deactivate();

	}

	//--------------------------------------------------------

	void CCreditsState::tick(unsigned int msecs) 
	{
		CApplicationState::tick(msecs);

		
		if(_creditsShowing)
		{
			_creditsHiding = false;
			float posX = _creditsWindow->getPosition().d_x.d_scale;
			_creditsWindow->setPosition(CEGUI::UVector2( CEGUI::UDim(posX + (1.0f/TIME_FOR_SHOWING) * msecs, 0.0f ), CEGUI::UDim(0.0f, 0.0f ) ));

			if(_creditsWindow->getPosition().d_x.d_scale > 0.0f)
			{
				_creditsWindow->setPosition(CEGUI::UVector2( CEGUI::UDim(0.0f, 0.0f ), CEGUI::UDim(0.0f, 0.0f ) ));
				_creditsShowing = false;
				_startCredits = true;
			}
		}

		if(_creditsHiding)
		{
			_creditsShowing = false;
			float posX = _creditsWindow->getPosition().d_x.d_scale;
			_creditsWindow->setPosition(CEGUI::UVector2( CEGUI::UDim(posX - (1.0f/TIME_FOR_SHOWING) * msecs, 0.0f ), CEGUI::UDim(0.0f, 0.0f ) ));

			if(_creditsWindow->getPosition().d_x.d_scale < -1.0f)
			{
				_creditsWindow->setPosition(CEGUI::UVector2( CEGUI::UDim(-1.0f, 0.0f ), CEGUI::UDim(0.0f, 0.0f ) ));
				_creditsHiding = false;
				_app->unstackState();
			}
		}

		if (_creditsFinished)
		{
			_timeHoldingCredits +=  msecs;
			if (TIME_HOLDING_CREDITS <= _timeHoldingCredits)
			{
				_creditsFinished = false;
				_timeHoldingCredits = 0;
				((Logic::CCameraMenu*)Logic::CServer::getSingletonPtr()->getMap()->getEntityByName("CameraMenu")->getComponent("CCameraMenu"))->goToMenu();
				_creditsHiding = true;
			}
		}

		if (_startCredits)
		{
			for (int i = 0; i < numberOfPanels; ++i)
			{
				float posY = _creditsWindow->getChildElement("CreditsText" + std::to_string(i))->getPosition().d_y.d_scale;
				float posX = _creditsWindow->getChildElement("CreditsText" + std::to_string(i))->getPosition().d_x.d_scale;
				_creditsWindow->getChildElement("CreditsText" + std::to_string(i))->setPosition(
					CEGUI::UVector2( CEGUI::UDim(posX, 0.0f ), CEGUI::UDim(posY - (1.0f/TIME_FOR_HIDING_CREDITS) * msecs, 0.0f ) ));
			}
			if(_creditsWindow->getChildElement("CreditsText0")->getPosition().d_y.d_scale < -(1.0f * numberOfPanels))
			{
				for (int i = 0; i < numberOfPanels; ++i)
					_creditsWindow->getChildElement("CreditsText" + std::to_string(i))->
						setPosition(CEGUI::UVector2( CEGUI::UDim(0.0f, 0.0f ), CEGUI::UDim(-(1.0f*numberOfPanels-i), 0.0f ) ));
				_creditsFinished = true;
				_startCredits = false;
			}

			
		}

		// Simulación física
		Physics::CServer::getSingletonPtr()->tick(msecs);

		// Actualizamos la lógica de juego.
		Logic::CServer::getSingletonPtr()->tick(msecs);

		_time += msecs;

	}

	//--------------------------------------------------------

	bool CCreditsState::keyPressed(GUI::TKey key)
	{
		return false;
	}

	//--------------------------------------------------------

	bool CCreditsState::keyReleased(GUI::TKey key)
	{
		switch(key.keyId)
		{
		case GUI::Key::ESCAPE:
			CApplicationState::buttonClickedSound->play();
			((Logic::CCameraMenu*)Logic::CServer::getSingletonPtr()->getMap()->getEntityByName("CameraMenu")->getComponent("CCameraMenu"))->goToMenu();
			_creditsHiding = true;
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
	
	bool CCreditsState::mouseMoved(const GUI::CMouseState &mouseState)
	{
		return false;
	}

	//--------------------------------------------------------
		
	bool CCreditsState::mousePressed(const GUI::CMouseState &mouseState)
	{
		return false;
	}

	//--------------------------------------------------------


	bool CCreditsState::mouseReleased(const GUI::CMouseState &mouseState)
	{
		return false;
	}
			
	//--------------------------------------------------------

	bool CCreditsState::backReleased(const CEGUI::EventArgs& e)
	{
		CApplicationState::buttonClickedSound->play();
		((Logic::CCameraMenu*)Logic::CServer::getSingletonPtr()->getMap()->getEntityByName("CameraMenu")->getComponent("CCameraMenu"))->goToMenu();
		_creditsHiding = true;
		
		return true;

	}

	//--------------------------------------------------------

	void CCreditsState::reloadGUI()
	{
		this->deactivate();
		this->release();
		this->init();
		this->activate();
	}

	//--------------------------------------------------------
		
	bool CCreditsState::buttonHover(const CEGUI::EventArgs& e)
	{
		buttonHoveredSound->play();
		return true;
	}

	//--------------------------------------------------------
		
	bool CCreditsState::buttonClicked(const CEGUI::EventArgs& e)
	{
		buttonClickedSound->play();
		return true;
	}
}
