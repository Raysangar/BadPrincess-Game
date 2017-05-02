/**
@file Discover.cpp
*/

#include "Discover.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"
#include "Physics/Server.h"
#include "Physics/PhysicPerceptionManager.h"
#include "Physics/ActorFactory.h"
#include "Physics/SceneManager.h"
#include "Physics/ActorController.h"
#include "Graphics/Scene.h"
#include <OgreSceneManager.h>
#include <OgreEntity.h>
#include "AnimatedGraphics.h"
#include "Logic/GraphicsMessages.h"
#include <PxPhysicsAPI.h>

#include "Graphics/Server.h"
#include "Graphics/FogOfWar.h"

namespace Logic 
{
	IMP_FACTORY(CDiscover);

	const Physics::CollisionGroup PHYSIC_ENEMY_FILTER[]={Physics::CollisionGroup::eEnemy};

	//---------------------------------------------------------

	CDiscover::CDiscover(std::string componentName) : IPhysics(componentName), _triggerDetector(0), _enableFOW(true)
	{
		_actorFactory = Physics::CServer::getSingletonPtr()->getActorFactory();
		_actorController = Physics::CServer::getSingletonPtr()->getActorController();
	
	}
	//---------------------------------------------------------

	bool CDiscover::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;		

		assert(entityInfo->hasAttribute("discovery_range"));
		_discoveryRange = entityInfo->getFloatAttribute("discovery_range");
		_triggerDetector = createTriggerDetector(entityInfo);

		return true;
	} 

	//---------------------------------------------------------

	CDiscover::~CDiscover() 
	{
		if (_triggerDetector) 
		{
			_actorFactory->destroyActor(_triggerDetector);
			_triggerDetector = NULL;
		}

		_actorFactory = nullptr;		
	} 

	//---------------------------------------------------------

	void CDiscover::awake()
	{

	}

	//---------------------------------------------------------
	
	bool CDiscover::activate()
	{
		Graphics::CFogOfWar::VisionAttributes visionAttributes = std::pair<Vector3, float>(_entity->getPosition(), getDiscoverRange());
		Graphics::CServer::getSingletonPtr()->getFogOfWarController()->addUnit(_entity, visionAttributes);

		if (_triggerDetector)
			Physics::CServer::getSingletonPtr()->getSceneManager()->addToScene(_triggerDetector);
		return true;
	}

	//---------------------------------------------------------

	void CDiscover::deactivate()
	{
		Graphics::CServer::getSingletonPtr()->getFogOfWarController()->removeUnit(_entity);
		if (_triggerDetector)
			Physics::CServer::getSingletonPtr()->getSceneManager()->removeFromScene(_triggerDetector);
	}

	//---------------------------------------------------------

	bool CDiscover::acceptN(const std::shared_ptr<NMessage> &message)
	{
		return (message->type.compare("SwitchFOW") == 0) || (message->type == "TransformMessage") || (message->type == "EntityDying");
	}

	//---------------------------------------------------------

	void CDiscover::processN(const std::shared_ptr<NMessage> &message)
	{		
		if (message->type.compare("SwitchFOW") == 0)
		{
			//std::cout << "SwitchFOW" << std::endl;
			_enableFOW = !_enableFOW;	
		}
		else if  (message->type == "EntityDying")
		{
			if (_entity->getType() == "Princess") return;
			std::list<Logic::CEntity*>* unitsInRadiusOfEffect = Physics::CServer::getSingletonPtr()->getPhysicPerceptionManager()->overlapQueries(_entity->getPosition(), 
			_discoveryRange, &std::vector<Physics::CollisionGroup> (PHYSIC_ENEMY_FILTER, PHYSIC_ENEMY_FILTER + sizeof(PHYSIC_ENEMY_FILTER) / sizeof(PHYSIC_ENEMY_FILTER[0])));
				
			auto coverMessage = std::make_shared<Cover>();
			coverMessage->sender = _entity;
				
			for (std::list<Logic::CEntity*>::iterator it = unitsInRadiusOfEffect->begin(); it != unitsInRadiusOfEffect->end(); ++it)
			{				
				(*it)->emitMessageN(coverMessage);
			}
		}
		else
			Graphics::CServer::getSingletonPtr()->getFogOfWarController()->updateUnitPosition(_entity, _entity->getPosition());
	}
	

	//------------------------------------------------


	physx::PxRigidDynamic* CDiscover::createTriggerDetector(const Map::CEntity *entityInfo)
	{
		assert(entityInfo->hasAttribute("discovery_collision_group"));
		int detectorCollisionGroup = entityInfo->getIntAttribute("discovery_collision_group");

		Vector3 pose = _entity->getPosition();
		pose.y -= _discoveryRange;

		return _actorFactory->createDynamicSphere(pose,_discoveryRange,1,true,true,detectorCollisionGroup,this);
	}

	//--------------------------------------------

	void CDiscover::onTrigger(IPhysics *otherComponent, bool enter)
	{
		if(_enableFOW) // Si la niebla de guerra (FOW) está activada.
		{
			if (enter) // Cuando una entidad entra en el trigger, se busca su nodo padre y se hace invisible. (Cambiar a visible).
			{
				_discoveredEntity = otherComponent->getEntity();

				auto uncoverMessage = std::make_shared<Uncover>();
				uncoverMessage->sender = _entity;
				_discoveredEntity->emitMessageN(uncoverMessage);	
				//std::cout << "Enter" << std::endl;
			} else // Cuando sale, se hace lo contrario.
			{
				_discoveredEntity = otherComponent->getEntity();

				auto coverMessage = std::make_shared<Cover>();
				coverMessage->sender = _entity;
				_discoveredEntity->emitMessageN(coverMessage);
				//std::cout << "Exit" << std::endl;
			}
		}
	}

	
	//---------------------------------------------------------

	void CDiscover::tick(unsigned int msecs) 
	{
		// Invocar al método de la clase padre (IMPORTANTE)
		IComponent::tick(msecs);

		if(_triggerDetector)
		{
			//movemos el detector
			Vector3 pos =  _entity->getPosition();
			pos.y -= _discoveryRange;
			_actorController->setGlopalPosition(_triggerDetector,pos);
		}
	}
}








