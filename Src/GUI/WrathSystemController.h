/**
@file WrathSystemController.h

Contiene la declaración de la clase CWrathSystemController. Se encarga de gestionar los
eventos en la parte de la interfaz dedicada a la ira de la princesa

@author Ivan Leon
@date Mayo, 2015
*/
#ifndef __GUI_WrathSystemController_H
#define __GUI_WrathSystemController_H

#include "InputManager.h"


namespace CEGUI
{	
	class Window;
	class EventArgs;
	class ProgressBar;
	class PushButton;
}

namespace Logic
{
	class CEntity;
	class CPrincessManager;

}


// Declaración de la clase
namespace GUI
{
	/**
	
	*/
	class CWrathSystemController : public CKeyboardListener, public CMouseListener
	{
	public:

		/**
		Constructor.
		*/
		CWrathSystemController::CWrathSystemController():_activeUnit(0),_princessPortrait(0),_wrathBarIn(0),_wrathBarOut(0),_abilityQ(0),_abilityW(0),_abilityE(0),_abilityR(0),_currentWrath(0),
			clickAction(false),_princessComponent(0)
		{}

		/**
		Destructor.
		*/
		~CWrathSystemController();

		void awake();

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
		/*
		Resetea todas las informaciones relativa a una sigola partida (GameState)
		*/
		void reset();
		/** 
		Comprueba si ha habido eventos en los dispositivos de entrada.
		Debe llamarse en cada vuelta de la aplicación.
		*/
		void tick();

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
		//Método que será invocado siempre que se termine la pulsación
		//de un botón.

		//@param mouseState Estado del ratón cuando se lanza el evento.
		//@return true si el evento ha sido procesado. En este caso 
		//el gestor no llamará a otros listeners.
		*/
		bool mouseReleased(const CMouseState &mouseState,const bool alreadyManaged);

		void setWrathLevel(int wrath) { _currentWrath = wrath; }

	protected:

		//Variables referentes a la unidad
		Logic::CEntity* _activeUnit;

		//Elementos de la interfaz
		CEGUI::Window* _wrathBarIn;
		CEGUI::Window* _wrathBarOut;
		CEGUI::Window* _princessPortrait;

		CEGUI::PushButton *_abilityQ;

		CEGUI::PushButton *_abilityW;

		CEGUI::PushButton *_abilityE;

		CEGUI::PushButton *_abilityR;

		std::list<CEGUI::Window*> elements;

		int _currentWrath;

		/*Variable que indica que hemos gestionado un evento de CEGUI 
			se usa porque gestionamos el evento CEGUI::PushButton::EventMouseButtonDown asi que podemos decir a los otro componente que escuchan el mouseRelese de CEGUI que hemos
			hecho algo en la interfaz
		*/
		bool clickAction;


		
	private :

		/*
		Metodo que recoge los clicks sin funcionalidad en esta parte de la interfaz

		@param e Es el evento de CEGUI a que somos registrado
		*/
		bool handleMouseClick(const CEGUI::EventArgs &e);

		Logic::CPrincessManager *_princessComponent;

		
	}; // class CWrathSystemController

} // namespace GUI

#endif // __GUI_WrathSystemController_H
