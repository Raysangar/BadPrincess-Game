//---------------------------------------------------------------------------
// GameOverState.cpp
//---------------------------------------------------------------------------

/**
@file GameOverState.cpp

Contiene la implementación del estado de game over.

@see Application::CApplicationState
@see Application::CGameOverState

@author David Llansó
@date Agosto, 2010
*/

#include "GameOverState.h"
#include "LoadingGameState.h"
#include "GameState.h"

#include "BaseSubsystems\ScriptManager.h"

#include "AI\Server.h"
#include "AI\GameManager.h"

#include "GUI/Server.h"

#include <CEGUI/CEGUI.h>

namespace Application {

	CGameOverState::~CGameOverState() 
	{
	} // ~CGameOverState

	//--------------------------------------------------------

	bool CGameOverState::init() 
	{
		CApplicationState::init();

		// Cargamos la ventana que muestra el menú
		_gameOverWindow  = CEGUI::WindowManager::getSingletonPtr()->loadLayoutFromFile("GameOver.layout");
		
		_gameOverWindow->getChildElement("BackGround/Retry")->
			subscribeEvent(CEGUI::PushButton::EventClicked, 
			CEGUI::SubscriberSlot(&CGameOverState::retryReleased, this));
		_gameOverWindow->getChildElement("BackGround/Retry")->
			subscribeEvent(CEGUI::PushButton::EventMouseEntersSurface, 
			CEGUI::SubscriberSlot(&CGameOverState::buttonHover, this));

		_gameOverWindow->getChildElement("BackGround/Menu")->
			subscribeEvent(CEGUI::PushButton::EventClicked, 
			CEGUI::SubscriberSlot(&CGameOverState::menuReleased, this));
		_gameOverWindow->getChildElement("BackGround/Menu")->
			subscribeEvent(CEGUI::PushButton::EventMouseEntersSurface, 
			CEGUI::SubscriberSlot(&CGameOverState::buttonHover, this));

		_gameOverWindow->getChildElement("BackGround/Continue")->
			subscribeEvent(CEGUI::PushButton::EventClicked, 
			CEGUI::SubscriberSlot(&CGameOverState::continueReleased, this));
		_gameOverWindow->getChildElement("BackGround/Continue")->
			subscribeEvent(CEGUI::PushButton::EventMouseEntersSurface, 
			CEGUI::SubscriberSlot(&CGameOverState::buttonHover, this));
	
		return true;

	} // init

	//--------------------------------------------------------

	void CGameOverState::release() 
	{
		CApplicationState::release();

	} // release

	//--------------------------------------------------------

	void CGameOverState::activate() 
	{
		CApplicationState::activate();

		// Activamos la ventana que nos muestra el menú y activamos el ratón.
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().setRootWindow(_gameOverWindow);
		_gameOverWindow->setVisible(true);
		_gameOverWindow->activate();
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().show();

		AI::GameProperties lastGame = AI::CServer::getSingletonPtr()->getGameManager()->getLastGameProperties();

		if(lastGame._currentGameType.compare("survivor")==0)
		{
			int survivorRecord = ScriptManager::CScriptManager::GetPtrSingleton()->getField("UserProfile","survivorRecord",0);
			if(survivorRecord<lastGame._currentSurvivalPoint)
			{
				ScriptManager::CScriptManager::GetPtrSingleton()->setGlobalField("UserProfile","survivorRecord",lastGame._currentSurvivalPoint);
				ScriptManager::CScriptManager::GetPtrSingleton()->executeProcedure("saveUserProfile");
			}
			setupSurvivalPointUI(survivorRecord,lastGame._currentSurvivalPoint);
		}else if(lastGame._currentGameType.compare("campaign")==0)
		{
			if(lastGame._hasWin)
			{
				int userCampaignMaxLevel = ScriptManager::CScriptManager::GetPtrSingleton()->getField("UserProfile","campaignLevel",0);
				int campaignMaxLevel = ScriptManager::CScriptManager::GetPtrSingleton()->getField("UserProfile","maxCampaignLevel",0);
				//increase the campaign leve only if the user it isn't at last level and it still play to last mission available
				if(lastGame._currentCampaignLevel==userCampaignMaxLevel && lastGame._currentCampaignLevel<campaignMaxLevel)
				{
					ScriptManager::CScriptManager::GetPtrSingleton()->setGlobalField("UserProfile","campaignLevel",(lastGame._currentCampaignLevel+1));
					ScriptManager::CScriptManager::GetPtrSingleton()->executeProcedure("saveUserProfile");
				}
			}
			setupCampaignPointUI(lastGame._hasWin);
		}
		else if(lastGame._currentGameType.compare("tutorial")==0)
		{
			setupTutorialUI(lastGame._hasWin);
		}
	} // activate

	//--------------------------------------------------------

	void CGameOverState::deactivate() 
	{		
		// Desactivamos la ventana GUI con el menú y el ratón.
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().hide();
		_gameOverWindow->deactivate();
		_gameOverWindow->setVisible(false);

		CApplicationState::deactivate();

	} // deactivate

	//--------------------------------------------------------

	void CGameOverState::tick(unsigned int msecs) 
	{
		CApplicationState::tick(msecs);

	} // tick

	//--------------------------------------------------------

	bool CGameOverState::keyPressed(GUI::TKey key)
	{
		return false;

	} // keyPressed

	//--------------------------------------------------------

	bool CGameOverState::keyReleased(GUI::TKey key)
	{
		switch(key.keyId)
		{
		case GUI::Key::ESCAPE:
			goToMenu();
			break;
		case GUI::Key::RETURN:
			restartGame();
			break;
			#ifdef _DEBUG
		case GUI::Key::F5:
			reloadGUI();
			break;
		#endif
		default:
			return false;
		}
		return true;

	} // keyReleased

	//--------------------------------------------------------
	
	bool CGameOverState::mouseMoved(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mouseMoved

	//--------------------------------------------------------
		
	bool CGameOverState::mousePressed(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mousePressed

	//--------------------------------------------------------


	bool CGameOverState::mouseReleased(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mouseReleased
			
	//--------------------------------------------------------

		
	bool CGameOverState::buttonHover(const CEGUI::EventArgs& e)
	{
		buttonHoveredSound->play();
		return true;
	}

	//--------------------------------------------------------
	bool CGameOverState::menuReleased(const CEGUI::EventArgs& e)
	{
		buttonHoveredSound->stop();		
		menuMusic->stop();
		menuMusic->unload();
		goToMenu();
		return true;
	}

	bool CGameOverState::retryReleased(const CEGUI::EventArgs& e)
	{
		buttonHoveredSound->stop();		
		menuMusic->stop();
		menuMusic->unload();
		restartGame();
		return true;
	}
	bool CGameOverState::continueReleased(const CEGUI::EventArgs& e)
	{
		buttonHoveredSound->stop();		
		menuMusic->stop();
		menuMusic->unload();
		goToNextLevel();
		return true;
	}
	void CGameOverState::goToNextLevel()
	{
		AI::GameProperties lastGame = AI::CServer::getSingletonPtr()->getGameManager()->getLastGameProperties();
		if(_app->setState("loading"))
		{
			std::string name= static_cast<CGameState*>( _app->findState("game"))->getName();
			CLoadingGameState *loading= static_cast<CLoadingGameState*>( _app->findState("loading"));
			//we have to sum 2 at the last campaign level because +1 to go to next level and +1 because the level name start from 1 to N but the game level integer starts from 0 to N-1 
			loading->loadGame("level"+std::to_string((lastGame._currentCampaignLevel+2)));
		}
	}

	void CGameOverState::restartGame()
	{
		if(_app->setState("loading"))
		{
			std::string name= static_cast<CGameState*>( _app->findState("game"))->getName();
			CLoadingGameState *loading= static_cast<CLoadingGameState*>( _app->findState("loading"));
			loading->loadGame(name);
		}
	}

	void CGameOverState::goToMenu()
	{
		CLoadingGameState *loading= static_cast<CLoadingGameState*>( _app->findState("loading"));
		if(_app->setState("loading"))
		{			
			loading->loadGame("Menu");
		}
	}
	void CGameOverState::reloadGUI()
	{
		this->deactivate();
		this->release();
		this->init();
		this->activate();
	}
	void CGameOverState::setupCampaignPointUI(bool playerWon)
	{
		_gameOverWindow->getChildElement("BackGround/Survivor")->setProperty("Visible","false");
		_gameOverWindow->getChildElement("BackGround/Campaign")->setProperty("Visible","true");
		_gameOverWindow->getChildElement("BackGround/Tutorial")->setProperty("Visible","false");
		if(playerWon)
		{
					
			_gameOverWindow->getChildElement("BackGround/Campaign/TitleWin")->setProperty("Visible","true");
			_gameOverWindow->getChildElement("BackGround/Campaign/TitleLost")->setProperty("Visible","false");
			//check if the user has more campaign to play
			int campaignMaxLevel = ScriptManager::CScriptManager::GetPtrSingleton()->getField("UserProfile","maxCampaignLevel",0);
			AI::GameProperties lastGame = AI::CServer::getSingletonPtr()->getGameManager()->getLastGameProperties();
			if(lastGame._currentCampaignLevel < campaignMaxLevel)
			{
				_gameOverWindow->getChildElement("BackGround/Continue")->setProperty("Visible","true");
				menuMusic = new Audio::CEvent("event:/Music/WonGame");
			}
			else
			{
				menuMusic = new Audio::CEvent("event:/Music/CampaignEnding");
				_gameOverWindow->getChildElement("BackGround/Continue")->setProperty("Visible","false");
			}
		}else
		{
			menuMusic = new Audio::CEvent("event:/Music/LostGame");
			_gameOverWindow->getChildElement("BackGround/Campaign/TitleWin")->setProperty("Visible","false");
			_gameOverWindow->getChildElement("BackGround/Campaign/TitleLost")->setProperty("Visible","true");
			_gameOverWindow->getChildElement("BackGround/Continue")->setProperty("Visible","false");
		}
		menuMusic->load();
		menuMusic->play();

	}
	void CGameOverState::setupSurvivalPointUI(int record,int currentPoint)
	{
		_gameOverWindow->getChildElement("BackGround/Survivor")->setProperty("Visible","true");
		_gameOverWindow->getChildElement("BackGround/Campaign")->setProperty("Visible","false");
		_gameOverWindow->getChildElement("BackGround/Tutorial")->setProperty("Visible","false");
		menuMusic = new Audio::CEvent("event:/Music/LostGame");
		if(currentPoint>=record)
		{
			_gameOverWindow->getChildElement("BackGround/Survivor/ScoreRecordIcon")->setProperty("Visible","true");
			_gameOverWindow->getChildElement("BackGround/Survivor/BestScoreLabel")->setProperty("Text","Your best score is: "+std::to_string(currentPoint));
		}
		else
		{
			_gameOverWindow->getChildElement("BackGround/Survivor/ScoreRecordIcon")->setProperty("Visible","false");
			_gameOverWindow->getChildElement("BackGround/Survivor/BestScoreLabel")->setProperty("Text","Your best score is: "+std::to_string(record));
		}
		_gameOverWindow->getChildElement("BackGround/Survivor/ScoreLabel2")->setProperty("Text","[font='Girls-48']"+std::to_string(currentPoint)+"[font='Girls-20']      waves ");
		_gameOverWindow->getChildElement("BackGround/Continue")->setProperty("Visible","false");
		menuMusic->load();
		menuMusic->play();
	}

	void CGameOverState::setupTutorialUI(bool playerWon)
	{
		_gameOverWindow->getChildElement("BackGround/Survivor")->setProperty("Visible","false");
		_gameOverWindow->getChildElement("BackGround/Campaign")->setProperty("Visible","false");
		_gameOverWindow->getChildElement("BackGround/Tutorial")->setProperty("Visible","true");
		if(playerWon)
		{
			menuMusic = new Audio::CEvent("event:/Music/WonGame");		
			_gameOverWindow->getChildElement("BackGround/Tutorial/TitleWin")->setProperty("Visible","true");
			_gameOverWindow->getChildElement("BackGround/Tutorial/TitleLost")->setProperty("Visible","false");
		}else
		{
			menuMusic = new Audio::CEvent("event:/Music/LostGame");
			_gameOverWindow->getChildElement("BackGround/Tutorial/TitleWin")->setProperty("Visible","false");
			_gameOverWindow->getChildElement("BackGround/Tutorial/TitleLost")->setProperty("Visible","true");
		}
			_gameOverWindow->getChildElement("BackGround/Continue")->setProperty("Visible","false");

		menuMusic->load();
		menuMusic->play();
	}

} // namespace Application
