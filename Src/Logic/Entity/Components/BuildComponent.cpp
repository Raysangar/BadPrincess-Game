/**
@file BuildComponent.cpp
*/

#include "BuildComponent.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Logic/EntityMessages.h"
#include "Logic/CombatMessages.h"
#include "Logic/MovementMessages.h"
#include "Logic/WorkmanMessages.h"
#include "Logic\Entity\Components\Graphics.h"
#include "Logic/Server.h"


#include "Map/MapEntity.h"




namespace Logic 
{
	IMP_FACTORY(CBuildComponent);

	//---------------------------------------------------------

	CBuildComponent::~CBuildComponent() 
	{

	} 

	//---------------------------------------------------------

	bool CBuildComponent::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		assert(entityInfo->hasAttribute("timeToBuild"));
		_timeBuild = entityInfo->getIntAttribute("timeToBuild");

		assert(entityInfo->hasAttribute("buildedModelName"));
		_buildedModelName = entityInfo->getStringAttribute("buildedModelName");

		assert(entityInfo->hasAttribute("buildedModelScale"));
		_buildedModelScale = entityInfo->getFloatAttribute("buildedModelScale");

		assert(entityInfo->hasAttribute("isIndestructible"));
		_isIndestructible = entityInfo->getBoolAttribute("isIndestructible");		

		assert(entityInfo->hasAttribute("isInitiallyBuilded"));
		_isInitiallyBuilded = entityInfo->getBoolAttribute("isInitiallyBuilded");		

		return true;

	} 
	bool CBuildComponent::acceptN(const std::shared_ptr<NMessage> &message)
	{
		return false;
	}

	void CBuildComponent::processN(const std::shared_ptr<NMessage> &message)
	{
	
	}
	void CBuildComponent::awake()
	{
		//set the  indestructible during the construction
		auto m= std::make_shared<SetInvicible>();
		m->isInvicible=true;
		_entity->emitMessageN(m);

		//update position
		auto tMessage = std::make_shared<TransformMessage>();
		tMessage->transform = _entity->getTransform();
		_entity->emitMessageN(tMessage);

		_isBuilded = _isInitiallyBuilded;

		if(!_isInitiallyBuilded)
		{
			_currentBuildTime = 0;
			_isBuilded = false;
			_healtReparation=0;

		}else
			setBuilded();
	}


	void CBuildComponent::setBuilded()
	{
		Logic::IComponent *ic =_entity->getComponent("CGraphics");
		assert(ic);
		Logic::CGraphics *graphics = static_cast<Logic::CGraphics*>(ic);
		graphics->changeModel(getBuildedModelName(),getBuildedModelScale());
		_isBuilded=true;

		//if the  is destructible send a message to enable the  to recive damage
		if(!_isIndestructible)
		{
			auto m= std::make_shared<SetInvicible>();
			m->isInvicible=false;
			_entity->emitMessageN(m);
		}

		auto m= std::make_shared<BuildingEnded>();
		_entity->emitMessageN(m);


	}
}

