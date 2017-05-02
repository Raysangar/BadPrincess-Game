#ifndef Event_h
#define Event_h

#include "Sound.h"

#include <string>
#include <map>

namespace FMOD
{
	namespace Studio
	{
		class System;
		class EventDescription;
		class EventInstance;
		class ParameterInstance;
	}
}

namespace Audio
{
	class CEvent : public CSound
	{
	public:
		CEvent(const std::string &soundName);

		virtual ~CEvent();

		virtual void load();

		virtual void unload();

		virtual void play();

		virtual void pause();

		virtual void resume();

		virtual void stop();

		virtual float getParameterValue(const std::string &parameterName);

		virtual void setParameterValue(const std::string &parameterName, float value);

		virtual void setVolume(float volume);

		virtual void volumeChanged();

		virtual std::string getName() { return eventName; }

		virtual bool isPaused() { return paused; }

	protected:
		typedef std::map<std::string, FMOD::Studio::ParameterInstance*> SoundParameters;

		std::string eventName;

		bool paused;

		SoundParameters soundParameters;

		FMOD::Studio::EventDescription* eventDescription;

		FMOD::Studio::EventInstance* eventInstance;

		FMOD::Studio::System* fmodStudio;

		float volume;

		FMOD::Studio::ParameterInstance* getParameterInstance(const std::string &parameterName);
	};
}

#endif