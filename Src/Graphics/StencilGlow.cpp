//---------------------------------------------------------------------------
// StencilGlow.cpp
//---------------------------------------------------------------------------

#include "StencilGlow.h"
#include "Scene.h"

#include <OgreSceneNode.h>
#include <OgreRoot.h>
#include <OgreSceneManager.h>
#include <OgreEntity.h>
#include <OgreSubEntity.h>
#include <OgreSubMesh.h>

// render queues
#define RENDER_QUEUE_MAIN 60
#define RENDER_QUEUE_OUTLINE_GLOW_OBJECTS	RENDER_QUEUE_MAIN + 1
#define RENDER_QUEUE_OUTLINE_GLOW_GLOWS		RENDER_QUEUE_MAIN + 2
#define RENDER_QUEUE_FULL_GLOW_ALPHA_GLOW	RENDER_QUEUE_MAIN + 3
#define RENDER_QUEUE_FULL_GLOW_GLOW			RENDER_QUEUE_MAIN + 4
#define LAST_STENCIL_OP_RENDER_QUEUE		RENDER_QUEUE_FULL_GLOW_GLOW

// stencil values
#define STENCIL_VALUE_FOR_OUTLINE_GLOW 1
#define STENCIL_VALUE_FOR_FULL_GLOW 2
#define STENCIL_FULL_MASK 0xFFFFFFFF

namespace Graphics 
{
	//--------------------------------------------------------

	//void StencilOpQueueListener::renderQueueStarted(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& skipThisInvocation)
	//{	
 //
	//		Ogre::RenderSystem * rendersys = Ogre::Root::getSingleton().getRenderSystem(); 

	//		rendersys->clearFrameBuffer(Ogre::FBT_STENCIL); 
	//		rendersys->setStencilCheckEnabled(true); 
	//		rendersys->setStencilBufferParams(Ogre::CMPF_ALWAYS_PASS,0,
	//			STENCIL_VALUE_FOR_OUTLINE_GLOW, STENCIL_FULL_MASK, 
	//			Ogre::SOP_KEEP,Ogre::SOP_KEEP,Ogre::SOP_REPLACE,false);       

	//}

	////--------------------------------------------------------

	//void StencilOpQueueListener::renderQueueEnded(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& repeatThisInvocation) 
	//{ 

	//		Ogre::RenderSystem * rendersys = Ogre::Root::getSingleton().getRenderSystem(); 
	//		rendersys->setStencilCheckEnabled(false); 
	//		rendersys->setStencilBufferParams(); 
	//	
	//} 

	//--------------------------------------------------------

	CStencilGlow::CStencilGlow(CScene *scene)
	{	
		_scene = scene;
		//mStencilOpFrameListener = new StencilOpQueueListener();
		//_scene->getSceneMgr()->addRenderQueueListener(mStencilOpFrameListener);

	} // CCamera

	//--------------------------------------------------------

	CStencilGlow::~CStencilGlow() 
	{
		//delete mStencilOpFrameListener;
		_scene = NULL;

	} // ~CCamera

	//--------------------------------------------------------

	void CStencilGlow::createGlow(Ogre::Entity *entity, Ogre::Entity *glowEntity)
	{
		// outline glow entity		
		entity->setRenderQueueGroup(RENDER_QUEUE_OUTLINE_GLOW_OBJECTS);		
		glowEntity->setRenderQueueGroup(RENDER_QUEUE_OUTLINE_GLOW_GLOWS); 
		glowEntity->setMaterialName("cg/glow"); 
	}

	//--------------------------------------------------------		

	void CStencilGlow::activateGlow(Ogre::Entity *entity, bool isEnemy)
	{	 
		const Ogre::String file_name = isEnemy? "rim_red.dds" : "rim_green.dds";
		const Ogre::String rim_material_name = "_rim_";
 
		unsigned short count = entity->getNumSubEntities();

		for (unsigned short i = 0; i < count; ++i)
		{
			Ogre::SubEntity* subentity = entity->getSubEntity (i);
 
			const Ogre::String& old_material_name = subentity->getMaterialName();
			Ogre::String new_material_name = old_material_name + rim_material_name + entity->getName();
 
			Ogre::MaterialPtr new_material = Ogre::MaterialManager::getSingleton().getByName(new_material_name);
 
			if (new_material.isNull())
			{
				Ogre::MaterialPtr old_material = Ogre::MaterialManager::getSingleton().getByName (old_material_name);
				new_material = old_material->clone (new_material_name);
 
				Ogre::Pass* pass = new_material->getTechnique(0)->getPass(0);
				Ogre::TextureUnitState* texture = pass->createTextureUnitState();
				texture->setCubicTextureName (&file_name, true);
				texture->setTextureAddressingMode (Ogre::TextureUnitState::TAM_CLAMP);
				texture->setColourOperationEx (Ogre::LBX_ADD, Ogre::LBS_TEXTURE, Ogre::LBS_CURRENT);
				texture->setColourOpMultipassFallback (Ogre::SBF_ONE, Ogre::SBF_ONE);
				texture->setEnvironmentMap (true, Ogre::TextureUnitState::ENV_NORMAL);
			}
 
			subentity->setMaterial (new_material);
		}
	}

	//--------------------------------------------------------
	
	void CStencilGlow::deactivateGlow(Ogre::Entity *entity, bool isEnemy)
	{
		 unsigned short count = entity->getNumSubEntities();
 
		for (unsigned short i = 0; i < count; ++i)
		{
			Ogre::SubEntity* subentity = entity->getSubEntity (i);
			Ogre::SubMesh* submesh = subentity->getSubMesh();
 
			const Ogre::String& old_material_name = submesh->getMaterialName();
			const Ogre::String& new_material_name = subentity->getMaterialName();
 
			// if the entity is already using the original material then we're done. 
			if (0 == stricmp (old_material_name.c_str(), new_material_name.c_str()))
				continue;
 
			// otherwise restore the original material name.
			subentity->setMaterialName (old_material_name);
 
		}
	}

	//--------------------------------------------------------
} // namespace Graphics
