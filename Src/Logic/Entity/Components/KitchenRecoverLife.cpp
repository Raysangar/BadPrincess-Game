/**
@file KitchenRecoverLife.cpp
*/

#include "KitchenRecoverLife.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"
#include "Logic/AllyUnitMessages.h"
#include "Logic/UnitMessages.h"
#include "Logic/GraphicsMessages.h"

namespace Logic 
{
	IMP_FACTORY(CKitchenRecoverLife);

	//---------------------------------------------------------

	CKitchenRecoverLife::CKitchenRecoverLife(std::string componentName) : IComponent(componentName), _healQuantity(0),_healMaxTime(0),_healCurrentTime(0),_healParticleName(""),_recoveryStarted(false)
	{	
	}
	//---------------------------------------------------------

	bool CKitchenRecoverLife::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;		

		assert(entityInfo->hasAttribute("kitchen_heal_quantity"));
		_healQuantity = entityInfo->getFloatAttribute("kitchen_heal_quantity");

		assert(entityInfo->hasAttribute("kitchen_heal_time"));
		_healMaxTime = entityInfo->getIntAttribute("kitchen_heal_time");

		assert(entityInfo->hasAttribute("kitchen_heal_particle_name"));
		_healParticleName = entityInfo->getStringAttribute("kitchen_heal_particle_name");

		return true;
	} 

	//---------------------------------------------------------

	CKitchenRecoverLife::~CKitchenRecoverLife() 
	{
	} 

	//---------------------------------------------------------

	void CKitchenRecoverLife::awake()
	{
		_recoveryStarted = false;
		_healCurrentTime = 0;
	}

	//---------------------------------------------------------
	
	bool CKitchenRecoverLife::activate()
	{
		return true;
	}

	//---------------------------------------------------------

	void CKitchenRecoverLife::deactivate()
	{
	}

	//---------------------------------------------------------

	bool CKitchenRecoverLife::acceptN(const std::shared_ptr<NMessage> &message)
	{
		return message->type.compare("KitchenRecoveryStart")==0 ||  message->type.compare("KitchenRecoveryEnd")==0;
	}

	//---------------------------------------------------------

	void CKitchenRecoverLife::processN(const std::shared_ptr<NMessage> &message)
	{		
		if( message->type.compare("KitchenRecoveryStart")==0)
		{
			_recoveryStarted = true;

			auto startParticleMessage = std::make_shared<PlayParticle>();
			startParticleMessage->particleName = _healParticleName;
			_entity->emitMessageN(startParticleMessage);

		}else if( message->type.compare("KitchenRecoveryEnd")==0)
		{
			_recoveryStarted = false;

			auto stopParticleMessage = std::make_shared<StopParticle>();
			stopParticleMessage->particleName = _healParticleName;
			_entity->emitMessageN(stopParticleMessage);

		}
	}
	

	//------------------------------------------------



	//--------------------------------------------


	
	//---------------------------------------------------------

	void CKitchenRecoverLife::tick(unsigned int msecs) 
	{
		// Invocar al método de la clase padre (IMPORTANTE)
		IComponent::tick(msecs);

		if(_recoveryStarted)
		{
			_healCurrentTime += msecs;
			if(_healCurrentTime>=_healMaxTime)
			{
				auto healMessage = std::make_shared<HealUnit>();
				healMessage->healing = _healQuantity;
				_entity->emitMessageN(healMessage);

				_healCurrentTime = 0;
			}
		}
	}
}








