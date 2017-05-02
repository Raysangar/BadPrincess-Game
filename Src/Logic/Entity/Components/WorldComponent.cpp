/**
@file WorldComponent.cpp

Componente que recibe la textura y e frustum para dibujar l'area de efecto en suelo

@see Logic::IComponent

@author Guidi Giacomo
@date March, 2015
*/

#include "WorldComponent.h"

#include "Logic/GraphicsMessages.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Components/Graphics.h"
#include "Logic/Entity/Components/UnitSelected.h"
#include "Logic\AllyUnitMessages.h"

#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include "Graphics/Scene.h"
#include "Graphics/MeshEntity.h"
#include "Graphics/FrustumEntity.h"

#define DESTINATION_MARK_TIME 1000
#define MAX_SELECTED_UNITS 9

namespace Logic 
{
	IMP_FACTORY(CWorldComponent);
	
	//---------------------------------------------------------

	CWorldComponent::~CWorldComponent() 
	{
	//	_circleNode->detachAllObjects();
	//	_scene->getSceneMgr()->destroySceneNode(_circleNode);


		if(_destinationMarkNode)
		{			
			_scene->removeEntity(_destinationMarkNode);
			delete _destinationMarkNode;
			_destinationMarkNode = 0;
		}


	} // ~CGraphics
	 
	//---------------------------------------------------------

	bool CWorldComponent::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		_scene = _entity->getMap()->getScene();
		_destinationMarkNode = new Graphics::CFrustumEntity(_entity->getName() + "DestinationMark");

		if(!_scene->addEntity(_destinationMarkNode))
			return false;


		//Destination mark variables
		if(entityInfo->hasAttribute("destination_texture_width"))
			_destinationMarksize.x = entityInfo->getFloatAttribute("destination_texture_width");
		if(entityInfo->hasAttribute("destination_texture_height"))
			_destinationMarksize.y = entityInfo->getFloatAttribute("destination_texture_height");		
		if(entityInfo->hasAttribute("destination_projection_height"))
			_destinationMarkProjectionHeight = entityInfo->getFloatAttribute("destination_projection_height");		
		if(entityInfo->hasAttribute("destination_material"))
			 _destinationMarkMaterial = entityInfo->getStringAttribute("destination_material");

		if(entityInfo->hasAttribute("terrain_material"))
			_materialName = entityInfo->getStringAttribute("terrain_material");

		if(entityInfo->hasAttribute("physic_bounds"))
			_physicBounds = entityInfo->getVector3Attribute("physic_bounds");
				

		initDecal(_destinationMarkNode,_destinationMarksize,_destinationMarkProjectionHeight);

		return true;

	} // spawn	

	//---------------------------------------------------------

	void CWorldComponent::awake()
	{
		hide(_destinationMarkNode);
		_projectionHeight = _destinationMarkProjectionHeight;
	}

	//---------------------------------------------------------

	bool CWorldComponent::acceptN(const std::shared_ptr<NMessage> &message)
	{
		return (message->type.compare("AreaOfEffect") == 0) || (message->type.compare("PointDestination") == 0);
	} // acceptN

	//---------------------------------------------------------

	void CWorldComponent::processN(const std::shared_ptr<NMessage> &message)
	{
		if(message->type.compare("AreaOfEffect") == 0)
		{
			std::shared_ptr<AreaOfEffect> m = std::static_pointer_cast<AreaOfEffect>(message);
			Logic::CGraphics *g=static_cast<Logic::CGraphics*>(_entity->getComponent("CGraphics"));
			if(g)
			{
				g->getGraphicsEntity()->projectTexture(_materialName,0,m->frustum,m->materialToProject,m->entityName);
			}
		}
		else if(message->type.compare("PointDestination") == 0)
		{
			if(_firstTick)
			{
				auto m3= std::make_shared<AreaOfEffect>();
				m3->entityName = _entity->getName();
				m3->materialToProject=_destinationMarkMaterial;
				m3->frustum = _destinationMarkNode;
				_entity->emitMessageN(m3);
				_firstTick = false;
			}
			
			_elapsedTime = 1;
			std::shared_ptr<PointDestination> m = std::static_pointer_cast<PointDestination>(message);
			_destinationMarkNode->setPosition(m->destination);
		}
		
	} // processN

	//---------------------------------------------------------

	void CWorldComponent::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);

		if(_elapsedTime > 0)
		{
			_elapsedTime += msecs;
			if(_elapsedTime >= DESTINATION_MARK_TIME)
			{
				_elapsedTime = 0;
				hide(_destinationMarkNode);
				_projectionHeight = _destinationMarkProjectionHeight;
			}

		}
	}

	//---------------------------------------------------------

	void CWorldComponent::initDecal(Graphics::CFrustumEntity* node, Vector2 size, float projectionHeight)
    {
		node->setProjectionType(Ogre::PT_ORTHOGRAPHIC);
        node->setOrientation(Ogre::Quaternion(Ogre::Degree(90),Ogre::Vector3::UNIT_X));
 
		// update aspect ratio
		node->setAspectRatio(size.x/size.y);
 
        // update height
		node->setOrthoWindowHeight(projectionHeight); 

		node->setFarClipDistance(projectionHeight); 
    
		node->setDebugDisplayEnabled(true);

		node->setProjectionType(Ogre::PT_ORTHOGRAPHIC);
        node->setOrientation(Ogre::Quaternion(Ogre::Degree(90),Ogre::Vector3::UNIT_X));
 
	  // update aspect ratio
		node->setAspectRatio(size.x/size.y);
 
        // update height
		node->setOrthoWindowHeight(projectionHeight); 

		node->setFarClipDistance(projectionHeight); 
    
		node->setDebugDisplayEnabled(true);

    }

	//---------------------------------------------------------
	
	void CWorldComponent::hide(Graphics::CFrustumEntity* node)
	{
		node->setPosition(Vector3(10000,0,10000));
	}

} // namespace Logic

