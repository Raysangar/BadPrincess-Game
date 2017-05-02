/**
@file SetPhysicEntityHelperComponent.cpp

Contains the implementation of the SetPhysicEntityHelper Component

@see Logic::CGraphics
@see Logic::IComponent

@author Rayco Sánchez García
@date March, 2015
*/

#include "SetPhysicEntityHelperComponent.h"

#include "AI/Server.h"
#include "AI/EntityManager.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Components/Graphics.h"
#include "Logic/Entity/Components/PrincessManager.h"

#include "Logic/Maps/Map.h"

#include "Physics/Server.h"
#include "Physics/PhysicPerceptionManager.h"

#include "Map/MapEntity.h"

#include "Graphics/MeshEntity.h"
#include "Graphics/Server.h"

#include "GUI\Server.h"
#include "GUI\PlayerController.h"

#define MAX_RAY_DISTANCE 1000
#define WELL_RADIUS 6

namespace Logic 
{
	
	IMP_FACTORY(CSetPhysicEntityHelperComponent);
	
	//this vector contains all the physic groups of selectable entities (with left mouse button)
	const Physics::CollisionGroup PHYSIC_GROUPS_HELPER[]={Physics::CollisionGroup::eWorld, Physics::CollisionGroup::eEnemy,
		Physics::CollisionGroup::eResource, Physics::CollisionGroup::eChest };
	std::vector<Physics::CollisionGroup> PHYSIC_GROUPS(PHYSIC_GROUPS_HELPER, PHYSIC_GROUPS_HELPER + sizeof(PHYSIC_GROUPS_HELPER) / sizeof(PHYSIC_GROUPS_HELPER[0]) );

	//the well couldn't builded over the player (included other well)
	const Physics::CollisionGroup PHYSIC_GROUPS_WELL_HELPER[]={Physics::CollisionGroup::eWorld,Physics::CollisionGroup::ePlayer, Physics::CollisionGroup::eEnemy,
		Physics::CollisionGroup::eResource, Physics::CollisionGroup::eChest };

	const Physics::CollisionGroup PHYSIC_WORLD_FILTER []={Physics::CollisionGroup::eWorldGround};	

	//---------------------------------------------------------

	CSetPhysicEntityHelperComponent::~CSetPhysicEntityHelperComponent() {}
	 
	//---------------------------------------------------------

	bool CSetPhysicEntityHelperComponent::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		assert(entityInfo->hasAttribute("correctPositionMaterial"));
		correctPositionMaterial = entityInfo->getStringAttribute("correctPositionMaterial");

		assert(entityInfo->hasAttribute("wrongPositionMaterial"));
		wrongPositionMaterial = entityInfo->getStringAttribute("wrongPositionMaterial");

		return true;

	} // spawn	

	//---------------------------------------------------------

	void CSetPhysicEntityHelperComponent::awake()
	{
		Logic::CGraphics* graphicComponent = static_cast<Logic::CGraphics*>(_entity->getComponent("CGraphics"));
		assert(graphicComponent);
		meshEntity = graphicComponent->getGraphicsEntity();
		meshEntity->changeEntityMaterial(correctPositionMaterial);
		currentPitch = Ogre::Vector3::UNIT_Y.getRotationTo(Ogre::Vector3::UNIT_Y);
	}

	//---------------------------------------------------------

	bool CSetPhysicEntityHelperComponent::activate()
	{
		return true;
	}

	void CSetPhysicEntityHelperComponent::deactivate()
	{
	}

	//---------------------------------------------------------

	bool CSetPhysicEntityHelperComponent::acceptN(const std::shared_ptr<NMessage> &message)
	{
		return false;
	}

	//---------------------------------------------------------

	void CSetPhysicEntityHelperComponent::processN(const std::shared_ptr<NMessage> &message)
	{
		
	}

	//---------------------------------------------------------

	void CSetPhysicEntityHelperComponent::tick(unsigned int msecs)
	{
		_entity->setPositionN(GUI::CServer::getSingletonPtr()->getPlayerController()->getActualMouseWorldPosition());


		if (!this->checkIsCorrectPosition())
		{
			
			meshEntity->changeEntityMaterial(wrongPositionMaterial);
			isOnCorrectPosition = false;
		}
		else
		{
			meshEntity->changeEntityMaterial(correctPositionMaterial);
			isOnCorrectPosition = true;
		}

						
		adaptEntityPitch();
		
	}

	//---------------------------------------------------------

	bool CSetPhysicEntityHelperComponent::isPhysicEntityOnCorrectPosition()
	{
		return isOnCorrectPosition;
	}

	//---------------------------------------------------------

	void CSetPhysicEntityHelperComponent::adaptEntityPitch()
	{
		Ogre::Ray ray = Ogre::Ray(_entity->getPosition() + Ogre::Vector3(0,100,0),Ogre::Vector3::NEGATIVE_UNIT_Y);
		
		Vector3 point = Vector3::ZERO;
		Vector3 normal = Vector3::ZERO;
		Logic::CEntity* e =	Physics::CServer::getSingletonPtr()->getPhysicPerceptionManager()->raycastAdvanced(ray,MAX_RAY_DISTANCE,point,normal,
			&std::vector<Physics::CollisionGroup> (PHYSIC_WORLD_FILTER, PHYSIC_WORLD_FILTER + sizeof(PHYSIC_WORLD_FILTER) / sizeof(PHYSIC_WORLD_FILTER[0])));


		Ogre::Quaternion q = Ogre::Vector3::UNIT_Y.getRotationTo(normal);
		if(currentPitch != q)
		{
			currentPitch = q;
			Ogre::Quaternion r = Ogre::Quaternion((Ogre::Radian)_entity->getYaw(), Ogre::Vector3::UNIT_Y);
			q = q * r;
			Ogre::Matrix3 m;
			q.ToRotationMatrix(m);		

			_entity->setOrientationN(m);		
		}
		
	}

	//---------------------------------------------------------

	bool CSetPhysicEntityHelperComponent::checkIsCorrectPosition()
	{
		bool res = false;
		//check if the item collide with some other object in scene
		std::vector<Vector3> corners = meshEntity->getMeshCorners();
		std::list<CEntity*>* selectedEntities;
		if(_entity->getType().compare("WellHelper")==0)
			selectedEntities = Physics::CServer::getSingletonPtr()->getPhysicPerceptionManager()->overlapQueries(_entity->getPosition(),WELL_RADIUS, 
			&std::vector<Physics::CollisionGroup> (PHYSIC_GROUPS_WELL_HELPER, PHYSIC_GROUPS_WELL_HELPER + sizeof(PHYSIC_GROUPS_WELL_HELPER) / sizeof(PHYSIC_GROUPS_WELL_HELPER[0])));
		else
			selectedEntities = Physics::CServer::getSingletonPtr()->getPhysicPerceptionManager()->overlapQueries(corners[0],
			corners[1], corners[2], corners[3], 200, &PHYSIC_GROUPS);


		if(selectedEntities->size()>0)
			res=false;
		else
			res=true;
		delete selectedEntities;

		//if the entity to instantiate is a well check if it point into a wrath area
		if(_entity->getType().compare("WellHelper")==0)
		{
			Logic::CEntity *princess= AI::CServer::getSingletonPtr()->getEntityManager()->getClosestEntity(AI::CEntityManager::EntityType::Princess,_entity->getPosition());
			Logic::IComponent *ic = princess->getComponent("CPrincessManager");
			assert(ic);
			Logic::CPrincessManager *princessState = static_cast<Logic::CPrincessManager*>(ic);
			float d = _entity->getPosition().distance(princess->getPosition());
			res= res && (d<=princessState->getWrathAreaRadius());
			
		}
		return res;
	}


} // namespace Logic

