//---------------------------------------------------------------------------
// LoadingGameState.h
//---------------------------------------------------------------------------

/**
@file LoadingGameState.h

Contiene la declaración del estado de carga del juego

@see Application::CApplicationState
@see Application::CLoadingGameState

@author Guidi Giacomo
@date Mayo, 2015
*/

#ifndef __Application_LoadingGameState_H
#define __Application_LoadingGameState_H

#include "ApplicationState.h"

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Application 
{
	class CBaseApplication;
}

namespace CEGUI
{
	class ProgressBar;
	class Window;
}

namespace Application 
{

	class CLoadingGameState : public CApplicationState 
	{
	public:
		/** 
		Constructor de la clase 
		*/
		CLoadingGameState(CBaseApplication *app) : CApplicationState(app),_gameName(""),_progressBar(nullptr),_loaded(true),_isFirstLaunch(true){}

		/** 
		Destructor 
		*/
		virtual ~CLoadingGameState();

		/**
		Función llamada cuando se crea el estado (se "engancha" en la
		aplicación, para que inicialice sus elementos.

		@return true si todo fue bien.
		*/
		virtual bool init();

		/**
		Función llamada cuando se elimina ("desengancha") el
		estado de la aplicación.
		*/
		virtual void release();

		/**
		Función llamada por la aplicación cuando se activa
		el estado.
		*/
		virtual void activate();

		/**
		Función llamada por la aplicación cuando se desactiva
		el estado.
		*/
		virtual void deactivate();

		/**
		Función llamada por la aplicación para que se ejecute
		la funcionalidad del estado.

		@param msecs Número de milisegundos transcurridos desde
		la última llamada (o desde la áctivación del estado, en caso
		de ser la primera vez...).
		*/
		virtual void tick(unsigned int msecs);

		virtual void reloadGUI();

		// Métodos de CKeyboardListener
		
		/**
		Método que será invocado siempre que se pulse una tecla. 
		Será la aplicación quién llame a este método cuando el 
		estado esté activo. Esta clase NO se registra en el 
		InputManager sino que es la aplicación quien lo hace y 
		delega en los estados.

		@param key Código de la tecla pulsada.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamará a otros listeners.
		*/
		virtual bool keyPressed(GUI::TKey key);
		
		/**
		Método que será invocado siempre que se termine la pulsación
		de una tecla. Será la aplicación quién llame a este método 
		cuando el estado esté activo. Esta clase NO se registra en
		el InputManager sino que es la aplicación quien lo hace y 
		delega en los estados.

		@param key Código de la tecla pulsada.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamará a otros listeners.
		*/
		virtual bool keyReleased(GUI::TKey key);

		// Métodos de CMouseListener
		
		/**
		Método que será invocado siempre que se mueva el ratón. La
		aplicación avisa de este evento al estado actual.

		@param mouseState Estado del ratón cuando se lanza el evento.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamará a otros listeners.
		*/
		virtual bool mouseMoved(const GUI::CMouseState &mouseState);
		
		/**
		Método que será invocado siempre que se pulse un botón. La
		aplicación avisa de este evento al estado actual.

		@param mouseState Estado del ratón cuando se lanza el evento.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamará a otros listeners.
		*/
		virtual bool mousePressed(const GUI::CMouseState &mouseState);

		/**
		Método que será invocado siempre que se termine la pulsación
		de un botón. La aplicación avisa de este evento al estado 
		actual.

		@param mouseState Estado del ratón cuando se lanza el evento.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamará a otros listeners. 
		*/
		virtual bool mouseReleased(const GUI::CMouseState &mouseState);

		void loadGame(std::string name);
	private:

		/**
		Ventana CEGUI que muestra el menú.
		*/
		CEGUI::Window* _menuWindow;

		CEGUI::ProgressBar* _progressBar;
		
		std::string _gameName;
		bool _loaded;
		
		void initGraphicsResource();

		void setupUI(int step);

		bool _isFirstLaunch;

	}; // CLoadingGameState

} // namespace Application

#endif //  __Application_LoadingGameState_H
