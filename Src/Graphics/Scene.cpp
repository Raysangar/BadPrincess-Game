//---------------------------------------------------------------------------
// Scene.cpp
//---------------------------------------------------------------------------

/**
@file Scene.cpp

Contiene la implementación de la clase contenedora de los elementos
de una escena.

@see Graphics::CScene

@author David Llansó
@date Julio, 2010
*/

#include "Scene.h"
#include "Camera.h"
#include "Server.h"
#include "StaticEntity.h"
#include "DrawManager.h"
#include "StencilGlow.h"
#include "BaseSubsystems/Server.h"

#include <assert.h>

#include <OgreRoot.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreViewport.h>
#include <OgreStaticGeometry.h>
#include <OgreColourValue.h>

namespace Graphics 
{
	CScene::CScene(const std::string& name) : _viewport(0), 
			_staticGeometry(0), _directionalLight(0), _stencilGlow(0)
	{
		_root = BaseSubsystems::CServer::getSingletonPtr()->getOgreRoot();
		_sceneMgr = _root->createSceneManager(Ogre::ST_INTERIOR, name);
		_camera = new CCamera(name,this);
		_drawManager = new CDrawManager(this);
		_stencilGlow = new CStencilGlow(this);
		_name = name;

	} // CScene

	//--------------------------------------------------------

	CScene::~CScene() 
	{
		deactivate();
		_sceneMgr->destroyStaticGeometry(_staticGeometry);
		delete _camera;
		delete _drawManager;
		delete _stencilGlow;
		_root->destroySceneManager(_sceneMgr);

	} // ~CScene

	//--------------------------------------------------------

	bool CScene::addEntity(CEntity* entity)
	{
		if (std::find(_dynamicEntities.begin(), _dynamicEntities.end(), entity) != _dynamicEntities.end())
			return true;
		if(!entity->attachToScene(this))
			return false;
		_dynamicEntities.push_back(entity);
		return true;

	} // addEntity


	bool CScene::addStaticEntity(CStaticEntity* entity)
	{
		if (std::find(_staticEntities.begin(), _staticEntities.end(), entity) != _staticEntities.end())
			return true;
		if(!entity->attachToScene(this))
			return false;
		_staticEntities.push_back(entity);
		return true;

	} // addStaticEntity

	//--------------------------------------------------------

	void CScene::removeEntity(CEntity* entity)
	{
		entity->deattachFromScene();
		_dynamicEntities.remove(entity);

	} // addEntity
	//--------------------------------------------------------

	void CScene::removeStaticEntity(CStaticEntity* entity)
	{
		entity->deattachFromScene();
		_staticEntities.remove(entity);

	} // addStaticEntity

	//--------------------------------------------------------

	void CScene::activate()
	{
		buildStaticGeometry();
		// HACK en pruebas
		_viewport = BaseSubsystems::CServer::getSingletonPtr()->getRenderWindow()
						->addViewport(_camera->getCamera());
		_viewport->setBackgroundColour(Ogre::ColourValue::Black);

		_sceneMgr->setAmbientLight(Ogre::ColourValue(0.6f,0.6f,0.6f));
		_sceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_MODULATIVE);
		_sceneMgr->setShadowTextureSize(500);

		std::string shadowsAttribute = BaseSubsystems::CServer::getSingletonPtr()->getGameOptions()->getAttribute("currentShadowsId");
		int numb;
		std::istringstream ( shadowsAttribute ) >> numb;
		_sceneMgr->setShadowTextureFSAA(numb*2);

		_sceneMgr->setShadowTextureSelfShadow(false);
		_sceneMgr->setShadowDirectionalLightExtrusionDistance(10000); 
		_sceneMgr->setShadowTextureCount(1);
		_sceneMgr->setShadowColour(Ogre::ColourValue(0.4f, 0.4f, 0.4f));

		_sceneMgr->setSkyBox(true,"BadPrincessSkybox");
		
	} // activate

	//--------------------------------------------------------

	void CScene::deactivate()
	{
		if(_directionalLight)
		{
			_sceneMgr->destroyLight(_directionalLight);
			_directionalLight = 0;
		}
		if(_viewport)
		{
			BaseSubsystems::CServer::getSingletonPtr()->getRenderWindow()->
					removeViewport(_viewport->getZOrder());
			_viewport = 0;
		}

	} // deactivate
	
	//--------------------------------------------------------

	
	void CScene::tick(float secs)
	{	
		TEntityList::const_iterator it = _dynamicEntities.begin();
		TEntityList::const_iterator end = _dynamicEntities.end();
		for(; it != end; it++)
			(*it)->tick(secs);

	} // tick

	//--------------------------------------------------------

	void CScene::buildStaticGeometry()
	{
		if(!_staticGeometry && !_staticEntities.empty())
		{
			_staticGeometry = 
					_sceneMgr->createStaticGeometry("static");

			TStaticEntityList::const_iterator it = _staticEntities.begin();
			TStaticEntityList::const_iterator end = _staticEntities.end();
			for(; it != end; it++)
				(*it)->addToStaticGeometry();

			_staticGeometry->build();
		}

	} // buildStaticGeometry

} // namespace Graphics
