/**
@file StatisticsController.h

Contiene la declaración de la clase que se encarga de mostrar en pantalla
las estadistica de unidad enenmiga de la oleada, el tiempo entre cada oleada y las unidades aliada que tenemos

@see GUI::CStatisticsController

@author Guidi Giacomo
@date Mayo, 2015
*/
#ifndef __GUI_StatisticsController_H
#define __GUI_StatisticsController_H



#include "InputManager.h"


namespace CEGUI
{
	class Window;
	class EventArgs;	
}


// Declaración de la clase
namespace GUI
{

	class CStatisticsController : public CKeyboardListener, public CMouseListener
	{
	public:

		/**
		Constructor.
		*/
		CStatisticsController ();

		/**
		Destructor.
		*/
		~CStatisticsController();

	

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

		void setTimerTextColor(std::string textColor);

		void setTimerText(std::string text);

		void setTimerVisibility(bool isVisible);

		void setGoblinCounter(int num);

		void addEntityCounter(std::string entityType,int qta);
		/*
			Function that show the waves counter icon and label
		*/
		void showWavesCounter();
		/*
			Function that hide the waves counter icon and label
		*/
		void hideWavesCounter();
		/*
			Function that set a text into waves counter label
		*/
		void setWavesCounterText(std::string txt);
	protected:

		
	private :


		void subscribeAllElements();
		//Timer 
		CEGUI::Window *_timerWindow;

		//Counter label
		CEGUI::Window *_looterWindow;
		int _looterCounter;

		CEGUI::Window *_assassinWindow;
		int _assassinCounter;

		CEGUI::Window *_knightWindow;
		int _knightCounter;
	
		CEGUI::Window* _goblinQuantityText;

		CEGUI::Window *_wavesCounterWindow;
		CEGUI::Window *_wavesCounterLabel;

		void addKnightCounter(int number);

		void addAssassinCounter(int number);

		void addLooterCounter(int number);


		
	}; // class CStatisticsController

} // namespace GUI

#endif // __GUI_MapController_H
