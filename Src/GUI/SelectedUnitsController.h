/**
@file SelectedUnitsController.h

Contiene la declaraci�n de la clase CSelectedUnitsController. Se encarga de gestionar los
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


// Declaraci�n de la clase
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
		Activa la la clase, se registra en el CInputManager y as� empieza a 
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
		Debe llamarse en cada vuelta de la aplicaci�n.
		*/
		void tick();

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
		//M�todo que ser� invocado siempre que se termine la pulsaci�n
		//de un bot�n.

		//@param mouseState Estado del rat�n cuando se lanza el evento.
		//@return true si el evento ha sido procesado. En este caso 
		//el gestor no llamar� a otros listeners.
		*/
		bool mouseReleased(const CMouseState &mouseState,const bool alreadyManaged);

		/**
		M�todo para activar todos los elementos de la interfaz que cambien con la unidad
		*/
		void activateElements();		

		/**
		M�todo para desactivar todos los elementos de la interfaz que cambien con la unidad
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
		M�todo para activar un elemento concreto

		@param element Elemento a activar
		*/
		void activateElement(CEGUI::Window* element);

		/*
		M�todo para desactivar un elemento concreto

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
