/**
@file Camera.cpp

Contiene la implementación del componente que controla la cámara gráfica
de una escena.
 
@see Logic::CCamera
@see Logic::IComponent

@author David Llansó
@date Agosto, 2010
*/

#include "Camera.h"

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

#include "AI/Server.h"
#include "AI/EntityManager.h"

#define MIN_ZOOM_THRESHOLD_PERCENTAGE 10

namespace Logic 
{

	IMP_FACTORY(CCamera);
	
	//---------------------------------------------------------

	bool CCamera::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		_graphicsCamera = _entity->getMap()->getScene()->getCamera();
		if(!_graphicsCamera)
			return false;

		if(entityInfo->hasAttribute("minHeight"))
			_minHeight = entityInfo->getFloatAttribute("minHeight");

		if(entityInfo->hasAttribute("maxHeight"))
			_maxHeight = entityInfo->getFloatAttribute("maxHeight");

		if(entityInfo->hasAttribute("targetDistanceZ"))
			_targetDistanceZ = entityInfo->getFloatAttribute("targetDistanceZ");

		if(entityInfo->hasAttribute("sensitivity"))
			_sensitivity = entityInfo->getFloatAttribute("sensitivity");

		if(entityInfo->hasAttribute("topLimit"))
			_topLimit = entityInfo->getFloatAttribute("topLimit");

		if(entityInfo->hasAttribute("bottomLimit"))
			_bottomLimit = entityInfo->getFloatAttribute("bottomLimit");

		if(entityInfo->hasAttribute("rightLimit"))
			_rightLimit = entityInfo->getFloatAttribute("rightLimit");

		if(entityInfo->hasAttribute("leftLimit"))
			_leftLimit = entityInfo->getFloatAttribute("leftLimit");

		_maxZoom = _maxHeight - _minHeight;
		// Camara mas arriba
		Vector3 pos(_entity->getPosition().x,_maxHeight/5 * 4,_entity->getPosition().z);
		_graphicsCamera->setCameraPosition(pos);

		return true;

	} // spawn
	
	//---------------------------------------------------------

	void CCamera::awake()
	{
		CPhysicEntity* physicComponent = (CPhysicEntity*) CServer::getSingletonPtr()->getMap()->getEntityByName("Ground")->getComponent("CPhysicEntity");
		_cameraLimits = physicComponent->getActorDimensions()/2;
		
		_isMoving = false;		
		_cameraBlocked = false;
		_cameraOnUnit = false;

		_topLimit = physicComponent->getActorDimensions().z * _topLimit;
		_bottomLimit = physicComponent->getActorDimensions().z * _bottomLimit;
		_rightLimit = physicComponent->getActorDimensions().x * _rightLimit;
		_leftLimit = physicComponent->getActorDimensions().x * _leftLimit;
	}

	//---------------------------------------------------------

	bool CCamera::activate()
	{
		return true;

	} // activate
	
	//---------------------------------------------------------

	void CCamera::deactivate()
	{
		_target = 0;

	} // deactivate
	
	//---------------------------------------------------------

	bool CCamera::acceptN(const std::shared_ptr<NMessage> &message)
	{
		return (message->type.compare("CameraControl") == 0) || (message->type.compare("CameraPosition") == 0) || 
			(message->type.compare("CameraMoveTo") == 0) || message->type == "EnableCompositor" || message->type == "DisableCompositor"
			|| message->type.compare("CameraScroll") == 0;

	} // accept

	//---------------------------------------------------------

	void CCamera::processN(const std::shared_ptr<NMessage> &message)
	{
		if(!_isMoving){
			if (message->type.compare("CameraControl") == 0) {
				if(!_cameraBlocked) {
					std::shared_ptr<CameraControl> m = std::static_pointer_cast<CameraControl>(message);
					_movementDirection = m->direction;
				}
			}
			else if  (message->type.compare("CameraPosition") == 0)
			{
				if(!_cameraBlocked) {
					auto m = std::static_pointer_cast<CameraPosition>(message);
					float _distanceZToTarget = _graphicsCamera->getCameraPosition().z - _graphicsCamera->getTargetCameraPosition().z;
					Vector3 newPosition = m->position;
					newPosition.y = _graphicsCamera->getCameraPosition().y;
					newPosition.z += _distanceZToTarget;
					_graphicsCamera->setCameraPosition(newPosition);
					_entity->setPositionN(newPosition);
				}
			}
			else if (message->type.compare("CameraMoveTo") == 0) {
				std::shared_ptr<CameraMoveTo> m = std::static_pointer_cast<CameraMoveTo>(message);
				_destination = m->position;
				_destination.z += _targetDistanceZ;
				_movementDirection = _destination - _entity->getPosition();	
				_movementDirection.normalise();
				_isMoving = true;
			}
			else if (message->type.compare("CameraScroll") == 0)
			{
				auto m = std::static_pointer_cast<CameraScroll>(message);
				Vector3 direction = _graphicsCamera->getCameraPosition() - _graphicsCamera->getTargetCameraPosition();
				direction  = direction.normalisedCopy() * m->scroll;				
				Vector3 newPosition = _graphicsCamera->getCameraPosition() + direction;

				if(newPosition.distance(_graphicsCamera->getTargetCameraPosition()) < _maxHeight &&
					newPosition.distance(_graphicsCamera->getTargetCameraPosition()) > _minHeight && newPosition.y > 0)
				{
					_graphicsCamera->setCameraPosition(newPosition);
					_entity->setPositionN(newPosition);			
					_targetDistanceZ = _graphicsCamera->getCameraPosition().z - _graphicsCamera->getTargetCameraPosition().z;
				}
			}
		}
		if (message->type == "EnableCompositor")
			_graphicsCamera->enableCompositor(std::static_pointer_cast<EnableCompositor>(message)->compositorName);
		else if (message->type == "DisableCompositor")
			_graphicsCamera->enableCompositor(std::static_pointer_cast<DisableCompositor>(message)->compositorName);
	} // processN

	//--------------------------------------------------------

	void CCamera::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);

		if(!_cameraOnUnit)
		{
			if(_entityFollowed != nullptr)
			{
				float _distanceZToTarget = _graphicsCamera->getCameraPosition().z - _graphicsCamera->getTargetCameraPosition().z;

				Vector3 ef = _entityFollowed->getPosition();
				Vector3 e = _graphicsCamera->getCameraPosition();
				e.y = ef.y;
				ef.z += _distanceZToTarget;

				if(e.distance(ef) < 2)
				{
					_movementDirection = Vector3::ZERO;
					_cameraOnUnit = true;
				}
				else
				{
					_movementDirection = ef - e;
					_movementDirection.y = 0;
					_movementDirection.normalise();
				}
			}

			//calculate the current zoom
			float currentZoom = _graphicsCamera->getCameraPosition().y -_minHeight;
			float percentage = currentZoom*100/(_maxHeight-_minHeight);
			if(percentage <= MIN_ZOOM_THRESHOLD_PERCENTAGE)
				currentZoom = (_maxHeight-_minHeight)*MIN_ZOOM_THRESHOLD_PERCENTAGE / 100;
			Vector3 deltaMovement =  _movementDirection  * _sensitivity * (float)msecs/4;
			//the movement on x and z axes depend of the zoom level
			deltaMovement.x	*= (currentZoom / _maxZoom);
			deltaMovement.z	*= (currentZoom / _maxZoom);
		
			// Actualizamos la posición de la cámara.	
			Vector3 newPosition = _graphicsCamera->getCameraPosition() + deltaMovement;

			adjustNewPositionToCameraLimits(newPosition);

			_graphicsCamera->setCameraPosition(newPosition);
			_entity->setPositionN(newPosition);
			_target = Vector3( _graphicsCamera->getCameraPosition().x, 0, _graphicsCamera->getCameraPosition().z - _targetDistanceZ);
			_graphicsCamera->setTargetCameraPosition(_target);

			if(_isMoving && _entity->getPosition().positionCloses(_destination))
			{
				_movementDirection = Vector3::ZERO;
				_isMoving = false;
			}
			
		}
		else
		{
			float distanceZToTarget = _graphicsCamera->getCameraPosition().z - _graphicsCamera->getTargetCameraPosition().z;
			Vector3 ef = _entityFollowed->getPosition();
			Vector3 e = _graphicsCamera->getCameraPosition();
			e.y = ef.y;
			ef.z += distanceZToTarget;

			Vector3 distance = ef - e;
			distance.y = 0;

			Vector3 newPosition = _graphicsCamera->getCameraPosition() + distance;
			adjustNewPositionToCameraLimits(newPosition);

			_graphicsCamera->setCameraPosition(newPosition);
			_entity->setPositionN(newPosition);
			_target = Vector3( _graphicsCamera->getCameraPosition().x, 0, _graphicsCamera->getCameraPosition().z - _targetDistanceZ);
			_graphicsCamera->setTargetCameraPosition(_target);
		}

	} // tick
		
	//--------------------------------------------------------

	Vector3 CCamera::getCameraTargetPosition()
	{
		return _graphicsCamera->getTargetCameraPosition();
	}

	//--------------------------------------------------------

	float CCamera::getAspectRatio() const
	{
		return _graphicsCamera->getAspectRatio();
	}

	//--------------------------------------------------------

	void CCamera::adjustNewPositionToCameraLimits(Vector3& newPosition)
	{
		float frustumHeight = getFrustumHeight();
		float frustumWidth = frustumHeight * getAspectRatio();

		Quaternion q = _graphicsCamera->getCameraOrientation();

		if (newPosition.x > _cameraLimits.x - frustumWidth/2 - _rightLimit)
			newPosition.x = _cameraLimits.x - frustumWidth/2 - _rightLimit;
		
		if (newPosition.z > _cameraLimits.z - _bottomLimit)
			newPosition.z = _cameraLimits.z - _bottomLimit;

		if (newPosition.x < -_cameraLimits.x + frustumWidth/2 + _leftLimit)
			newPosition.x = -_cameraLimits.x + frustumWidth/2 + _leftLimit;
		
		if (newPosition.z < -_cameraLimits.z + frustumHeight/2 + _topLimit + _targetDistanceZ)
			newPosition.z = -_cameraLimits.z + frustumHeight/2 + _topLimit + _targetDistanceZ;
	}

	//--------------------------------------------------------

	float CCamera::getFrustumHeight() const
	{
		return ((float) 2.0 * _entity->getPosition().y * Ogre::Math::Tan(_graphicsCamera->getCamera()->getFOVy() * 0.5));
	}

	//--------------------------------------------------------

	float CCamera::getFrustumWidth() const
	{
		return ((float) 2.0 * _target.distance(_entity->getPosition()) * Ogre::Math::Tan(_graphicsCamera->getCamera()->getFOVy() * 0.5) * getAspectRatio());
	}

	//--------------------------------------------------------

	void CCamera::setCameraBlock(bool bc) 
	{ 
		_cameraBlocked = bc;
	}

	//--------------------------------------------------------

	void CCamera::setEntityFollowed(Logic::CEntity* ef)
	{
		_entityFollowed = ef;
		_cameraOnUnit = false;
	}

	//--------------------------------------------------------

	bool CCamera::isEntityOnCamera(const Vector3 &entityPosition) const
	{
		float frustumHeight = getFrustumHeight();
		float frustumWidth = frustumHeight * getAspectRatio();
		Vector3 cameraPosition = _entity->getPosition();

		if (entityPosition.x > cameraPosition.x + frustumWidth/2)
			return false;
		
		if (entityPosition.z > cameraPosition.z + frustumHeight/2 - _targetDistanceZ)
			return false;

		if (entityPosition.x < cameraPosition.x - frustumWidth/2)
			return false;
		
		if (entityPosition.z < cameraPosition.z - frustumHeight/2 - _targetDistanceZ)
			return false;

		return true;
	}

} // namespace Logic

