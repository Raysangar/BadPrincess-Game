/**
@file DamageTrap.cpp

Implementation of DamageTrap component

@see Logic::IComponent

@author Rayco Sánchez García
@date February, 2015
*/

#include "MudTrap.h"

#include "Logic/UnitMessages.h"
#include "Logic/RangerMessages.h"
#include "Logic/PhysicMessages.h"
#include "Logic/AudioMessages.h"


#include "Logic/Entity/Entity.h"
#include "AnimatedGraphics.h"
#include "Graphics.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include "Physics/Server.h"
#include "Physics/PhysicPerceptionManager.h"

#include "GUI/Server.h"
#include "GUI/MinimapController.h"

namespace Logic 
{
	IMP_FACTORY(CMudTrap);

	//---------------------------------------------------------

	CMudTrap::~CMudTrap() {}
	
	//---------------------------------------------------------

	bool CMudTrap::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		assert(entityInfo->hasAttribute("slowPercentage") && "Atributo de reducción de movimiento no especificado para el componente trampa");
		slowPercentage = entityInfo->getFloatAttribute("slowPercentage");

		assert(entityInfo->hasAttribute("textureName") && "Nombre de textura no especificado para el componente trampa");
		textureName = entityInfo->getStringAttribute("textureName");
		
		assert(entityInfo->hasAttribute("slowDuration") && "Nombre de textura no especificado para el componente trampa");
		slowDuration = entityInfo->getIntAttribute("slowDuration");

		assert(entityInfo->hasAttribute("durationTime") && "Tiempo de duración de trampa sin especificar");
		durationTime = entityInfo->getIntAttribute("durationTime");

		assert(entityInfo->hasAttribute("activatedTrapModelName") && "Nombre del modelo tras activación de la trampa sin especificar");
		activatedTrapModelName = entityInfo->getStringAttribute("activatedTrapModelName");

		assert(entityInfo->hasAttribute("activatedTrapModelScale") && "Escala del modelo tras activación de la trampa sin especificar");
		activatedTrapModelScale = entityInfo->getFloatAttribute("activatedTrapModelScale");

		return true;

	}
	void CMudTrap::awake()
	{
		trapOwner=nullptr;
	}

	//---------------------------------------------------------

	void CMudTrap::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);
		if (activated)
		{
			durationTime -= msecs;
			if (durationTime <= 0)
				Logic::CEntityFactory::getSingletonPtr()->deferredDeleteEntity(_entity);
		}
	}

	//---------------------------------------------------------

	bool CMudTrap::acceptN(const std::shared_ptr<NMessage> &message)
	{

		return (message->type.compare("OnTriggerEnter") == 0) || (message->type.compare("SetOwner") == 0) || 
			(message->type.compare("OnTriggerExit") == 0);
	}

	//---------------------------------------------------------

	void CMudTrap::processN(const std::shared_ptr<NMessage> &message)
	{
		if (message->type.compare("SetOwner") == 0)
		{
			auto setOwnerMessage = std::static_pointer_cast<SetOwner>(message);
			trapOwner = setOwnerMessage->owner;
		}
		else if (message->type.compare("OnTriggerEnter") == 0)
		{
			auto onTriggerEnter = std::static_pointer_cast<OnTriggerEnter>(message);
			if (onTriggerEnter->collisionEntity->isEnemy())
			{
				if (!activated)
				{
					activated = true;
					((CGraphics*)_entity->getComponent("CGraphics"))->changeModel(activatedTrapModelName, activatedTrapModelScale);
					auto playAudioMessage = std::make_shared<PlayAudio>();
					playAudioMessage->eventName = "default";
					_entity->emitMessageN(playAudioMessage);
				}
				GUI::CServer::getSingletonPtr()->getMinimapController()->addMinimapEvent(_entity->getName(),GUI::EventType::trap,_entity->getPosition());
				auto speedModifierMessage = std::make_shared<ModifyStat>();
				speedModifierMessage->modID = "MudTrap";
				speedModifierMessage->modType = 2;
				speedModifierMessage->durationMsecs = slowDuration;
				speedModifierMessage->percentage = slowPercentage;
				onTriggerEnter->collisionEntity->emitMessageN(speedModifierMessage);

				auto muddyMessage = std::make_shared<Muddy>();
				onTriggerEnter->collisionEntity->emitMessageN(muddyMessage,this);

				/*
				CAnimatedGraphics* component = (CAnimatedGraphics*) onTriggerEnter->collisionEntity->getComponent("CAnimatedGraphics");
				component->getGraphicsEntity()->addTexture(textureName);
				*/
			}
		}
		else
		{
			auto onTriggerExit = std::static_pointer_cast<OnTriggerExit>(message);
			if (onTriggerExit->collisionEntity->isEnemy())
			{
				/*
				CAnimatedGraphics* component = (CAnimatedGraphics*) onTriggerExit->collisionEntity->getComponent("CAnimatedGraphics");
				component->getGraphicsEntity()->removePass(-1);
				*/

				auto restoreSpeed = std::make_shared<RestoreStat>();
				restoreSpeed->modID = "MudTrap";
				onTriggerExit->collisionEntity->emitMessageN(restoreSpeed);
				
				auto speedModifierMessage = std::make_shared<ModifyStat>();
				speedModifierMessage->modID = "MudTrap";
				speedModifierMessage->modType = 2;
				speedModifierMessage->durationMsecs = slowDuration;
				speedModifierMessage->percentage = slowPercentage;
				
				onTriggerExit->collisionEntity->emitMessageN(speedModifierMessage,this);
			}
		}
	}

} // namespace Logic

