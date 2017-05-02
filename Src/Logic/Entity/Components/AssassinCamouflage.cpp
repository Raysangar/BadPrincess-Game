/**
@file DamageTrap.cpp

Implementation of DamageTrap component

@see Logic::IComponent

@author Rayco Sánchez García
@date February, 2015
*/

#include "AssassinCamouflage.h"

#include "Logic/Entity/Entity.h"
#include "AnimatedGraphics.h"
#include "Visibility.h"
#include "Life.h"

#include "Logic/UnitMessages.h"
#include "Logic/GraphicsMessages.h"
#include "Logic/AudioMessages.h"

#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"



namespace Logic 
{
	IMP_FACTORY(CAssassinCamouflage);

	//---------------------------------------------------------

	CAssassinCamouflage::~CAssassinCamouflage() {}
	
	//---------------------------------------------------------

	bool CAssassinCamouflage::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
	
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		assert(entityInfo->hasAttribute("timeTillAppears"));
		timeTillAppears = entityInfo->getIntAttribute("timeTillAppears")*1000;

		assert(entityInfo->hasAttribute("timeTillDisappears"));
		timeTillDisappears = entityInfo->getIntAttribute("timeTillDisappears")*1000;

		assert(entityInfo->hasAttribute("timeTillDisappearsAfterReceiveDamege"));
		timeTillDisappearsAfterReceiveDamege = entityInfo->getIntAttribute("timeTillDisappearsAfterReceiveDamege")*1000;

		return true;

	}
	void CAssassinCamouflage::awake()
	{
		animatedGraphics = (CAnimatedGraphics*) _entity->getComponent("CAnimatedGraphics");
		animatedGraphics->getGraphicsEntity()->setVisible(isCamouflaged);
		visibilityComponent = (CVisibility*) _entity->getComponent("CVisibility");
		lifeComponent = (CLife*) _entity->getComponent("CLife");
	}

	//---------------------------------------------------------

	void CAssassinCamouflage::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);
		
		if (isMuddy) 
		{
			setVisibility(visibilityComponent->isVisible());
			return;
		}

		(attacked ? timeSinceLastReceivedAttack : timeSinceLastChange) += msecs;
		
		if (timeSinceLastReceivedAttack >= timeTillDisappearsAfterReceiveDamege)
		{
			setVisibility(false);
			attacked = false;
			isCamouflaged = true;
			timeSinceLastReceivedAttack = 0;
			disappear();
		}

		if (timeSinceLastChange >= (isCamouflaged ? timeTillAppears : timeTillDisappears))
		{
			isCamouflaged = !isCamouflaged;
			if (!(!isCamouflaged && visibilityComponent->isVisible() || isAttacking) && animatedGraphics->getGraphicsEntity()->getVisible())
				disappear();
			setVisibility(!isCamouflaged && visibilityComponent->isVisible() || isAttacking);
			
			timeSinceLastChange -= (isCamouflaged ? timeTillAppears : timeTillDisappears);
		}

	}

	//---------------------------------------------------------

	bool CAssassinCamouflage::acceptN(const std::shared_ptr<NMessage> &message)
	{
		return message->type == "Muddy" || message->type == "Attacking"
			|| message->type == "DamageMessage";
	}

	//---------------------------------------------------------

	void CAssassinCamouflage::processN(const std::shared_ptr<NMessage> &message)
	{
		if (message->type == "Attacking")
		{
			isAttacking = std::static_pointer_cast<Attacking>(message)->attacking;
			if (isAttacking)
				setVisibility(visibilityComponent->isVisible());
		}
		else if (message->type == "Muddy")
		{
			isMuddy = true;
			isCamouflaged = false;
			timeSinceLastChange = 0;
			setVisibility(visibilityComponent->isVisible());
		}
		else
		{
			timeSinceLastReceivedAttack = 0;
			timeSinceLastChange = 0;
			setVisibility(visibilityComponent->isVisible());
			attacked = true;
		}
	}

	//---------------------------------------------------------

	void CAssassinCamouflage::setVisibility(bool isVisible)
	{
		animatedGraphics->getGraphicsEntity()->setVisible(isVisible);
		lifeComponent->setLifebarVisible(isVisible);
		visibilityComponent->sendChangeCollisionGroupMessage(isVisible);
	}

	//---------------------------------------------------------

	void CAssassinCamouflage::disappear()
	{
		auto playParticleMessage = std::make_shared<PlayParticle>();
		playParticleMessage->particleName = "assassinStealth";
		_entity->emitMessageN(playParticleMessage);

		auto playAudioMessage = std::make_shared<PlayAudio>();
		playAudioMessage->eventName = "event:/SmokeBomb";
		_entity->emitMessageN(playAudioMessage);
	}

} // namespace Logic

