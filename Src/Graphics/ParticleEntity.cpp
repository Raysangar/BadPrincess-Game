/**
@file ParticleEntity.cpp

Contains the implementation of CParticleEntity class.

@see Graphics::CParticleEntity

@author Rayco Sánchez García
@date March, 2015
*/

#include "ParticleEntity.h"
#include "../ParticleUniverse/ParticleUniverseSystemManager.h"
#include "Scene.h"

#include <assert.h>

#include <OgreParticleSystem.h>
#include <OgreParticleEmitter.h>
#include <OgreSceneManager.h>

namespace Graphics 
{
	CParticleEntity::CParticleEntity(const std::string &name, const std::string &particleSystemName)
		: CEntity(name), _particleSystemName(particleSystemName), _particleSystem(nullptr) {} 

	//--------------------------------------------------------

	CParticleEntity::~CParticleEntity() 
	{
		assert(!_scene && "¡¡Para destruir una entidad esta no puede pertenecer a una escena!!");
	} 
	
	
	//--------------------------------------------------------
		
	bool CParticleEntity::load()
	{
		ParticleUniverse::ParticleSystemManager* pManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
		_particleSystem = pManager->createParticleSystem(_name, _particleSystemName, _scene->getSceneMgr());

		_entityNode = _scene->getSceneMgr()->getRootSceneNode()->
								createChildSceneNode(_name + "_particle_node");
		_entityNode->attachObject(_particleSystem);

		_loaded = true;

		return true;
	} 
	
	//--------------------------------------------------------
		
	void CParticleEntity::unload()
	{
		if(_entityNode)
		{
			_particleSystem->stop();
			ParticleUniverse::ParticleSystemManager::getSingletonPtr()->destroyParticleSystem(_particleSystem, _scene->getSceneMgr());
			// desacoplamos la entidad de su nodo
			_entityNode->detachAllObjects();
			_scene->getSceneMgr()->destroySceneNode(_entityNode);
			_entityNode = 0;
		}

		_loaded = false;
	}

	//--------------------------------------------------------
		
	void CParticleEntity::tick(float secs)
	{
	} // tick
			
	//--------------------------------------------------------

	const bool CParticleEntity::getVisible()
	{
		if(_entityNode)
			return _particleSystem->getVisible();

		throw new std::exception("La entidad no ha sido cargada");

	}

	//--------------------------------------------------------

	void CParticleEntity::setScale(float scale)
	{	
		_particleSystem->setScale(Vector3(scale));
	}

	void CParticleEntity::start()
	{
		_particleSystem->start();
	}

	//--------------------------------------------------------

	void CParticleEntity::pause()
	{
		_particleSystem->pause();
	}

	//--------------------------------------------------------

	void CParticleEntity::stop()
	{
		_particleSystem->stop();
	}

} // namespace Graphics
