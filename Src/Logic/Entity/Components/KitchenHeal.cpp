/**
@file KitchenHeal.cpp
*/

#include "KitchenHeal.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"
#include "Physics/Server.h"
#include "Physics/ActorFactory.h"
#include "Physics/SceneManager.h"
#include <PxPhysicsAPI.h>
//#include "Logic/GraphicsMessages.h"
#include "Logic/AllyUnitMessages.h"
#include "Logic/Entity/Components/BuildComponent.h"

namespace Logic 
{
	IMP_FACTORY(CKitchenHeal);

	//---------------------------------------------------------

	CKitchenHeal::CKitchenHeal(std::string componentName) : IPhysics(componentName), _triggerDetector(0),_buildComponent(nullptr)
	{
		_actorFactory = Physics::CServer::getSingletonPtr()->getActorFactory();
	
	}
	//---------------------------------------------------------

	bool CKitchenHeal::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;		

		assert(entityInfo->hasAttribute("kitchen_heal_zone"));
		_healZone = entityInfo->getVector3Attribute("kitchen_heal_zone");


		Logic::IComponent *ic =_entity->getComponent("CBuildComponent");
		assert(ic);
		_buildComponent = static_cast<Logic::CBuildComponent*>(ic);

		_triggerDetector = createTriggerDetector(entityInfo);

		return true;
	} 

	//---------------------------------------------------------

	CKitchenHeal::~CKitchenHeal() 
	{
		if (_triggerDetector) 
		{
			_actorFactory->destroyActor(_triggerDetector);
			_triggerDetector = NULL;
		}

		_actorFactory = nullptr;		
	} 

	//---------------------------------------------------------

	void CKitchenHeal::awake()
	{

	}

	//---------------------------------------------------------
	
	bool CKitchenHeal::activate()
	{
		if (_triggerDetector)
			Physics::CServer::getSingletonPtr()->getSceneManager()->addToScene(_triggerDetector);
		return true;
	}

	//---------------------------------------------------------

	void CKitchenHeal::deactivate()
	{
		if (_triggerDetector)
			Physics::CServer::getSingletonPtr()->getSceneManager()->removeFromScene(_triggerDetector);
	}

	//---------------------------------------------------------

	bool CKitchenHeal::acceptN(const std::shared_ptr<NMessage> &message)
	{
		return message->type.compare("BuildingEnded")==0;
	}

	//---------------------------------------------------------

	void CKitchenHeal::processN(const std::shared_ptr<NMessage> &message)
	{		
		if(message->type.compare("BuildingEnded")==0)
		{
			//send a message to all entities already into the kitchen when the building is ended
			auto healMessage = std::make_shared<KitchenRecoveryStart>();

			std::list<CEntity*>::const_iterator it = _healList.begin();

			for (; it != _healList.end(); ++it)
			{
				(*it)->emitMessageN(healMessage);
			}
		}
	}
	

	//------------------------------------------------


	physx::PxRigidDynamic* CKitchenHeal::createTriggerDetector(const Map::CEntity *entityInfo)
	{
		assert(entityInfo->hasAttribute("kitchen_heal_collision_group"));
		int detectorCollisionGroup = entityInfo->getIntAttribute("kitchen_heal_collision_group");

		return _actorFactory->createDynamicBox(_entity->getPosition(),(_healZone * Vector3(0.5f,0.5f,0.5f)),1,true,true,detectorCollisionGroup,this);

	}

	//--------------------------------------------

	void CKitchenHeal::onTrigger(IPhysics *otherComponent, bool enter)
	{
			if (enter) // Cuando una entidad entra en el trigger, se añade a la lista de curación.
			{
				if(otherComponent->getEntity()->isPlayer())
				{
					_healList.push_back(otherComponent->getEntity());
					//std::cout << "HealList: Added" << std::endl;
					//if the kitchen is builded send a heal messages
					if(_buildComponent->isBuilded())
					{
						auto startRecover = std::make_shared<KitchenRecoveryStart>();
						otherComponent->getEntity()->emitMessageN(startRecover);
					}
					/*
					auto startParticleMessage = std::make_shared<PlayParticle>();
					startParticleMessage->particleName = "spiralStars";
					otherComponent->getEntity()->emitMessageN(startParticleMessage);
					*/
				}
			} else // Cuando sale, se hace lo contrario.
			{
				if(otherComponent->getEntity()->isPlayer())
				{
					_healList.remove(otherComponent->getEntity());
					//std::cout << "HealList: Removed" << std::endl;

					if(_buildComponent->isBuilded())
					{
						auto endRecover = std::make_shared<KitchenRecoveryEnd>();
						otherComponent->getEntity()->emitMessageN(endRecover);
					}

					/*
					auto stopParticleMessage = std::make_shared<StopParticle>();
					stopParticleMessage->particleName = "spiralStars";
					otherComponent->getEntity()->emitMessageN(stopParticleMessage);
					*/
				}
			}
	}

	
	//---------------------------------------------------------

	void CKitchenHeal::tick(unsigned int msecs) 
	{
		// Invocar al método de la clase padre (IMPORTANTE)
		IComponent::tick(msecs);

	}
}








