/**
@file DamageTrap.cpp

Implementation of DamageTrap component

@see Logic::IComponent

@author Rayco Sánchez García
@date February, 2015
*/

#include "DamageTrap.h"

#include "Logic/GraphicsMessages.h"
#include "Logic/CombatMessages.h"
#include "Logic/RangerMessages.h"
#include "Logic/PhysicMessages.h"
#include "Logic/AudioMessages.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include "Physics/Server.h"
#include "Physics/PhysicPerceptionManager.h"

#include "GUI/Server.h"
#include "GUI/MinimapController.h"


namespace Logic 
{
	IMP_FACTORY(CDamageTrap);

	const Physics::CollisionGroup PHYSIC_ENEMY_FILTER[]={Physics::CollisionGroup::eEnemy,Physics::CollisionGroup::eUntargetable};

	//---------------------------------------------------------

	CDamageTrap::~CDamageTrap() {}
	
	//---------------------------------------------------------

	bool CDamageTrap::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		assert(entityInfo->hasAttribute("trapDamage") && "Atributo de daño no especificado para el componente trampa");
		trapDamage = entityInfo->getIntAttribute("trapDamage");

		assert(entityInfo->hasAttribute("radiusOfEffect") && "Atributo de radio de efecto no especificado para el componente trampa");
		radiusOfEffect = entityInfo->getFloatAttribute("radiusOfEffect");

		return true;

	}
	void CDamageTrap::awake()
	{
		trapOwner=nullptr;
		activated=false;
		timeToDestroy=500;
	}

	//---------------------------------------------------------

	void CDamageTrap::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);
		if (activated)
		{
			timeToDestroy -= msecs;
			if (timeToDestroy <= 0)
				Logic::CEntityFactory::getSingletonPtr()->deferredDeleteEntity(_entity);
		}
	}

	//---------------------------------------------------------

	bool CDamageTrap::acceptN(const std::shared_ptr<NMessage> &message)
	{
		return (message->type.compare("OnTriggerEnter") == 0) || (message->type.compare("SetOwner") == 0);
	}

	//---------------------------------------------------------

	void CDamageTrap::processN(const std::shared_ptr<NMessage> &message)
	{
		if (message->type.compare("SetOwner") == 0)
			trapOwner = std::static_pointer_cast<SetOwner>(message)->owner;
		else if (!activated)
		{
			auto onTriggerEnter = std::static_pointer_cast<OnTriggerEnter>(message);
			if (onTriggerEnter->collisionEntity->isEnemy())
			{
				GUI::CServer::getSingletonPtr()->getMinimapController()->addMinimapEvent(_entity->getName(),GUI::EventType::trap,_entity->getPosition());
				std::list<Logic::CEntity*>* enemiesInRadiusOfEffect = Physics::CServer::getSingletonPtr()->getPhysicPerceptionManager()->overlapQueries(_entity->getPosition(), 
					radiusOfEffect, &std::vector<Physics::CollisionGroup> (PHYSIC_ENEMY_FILTER, PHYSIC_ENEMY_FILTER + sizeof(PHYSIC_ENEMY_FILTER) / sizeof(PHYSIC_ENEMY_FILTER[0])));
				
				auto damageMessage = std::make_shared<DamageMessage>();
				damageMessage->damage = trapDamage;

				for (std::list<Logic::CEntity*>::iterator it = enemiesInRadiusOfEffect->begin(); it != enemiesInRadiusOfEffect->end(); ++it)
					(*it)->emitMessageN(damageMessage);

				delete enemiesInRadiusOfEffect;

				auto trapDestroyedMessage = std::make_shared<TrapDestroyed>();
				trapDestroyedMessage->trapType = _entity->getType();
				trapOwner->emitMessageN(trapDestroyedMessage);

				auto playParticle = std::make_shared<PlayParticle>();
				playParticle->particleName = "explosion";
				_entity->emitMessageN(playParticle);

				_entity->deactivate("CGraphics");

				auto playAudioMessage = std::make_shared<PlayAudio>();
				playAudioMessage->eventName = "default";
				_entity->emitMessageN(playAudioMessage);

				activated = true;
			}
		}
	}

} // namespace Logic

