//---------------------------------------------------------------------------
// Entity.cpp
//---------------------------------------------------------------------------

/**
@file Entity.cpp

Contiene la implementación de la clase que representa una entidad gráfica.

@see Graphics::CMeshEntity

@author David Llansó
@date Julio, 2010
*/

#include "MeshEntity.h"
#include "Scene.h"
#include "FrustumEntity.h"

#include "Graphics/Server.h"
#include "Graphics/FogOfWar.h"

#include "BaseSubsystems/Server.h"
#include "BaseSubsystems/Math.h"

#include <assert.h>

#include <OgreEntity.h>
#include <OgreSceneNode.h>
#include <OgreMeshManager.h>
#include <OgreSceneManager.h>
#include <OgreSubEntity.h>
#include <OgreSubMesh.h>
#include <OgrePrerequisites.h>
#include <OgreTagPoint.h>


namespace Graphics 
{
	CMeshEntity::CMeshEntity(const std::string &name, const std::string &mesh)
		: CEntity(name), _entity(0),_attached(false)
	{
		_mesh = mesh;
	} // CMeshEntity

	//--------------------------------------------------------

	CMeshEntity::~CMeshEntity() 
	{
		assert(!_scene && "¡¡Para destruir una entidad esta no puede pertenecer a una escena!!");

	} // ~CMeshEntity


	//--------------------------------------------------------
	bool CMeshEntity::instantiateEntity(CScene*scene)
	{
		//if it's already created return false
		if(_loaded)
			return false;

		try
		{
			_entity = scene->getSceneMgr()->createEntity(_name, _mesh);

			if(_entity->getName() != "Ground" && _entity->getName() != "WoodenBridge" && _entity->getName() != "WoodenBridge2" && _entity->getName() != "StoneBridge" )
			{
				_entity->getMesh()->buildEdgeList();

				for(unsigned int i=0;i<_entity->getNumSubEntities();++i)
				{
					
					if(_entity->getName() != "CastleBack")
						_entity->getSubEntity(i)->getMaterial()->setReceiveShadows(false);
					else
						if (_entity->getSubEntity(i)->getMaterial()->getName() != "castle_floor")
							_entity->getSubEntity(i)->getMaterial()->setReceiveShadows(false);
				}
				
			}

			_entity->setCastShadows(_entity->getName() != "Ground" && _entity->getName() != "CastleFront");

//			assert(_entity->getReceivesShadows() && "Peluda cosa may frend");
			_loaded=true;
			return true;
		}
		catch(std::exception e)
		{
			return false;
		}

	}

	bool CMeshEntity::load()
	{
		if(!instantiateEntity(_scene))
			return false;
		//attach the entity to father scene node
		_entityNode = _scene->getSceneMgr()->getRootSceneNode()->
			createChildSceneNode(_name + "_node");
		_entityNode->attachObject(_entity);
		_loaded = true;
		_attached = true;
		return true;

	} // load

	//--------------------------------------------------------

	void CMeshEntity::unload()
	{
		if(_entityNode)
		{
			// desacoplamos la entidad de su nodo
			_entityNode->detachAllObjects();
			_scene->getSceneMgr()->destroySceneNode(_entityNode);
			_entityNode = 0;
		}
		if(_entity)
		{
			_scene->getSceneMgr()->destroyEntity(_entity);
			_entity = 0;
		}

		_loaded = false;
	} // load

	//--------------------------------------------------------

	void CMeshEntity::tick(float secs)
	{
	} // tick

	//--------------------------------------------------------



	//--------------------------------------------------------

	const bool CMeshEntity::getVisible()
	{
		if(_entityNode)
			return _entity->isVisible();

		throw new std::exception("La entidad no ha sido cargada");

	} // getPosition

	void CMeshEntity::setMaterialTransparency(std::string name,int tecnique,int pass,int unitState,float tranparency)
	{
		Ogre::MaterialPtr materialPtr;
		//build an unique material name for the instance
		std::string clonedMaterialName=_name+"_"+name;
		//if i have already modify the transparency i had already create the new material instance
		if(!Ogre::MaterialManager::getSingleton().resourceExists(clonedMaterialName))
		{
			//create a new material instance that affect only this entity
			materialPtr = Ogre::MaterialManager::getSingleton().getByName(name)->clone(clonedMaterialName);
			//applay a new material instance only at the correct subentity
			for(unsigned int i=0;i<_entity->getNumSubEntities();++i)
			{
				if(_entity->getSubEntity(i)->getMaterialName()==name)
					this->changeSubEntityMaterial(_entity->getSubEntity(i),clonedMaterialName);
			}
		}else
			materialPtr = Ogre::MaterialManager::getSingleton().getByName(clonedMaterialName);

		//allow to show any entity behind this entity
		if(tranparency < 1.0f)
		{
			materialPtr->getTechnique(tecnique)->getPass(pass)->setDepthWriteEnabled (false);					
		}
		else
			materialPtr->getTechnique(tecnique)->getPass(pass)->setDepthWriteEnabled (true);

		materialPtr->getTechnique(tecnique)->getPass(pass)->setSceneBlending (Ogre::SceneBlendType::SBT_TRANSPARENT_ALPHA);	
		Ogre::TextureUnitState* ptus = materialPtr->getTechnique(tecnique)->getPass(pass)->getTextureUnitState(unitState); //2nd pass, first texture unit
		ptus->setAlphaOperation(Ogre::LBX_MODULATE, Ogre::LBS_MANUAL, Ogre::LBS_TEXTURE, tranparency);

		
	}

	void CMeshEntity::setMaterialTransparency(std::vector<std::string> name,int tecnique,int pass,int unitState,float tranparency)
	{
		Ogre::MaterialPtr materialPtr;
		//build an unique material name for the instance

		for(int j = 0; j < (int) name.size(); ++j)
		{

			std::string clonedMaterialName=_name+"_"+name[j];
			//if i have already modify the transparency i had already create the new material instance
			if(!Ogre::MaterialManager::getSingleton().resourceExists(clonedMaterialName))
			{
				//create a new material instance that affect only this entity
				materialPtr = Ogre::MaterialManager::getSingleton().getByName(name[j])->clone(clonedMaterialName);
				//applay a new material instance only at the correct subentity
				for(unsigned int i=0;i<_entity->getNumSubEntities();++i)
				{
					if(_entity->getSubEntity(i)->getMaterialName()==name[j])
						this->changeSubEntityMaterial(_entity->getSubEntity(i),clonedMaterialName);
				}
			}else
				materialPtr = Ogre::MaterialManager::getSingleton().getByName(clonedMaterialName);

			//allow to show any entity behind this entity
			if(tranparency < 1.0f)
			{
				materialPtr->getTechnique(tecnique)->getPass(pass)->setDepthWriteEnabled (false);					
			}
			else
				materialPtr->getTechnique(tecnique)->getPass(pass)->setDepthWriteEnabled (true);

			materialPtr->getTechnique(tecnique)->getPass(pass)->setSceneBlending (Ogre::SceneBlendType::SBT_TRANSPARENT_ALPHA);	
			Ogre::TextureUnitState* ptus = materialPtr->getTechnique(tecnique)->getPass(pass)->getTextureUnitState(unitState); //2nd pass, first texture unit
			ptus->setAlphaOperation(Ogre::LBX_MODULATE, Ogre::LBS_MANUAL, Ogre::LBS_TEXTURE, tranparency);
		}
		
	}

	void CMeshEntity::changeEntityMaterial(std::string name)
	{
		Ogre::MaterialPtr mat= Ogre::MaterialManager::getSingletonPtr()->getByName(name);
		_entity->setMaterial(mat);
	}

	void CMeshEntity::changeSubEntityMaterial(Ogre::SubEntity *sub, std::string name)
	{
		sub->setMaterialName(name);
	}

	void CMeshEntity::projectTexture(std::string destinationMaterialName, int technique, Graphics::CFrustumEntity * frustum, std::string materialToProject, std::string sourceEntityName)
	{
		// get the material ptr
		Ogre::MaterialPtr mat = Ogre::MaterialManager::getSingleton().getByName(destinationMaterialName);
		Ogre::MaterialPtr mat2;
		std::string clonedMaterialName= materialToProject +"_"+ sourceEntityName;

		if(!Ogre::MaterialManager::getSingleton().resourceExists(clonedMaterialName))
		{
			//create a new material instance that affect only this entity
			mat2 = Ogre::MaterialManager::getSingleton().getByName(materialToProject)->clone(clonedMaterialName);
			//applay a new material instance only at the correct subentity

		}else
			mat2 = Ogre::MaterialManager::getSingleton().getByName(clonedMaterialName);


		// create a new pass in the material to render the decal
		Ogre::Pass* pass = mat->getTechnique(technique)->createPass();	
		Ogre::Pass* pass2 = mat2->getTechnique(technique)->getPass(technique); 
		
		//pass->setSceneBlendingOperation(pass2->getSceneBlendingOperation());		

		Ogre::Pass::TextureUnitStateIterator textIt = pass2->getTextureUnitStateIterator();

		Ogre::TextureUnitState* texState;
		for(int i = 1; i <= mat2->getTechnique(technique)->getPass(technique)->getNumTextureUnitStates(); i++)
		{
			texState = new Ogre::TextureUnitState(*textIt.getNext());
			texState->_notifyParent(pass);					
			pass->addTextureUnitState(texState);
		}

		

		texState->setProjectiveTexturing(true, frustum->getFrustum());
		texState->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP);
		texState->setTextureFiltering(Ogre::FO_POINT, Ogre::FO_LINEAR, Ogre::FO_NONE);		
		// set up the decal's texture unit
		//Ogre::TextureUnitState *texState = pass->createTextureUnitState(texture);
		//

		//texState->setProjectiveTexturing(true, frustum->getFrustum());
		//texState->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP);
		//texState->setTextureFiltering(Ogre::FO_POINT, Ogre::FO_LINEAR, Ogre::FO_NONE);

		// set our pass to blend the decal over the model's regular texture
		pass->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);		
		pass->setDepthWriteEnabled(false);

		pass->setDepthBias(1);

		// set the decal to be self illuminated instead of lit by scene lighting
		pass->setLightingEnabled(false);

	}

	std::vector<Vector3> CMeshEntity::getMeshCorners()
	{
		std::vector<Vector3> corners;
		const Ogre::Vector3 position = _entityNode->getPosition();
		Ogre::AxisAlignedBox b = _entity->getBoundingBox();

		corners.push_back(Vector3(position.x +b.getMaximum().x, position.y, position.z + b.getMaximum().z));
		corners.push_back(Vector3(position.x + b.getMaximum().x, position.y, position.z + b.getMinimum().z));
		corners.push_back(Vector3(position.x + b.getMinimum().x, position.y, position.z + b.getMinimum().z));
		corners.push_back(Vector3(position.x + b.getMinimum().x, position.y, position.z + b.getMaximum().z));
		return corners;
	}

	void CMeshEntity::addTexture(std::string textureName)
	{
		for(unsigned int i=0;i<_entity->getNumSubEntities();++i)
		{
			Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName(_entity->getSubEntity(i)->getMaterialName());
			Ogre::MaterialPtr clonedMaterial = material->clone(material->getName() + _mesh + std::to_string(i));
			clonedMaterial->getTechnique(0)->createPass()->createTextureUnitState()->setTextureName(textureName);
			this->changeSubEntityMaterial(_entity->getSubEntity(i),clonedMaterial->getName());
		}
	}

	void CMeshEntity::removePass(int passIndex)
	{
		for(unsigned int i=0;i<_entity->getNumSubEntities();++i)
		{
			Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName(_entity->getSubEntity(i)->getMaterialName());
			Ogre::MaterialPtr clonedMaterial = material->clone(material->getName() + _mesh + std::to_string(i));
			clonedMaterial->getTechnique(0)->removePass((passIndex == -1) ? clonedMaterial->getTechnique(0)->getNumPasses() - 1 : passIndex);
			this->changeSubEntityMaterial(_entity->getSubEntity(i),clonedMaterial->getName());
		}
	}

	void CMeshEntity::attachObjectToBone(std::string boneName,CMeshEntity* obj,const Quaternion &offsetOrientation,const Vector3 &offsetPosition)
	{
		Ogre::TagPoint* bone=_entity->attachObjectToBone(boneName,obj->getEntity(),offsetOrientation,offsetPosition);
		obj->setAttachedToBone(true);
		bone->setInheritScale(false);
		bone->setInheritParentEntityScale(false);
	}

	void CMeshEntity::detachObjectFromBone(CMeshEntity* obj)
	{
		if(_entity!=nullptr)
			_entity->detachObjectFromBone(obj->getEntity());
		obj->setAttachedToBone(false);
	}	
	
	
	void CMeshEntity::registerSubEntitiesOnFogOfWarController()
	{
		for(unsigned int i=0;i<_entity->getNumSubEntities();++i)
			Graphics::CServer::getSingletonPtr()->getFogOfWarController()->addFogOfWarSubEntity(_entity->getSubEntity(i));
	}

	void CMeshEntity::unregisterSubEntitiesOnFogOfWarController()
	{
		if (_entity == nullptr) 
			return;
		for(unsigned int i=0;i<_entity->getNumSubEntities();++i)
			Graphics::CServer::getSingletonPtr()->getFogOfWarController()->removeFogOfWarSubEntity(_entity->getSubEntity(i));
	}

	std::list<Ogre::SubEntity*> CMeshEntity::getSubEntities()
	{
		std::list<Ogre::SubEntity*> subEntities;
		for (unsigned int i = 0; i < _entity->getNumSubEntities(); ++i)
			subEntities.push_back(_entity->getSubEntity(i));
		return subEntities;
	}
} // namespace Graphics
