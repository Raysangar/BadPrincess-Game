#ifndef Logic_SoundEvent_h
#define Logic_SoundEvent_h

#include "Logic/Entity/Component.h"

namespace Audio
{
	class CEvent;
}

namespace Logic
{
	class CSoundEvent : public IComponent
	{
		DEC_FACTORY(CSoundEvent);

		public:

		CSoundEvent(std::string componentName);

		virtual ~CSoundEvent();
		
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		virtual bool activate();

		virtual void deactivate();

		virtual void pause();

		virtual void restart();

		virtual void tick(unsigned int msecs);

		virtual bool acceptN(const std::shared_ptr<NMessage> &message);

		virtual void processN(const std::shared_ptr<NMessage> &message);

		virtual float getParameterValue(std::string parameterName, std::string eventName = "default");

		virtual void setParamaterValue(std::string parameterName, float value, std::string eventName = "default");

	protected:

		Audio::CEvent* defaultSoundEvent;

		typedef std::map<std::string, std::pair<Audio::CEvent*, bool>> SoundEventsMap;

		SoundEventsMap soundEvents;

		void loadBanks(const Map::CEntity *entityInfo);

		Audio::CEvent* getSoundEvent(std::string eventName);

		std::string getDefaultSoundName();
	};

	REG_FACTORY(CSoundEvent);
}


#endif