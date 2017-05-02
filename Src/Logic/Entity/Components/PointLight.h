/**
@file PointLight.h

*/

#ifndef __Logic_PointLight_H
#define __Logic_PointLight_H

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
	
	class CPointLight : public IComponent
	{

	DEC_FACTORY(CPointLight);

	public:	
		CPointLight(std::string componentName) : IComponent(componentName){}

		virtual ~CPointLight();
		
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		virtual void tick(unsigned int msecs);

		virtual bool acceptN(const std::shared_ptr<NMessage> &message);

		virtual void processN(const std::shared_ptr<NMessage> &message);

	protected:
		Ogre::Light *PointLight;
		Graphics::CScene* _scene;
	}; 

	REG_FACTORY(CPointLight);
} 

#endif
