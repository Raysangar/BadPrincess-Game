#include "3DSoundEvent.h"

#include "Logic/Entity/Entity.h"
#include "Logic/MovementMessages.h"
#include "Map/MapEntity.h"
#include "Audio/Server.h"
#include "Audio/3DEvent.h"

#include <cassert>

namespace Logic
{
	IMP_FACTORY(C3DSoundEvent);

	C3DSoundEvent::C3DSoundEvent(std::string componentName) : CSoundEvent(componentName) {}

	//---------------------------------------------------------

	C3DSoundEvent::~C3DSoundEvent() {}

	//---------------------------------------------------------

	bool C3DSoundEvent::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		CSoundEvent::loadBanks(entityInfo);

		assert(entityInfo->hasAttribute("sound_event_count"));
		for(int i = 0; i < entityInfo->getIntAttribute("sound_event_count"); ++i)
			soundEvents[entityInfo->getStringAttribute("sound_event_" + std::to_string(i))].first = new Audio::C3DEvent(entityInfo->getStringAttribute("sound_event_" + std::to_string(i)), _entity->getPosition());
		if (entityInfo->hasAttribute("defaultEvent") && entityInfo->getStringAttribute("defaultEvent") != "")
		{
			defaultSoundEvent = new Audio::CEvent(entityInfo->getStringAttribute("defaultEvent"));
			soundEvents[entityInfo->getStringAttribute("defaultEvent")].first = defaultSoundEvent;
		}
		return true;
	}

	//---------------------------------------------------------

	bool C3DSoundEvent::acceptN(const std::shared_ptr<NMessage> &message)
	{
		if (CSoundEvent::acceptN(message))
			return true;
		return (message->type == "AvatarMove") || (message->type == "TransformMessage");
	}

	//---------------------------------------------------------

	void C3DSoundEvent::processN(const std::shared_ptr<NMessage> &message) 
	{		
		if (message->type == "AvatarMove")
			for (SoundEventsMap::iterator it = soundEvents.begin(); it != soundEvents.end(); ++it)
				((Audio::C3DEvent*)it->second.first)->setMovement(std::static_pointer_cast<AvatarMove>(message)->movement);
		else if ((message->type == "TransformMessage"))
			for (SoundEventsMap::iterator it = soundEvents.begin(); it != soundEvents.end(); ++it)
				((Audio::C3DEvent*)it->second.first)->setPosition(std::static_pointer_cast<TransformMessage>(message)->transform.getTrans());
		else
			CSoundEvent::processN(message);
	}
}