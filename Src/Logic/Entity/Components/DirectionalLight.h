/**
@file DirectionalLight.h

*/

#ifndef __Logic_DirectionalLight_H
#define __Logic_DirectionalLight_H

#include "Logic/Entity/Component.h"

namespace Graphics 
{
	class CEntity;
	class CScene;
}

namespace Logic 
{
	class CEntity;
	
/**
	Componente que se encarga...
*/
	
	class CDirectionalLight : public IComponent
	{

	DEC_FACTORY(CDirectionalLight);

	public:	
		CDirectionalLight(std::string componentName) : IComponent(componentName){}

		virtual ~CDirectionalLight();
		
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		virtual void tick(unsigned int msecs);

		virtual bool acceptN(const std::shared_ptr<NMessage> &message);

		virtual void processN(const std::shared_ptr<NMessage> &message);

	protected:
		Ogre::Light *directionalLight;
		Graphics::CScene* _scene;
	}; 

	REG_FACTORY(CDirectionalLight);
} 

#endif
