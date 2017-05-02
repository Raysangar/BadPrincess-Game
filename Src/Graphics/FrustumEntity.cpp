//---------------------------------------------------------------------------
// Entity.cpp
//---------------------------------------------------------------------------

/**
@file Entity.cpp

Contiene la implementación de la clase que representa una entidad gráfica.

@see Graphics::CFrustumEntity

@author Giacomo Guidi
@date March, 2015
*/

#include "FrustumEntity.h"
#include "Scene.h"

#include <assert.h>

#include <OgreFrustum.h>
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>

namespace Graphics 
{
	CFrustumEntity::CFrustumEntity(const std::string &name)
		: CEntity(name), _entity(0)
	{
	} // CFrustumEntity

	//--------------------------------------------------------

	CFrustumEntity::~CFrustumEntity() 
	{
		assert(!_scene && "¡¡Para destruir una entidad esta no puede pertenecer a una escena!!");
		
	} // ~CFrustumEntity
	
	
	//--------------------------------------------------------
		
	bool CFrustumEntity::load()
	{
		_entity = new Ogre::Frustum();
		
		_entityNode = _scene->getSceneMgr()->getRootSceneNode()->
								createChildSceneNode(_name + "_frustum_node");
		_entityNode->attachObject(_entity);

		_loaded = true;

		return true;

	} // load
	
	//--------------------------------------------------------
		
	void CFrustumEntity::unload()
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
		      // delete frustum
			delete _entity;
			_entity = 0;
		}

		_loaded = false;

	} // load

	//--------------------------------------------------------
		
	void CFrustumEntity::tick(float secs)
	{
	} // tick
			
	const bool CFrustumEntity::getVisible()
	{
		if(_entityNode)
			return _entity->getVisible();

		throw new std::exception("La entidad no ha sido cargada");

	} // getPosition

	void CFrustumEntity::setAspectRatio(float aspectRatio)
	{
		_entity->setAspectRatio(aspectRatio);
	}

	void CFrustumEntity::setOrthoWindowHeight(float height)
	{
		_entity->setOrthoWindowHeight(height);
	}
	void CFrustumEntity::setFarClipDistance(float height)
	{
		_entity->setFarClipDistance(height);
	}

	void CFrustumEntity::setDebugDisplayEnabled(bool value)
	{
		_entity->setDebugDisplayEnabled(value);
	}
	void CFrustumEntity::setProjectionType(Ogre::ProjectionType type)
	{
		_entity->setProjectionType(type);
	}

} // namespace Graphics
