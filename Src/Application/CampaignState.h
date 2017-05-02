#ifndef __Application_CampaignState_H
#define __Application_CampaignState_H

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
	class CCampaignState : public CApplicationState 
	{
	public:
	
		CCampaignState(CBaseApplication *app) : CApplicationState(app),_campaignShowing(false),_campaignHiding(false) {}

	
		virtual ~CCampaignState();

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

		bool _campaignShowing;
		bool _campaignHiding;
		
		/**
		Tiempo de juego en milisegundos.
		*/
		unsigned int _time;

		CEGUI::Window* _campaignWindow;

		bool level1Released(const CEGUI::EventArgs& e);

		bool level2Released(const CEGUI::EventArgs& e);

		bool level3Released(const CEGUI::EventArgs& e);

		bool level4Released(const CEGUI::EventArgs& e);

		bool level5Released(const CEGUI::EventArgs& e);

		bool backReleased(const CEGUI::EventArgs& e);

		bool buttonHover(const CEGUI::EventArgs& e);

		bool buttonClicked(const CEGUI::EventArgs& e);

		void startGame(std::string gameName);

	};

}

#endif