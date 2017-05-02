/**
@file DirectionalLight.cpp
*/

#include "DirectionalLight.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"
#include <OgreSceneManager.h>

#include "Graphics/Scene.h"
#include "Graphics/Entity.h"
#include "Graphics/StaticEntity.h"

#include "BaseSubsystems/Server.h"

namespace Logic 
{
	IMP_FACTORY(CDirectionalLight);

	//---------------------------------------------------------

	CDirectionalLight::~CDirectionalLight() 
	{

	} 

	//---------------------------------------------------------

	bool CDirectionalLight::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{

		_scene = _entity->getMap()->getScene();

		directionalLight = _scene->getSceneMgr()->createLight(entityInfo->getName());
		directionalLight->setType(Ogre::Light::LT_DIRECTIONAL);
		directionalLight->setCastShadows(BaseSubsystems::CServer::getSingletonPtr()->getGameOptions()->getAttribute("currentShadowsId") != "0");
		
		if(entityInfo->hasAttribute("diffuse_color"))
		{
			Vector3 diffuseColor = entityInfo->getVector3Attribute("diffuse_color");
			directionalLight->setDiffuseColour(diffuseColor.x,diffuseColor.y,diffuseColor.z);
		}
		else
			directionalLight->setDiffuseColour(.5f,.5f,.5f);

		if(entityInfo->hasAttribute("specular_color"))
		{
			Vector3 specularColor = entityInfo->getVector3Attribute("specular_color");
			directionalLight->setSpecularColour(specularColor.x,specularColor.y,specularColor.z);
		}
		else
			directionalLight->setSpecularColour(.5f,.5f,.5f);

		if(entityInfo->hasAttribute("position"))
		{
			Vector3 position = entityInfo->getVector3Attribute("position");
			directionalLight->setPosition(position);
		}
		else
			directionalLight->setPosition(0, 500, 0);

		directionalLight->setDirection((Ogre::Vector3::ZERO - directionalLight->getPosition()).normalisedCopy());

		return true;
	} 

	void CDirectionalLight::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);
	}

	//---------------------------------------------------------

	bool CDirectionalLight::acceptN(const std::shared_ptr<NMessage> &message)
	{
		return true;
	}

	//---------------------------------------------------------

	void CDirectionalLight::processN(const std::shared_ptr<NMessage> &message)
	{
		
	}
}

