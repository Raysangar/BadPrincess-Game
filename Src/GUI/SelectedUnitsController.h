/**
@file SelectedUnitsController.h

Contiene la declaración de la clase CSelectedUnitsController. Se encarga de gestionar los
eventos en la parte de la interfaz dedicada a la unidad activa entre las seleccionadas.

@author Ivan Leon
@date Mayo, 2015
*/
#ifndef __GUI_SelectedUnitsController_H
#define __GUI_SelectedUnitsController_H



#include "InputManager.h"


namespace CEGUI
{	
	class Window;
	class EventArgs;
	class ProgressBar;
}

namespace Logic
{
	class CEntity;
	class CGUIManager;
	class CLife;
}


// Declaración de la clase
namespace GUI
{
	/**
	
	*/
	class CSelectedUnitsController : public CKeyboardListener, public CMouseListener
	{
	public:

		/**
		Constructor.
		*/
		CSelectedUnitsController::CSelectedUnitsController():_selectedUnits(0),_selectedUnitsFrame(0),_lifebars(0),_portraits(0),_lifeComponent(0),_GUIManagerComponent(0),
			_elementsActivated(false),clickAction(false)
		{}

		/**
		Destructor.
		*/
		~CSelectedUnitsController();

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

		/**
		Método para activar todos los elementos de la interfaz que cambien con la unidad
		*/
		void activateElements();		

		/**
		Método para desactivar todos los elementos de la interfaz que cambien con la unidad
		*/
		void deactivateElements();

		void setSelectedUnitsStandMode(bool areAggressive);

	protected:

		/**
		Lista de unidades seleccionadas
		*/
		std::list<Logic::CEntity*>* _selectedUnits;
		Logic::CEntity* _activeUnit;

		Logic::CGUIManager *_GUIManagerComponent;
		Logic::CLife *_lifeComponent;

		//Variables para los elementos 
		CEGUI::Window* _selectedUnitsFrame;
		CEGUI::Window* _activeUnitFrame;
		std::vector<CEGUI::Window*> _portraits;
		std::vector<CEGUI::ProgressBar*> _lifebars;

		bool _elementsActivated;

		/*Variable que indica que hemos gestionado un evento de CEGUI 
			se usa porque gestionamos el evento CEGUI::PushButton::EventMouseButtonDown asi que podemos decir a los otro componente que escuchan el mouseRelese de CEGUI que hemos
			hecho algo en la interfaz
		*/
		bool clickAction;
		
	private :
				

		/*
		Método para activar un elemento concreto

		@param element Elemento a activar
		*/
		void activateElement(CEGUI::Window* element);

		/*
		Método para desactivar un elemento concreto

		@param element Elemento a desactivar
		*/
		void deactivateElement(CEGUI::Window* element);

		
		/*
		Metodo que modifica la unidad activa

		@param e Es el evento de CEGUI a que somos registrado
		*/
		bool changeActiveUnit(const CEGUI::EventArgs &e);

		/*
		Metodo que recoge los clicks sin funcionalidad en esta parte de la interfaz

		@param e Es el evento de CEGUI a que somos registrado
		*/
		bool handleMouseClick(const CEGUI::EventArgs &e);

		
	}; // class CSelectedUnitsController

} // namespace GUI

#endif // __GUI_SelectedUnitsController_H
