/**
@file UnitSelected.cpp

Contiene la implementación del componente que controla la condición
de selección de una entidad

@see Logic::IComponent

@author Iván León
@date Febrero, 2015
*/

#include "UnitSelected.h"

#include "Logic/GraphicsMessages.h"
#include "Logic/AllyUnitMessages.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Components/DirectionalLight.h"
#include "Logic/Entity/Components/WorldComponent.h"
#include "Logic/Server.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include "Graphics/Scene.h"
#include "Graphics/FrustumEntity.h"

namespace Logic 
{
	IMP_FACTORY(CUnitSelected);
	
	//---------------------------------------------------------

	CUnitSelected::~CUnitSelected() 
	{
		
		if(_nodeSelected)
		{
			
			_scene->removeEntity(_nodeSelected);
			delete _nodeSelected;
			_nodeSelected = 0;
		}
		if(_nodeActive)
		{
			
			_scene->removeEntity(_nodeActive);
			delete _nodeActive;
			_nodeActive = 0;
		}
		
	} // ~CGraphics
	 
	//---------------------------------------------------------

	bool CUnitSelected::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		_scene = _entity->getMap()->getScene();
		
		_nodeSelected = new Graphics::CFrustumEntity(_entity->getName() + "Selected");
		_nodeActive = new Graphics::CFrustumEntity(_entity->getName() + "Actived");
		if(!_scene->addEntity(_nodeSelected) || !_scene->addEntity(_nodeActive) )
			return false;
		
		if(entityInfo->hasAttribute("selection_texture_width"))
			_size.x = entityInfo->getFloatAttribute("selection_texture_width");
		if(entityInfo->hasAttribute("selection_texture_height"))
			_size.y = entityInfo->getFloatAttribute("selection_texture_height");
		
		if(entityInfo->hasAttribute("selection_projection_height"))
			_projectionHeight = entityInfo->getFloatAttribute("selection_projection_height");
		
		if(entityInfo->hasAttribute("selection_material"))
			 _selectedMaterial = entityInfo->getStringAttribute("selection_material");

		if(entityInfo->hasAttribute("activation_material"))
			 _activeMaterial = entityInfo->getStringAttribute("activation_material");

		initDecal();
		
		return true;

	} // spawn	

	//---------------------------------------------------------

	void CUnitSelected::awake(){
		
		_currentNode = _nodeSelected;
		hide();
		_currentNode = _nodeActive;
		hide();

		if(_selectedMaterial !="")
		{
			auto m= std::make_shared<AreaOfEffect>();
			m->entityName = _entity->getName();
			m->materialToProject = _selectedMaterial;
			m->frustum = _nodeSelected;
			_entity->getMap()->sendMessageToAll(m);

			auto m2= std::make_shared<AreaOfEffect>();
			m2->entityName = _entity->getName();
			m2->materialToProject = _activeMaterial;
			m2->frustum = _nodeActive;
			_entity->getMap()->sendMessageToAll(m2);
		}
				
	}

	bool CUnitSelected::activate()
	{
		return true;
	}

	void CUnitSelected::deactivate()
	{
		hide();
	}

	//---------------------------------------------------------

	void CUnitSelected::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);

		if(_selected || _active)
		{
			Vector3 p=_entity->getPosition();
//			p.y=1;
			_currentNode->setPosition(p);			
		}
		
	}

	//---------------------------------------------------------

	bool CUnitSelected::acceptN(const std::shared_ptr<NMessage> &message)
	{
		return (message->type.compare("AvatarSelection") == 0) || (message->type.compare("AvatarActivation") == 0) || (message->type.compare("EntityDying") == 0);
	} // acceptN

	//---------------------------------------------------------

	void CUnitSelected::processN(const std::shared_ptr<NMessage> &message)
	{
		if (message->type.compare("AvatarSelection") == 0)
		{
			std::shared_ptr<AvatarSelection> m = std::static_pointer_cast<AvatarSelection>(message);
			_selected = m->selected;
			if(_active)
			{
				_active = false;
				hide();
			}
			_currentNode = _nodeSelected;
			if(!_selected)
			{
				hide();		
			}
		}
		else if (message->type.compare("AvatarActivation") == 0)
		{
			std::shared_ptr<AvatarActivation> m = std::static_pointer_cast<AvatarActivation>(message);			
			_active = m->actived;
			if(_selected)
			{
				_selected = false;
				hide();
			}
			_currentNode = _nodeActive;
			if(!_active)
			{
				hide();
			}
		}
		else if (message->type.compare("EntityDying") == 0)
		{
			_active = false;
			_selected = false;
			hide();
		}
	} // processN
 

	void CUnitSelected::initDecal()
    {
		_nodeSelected->setProjectionType(Ogre::PT_ORTHOGRAPHIC);
        _nodeSelected->setOrientation(Ogre::Quaternion(Ogre::Degree(90),Ogre::Vector3::UNIT_X));
 
	  // update aspect ratio
		_nodeSelected->setAspectRatio(_size.x/_size.y);
 
        // update height
		_nodeSelected->setOrthoWindowHeight(_projectionHeight); 

		_nodeSelected->setFarClipDistance(_projectionHeight); 
    
		_nodeSelected->setDebugDisplayEnabled(true);

		_nodeActive->setProjectionType(Ogre::PT_ORTHOGRAPHIC);
        _nodeActive->setOrientation(Ogre::Quaternion(Ogre::Degree(90),Ogre::Vector3::UNIT_X));
 
	  // update aspect ratio
		_nodeActive->setAspectRatio(_size.x/_size.y);
 
        // update height
		_nodeActive->setOrthoWindowHeight(_projectionHeight); 

		_nodeActive->setFarClipDistance(_projectionHeight); 
    
		_nodeActive->setDebugDisplayEnabled(true);

    }
 

	void CUnitSelected::hide()
	{
		_currentNode->setPosition(Vector3(10000,0,10000));
	}


} // namespace Logic


