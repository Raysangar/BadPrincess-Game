/**
@file Visibility.cpp
*/

#include "Visibility.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"
#include "Physics/Server.h"
#include "Physics/ActorFactory.h"
#include "Physics/SceneManager.h"
#include "Physics/ActorController.h"
#include "Graphics/Scene.h"
#include <OgreSceneManager.h>
#include <OgreEntity.h>
#include "AnimatedGraphics.h"
#include "Logic/GraphicsMessages.h"

#include "Logic/PhysicMessages.h"
#include "Life.h"

namespace Logic 
{
	IMP_FACTORY(CVisibility);

	//---------------------------------------------------------
		
	CVisibility::CVisibility(std::string componentName) : IComponent(componentName), _observers(0), 
		graphicsComponent(nullptr){}

	//---------------------------------------------------------

	bool CVisibility::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;		

		return true;
	} 

	//---------------------------------------------------------

	CVisibility::~CVisibility() 
	{	
	} 

	//---------------------------------------------------------

	void CVisibility::awake()
	{
		graphicsComponent = static_cast<Logic::CAnimatedGraphics*>(_entity->getComponent("CAnimatedGraphics"));
		graphicsComponent->setVisible(false);
		lifeComponent = (CLife*) _entity->getComponent("CLife");
		lifeComponent->setLifebarVisible(false);
		_observers = 0;
	}

	//---------------------------------------------------------
	
	bool CVisibility::activate()
	{					
		return true;
	}

	//---------------------------------------------------------

	void CVisibility::deactivate()
	{
	}

	//---------------------------------------------------------

	bool CVisibility::acceptN(const std::shared_ptr<NMessage> &message)
	{
		return (message->type.compare("Uncover") == 0) || (message->type.compare("Cover") == 0);
	}

	//---------------------------------------------------------

	void CVisibility::processN(const std::shared_ptr<NMessage> &message)
	{	
		if(message->type.compare("Uncover") == 0) 
		{
			auto m=std::static_pointer_cast<Uncover>(message);	
 			_observers++;
			//std::cout << "Uncover" << std::endl;
		}

		if(message->type.compare("Cover") == 0)
		{
			_observers--;
			if(_observers < 0)
				_observers = 0;
			//std::cout << "Cover" << std::endl;
		}
		//std::cout << "Observers_Count: " <<  _observers << std::endl;

		if(_observers > 0 && _entity->getType() != "Assassin")
		{
			graphicsComponent->setVisible(true);
			lifeComponent->setLifebarVisible(true);
			sendChangeCollisionGroupMessage(true);
		}
		else if (_observers <= 0)
		{
			graphicsComponent->setVisible(false);
			lifeComponent->setLifebarVisible(false);
			sendChangeCollisionGroupMessage(false);
		}
	}

	//---------------------------------------------------------

	void CVisibility::tick(unsigned int msecs) 
	{
		// Invocar al método de la clase padre (IMPORTANTE)
		IComponent::tick(msecs);
		
	}

	//---------------------------------------------------------

	void CVisibility::sendChangeCollisionGroupMessage(bool entityIsTargetable)
	{
		auto changeCollisionGroupMessage = std::make_shared<ChangeCollisionGroup>();
		changeCollisionGroupMessage->collisionGroup = (entityIsTargetable ? 10 : 14);
		_entity->emitMessageN(changeCollisionGroupMessage);
	}
}








