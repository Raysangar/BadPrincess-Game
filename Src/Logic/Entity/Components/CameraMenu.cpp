/**
@file CameraMenu.cpp

Contiene la implementación del componente que controla la cámara gráfica
de una escena.
 
@see Logic::CCameraMenu
@see Logic::IComponent

@author David Llansó
@date Agosto, 2010
*/

#include "CameraMenu.h"

#include "Logic/CameraMessages.h"

#include "Logic/Server.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"
#include "PhysicEntity.h"

#include "GUI/Server.h"
#include "GUI/PlayerController.h"
#include <math.h>

#include "Graphics/Scene.h"
#include "Graphics/Camera.h"


#define CAMERA_MOVEMENT 5
#define CAMERA_TARGET_MOVEMENT 5

namespace Logic 
{

	IMP_FACTORY(CCameraMenu);
	
	//---------------------------------------------------------

	bool CCameraMenu::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		_graphicsCameraMenu = _entity->getMap()->getScene()->getCamera();
		if(!_graphicsCameraMenu)
			return false;

		if(entityInfo->hasAttribute("target_menu"))
			_targetMenu = entityInfo->getVector3Attribute("target_menu");

		if(entityInfo->hasAttribute("target_credits"))
			_targetCredits = entityInfo->getVector3Attribute("target_credits");

		if(entityInfo->hasAttribute("target_options"))
			_targetOptions = entityInfo->getVector3Attribute("target_options");
		
		if(entityInfo->hasAttribute("target_castle"))
			_targetCastle = entityInfo->getVector3Attribute("target_castle");

		if(entityInfo->hasAttribute("target_help"))
			_targetHelp = entityInfo->getVector3Attribute("target_help");

		if(entityInfo->hasAttribute("destination_menu"))
			_destinationMenu = entityInfo->getVector3Attribute("destination_menu");

		if(entityInfo->hasAttribute("destination_options"))
			_destinationOptions = entityInfo->getVector3Attribute("destination_options");
		
		if(entityInfo->hasAttribute("destination_castle"))
			_destinationCastle = entityInfo->getVector3Attribute("destination_castle");

		if(entityInfo->hasAttribute("destination_credits"))
			_destinationCredits = entityInfo->getVector3Attribute("destination_credits");

		if(entityInfo->hasAttribute("destination_help"))
			_destinationHelp = entityInfo->getVector3Attribute("destination_help");

		_target = Vector3::ZERO;
		_graphicsCameraMenu->setTargetCameraPosition(Vector3::ZERO);
		

		return true;

	} // spawn
	
	//---------------------------------------------------------

	void CCameraMenu::awake()
	{		
		
		_newTargetDirection = Vector3::ZERO;
		_movementDirection = Vector3::ZERO;
		goToMenu();
	}

	//---------------------------------------------------------

	bool CCameraMenu::activate()
	{

		return true;

	} // activate
	
	//---------------------------------------------------------

	void CCameraMenu::deactivate()
	{
		
	} // deactivate

	//--------------------------------------------------------

	void CCameraMenu::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);

		if(_movementDirection != Vector3::ZERO)
		{
			Vector3 newPos = _entity->getPosition() + _movementDirection * CAMERA_MOVEMENT;
			_entity->setPositionN(newPos);
			_graphicsCameraMenu->setCameraPosition(newPos);
			if(_entity->getPosition().positionCloses(_destination))
			{
				_movementDirection = Vector3::ZERO;		
				_entity->setPositionN(_destination);
				_graphicsCameraMenu->setCameraPosition(_destination);
			}
		}


		if(_newTargetDirection != Vector3::ZERO)
		{
			Vector3 newTarget = _graphicsCameraMenu->getTargetCameraPosition() + _newTargetDirection * CAMERA_TARGET_MOVEMENT;
			_graphicsCameraMenu->setTargetCameraPosition(newTarget);
			if(_graphicsCameraMenu->getTargetCameraPosition().positionCloses(_target))
			{
				_newTargetDirection = Vector3::ZERO;
				_graphicsCameraMenu->setTargetCameraPosition(_target);
			}
		}



			
	} // tick
		
	//--------------------------------------------------------

	void CCameraMenu::goToMenu()
	{
		_destination = _destinationMenu;
		_movementDirection = _destination - _entity->getPosition();
		_movementDirection.normalise();
		_newTargetDirection = _targetMenu - _target;
		_newTargetDirection.normalise();
		_target = _targetMenu;
		
	}
	
	//--------------------------------------------------------

	void CCameraMenu::goToCastle()
	{
		_destination = _destinationCastle;
		_movementDirection = _destination - _entity->getPosition();
		_movementDirection.normalise();
		_newTargetDirection = _targetCastle - _target;
		_newTargetDirection.normalise();
		_target = _targetCastle;
	}

	//--------------------------------------------------------

	void CCameraMenu::goToOptions()
	{
		_destination = _destinationOptions;
		_movementDirection = _destination - _entity->getPosition();
		_movementDirection.normalise();		
		_newTargetDirection = _targetOptions - _target;
		_newTargetDirection.normalise();
		_target = _targetOptions;
	}
	
	void CCameraMenu::goToHelp()
	{
		_destination = _destinationHelp;
		_movementDirection = _destination - _entity->getPosition();
		_movementDirection.normalise();
		_newTargetDirection = _targetHelp - _target;
		_newTargetDirection.normalise();
		_target = _targetHelp;
		
	}
	//--------------------------------------------------------

	void CCameraMenu::goToCredits()
	{
		_destination = _destinationCredits;
		_movementDirection = _destination - _entity->getPosition();
		_movementDirection.normalise();		
		_newTargetDirection = _targetCredits - _target;
		_newTargetDirection.normalise();
		_target = _targetCredits;
	}

} // namespace Logic

