/**
@file BillboardSetEntity.cpp

Contains the implementation of CBillboardSetEntity class.

@see Graphics::CBillboardSetEntity

@author Guidi Giacomo
@date March, 2015
*/

#include "BillboardSetEntity.h"
#include "Scene.h"

#include <assert.h>

#include <OgreSceneManager.h>
#include <OgreBillboardSet.h>
#include <OgreBillboard.h>

namespace Graphics 
{
	CBillboardSetEntity::CBillboardSetEntity(const std::string &name,BillboardType type,unsigned int poolSize)
		: CEntity(name),_type(type),_poolSize(poolSize),_billboardSet(nullptr)
	{	
	} 

	//--------------------------------------------------------

	CBillboardSetEntity::~CBillboardSetEntity() 
	{
		assert(!_scene && "¡¡Para destruir una entidad esta no puede pertenecer a una escena!!");
	} 
	
	
	//--------------------------------------------------------
		
	bool CBillboardSetEntity::load()
	{
		_billboardSet = _scene->getSceneMgr()->createBillboardSet(_name, _poolSize);
		_billboardSet->setBillboardType(Ogre::BillboardType(_type));

		_entityNode = _scene->getSceneMgr()->getRootSceneNode()->
								createChildSceneNode(_name + "_billboardset_node");
		_entityNode->attachObject(_billboardSet);

		_loaded = true;

		return true;

	} 
	
	//--------------------------------------------------------
		
	void CBillboardSetEntity::unload()
	{
		if(_entityNode)
		{
			_scene->getSceneMgr()->destroyBillboardSet(_billboardSet);
			// desacoplamos la entidad de su nodo
			_entityNode->detachAllObjects();
			_scene->getSceneMgr()->destroySceneNode(_entityNode);
			_entityNode = 0;
		}

		_loaded = false;

	}

	//--------------------------------------------------------
		
	void CBillboardSetEntity::tick(float secs)
	{
	} // tick
			
	const bool CBillboardSetEntity::getVisible()
	{
		if(_entityNode)
			return _billboardSet->getVisible();

		throw new std::exception("La entidad no ha sido cargada");

	}
	void CBillboardSetEntity::setMaterial(std::string materialName)
	{
		_billboardSet->setMaterialName(materialName);
	}


	void CBillboardSetEntity::addBillboard(Vector3 position,Vector2 dimension,float r,float g,float b)
	{
		assert(_billboardSet);

		Ogre::Billboard *billboard= _billboardSet->createBillboard(position);
		billboard->setDimensions(dimension.x,dimension.y);
		billboard->setColour(Ogre::ColourValue(r,g,b));

	}
	void CBillboardSetEntity::setBilboardColor(unsigned int billboardIdx,float r,float g,float b)
	{
		assert(_billboardSet);

		Ogre::Billboard *billboard= _billboardSet->getBillboard(billboardIdx);
		billboard->setColour(Ogre::ColourValue(r,g,b));
	}
	void CBillboardSetEntity::setBilboardDimension(unsigned int billboardIdx,float w,float h)
	{
		assert(_billboardSet);

		Ogre::Billboard *billboard= _billboardSet->getBillboard(billboardIdx);
		billboard->setDimensions(w,h);
	}
	void CBillboardSetEntity::setBilboardPosition(unsigned int billboardIdx,Vector3 pos)
	{
		assert(_billboardSet);

		Ogre::Billboard *billboard= _billboardSet->getBillboard(billboardIdx);
		billboard->setPosition(pos);
	}
	void CBillboardSetEntity::setBilboardTexCoordRect(unsigned int billboardIdx,float p0x,float p0y,float p1x,float p1y)
	{
		assert(_billboardSet);

		Ogre::Billboard *billboard= _billboardSet->getBillboard(billboardIdx);
		billboard->setTexcoordRect(p0x,p0y,p1x,p1y);
	}

	void CBillboardSetEntity::setBilboardSetOrigin(BillboardSetOrigin origin)
	{
		_billboardSet->setBillboardOrigin(Ogre::BillboardOrigin(origin));
	}

	void CBillboardSetEntity::setVisible(bool visible)
	{
		_billboardSet->setVisible(visible);
	}
} // namespace Graphics
