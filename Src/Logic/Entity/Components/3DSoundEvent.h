#ifndef Logic_DSoundEvent_h
#define Logic_DSoundEvent_h

#include "Logic/Entity/Component.h"
#include "SoundEvent.h"

namespace Audio
{
	class CEvent;
}

namespace Logic
{
	class C3DSoundEvent : public CSoundEvent
	{
		DEC_FACTORY(C3DSoundEvent);

		public:

		C3DSoundEvent(std::string componentName);

		virtual ~C3DSoundEvent();
		
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		virtual bool acceptN(const std::shared_ptr<NMessage> &message);

		virtual void processN(const std::shared_ptr<NMessage> &message);

	};

	REG_FACTORY(C3DSoundEvent);
}


#endif