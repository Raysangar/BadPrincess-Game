/**
@file ResourceCountController.h

Contiene la declaración de la clase CMapController. Se encarga de
recibir eventos del teclado y del ratón y de interpretarlos para
mover al jugador.

@see GUI::CResourceCountController

@author David Llansó
@date Agosto, 2010
*/
#ifndef __GUI_ResourceCountController_H
#define __GUI_ResourceCountController_H



#include "InputManager.h"


namespace CEGUI
{
	class Listbox;
	class Window;
	class EventArgs;	
}


// Declaración de la clase
namespace GUI
{
	/**
	Esta clase sirve para gestionar el teclado y el ratón, y mover un avatar 
	acorde con las pulsaciones y el movimiento. En el constructor se registra en 
	el gestor de teclado y en el de ratón (InputManager) para ser avisado ante 
	los eventos. No obstante, puede activarse y desactivarse a voluntad con 
	activate() y deactivate(). El componente CAvatarController del jugador
	es responsable de ponerse como foco a traves de GUI::CServer y así recibir 
	las instrucciones dadas por esta clase.

	@ingroup GUIGroup

	@author David Llansó
	@date Agosto, 2010
	*/
	class CResourceCountController : public CKeyboardListener, public CMouseListener
	{
	public:

		/**
		Constructor.
		*/
		CResourceCountController ();

		/**
		Destructor.
		*/
		~CResourceCountController();

	

		/**
		Activa la la clase, se registra en el CInputManager y así empieza a 
		escuchar eventos.
		*/
		void activate();

		/**
		Desctiva la la clase, se deregistra en el CInputManager y deja de 
		escuchar eventos.
		*/
		void deactivate();

		void awake();
		/*
		Resetea todas las informaciones relativa a una sigola partida (GameState)
		*/
		void reset();
		/***************************************************************
		Métodos de CKeyboardListener
		***************************************************************/
		
		/**
		Método que será invocado siempre que se pulse una tecla.

		@param key Código de la tecla pulsada.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamará a otros listeners.
		*/
		bool keyPressed(TKey key);
		
		/**
		Método que será invocado siempre que se termine la pulsación
		de una tecla.

		@param key Código de la tecla pulsada.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamará a otros listeners.
		*/
		bool keyReleased(TKey key);

		/***************************************************************
		Métodos de CKeyboardListener
		***************************************************************/
		
		/**
		Método que será invocado siempre que se mueva el ratón.

		@param mouseState Estado del ratón cuando se lanza el evento.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamará a otros listeners.
		*/
		bool mouseMoved(const CMouseState &mouseState,const bool alreadyManaged);
		
		/**
		Método que será invocado siempre que se pulse un botón.

		@param mouseState Estado del ratón cuando se lanza el evento.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamará a otros listeners.
		*/
		bool mousePressed(const CMouseState &mouseState,const bool alreadyManaged);

		/**
		Método que será invocado siempre que se termine la pulsación
		de un botón.

		@param mouseState Estado del ratón cuando se lanza el evento.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamará a otros listeners.
		*/
		bool mouseReleased(const CMouseState &mouseState,const bool alreadyManaged);

		/*
		Metodo que se invoca para escribir un mensaje en la ventana del log

		@param message Stringa que queriamos imprimir en la ventana
		*/
		void printMessageToLog(const std::string message);

		void setWoodQuantity(int quantity);
		void setIronQuantity(int quantity);
		void setStoneQuantity(int quantity);
		void setWrathLevel(int wrath);

	private :

		//CEGUI::Listbox* _listboxWindow;
		CEGUI::Window* stoneQuantityText;
		CEGUI::Window* woodQuantityText;
		CEGUI::Window* ironQuantityText;

		CEGUI::Window* stoneIcon;
		CEGUI::Window* woodIcon;
		CEGUI::Window* ironIcon;
		
		std::list<CEGUI::Window *> elements;

		///**
		//Entero para guardar el ancho de la ventana
		//*/
		//unsigned int _windowWidth;
		//
		///**
		//Entero para guardar el alto de la ventana
		//*/
		//unsigned int _windowHeight;
		//
	

		int _stoneQuantity;
		int _ironQuantity;
		int _woodQuantity;

		///*
		//Metodo que se invoca para abrir la ventana del log

		//@param e Es el evento de CEGUI a que somos registrado
		//*/
		//bool showLogList(const CEGUI::EventArgs &e);
		///*
		//Metodo que se invoca para cerrar la ventana del log

		//@param e Es el evento de CEGUI a que somos registrado
		//*/
		//bool closeLogList(const CEGUI::EventArgs &e);
		///*
		//Metodo que se invoca cuando pulsamo en la ventana del log (por ejemplo sirve para decir que estamos haciendo scroll sobre esa ventana)

		//@param e Es el evento de CEGUI a que somos registrado
		//*/
		//bool logWindowMouseMove(const CEGUI::EventArgs &e);

		/*
		Metodo que recoge los clicks sin funcionalidad en esta parte de la interfaz

		@param e Es el evento de CEGUI a que somos registrado
		*/
		bool handleMouseClick(const CEGUI::EventArgs &e);

		void subscribeAllElements();
		
		/*Variable que indica que hemos gestionado un evento de CEGUI 
			se usa porque gestionamos el evento CEGUI::PushButton::EventMouseButtonDown asi que podemos decir a los otro componente que escuchan el mouseRelese de CEGUI que hemos
			hecho algo en la interfaz
		*/
		bool clickAction;

		
	}; // class CResourceCountController

} // namespace GUI

#endif // __GUI_MapController_H
