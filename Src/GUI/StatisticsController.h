/**
@file StatisticsController.h

Contiene la declaraci�n de la clase que se encarga de mostrar en pantalla
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


// Declaraci�n de la clase
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
		Activa la la clase, se registra en el CInputManager y as� empieza a 
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
