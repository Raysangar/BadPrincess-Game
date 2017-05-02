//---------------------------------------------------------------------------
// StencilGlow.h
//---------------------------------------------------------------------------

#ifndef __Graphics_StencilGlow_H
#define __Graphics_StencilGlow_H

#include <OgreRenderQueueListener.h> 
#include "BaseSubsystems\Math.h"



namespace Graphics 
{
	class CScene;
}

namespace Ogre 
{
	class Entity;
	class SceneNode;
}

namespace Graphics 
{

	//// a Render queue listener to change the stencil mode
	//class StencilOpQueueListener : public Ogre::RenderQueueListener 
	//{ 
	//	public: 
	//		virtual void renderQueueStarted(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& skipThisInvocation);
	//
	//		virtual void renderQueueEnded(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& repeatThisInvocation); 

	//}; 

	class CStencilGlow 
	{
	public:

		/**
		Constructor de la clase.

		@param sceneMgr Gestor de la escena de Ogre a la que pertenece.
		*/
		CStencilGlow(CScene *scene);
		
		/**
		Destructor de la aplicación.
		*/
		virtual ~CStencilGlow();

		void createGlow(Ogre::Entity *entity, Ogre::Entity *glowEntity);
		void activateGlow(Ogre::Entity *entity, bool isEnemy); 
		void deactivateGlow(Ogre::Entity *entity, bool isEnemy); 
				
	protected:

		/**
		Controla todos los elementos de una escena. Su equivalente
		en la lógica del juego sería el mapa o nivel. 
		*/
		CScene *_scene;

		//StencilOpQueueListener * mStencilOpFrameListener;

	}; // class StencilGlow

} // namespace Graphics

#endif // __Graphics_StencilGlow_H
