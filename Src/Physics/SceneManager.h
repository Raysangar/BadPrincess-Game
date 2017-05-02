/**
@file SceneManager.h

Contains the declaration of the PhysX scene manager. 

@author Rayco Sánchez García
@date February, 2015
*/

#ifndef __Physics_SceneManager_H
#define __Physics_SceneManager_H

namespace Physics
{
	class PhysXServer;
};

namespace physx {
	class PxScene;
	class PxCudaContextManager;
	class PxDefaultCpuDispatcher;
	class PxSceneDesc;
	class PxActor;
};


namespace Physics
{
	class SceneManager
	{
	public:
		SceneManager(PhysXServer* physXServer);

		~SceneManager();

		/**
		@author Rayco Sánchez García
		@date February, 2015

		@param msecs Time since last simulation in milliseconds.
		@return True if simulation finished correctly.
		*/
		bool simulatePhysic(unsigned int msecs);

		/**
		Removes the actor from scene but don't release it

		@param actor The actor to be removed.
		*/
		void removeFromScene(physx::PxActor *actor);

		/**
		Adds the actor to the scene

		@param actor The actor to be added.
		*/
		void addToScene(physx::PxActor *actor);

		/**
		return The current physic scene
		*/
		physx::PxScene* getScene() { return scene; }

	private:
		// Physic Scene
		physx::PxScene *scene;

		/**
		@author Rayco Sánchez García
		@date February, 2015
		*/
		void createScene();

		/**
		@author Rayco Sánchez García
		@date February, 2015
		*/
		void destroyScene();

		/**
		@author Rayco Sánchez García
		@date February, 2015

		Function which creates a CudaContextManager for using 
		GPU on physic simulation. Only available on windows.
		*/
		void createCudaContextManager();

		/**
		@author Rayco Sánchez García
		@date February, 2015

		Set attributes of the scene descriptor such as gravity
		and cpuDispatcher.

		@param sceneDesc The scene descriptor to be initialized
		*/
		void setSceneDescParameters(physx::PxSceneDesc *sceneDesc);

		/**
		@author Rayco Sánchez García
		@date February, 2015ç

		Function that sets debug visualization parameters. 
		*/
		void setSceneVisualizationParameters();

		//PhysX SDK server
		PhysXServer* physXServer;

		/**
		Object that encapsules information for using
		GPU on physic simulation. Only on Windows.
		*/
		physx::PxCudaContextManager *cudaContextManager;

		// PhysX CPU task manager
		physx::PxDefaultCpuDispatcher *cpuDispatcher;

	};
} //namespace Physics

#endif // __Physics_SceneManager_H