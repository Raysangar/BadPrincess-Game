//---------------------------------------------------------------------------
// Entity.cpp
//---------------------------------------------------------------------------

/**
@file Entity.cpp

Contiene la implementación de la clase que representa una entidad gráfica.

@see Graphics::CManualEntity

@author David Llansó
@date Julio, 2010
*/

#include "ManualEntity.h"
#include "Scene.h"
#include "DrawManager.h"

#include "BaseSubsystems/Server.h"
#include "BaseSubsystems/Math.h"

#include <assert.h>

#include <OgreManualObject.h>
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>

namespace Graphics 
{
	CManualEntity::CManualEntity(const std::string &name)
		: CEntity(name), _entity(0),_isEmpty(true)
	{
	} // CManualEntity

	//--------------------------------------------------------

	CManualEntity::~CManualEntity() 
	{
		assert(!_scene && "¡¡Para destruir una entidad esta no puede pertenecer a una escena!!");
		
	} // ~CManualEntity
	
	//--------------------------------------------------------
		
	bool CManualEntity::load()
	{
		
		if(!_scene->getSceneMgr()->hasManualObject(_name))
			_entity = _scene->getSceneMgr()->createManualObject(_name);
		else
			_entity = _scene->getSceneMgr()->getManualObject(_name);
	
		_entityNode = _scene->getSceneMgr()->getRootSceneNode()->
								createChildSceneNode(_name + "_node");
		_entityNode->attachObject(_entity);
		_loaded = true;

		return true;

	} // load
	
	//--------------------------------------------------------
		
	void CManualEntity::unload()
	{
		if(_entityNode)
		{
			// desacoplamos la entidad de su nodo
			_entityNode->detachAllObjects();
			_scene->getSceneMgr()->destroySceneNode(_entityNode);
			_entityNode = 0;
		}
		if(_entity)
		{
			_scene->getSceneMgr()->destroyManualObject(_entity);
			_entity = 0;
		}
		_scene = nullptr;
		_loaded = false;

	} // load

	//--------------------------------------------------------
		
	void CManualEntity::tick(float secs)
	{
	} // tick
	
		
	const bool CManualEntity::getVisible()
	{
		if(_entityNode)
			return _entity->isVisible();

		throw new std::exception("La entidad no ha sido cargada");

	} // getPosition
	
	void CManualEntity::drawUnitaryBoxShape()
	{
		_scene->getDrawManager()->drawUnitaryBoxShape(_entity);
		_isEmpty=false;
	}
	void CManualEntity::drawCapsuleShape(const float halfHeight,const float radius)
	{
		_scene->getDrawManager()->drawCapsuleShape(_entity,halfHeight,radius);
		_isEmpty=false;
	}

	void CManualEntity::drawSegment(Vector3 start,Vector3 end,char color)
	{
		_scene->getDrawManager()->drawLine(start,end,_entityNode,color);
		_isEmpty=false;
	}

	void CManualEntity::drawCircleXZ(const float radius)
	{
		_scene->getDrawManager()->drawCircleShapeXZ(_entity,Vector3::ZERO,radius);
		_isEmpty = false;
	}


} // namespace Graphics
