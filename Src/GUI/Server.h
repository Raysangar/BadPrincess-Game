/**
@file Server.h

Contiene la declaraci�n de la clase CServer, Singleton que se encarga de
la gesti�n de la interfaz con el usuario (entrada de perif�ricos, CEGui...).

@see GUI::CServer

@author David Llans�
@date Agosto, 2010
*/
#ifndef __GUI_Server_H
#define __GUI_Server_H

#include "InputManager.h"

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
namespace Logic 
{
	class CAvatarController;
}

namespace GUI 
{
	class CPlayerController;	
	class CResourceCountController;
	class CMinimapController;
	class CActiveUnitController;
	class CWrathSystemController;
	class CSelectedUnitsController;
	class CMessagesController;
	class CStatisticsController;
	class CEventsController;
}

namespace CEGUI
{
	class System;
}

// Declaraci�n de la clase
namespace GUI
{
	/**
	Servidor del m�dulo GUI que hace de interfaz con el usuario. Est� 
	implementado como un singlenton de inicializaci�n expl�cita. Sirve 
	para comunicar a CEGUI los eventos producidos ya as� hacer funcionar
	la interfaz gr�fica de usuario y permite que se establezca que entidad 
	es controlada por el GUI::CPlayerController. Este control se hace a 
	trav�s del componente Logic::CAvatarController.

	@ingroup GUIGroup

	@author David Llans�
	@date Agosto, 2010
	*/
	class CServer : public CKeyboardListener, public CMouseListener
	{
	public:

		/**
		Devuelve la �nica instancia de la clase CServer.
		
		@return �nica instancia de la clase CServer.
		*/
		static CServer* getSingletonPtr() {return _instance;}

		/**
		Inicializa la instancia

		@return Devuelve false si no se ha podido inicializar.
		*/
		static bool Init();

		/**
		Libera la instancia de CServer. Debe llamarse al finalizar la 
		aplicaci�n.
		*/
		static void Release();

		/**
		Devuelve la instancia de la clase GUI que se encarga de la gestion de la interfaz del usuario

		@return Instancia de la clase GUI que controla la interfaz.
		*/
		CPlayerController *getPlayerController(){return _playerController;}

		CResourceCountController *getResourceCountController() {return _resourceCountController;}

		CMinimapController *getMinimapController() { return _minimapController;}

		CActiveUnitController *getActiveUnitController() { return _activeUnitController; }

		CWrathSystemController *getWrathSystemController() { return _wrathSysyemController; }

		CSelectedUnitsController *getSelectedUnitsController() { return _selectedUnitsController; }

		CMessagesController *getMessagescontroller() { return _messagesController; }

		CStatisticsController *getStatisticsController() { return _statisticsController; }

		CEventsController *getEventscontroller() { return _eventsController; }

		
		/***************************************************************
		M�todos de CKeyboardListener
		***************************************************************/
		
		/**
		M�todo que ser� invocado siempre que se pulse una tecla.

		@param key C�digo de la tecla pulsada.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamar� a otros listeners.
		*/
		bool keyPressed(TKey key);
		
		/**
		M�todo que ser� invocado siempre que se termine la pulsaci�n
		de una tecla.

		@param key C�digo de la tecla pulsada.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamar� a otros listeners.
		*/
		bool keyReleased(TKey key);
		
		/***************************************************************
		M�todos de CKeyboardListener
		***************************************************************/
		
		/**
		M�todo que ser� invocado siempre que se mueva el rat�n.

		@param mouseState Estado del rat�n cuando se lanza el evento.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamar� a otros listeners.
		*/
		bool mouseMoved(const CMouseState &mouseState,const bool alreadyManaged);
		
		/**
		M�todo que ser� invocado siempre que se pulse un bot�n.

		@param mouseState Estado del rat�n cuando se lanza el evento.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamar� a otros listeners.
		*/
		bool mousePressed(const CMouseState &mouseState,const bool alreadyManaged);

		/**
		M�todo que ser� invocado siempre que se termine la pulsaci�n
		de un bot�n.

		@param mouseState Estado del rat�n cuando se lanza el evento.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamar� a otros listeners.
		*/
		bool mouseReleased(const CMouseState &mouseState,const bool alreadyManaged);
		
		void activate();

		void deactivate();

		void awakeGameController();

		void resetGameController();

		void tick(unsigned int msecs);

		void setMouseCursor(const std::string &name);

	protected:

		/**
		Constructor.
		*/
		CServer ();

		/**
		Destructor.
		*/
		~CServer();

		/**
		Segunda fase de la construcci�n del objeto. Sirve para hacer
		inicializaciones de la propia instancia en vez de inicializaciones 
		est�ticas.

		@return true si todo fue correctamente.
		*/
		bool open();

		/**
		Segunda fase de la destrucci�n del objeto. Sirve para hacer liberar 
		los recursos de la propia instancia, la liberaci�n de los recursos 
		est�ticos se hace en Release().
		*/
		void close();

		/**
		Clase GUI que se encarga de gestionar la entrada de teclado para el jugador
		*/
		CPlayerController *_playerController;

		/**
		Clase GUI que se encarga de la gestion de la interfaz del usuario
		*/
		CResourceCountController *_resourceCountController;

		/**
		Clase GUI que se encarga de la gestion del minimapa
		*/
		CMinimapController *_minimapController;
		
		/**
		Clase GUI que se encarga de la gestion de la interfaz para la unidad activa
		*/
		CActiveUnitController *_activeUnitController;

		/**
		Clase GUI que se encarga de la gestion de la interfaz para la ira de la princesa
		*/
		CWrathSystemController *_wrathSysyemController;

		/**
		Clase GUI que se encarga de la gestion de la interfaz para la ira de la princesa
		*/
		CSelectedUnitsController *_selectedUnitsController;

		/**
		Clase GUI que se encarga de la gestion de los mensajes de la interfaz.
		*/
		CMessagesController *_messagesController;
		/**
		Clase GUI que se encarga de la gestion del timer y de los numero de entidades aliada y enemiga.
		*/
		CStatisticsController *_statisticsController;

		/**
		Clase GUI que se encarga de la gestion de los eventos de la interfaz.
		*/
		CEventsController *_eventsController;

		/**
		Sistema de la interfaz gr�fica de usuario CEGUI.
		*/
		CEGUI::System *_GUISystem;

	private:
		/**
		�nica instancia de la clase.
		*/
		static CServer* _instance;


	}; // class CServer

} // namespace GUI

#endif // __GUI_Server_H
