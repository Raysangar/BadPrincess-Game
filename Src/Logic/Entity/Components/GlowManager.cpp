/**
@file GlowManager.cpp
*/

#include "GlowManager.h"

#include "Logic/GraphicsMessages.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"
#include <OgreSceneManager.h>
#include <OgreEntity.h>

#include "Graphics/Scene.h"
#include "Graphics/StencilGlow.h"
#include "Graphics/Server.h"


#include "BaseSubsystems/Server.h"

namespace Logic 
{
	IMP_FACTORY(CGlowManager);

	//---------------------------------------------------------

	CGlowManager::~CGlowManager() 
	{

	} 

	//---------------------------------------------------------

	bool CGlowManager::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{


		return true;
	} 

	//---------------------------------------------------------

	void CGlowManager::awake()
	{
		_ogreEntity = _entity->getMap()->getScene()->getSceneMgr()->getEntity(_entity->getName());		
		//_glowEntity = _ogreEntity->clone(_ogreEntity->getName() + "_glow"); 
		//_glowNode = _ogreEntity->getParentSceneNode()->createChildSceneNode(_glowEntity->getName() + "outlineGlowNode"); 	

		//Graphics::CServer::getSingletonPtr()->getActiveScene()->getStencilGlow()->createGlow(_ogreEntity,_glowEntity);		
		//_glowNode->attachObject(_glowEntity);
	}

	//---------------------------------------------------------

	bool CGlowManager::activate()
	{
		if(_glowActive)
			//_glowNode->attachObject(_glowEntity);
			Graphics::CServer::getSingletonPtr()->getActiveScene()->getStencilGlow()->activateGlow(_ogreEntity,_entity->isEnemy());

		return true;
	}

	//---------------------------------------------------------

	void CGlowManager::deactivate()
	{
		if(_glowActive)
		//_glowNode->detachObject(_glowEntity->getName());
			Graphics::CServer::getSingletonPtr()->getActiveScene()->getStencilGlow()->deactivateGlow(_ogreEntity,_entity->isEnemy());
			
	}

	//---------------------------------------------------------

	void CGlowManager::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);

		//_glowNode->setPosition(_ogreEntity->getParentNode()->getPosition());
	}

	//---------------------------------------------------------

	bool CGlowManager::acceptN(const std::shared_ptr<NMessage> &message)
	{
		return (message->type.compare("ToggleGlow") == 0);
	}

	//---------------------------------------------------------

	void CGlowManager::processN(const std::shared_ptr<NMessage> &message)
	{
		if (message->type.compare("ToggleGlow") == 0)
		{
			auto glowMessage = std::static_pointer_cast<ToggleGlow>(message);
			toggleGlow(glowMessage->activate);
		}
	}

	//---------------------------------------------------------

	void CGlowManager::toggleGlow(bool activate)
	{
		if(activate && !_glowActive)
		{
			//_glowNode->detachObject(_glowEntity->getName());
			Graphics::CServer::getSingletonPtr()->getActiveScene()->getStencilGlow()->activateGlow(_ogreEntity,_entity->isEnemy());
			_glowActive = true;
		}
		else if(!activate && _glowActive)
		{
			//_glowNode->attachObject(_glowEntity);
			Graphics::CServer::getSingletonPtr()->getActiveScene()->getStencilGlow()->deactivateGlow(_ogreEntity,_entity->isEnemy());
			_glowActive = false;
		}
	}
}

