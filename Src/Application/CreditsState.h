#ifndef __Application_CreditsState_H
#define __Application_CreditsState_H

#include "ApplicationState.h"

namespace Application 
{
	class CBaseApplication;
}

namespace CEGUI
{
	class EventArgs;
	class Window;
}

namespace Audio
{
	class CEvent;
}

namespace BaseSubsystems
{
	class CServer;
	class Options;
}

namespace Application 
{
	class CCreditsState : public CApplicationState 
	{
	public:
	
		CCreditsState(CBaseApplication *app) : CApplicationState(app),_creditsShowing(false),_creditsHiding(false), 
			_creditsFinished(false), _startCredits(false), _timeHoldingCredits(0) {}

	
		virtual ~CCreditsState();

		virtual bool init();

		virtual void release();

		virtual void activate();

		virtual void deactivate();

		virtual void tick(unsigned int msecs);

		virtual void reloadGUI();
		
		virtual bool keyPressed(GUI::TKey key);
		
		virtual bool keyReleased(GUI::TKey key);

		virtual bool mouseMoved(const GUI::CMouseState &mouseState);
		
		virtual bool mousePressed(const GUI::CMouseState &mouseState);

		virtual bool mouseReleased(const GUI::CMouseState &mouseState);

	private:

		bool _creditsShowing;
		bool _creditsHiding;
		bool _startCredits;
		bool _creditsFinished;
		int _timeHoldingCredits;
		int numberOfPanels;

		/**
		Tiempo de juego en milisegundos.
		*/
		unsigned int _time;

		CEGUI::Window* _creditsWindow;

		bool backReleased(const CEGUI::EventArgs& e);

		bool buttonHover(const CEGUI::EventArgs& e);

		bool buttonClicked(const CEGUI::EventArgs& e);

	};

}

#endif