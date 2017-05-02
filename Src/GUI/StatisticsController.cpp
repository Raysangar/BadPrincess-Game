/**
@file StatisticsController.cpp

Contiene la implementación de la clase CStatisticsController. Se encarga de
recibir eventos del teclado y del ratón y de interpretarlos para
mover al jugador.

@see GUI::CStatisticsController

@author David Llansó
@date Agosto, 2010
*/

#include "StatisticsController.h"

#include <iostream>
#include <cassert>

#include "InputManager.h"
#include "BaseSubsystems/Server.h"
#include "AI\Server.h"
#include "AI\EntityManager.h"

#include "CEGUI\CEGUI.h"

#define MAX_UNITS "16"


namespace GUI {
	CStatisticsController::CStatisticsController():_timerWindow(nullptr),_looterWindow(nullptr),_assassinWindow(nullptr),_knightWindow(nullptr),_looterCounter(0),_assassinCounter(0),_knightCounter(0),_wavesCounterWindow(nullptr),_wavesCounterLabel(nullptr)
	{
	}
	// CStatisticsController

	//--------------------------------------------------------
	
	CStatisticsController::~CStatisticsController()
	{
	} // ~CStatisticsController

	//--------------------------------------------------------

	void CStatisticsController::awake()
	{
		_knightWindow =CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChild("Statistics/KnightCounterLabel");
		_assassinWindow =CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChild("Statistics/AssassinCounterLabel");
		_looterWindow =CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChild("Statistics/LooterCounterLabel");

		_timerWindow =CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChild("Statistics/Timer");

		_wavesCounterLabel =CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChild("Statistics/WavesCounter/WavesCounterLabel");
		_wavesCounterWindow =CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChild("Statistics/WavesCounter");

		reset();
		addKnightCounter(AI::CServer::getSingletonPtr()->getEntityManager()->getEntitiesNumber(AI::CEntityManager::Enemy,"Knight"));
		addAssassinCounter(AI::CServer::getSingletonPtr()->getEntityManager()->getEntitiesNumber(AI::CEntityManager::Enemy,"Assassin"));
		addLooterCounter(AI::CServer::getSingletonPtr()->getEntityManager()->getEntitiesNumber(AI::CEntityManager::Enemy,"ThiefEnemy"));
	}
	void CStatisticsController::reset()
	{
		_knightCounter = 0;
		_knightWindow->setText("0");
		_assassinCounter = 0;
		_assassinWindow->setText("0");
		_looterCounter = 0;
		_looterWindow->setText("0");
		_timerWindow->setVisible(false);
		setWavesCounterText("0");
//		hideWavesCounter();
	}
	//--------------------------------------------------------

	void CStatisticsController::showWavesCounter()
	{
		_wavesCounterWindow->setVisible(true);
	}
	void CStatisticsController::hideWavesCounter()
	{
		_wavesCounterWindow->setVisible(false);
	}
	void CStatisticsController::setWavesCounterText(std::string txt)
	{
		_wavesCounterLabel->setText(txt);
	}


	void CStatisticsController::activate()
	{		
		//Setup again the windows to use F5 button
		_timerWindow =CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChild("Statistics/Timer");
		_goblinQuantityText = CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChild("Statistics/GoblinQuantity");
		setGoblinCounter(AI::CServer::getSingletonPtr()->getEntityManager()->getEntitiesNumber(AI::CEntityManager::Ally,""));

		_wavesCounterLabel =CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChild("Statistics/WavesCounter/WavesCounterLabel");
		_wavesCounterWindow =CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChild("Statistics/WavesCounter");

		//CInputManager::getSingletonPtr()->addKeyListener(this);
		//CInputManager::getSingletonPtr()->addMouseListener(this);
		_knightWindow =CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChild("Statistics/KnightCounterLabel");
		addKnightCounter(0);
		_assassinWindow =CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChild("Statistics/AssassinCounterLabel");
		addAssassinCounter(0);
		_looterWindow =CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChild("Statistics/LooterCounterLabel");
		addLooterCounter(0);



	} // activate


	//--------------------------------------------------------
	void CStatisticsController::deactivate()
	{
		//CInputManager::getSingletonPtr()->removeKeyListener(this);
		//CInputManager::getSingletonPtr()->removeMouseListener(this);

		
	} // deactivate

	//--------------------------------------------------------

	bool CStatisticsController::keyPressed(TKey key){
		return false;

	} // keyPressed

	//--------------------------------------------------------

	bool CStatisticsController::keyReleased(TKey key){

		return false;

	} // keyReleased

	//--------------------------------------------------------
	
	bool CStatisticsController::mouseMoved(const CMouseState &mouseState,const bool alreadyManaged)
	{	
			
		return false;
		
	} // mouseMoved

	//--------------------------------------------------------
		
	bool CStatisticsController::mousePressed(const CMouseState &mouseState,const bool alreadyManaged)
	{
		return false;
	
	} // mousePressed

	//--------------------------------------------------------

	bool CStatisticsController::mouseReleased(const CMouseState &mouseState,const bool alreadyManaged)
	{
		
			return false;
	} // mouseReleased

	//--------------------------------------------------------

	void CStatisticsController::setTimerTextColor(std::string textColor)
	{
		_timerWindow->setProperty("NormalTextColour",textColor);
	}

	//--------------------------------------------------------

	void CStatisticsController::setTimerText(std::string text)
	{
		_timerWindow->setText(text);

	}

	//--------------------------------------------------------

	void CStatisticsController::setTimerVisibility(bool isVisible)
	{
		_timerWindow->setVisible(isVisible);
	}
	
	//--------------------------------------------------------

	void CStatisticsController::setGoblinCounter(int num)
	{
		if(num < 0)
			num = 0;
		_goblinQuantityText->setText(std::to_string(num) + " / " + MAX_UNITS);
	}
	void CStatisticsController::addKnightCounter(int number)
	{
		_knightCounter+=number;
		if(_knightCounter < 0)
			_knightCounter = 0;
		_knightWindow->setText(std::to_string(_knightCounter));
//		std::cout<<"knight counter ="<<_knightCounter<<std::endl;
	}

	void CStatisticsController::addAssassinCounter(int number)
	{
		_assassinCounter+=number;
		if(_assassinCounter < 0)
			_assassinCounter = 0;
		_assassinWindow->setText(std::to_string(_assassinCounter));
//		std::cout<<"assassin counter ="<<_assassinCounter<<std::endl;
	}

	void CStatisticsController::addLooterCounter(int number)
	{
		_looterCounter+=number;
		if(_looterCounter < 0)
			_looterCounter = 0;
		_looterWindow->setText(std::to_string(_looterCounter));
//		std::cout<<"looter counter ="<<_looterCounter<<std::endl;

	}
	void CStatisticsController::addEntityCounter(std::string entityType,int qta)
	{
//		std::cout<<"addEntityCounter "<<entityType<<" "<<qta<<std::endl;
		if(entityType.compare("Knight")==0)
			addKnightCounter(qta);
		else if(entityType.compare("Assassin")==0)
			addAssassinCounter(qta);
		else if(entityType.compare("ThiefEnemy")==0)
			addLooterCounter(qta);
	}
} // namespace GUI
