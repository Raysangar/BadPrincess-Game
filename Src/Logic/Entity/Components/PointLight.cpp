/**
@file PointLight.cpp
*/

#include "PointLight.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"
#include <OgreSceneManager.h>

#include "Graphics/Scene.h"
#include "Graphics/Entity.h"
#include "Graphics/StaticEntity.h"

namespace Logic 
{
	IMP_FACTORY(CPointLight);

	//---------------------------------------------------------

	CPointLight::~CPointLight() 
	{

	} 

	//---------------------------------------------------------

	bool CPointLight::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		
		_scene = _entity->getMap()->getScene();

		PointLight = _scene->getSceneMgr()->createLight(entityInfo->getName());
		PointLight->setType(Ogre::Light::LT_POINT);
		
		if(entityInfo->hasAttribute("diffuse_color"))
		{
			Vector3 diffuseColor = entityInfo->getVector3Attribute("diffuse_color");
			PointLight->setDiffuseColour(diffuseColor.x,diffuseColor.y,diffuseColor.z);
		}
		else
			PointLight->setDiffuseColour(.5f,.5f,.5f);

		if(entityInfo->hasAttribute("specular_color"))
		{
			Vector3 specularColor = entityInfo->getVector3Attribute("specular_color");
			PointLight->setSpecularColour(specularColor.x,specularColor.y,specularColor.z);
		}
		else
			PointLight->setSpecularColour(.5f,.5f,.5f);

		assert(entityInfo->hasAttribute("position"));
		Vector3 position = entityInfo->getVector3Attribute("position");
		PointLight->setPosition(position);

		float range = 100.0f;
	
		PointLight->setAttenuation(range, 1.0f, 4.5f/range, 75.0f/(range*range));

		return true;
	} 

	void CPointLight::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);
	}

	//---------------------------------------------------------

	bool CPointLight::acceptN(const std::shared_ptr<NMessage> &message)
	{
		return true;
	}

	//---------------------------------------------------------

	void CPointLight::processN(const std::shared_ptr<NMessage> &message)
	{
		
	}
}

