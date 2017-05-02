/**
@file VoiceManager.cpp

@see Logic::IComponent

@author Ivan Leon
@date June, 2015
*/

#include "VoiceManager.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include "Logic/MovementMessages.h"
#include "Logic/AudioMessages.h"
#include "Logic/RouteCalculationMessages.h"
#include "Logic/AllyUnitMessages.h"

#define MAX_MSECS_FOR_NEXT_VOICE 5000

namespace Logic 
{
	IMP_FACTORY(CVoiceManager);

	//---------------------------------------------------------

	bool CVoiceManager::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		assert(entityInfo->hasAttribute("order_voice") && "Nombre de textura no especificado para el componente trampa");
		_orderVoice = entityInfo->getStringAttribute("order_voice");

		assert(entityInfo->hasAttribute("attack_voice") && "Nombre de textura no especificado para el componente trampa");
		_attackVoice = entityInfo->getStringAttribute("attack_voice");

		assert(entityInfo->hasAttribute("selection_voice") && "Nombre de textura no especificado para el componente trampa");
		_selectionVoice = entityInfo->getStringAttribute("selection_voice");

		return true;

	} // spawn	

	//---------------------------------------------------------

	bool CVoiceManager::acceptN(const std::shared_ptr<NMessage> &message)
	{
		return (message->type.compare("AvatarActivation") == 0) || (message->type.compare("AvatarMoveToEntity") == 0) || 
			(message->type.compare("AvatarRouteTo") == 0);
	}

	//---------------------------------------------------------

	void CVoiceManager::processN(const std::shared_ptr<NMessage> &message)
	{
		if(message->type.compare("AvatarActivation") == 0)
		{
			auto m = std::static_pointer_cast<AvatarActivation>(message);
			if(m->actived && _msecsForNextSelectionVoice == 0)
			{
				_msecsForNextSelectionVoice = 1;
 				auto playAudioMessage = std::make_shared<PlayAudio>();
				playAudioMessage->eventName = getRandomVoice(_selectionVoice);
				_entity->emitMessageN(playAudioMessage);
			}
		}
		else if (message->type.compare("AvatarMoveToEntity") == 0)
		{
			auto m = std::static_pointer_cast<AvatarMoveToEntity>(message);

			if(m->target->isEnemy())
			{
				if(_msecsForNextAttackVoice == 0)
				{
					_msecsForNextAttackVoice = 1;
					auto playAudioMessage = std::make_shared<PlayAudio>();
					playAudioMessage->eventName = getRandomVoice(_attackVoice);
					_entity->emitMessageN(playAudioMessage);
				}
			}
			else
			{
				if(_msecsForNextOrderVoice == 0)
				{
					_msecsForNextOrderVoice = 1;
					auto playAudioMessage = std::make_shared<PlayAudio>();
					playAudioMessage->eventName = getRandomVoice(_orderVoice);
					_entity->emitMessageN(playAudioMessage);
				}
			}
		}
		else if (message->type.compare("AvatarRouteTo") == 0)
		{
			auto m = std::static_pointer_cast<AvatarRouteTo>(message);

			if(m->sender == NULL && _msecsForNextOrderVoice == 0)
			{
				_msecsForNextOrderVoice = 1;
				auto playAudioMessage = std::make_shared<PlayAudio>();
				playAudioMessage->eventName = getRandomVoice(_orderVoice);
				_entity->emitMessageN(playAudioMessage);
			}
		}
	}

	//---------------------------------------------------------

	void CVoiceManager::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);

		if(_msecsForNextAttackVoice > 0)
			_msecsForNextAttackVoice += msecs;

		if(_msecsForNextAttackVoice > MAX_MSECS_FOR_NEXT_VOICE)
			_msecsForNextAttackVoice = 0;

		if(_msecsForNextOrderVoice > 0)
			_msecsForNextOrderVoice += msecs;

		if(_msecsForNextOrderVoice > MAX_MSECS_FOR_NEXT_VOICE)
			_msecsForNextOrderVoice = 0;

		if(_msecsForNextSelectionVoice > 0)
			_msecsForNextSelectionVoice += msecs;

		if(_msecsForNextSelectionVoice > MAX_MSECS_FOR_NEXT_VOICE)
			_msecsForNextSelectionVoice = 0;

	}

	//---------------------------------------------------------

	std::string CVoiceManager::getRandomVoice(std::string rootVoice) const
	{
		int num = 1 + rand() % 3;
		return rootVoice + std::to_string(num);
	}

	//---------------------------------------------------------

} // namespace Logic

