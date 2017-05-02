/**
@file RemovableWorldComponent.cpp

@see Logic::IComponent

@author Ivan Leon
@date June, 2015
*/

#include "RemovableWorldComponent.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Components/Graphics.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include "AI/Server.h"
#include "AI/EntityManager.h"

#define TRIGGER_COLLISION_GROUP 12

namespace Logic 
{
	IMP_FACTORY(CRemovableWorldComponent);
	
	CRemovableWorldComponent::CRemovableWorldComponent(std::string componentName) : IComponent(componentName),_activateTransparency(false)
	{
	}

	//---------------------------------------------------------

	CRemovableWorldComponent::~CRemovableWorldComponent() 
	{
	//	_circleNode->detachAllObjects();
	//	_scene->getSceneMgr()->destroySceneNode(_circleNode);
		
		for(int i = 0; i<(int)_centers.size(); ++i)
		{
			if(_centers[i])
			{
				delete _centers[i];
			}
		}
		_centers.clear();
		_radiuses.clear();
	} // ~CGraphics
	 
	//---------------------------------------------------------

	bool CRemovableWorldComponent::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		for (int i = 0; i < entityInfo->getIntAttribute("center_count"); ++i)
		{
			Ogre::Vector3 *center = new Vector3(entityInfo->getVector3Attribute("center_" + std::to_string(i)));
			_centers.push_back(center);
			float radius = entityInfo->getFloatAttribute("radius_" + std::to_string(i));
			_radiuses.push_back(radius);
		}	

		for (int i = 0; i < entityInfo->getIntAttribute("material_count"); ++i)
		{
			std::string mat = entityInfo->getStringAttribute("material_" + std::to_string(i));
			_materials.push_back(mat);
		}

		return true;

	} // spawn	

	//---------------------------------------------------------

	void CRemovableWorldComponent::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);

		bool _someoneInside = false;

		for(int i = 0; i<(int)_centers.size(); ++i)
		{
			for(std::set<Logic::CEntity*>::const_iterator itChests = AI::CServer::getSingletonPtr()->getEntityManager()->getChestList()->begin();
				itChests != AI::CServer::getSingletonPtr()->getEntityManager()->getChestList()->end() && !_someoneInside; ++itChests)
			{	
				if((*itChests)->getPosition().distance(*_centers[i]) < _radiuses[i])			
					_someoneInside = true;			
			}
		

			if(!_someoneInside)
			{
				for(std::set<Logic::CEntity*>::const_iterator itWell = AI::CServer::getSingletonPtr()->getEntityManager()->getWellList()->begin();
					itWell != AI::CServer::getSingletonPtr()->getEntityManager()->getWellList()->end() && !_someoneInside; ++itWell)
				{	
					if((*itWell)->getPosition().distance(*_centers[i]) < _radiuses[i])
						_someoneInside = true;
				}
			}

			if(!_someoneInside)
			{
				for(std::set<Logic::CEntity*>::const_iterator itAllies = AI::CServer::getSingletonPtr()->getEntityManager()->getAlliesList()->begin();
					itAllies != AI::CServer::getSingletonPtr()->getEntityManager()->getAlliesList()->end() && !_someoneInside; ++itAllies)
				{	
					if((*itAllies)->getPosition().distance(*_centers[i]) < _radiuses[i])
						_someoneInside = true;
				}		
			}
		
			if(!_someoneInside)
			{
				for(std::set<Logic::CEntity*>::const_iterator itPrincess = AI::CServer::getSingletonPtr()->getEntityManager()->getPrincessesList()->begin();
					itPrincess != AI::CServer::getSingletonPtr()->getEntityManager()->getPrincessesList()->end() && !_someoneInside; ++itPrincess)
				{	
					if((*itPrincess)->getPosition().distance(*_centers[i]) < _radiuses[i])
						_someoneInside = true;
				}	
			}
		}

		if(_someoneInside && !_activateTransparency)
		{
			_activateTransparency = true;			
			((Logic::CGraphics*)_entity->getComponent("CGraphics"))->setMaterialTransparency(_materials,0,0,0,0.2f,true,250);
		}
		
		if(!_someoneInside && _activateTransparency)
		{
			_activateTransparency = false;			
			((Logic::CGraphics*)_entity->getComponent("CGraphics"))->setMaterialTransparency(_materials,0,0,0,1.0f,true,250);
		}

	}

	//---------------------------------------------------------
	

} // namespace Logic

