/**
@file GlowManager.h

*/

#ifndef __Logic_GlowManager_H
#define __Logic_GlowManager_H

#include "Logic/Entity/Component.h"

namespace Graphics 
{
	class CEntity;
	class CScene;
}

namespace Ogre
{
	class Entity;
	class SceneNode;
}

namespace Logic 
{
	class CEntity;
	
	class CGlowManager : public IComponent
	{

	DEC_FACTORY(CGlowManager);

	public:	
		CGlowManager(std::string componentName) : IComponent(componentName), _ogreEntity(0), _glowEntity(0), _glowNode(0),_glowActive(false){}

		virtual ~CGlowManager();

		virtual void awake();

		virtual bool activate();

		virtual void deactivate();
		
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		virtual void tick(unsigned int msecs);

		virtual bool acceptN(const std::shared_ptr<NMessage> &message);

		virtual void processN(const std::shared_ptr<NMessage> &message);

	protected:
	
		void toggleGlow(bool activate);

		bool _glowActive;

		Ogre::Entity *_ogreEntity;
		Ogre::Entity *_glowEntity;
		Ogre::SceneNode *_glowNode;

	}; 

	REG_FACTORY(CGlowManager);
} 

#endif
