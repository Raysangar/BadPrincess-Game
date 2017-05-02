#ifndef __Application_OptionsState_H
#define __Application_OptionsState_H

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
	class COptionsState : public CApplicationState 
	{
	public:
	
		COptionsState(CBaseApplication *app) : CApplicationState(app),_optionsShowing(false),_optionsHiding(false) {}

	
		virtual ~COptionsState();

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

		bool _optionsShowing;
		bool _optionsHiding;
		
		/**
		Tiempo de juego en milisegundos.
		*/
		unsigned int _time;

		BaseSubsystems::CServer* baseSubsystems;

		BaseSubsystems::Options* gameOptions;

		Audio::CEvent* SFXFeedback;

		CEGUI::Window* _optionsWindow;

		void initResolutionsCombobox();

		void initAntialiasingCombobox();

		void initShadowsCombobox();

		void initFullScreenOption();

		void initVSyncOption();

		void initAudioSettings();

		bool backReleased(const CEGUI::EventArgs& e);

		bool fullScreenCheckBoxClicked(const CEGUI::EventArgs& e);

		bool vSyncCheckBoxClicked(const CEGUI::EventArgs& e);

		bool volumeChanged(const CEGUI::EventArgs& e);

		bool SFXChanged(const CEGUI::EventArgs& e);

		bool SFXSliderReleased(const CEGUI::EventArgs& e);

		bool resolutionChanged(const CEGUI::EventArgs& e);

		bool antialiasingChanged(const CEGUI::EventArgs& e);

		bool shadowsChanged(const CEGUI::EventArgs& e);

		bool buttonHover(const CEGUI::EventArgs& e);

		bool buttonClicked(const CEGUI::EventArgs& e);

	};

}

#endif