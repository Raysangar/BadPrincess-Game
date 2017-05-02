/**
@file Particle.cpp

Contains the implementation of the particle component
 
@see Logic::CParticle
@see Logic::IComponent

@author Rayco Sánchez García
@date Rayco Sánchez García
*/

#include "Particle.h"

#include "Logic/MovementMessages.h"
#include "Logic/GraphicsMessages.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include "Graphics/Scene.h"
#include "Graphics/ParticleEntity.h"

namespace Logic 
{
	IMP_FACTORY(CParticle);
	
	//---------------------------------------------------------

	CParticle::~CParticle() 
	{
		deactivate();
		std::map<std::string, Graphics::CParticleEntity*>::iterator it;
		for (it = _particleEntities.begin(); it != _particleEntities.end(); ++it)
			delete it->second;		
	} // ~CGraphics
	
	//---------------------------------------------------------

	bool CParticle::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;
		
		_scene = _entity->getMap()->getScene();
		createParticleEntities(entityInfo);

		if (entityInfo->hasAttribute("defaultParticleSystem"))
			defaultParticleSystem = entityInfo->getStringAttribute("defaultParticleSystem");

		return true;

	} // spawn
	
	//---------------------------------------------------------

	bool CParticle::activate()
	{
		std::map<std::string, Graphics::CParticleEntity*>::iterator it;
		int i = 0;
		for (it = _particleEntities.begin(); it != _particleEntities.end(); ++it)
		{
			_scene->addEntity(it->second);
			it->second->setTransform(_entity->getTransform());
			++i;
		}


		if (defaultParticleSystem != "")
			_particleEntities[defaultParticleSystem]->start();
		return true;
	}

	void CParticle::deactivate()
	{
		std::map<std::string, Graphics::CParticleEntity*>::iterator it;
		for (it = _particleEntities.begin(); it != _particleEntities.end(); ++it)
			_scene->removeEntity(it->second);
	}

	void CParticle::awake()
	{
		std::map<std::string, Graphics::CParticleEntity*>::iterator it;
		for (it = _particleEntities.begin(); it != _particleEntities.end(); ++it)
			it->second->stop();

		if (defaultParticleSystem != "")
			_particleEntities[defaultParticleSystem]->start();

	}

	//---------------------------------------------------------

	void CParticle::createParticleEntities(const Map::CEntity *entityInfo)
	{		
		
		for (int i = 0; i < entityInfo->getIntAttribute("particle_systems_count"); ++i)
		{
			std::string particleSystemName = entityInfo->getStringAttribute("particle_systems_" + std::to_string(i));
			_particleEntities[particleSystemName] = new Graphics::CParticleEntity(_entity->getName() + particleSystemName, particleSystemName);
		}

	} 

	//---------------------------------------------------------
	
	bool CParticle::acceptN(const std::shared_ptr<NMessage> &message)
	{
		return (message->type == "TransformMessage") || (message->type == "PlayParticle") || (message->type == "StopParticle")
			|| (message->type == "PauseParticle") || (message->type == "ScaleParticle");
	} // acceptN
	
	
	//---------------------------------------------------------

	void CParticle::processN(const std::shared_ptr<NMessage> &message)
	{
		if (message->type.compare("TransformMessage") == 0)
		{
			std::shared_ptr<TransformMessage> m = std::static_pointer_cast<TransformMessage>(message);
			std::map<std::string, Graphics::CParticleEntity*>::iterator it;
			for (it = _particleEntities.begin(); it != _particleEntities.end(); ++it)
				it->second->setTransform(m->transform);
		}
		else if (message->type == "PlayParticle")
			_particleEntities[std::static_pointer_cast<PlayParticle>(message)->particleName]->start();
		else if (message->type == "PauseParticle")
			_particleEntities[std::static_pointer_cast<PauseParticle>(message)->particleName]->pause();
		else if (message->type == "StopParticle")
			_particleEntities[std::static_pointer_cast<StopParticle>(message)->particleName]->stop();
		else if (message->type == "ScaleParticle")
			_particleEntities[std::static_pointer_cast<StopParticle>(message)->particleName]->setScale(std::static_pointer_cast<ScaleParticle>(message)->scale);

	} // processN
	

} // namespace Logic

