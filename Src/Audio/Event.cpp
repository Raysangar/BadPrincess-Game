#include "Event.h"

#include "Server.h"

#include "fmod_studio.hpp"
#include "fmod.hpp"
#include <fmod_errors.h>

#include <cassert>

namespace Audio
{
	CEvent::CEvent(const std::string &eventName) : CSound(eventName), volume(1), paused(true), eventName(eventName)
	{
		fmodStudio = CServer::getInstance()->getStudio();

		FMOD_RESULT result = fmodStudio->getEvent(eventName.c_str(), &eventDescription);
		assert((result == FMOD_OK));

		result = eventDescription->loadSampleData();
		assert((result == FMOD_OK));

		result = eventDescription->createInstance(&eventInstance);
		assert((result == FMOD_OK));
	}
	//---------------------------------------------------------

	CEvent::~CEvent()
	{	
		eventDescription->unloadSampleData();
		eventInstance->release();
		eventInstance = nullptr;

		eventDescription = nullptr;
		
		fmodStudio = nullptr;
	}

	//---------------------------------------------------------

	void CEvent::load()
	{
		CServer::getInstance()->registerObserver(this);
	}

	//---------------------------------------------------------

	void CEvent::unload()
	{
		CServer::getInstance()->unregisterObserver(this);
	}

	//---------------------------------------------------------

	void CEvent::play()
	{
		paused = false;
		eventInstance->start();
	}

	//---------------------------------------------------------

	void CEvent::stop()
	{
		paused = true;
		eventInstance->stop(FMOD_STUDIO_STOP_IMMEDIATE);
	}

	//---------------------------------------------------------

	//---------------------------------------------------------

	void CEvent::pause()
	{
		paused = true;
		eventInstance->setPaused(true);
	}

	//---------------------------------------------------------

	void CEvent::resume()
	{
		eventInstance->setPaused(false);
	}

	//---------------------------------------------------------

	float CEvent::getParameterValue(const std::string &parameterName)
	{
		float value = 0;
		getParameterInstance(parameterName)->getValue(&value);
		return value;
	}

	//---------------------------------------------------------

	void CEvent::setParameterValue(const std::string &parameterName, float value)
	{
		getParameterInstance(parameterName)->setValue(value);
	}

	//---------------------------------------------------------

	FMOD::Studio::ParameterInstance* CEvent::getParameterInstance(const std::string &parameterName)
	{
		if (soundParameters.find(parameterName) == soundParameters.end())
			eventInstance->getParameter(parameterName.c_str(), &soundParameters[parameterName]);
		return soundParameters[parameterName];
	}

	//---------------------------------------------------------

	void CEvent::setVolume(float volume)
	{
		this->volume = volume;
		eventInstance->setVolume(volume * CServer::getInstance()->getGeneralVolume());
	}

	//---------------------------------------------------------

	void CEvent::volumeChanged()
	{
		eventInstance->setVolume(volume * CServer::getInstance()->getGeneralVolume());
	}

} // namespace Audio