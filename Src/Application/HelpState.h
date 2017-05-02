#ifndef __Application_HelpState_H
#define __Application_HelpState_H

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

namespace Application 
{
	class CHelpState : public CApplicationState 
	{
	public:
	
		CHelpState(CBaseApplication *app) : CApplicationState(app),_helpShowing(false),_helpHiding(false) {}

	
		virtual ~CHelpState();

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

		bool _helpShowing;
		bool _helpHiding;
		
		/**
		Tiempo de juego en milisegundos.
		*/
		unsigned int _time;

		CEGUI::Window* _helpWindow;

		bool backReleased(const CEGUI::EventArgs& e);

		bool buttonHover(const CEGUI::EventArgs& e);

		bool buttonClicked(const CEGUI::EventArgs& e);

	};

}

#endif