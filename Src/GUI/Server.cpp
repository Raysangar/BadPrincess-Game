/**
@file Server.cpp

Contiene la implementación de la clase CServer, Singleton que se encarga de
la gestión de la interfaz con el usuario (entrada de periféricos, CEGui...).

@see GUI::CServer

@author David Llansó
@date Agosto, 2010
*/

#include "Server.h"

#include "PlayerController.h"
#include "ResourceCountController.h"
#include "MinimapController.h"
#include "ActiveUnitController.h"
#include "SelectedUnitsController.h"
#include "WrathSystemController.h"
#include "MessagesController.h"
#include "StatisticsController.h"
#include "EventsController.h"

#include "BaseSubsystems/Server.h"

#include <cassert>
#include <CEGUI/System.h>
#include <CEGUI/WindowManager.h>
#include <CEGUI/Window.h>
#include <CEGUI/SchemeManager.h>
#include <CEGUI/FontManager.h>
#include <CEGUI/ImageManager.h>
#include <CEGUI\CEGUI.h>

namespace GUI {

	CServer* CServer::_instance = 0;

	//--------------------------------------------------------

	CServer::CServer() : _resourceCountController(0),_playerController(0),_minimapController(0),_activeUnitController(0),_messagesController(0),_statisticsController(0),_eventsController(0)
	{
		_instance = this;

	} // CServer

	//--------------------------------------------------------

	CServer::~CServer()
	{
		_instance = 0;

	} // ~CServer
	
	//--------------------------------------------------------

	bool CServer::Init()
	{
		assert(!_instance && "Segunda inicialización de GUI::CServer no permitida!");

		new CServer();

		if (!_instance->open())
		{
			Release();
			return false;
		}

		return true;

	} // Init

	//--------------------------------------------------------

	void CServer::Release()
	{
		assert(_instance && "GUI::CServer no está inicializado!");

		if(_instance)
		{
			_instance->close();
			delete _instance;
		}

	} // Release

	//--------------------------------------------------------

	bool CServer::open()
	{
		_playerController = new CPlayerController();		
		_resourceCountController = new CResourceCountController();
		_minimapController = new CMinimapController();
		_activeUnitController = new CActiveUnitController();
		_selectedUnitsController = new CSelectedUnitsController();
		_wrathSysyemController = new CWrathSystemController();
		_messagesController = new CMessagesController();
		_statisticsController = new CStatisticsController();
		_eventsController = new CEventsController();

		_GUISystem = BaseSubsystems::CServer::getSingletonPtr()->getGUISystem();

		// Cargamos las distintas plantillas o esquemas de fichero
		// que usaremos en nuestro GUI.
		// (automáticamente cargan los archivos looknfeel e imageset)
		CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
		CEGUI::SchemeManager::getSingleton().createFromFile("OgreTray.scheme");
		CEGUI::SchemeManager::getSingleton().createFromFile("Generic.scheme");
		CEGUI::SchemeManager::getSingleton().createFromFile("BadPrincessGame.scheme");
		
		CEGUI::BasicImage* mouseCursor = (CEGUI::BasicImage*) &CEGUI::ImageManager::getSingletonPtr()->get("BadPrincessHud/PointCursor");
		mouseCursor->setNativeResolution(CEGUI::Sizef(1600,1600));
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().setDefaultImage(mouseCursor);
		
		
		// Cargamos los archivos con las fuentes que usaremos.		
		CEGUI::Font& defaultFont = CEGUI::FontManager::getSingleton().createFromFile("Girls-24.font");
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().setDefaultFont(&defaultFont);

		CInputManager::getSingletonPtr()->addMouseListener(this);
		CInputManager::getSingletonPtr()->addKeyListener(this);

		return true;

	} // open

	//--------------------------------------------------------

	void CServer::close() 
	{
		CInputManager::getSingletonPtr()->removeKeyListener(this);
		CInputManager::getSingletonPtr()->removeMouseListener(this);

		delete _resourceCountController;		
		delete _activeUnitController;
		delete _selectedUnitsController;
		delete _playerController;
		delete _minimapController;
		delete _wrathSysyemController;
		delete _messagesController;
		delete _statisticsController;
		delete _eventsController;
	} // close

	//--------------------------------------------------------

	bool CServer::keyPressed(TKey key)
	{
		_GUISystem->getDefaultGUIContext().injectKeyDown((CEGUI::Key::Scan) key.keyId);    
		_GUISystem->getDefaultGUIContext().injectChar(key.text);    

		
		// Queremos que si hay más oyentes también reciban el evento
		return false;

	} // keyPressed

	//--------------------------------------------------------

	bool CServer::keyReleased(TKey key)
	{
		_GUISystem->getDefaultGUIContext().injectKeyUp((CEGUI::Key::Scan)key.keyId);

		
		// Queremos que si hay más oyentes también reciban el evento
		return false;

	} // keyReleased

	//--------------------------------------------------------
	
	bool CServer::mouseMoved(const CMouseState &mouseState,const bool alreadyManaged)
	{

#if defined NON_EXCLUSIVE_MODE_IN_WINDOW_MODE
		_GUISystem->getDefaultGUIContext().injectMousePosition((float)mouseState.posAbsX,(float)mouseState.posAbsY);
#else 
		_GUISystem->getDefaultGUIContext().injectMouseMove((float)mouseState.movX,(float)mouseState.movY);
#endif	
		// Queremos que si hay más oyentes también reciban el evento
		return false;

	} // mouseMoved

	//--------------------------------------------------------
		
	bool CServer::mousePressed(const CMouseState &mouseState,const bool alreadyManaged)
	{
		switch (mouseState.button)
		{
		case Button::LEFT:
			_GUISystem->getDefaultGUIContext().injectMouseButtonDown(CEGUI::LeftButton);
		case Button::RIGHT:
			_GUISystem->getDefaultGUIContext().injectMouseButtonDown(CEGUI::RightButton);
		case Button::MIDDLE:
			_GUISystem->getDefaultGUIContext().injectMouseButtonDown(CEGUI::MiddleButton);
		}

		// Queremos que si hay más oyentes también reciban el evento
		return false;

	} // mousePressed

	//--------------------------------------------------------

	bool CServer::mouseReleased(const CMouseState &mouseState,const bool alreadyManaged)
	{
		switch (mouseState.button)
		{
		case Button::LEFT:
			_GUISystem->getDefaultGUIContext().injectMouseButtonUp(CEGUI::LeftButton);
		case Button::RIGHT:
			_GUISystem->getDefaultGUIContext().injectMouseButtonUp(CEGUI::RightButton);
		case Button::MIDDLE:
			_GUISystem->getDefaultGUIContext().injectMouseButtonUp(CEGUI::MiddleButton);
		}

		// Queremos que si hay más oyentes también reciban el evento
		return false;

	} // mouseReleased
	
	void CServer::activate()
	{
		// Queremos que el GUI maneje la interfaz.	

		_playerController->activate();

		_minimapController->activate();

		_resourceCountController->activate();	

		_activeUnitController->activate();

		_selectedUnitsController->activate();

		_wrathSysyemController->activate();

		_messagesController->activate();

		_statisticsController->activate();

		_eventsController->activate();

	}

	void CServer::deactivate()
	{
		_wrathSysyemController->deactivate();

		_activeUnitController->deactivate();

		_selectedUnitsController->deactivate();

		_minimapController->deactivate();

		_resourceCountController->deactivate();

		_playerController->deactivate();

		_messagesController->deactivate();

		_statisticsController->deactivate();

		_eventsController->deactivate();

	}

	void CServer::awakeGameController()
	{
		_wrathSysyemController->awake();
		_resourceCountController->awake();
		_minimapController->awake();
		_playerController->awake();
		_selectedUnitsController->awake();
		_activeUnitController->awake();
		_statisticsController->awake();
	}

	void CServer::resetGameController()
	{
		_wrathSysyemController->reset();

		_activeUnitController->reset();

		_selectedUnitsController->reset();

		_minimapController->reset();

		_resourceCountController->reset();

		_playerController->reset();

		_messagesController->reset();

		_statisticsController->reset();

		//_eventsController->reset();
	}
	void CServer::tick(unsigned int msecs)
	{	 
		_minimapController->tick(msecs);
		_activeUnitController->tick();
		_selectedUnitsController->tick();
		_wrathSysyemController->tick();
		_messagesController->tick(msecs);
		_eventsController->tick(msecs);
		_playerController->tick(msecs);
	}

	//------------------------------

	void CServer::setMouseCursor(const std::string &name)
	{
		CEGUI::BasicImage* mouseCursor = (CEGUI::BasicImage*) &CEGUI::ImageManager::getSingletonPtr()->get(name);
		mouseCursor->setNativeResolution(CEGUI::Sizef(1600,1600));
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().setImage(mouseCursor);
	}

} // namespace GUI
