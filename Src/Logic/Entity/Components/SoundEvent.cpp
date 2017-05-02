#include "SoundEvent.h"

#include "Logic/Entity/Entity.h"
#include "Logic/AudioMessages.h"
#include "Map/MapEntity.h"
#include "Audio/Server.h"
#include "Audio/Event.h"

#include <cassert>

namespace Logic
{
	IMP_FACTORY(CSoundEvent);

	CSoundEvent::CSoundEvent(std::string componentName) : IComponent(componentName), defaultSoundEvent(nullptr) {}

	//---------------------------------------------------------

	CSoundEvent::~CSoundEvent() 
	{
		for (SoundEventsMap::iterator it = soundEvents.begin(); it != soundEvents.end(); ++it)
			delete it->second.first;
	}

	//---------------------------------------------------------

	bool CSoundEvent::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		loadBanks(entityInfo);

		assert(entityInfo->hasAttribute("sound_event_count"));
		for(int i = 0; i < entityInfo->getIntAttribute("sound_event_count"); ++i)
		{
			soundEvents[entityInfo->getStringAttribute("sound_event_" + std::to_string(i))].first = new Audio::CEvent(entityInfo->getStringAttribute("sound_event_" + std::to_string(i)));
			soundEvents[entityInfo->getStringAttribute("sound_event_" + std::to_string(i))].second = false;
		}
		if (entityInfo->hasAttribute("defaultEvent") && entityInfo->getStringAttribute("defaultEvent") != "")
		{
			defaultSoundEvent = new Audio::CEvent(entityInfo->getStringAttribute("defaultEvent"));
			soundEvents[entityInfo->getStringAttribute("defaultEvent")].first = defaultSoundEvent;
			soundEvents[entityInfo->getStringAttribute("defaultEvent")].second = false;
		}

		return true;
	}

	//---------------------------------------------------------

	void CSoundEvent::loadBanks(const Map::CEntity *entityInfo)
	{
		assert(entityInfo->hasAttribute("audio_bank_count"));

		for(int i = 0; i < entityInfo->getIntAttribute("audio_bank_count"); ++i)
			Audio::CServer::getInstance()->loadBank(entityInfo->getStringAttribute("audio_bank_" + std::to_string(i)));
	}

	//---------------------------------------------------------

	bool CSoundEvent::activate()
	{
		for (SoundEventsMap::iterator it = soundEvents.begin(); it != soundEvents.end(); ++it)
			it->second.first->load();
		return true;
	}

	//---------------------------------------------------------

	void CSoundEvent::pause()
	{
		for (SoundEventsMap::iterator it = soundEvents.begin(); it != soundEvents.end(); ++it)
		{
			if (it->second.second)
				it->second.first->pause();
		}
	}

	//---------------------------------------------------------

	void CSoundEvent::restart()
	{
		for (SoundEventsMap::iterator it = soundEvents.begin(); it != soundEvents.end(); ++it)
		{
			if (it->second.second)
				it->second.first->resume();
		}
	}

	//---------------------------------------------------------

	void CSoundEvent::deactivate()
	{
		for (SoundEventsMap::iterator it = soundEvents.begin(); it != soundEvents.end(); ++it)
		{
			it->second.first->stop();
			it->second.first->unload();
		}
	}

	//---------------------------------------------------------

	void CSoundEvent::tick(unsigned int msecs) 
	{
		IComponent::tick(msecs);
	}

	//---------------------------------------------------------

	bool CSoundEvent::acceptN(const std::shared_ptr<NMessage> &message)
	{
		return (message->type == "PlayAudio") || (message->type == "StopAudio") ||
			(message->type == "SetParameterValue") || (message->type == "StopAll");
	}

	//---------------------------------------------------------

	void CSoundEvent::processN(const std::shared_ptr<NMessage> &message) 
	{
		if(defaultSoundEvent || soundEvents.size() > 0)
		{
			if (message->type == "PlayAudio")
			{
				auto playMessage = std::static_pointer_cast<PlayAudio>(message);
				getSoundEvent(playMessage->eventName)->play();

				if (playMessage->eventName != "default")
					soundEvents[playMessage->eventName].second = true;
				else
					soundEvents[getDefaultSoundName()].second = true;
			}
			else if ((message->type == "StopAudio"))
			{
				auto stopMessage = std::static_pointer_cast<StopAudio>(message);
				getSoundEvent(stopMessage->eventName)->stop();

				if (stopMessage->eventName != "default")
					soundEvents[stopMessage->eventName].second = false;
				else
					soundEvents[getDefaultSoundName()].second = false;
			}
			else if (message->type == "StopAll")
			{
				for (SoundEventsMap::iterator it = soundEvents.begin(); it != soundEvents.end(); ++it)
				{
					it->second.second = false;
					it->second.first->stop();
				}
			}
			else
			{
				auto audioMessage = std::static_pointer_cast<SetParameterValue>(message);
				getSoundEvent(audioMessage->eventName)->setParameterValue(audioMessage->parameterName, audioMessage->value);
			}
		}
	}

	//---------------------------------------------------------

	float CSoundEvent::getParameterValue(std::string parameterName, std::string eventName)
	{
		return getSoundEvent(eventName)->getParameterValue(parameterName);
	}

	//---------------------------------------------------------

	void CSoundEvent::setParamaterValue(std::string parameterName, float value, std::string eventName)
	{	
		getSoundEvent(eventName)->setParameterValue(parameterName, value);
	}

	//---------------------------------------------------------

	Audio::CEvent* CSoundEvent::getSoundEvent(std::string eventName)
	{
		Audio::CEvent* soundEvent = (eventName == "default") ? defaultSoundEvent : soundEvents[eventName].first;
		assert(soundEvent);
		return soundEvent;
	}


	//---------------------------------------------------------

	std::string CSoundEvent::getDefaultSoundName()
	{
		for (SoundEventsMap::iterator it = soundEvents.begin(); it != soundEvents.end(); ++it)
			if (it->second.first == defaultSoundEvent)
				return it->first;
		return "";
	}
}