/**
@file SceneManager.cpp

Contains the implementation of the PhysX scene manager. 

@see Physics::CServer

@author Rayco Sánchez García
@date February, 2015
*/

#include "SceneManager.h"
#include "PhysXServer.h"
#include "CollisionManager.h"

#include <assert.h>

#include <PxPhysicsAPI.h>
#include <extensions\PxExtensionsAPI.h> 

using namespace Physics;
using namespace physx;


SceneManager::SceneManager(PhysXServer* physXServer) : physXServer(physXServer)
{
#ifdef PX_WINDOWS
	createCudaContextManager();
#endif

	createScene();

#ifdef _DEBUG
	setSceneVisualizationParameters();
#endif
}

//--------------------------------------------------------

void SceneManager::createScene ()
{
	PxSceneDesc sceneDesc(physXServer->_physics->getTolerancesScale());
	setSceneDescParameters(&sceneDesc);

	scene = physXServer->_physics->createScene(sceneDesc);

	assert(scene && "Error en PxPhysics::createScene");

	// Activamos la notificación de eventos entre entidades kinemáticas.
	//scene->setFlag(PxSceneFlag::eENABLE_KINEMATIC_PAIRS, true);
	//scene->setFlag(PxSceneFlag::eENABLE_KINEMATIC_STATIC_PAIRS, true);
	//scene->setFlag(PxSceneFlag::eENABLE_CCD, true);

}

//--------------------------------------------------------

void SceneManager::setSceneDescParameters(PxSceneDesc *sceneDesc)
{
	// Establecer la gravedad en el eje Y
	sceneDesc->gravity = PxVec3(0.0f, -9.81f, 0.0f);

	// Establecer el gestor de colisiones
	sceneDesc->simulationEventCallback = physXServer->_collisionManager;

	// Establecer un gestor de tareas por CPU
	if (!sceneDesc->cpuDispatcher) {
		int executionThreads = 1; // hilos de ejecución
		cpuDispatcher  = PxDefaultCpuDispatcherCreate(executionThreads);

		assert (cpuDispatcher && "Error en PxDefaultCpuDispatcherCreate");

		sceneDesc->cpuDispatcher = cpuDispatcher;
	}

	// Establecer el shader que controla las colisiones entre entidades.
	// Usamos un shader que emula la gestión de grupos de PhysX 2.8
	if (!sceneDesc->filterShader)
		sceneDesc->filterShader = physXServer->_collisionManager->filterShader;

	// Intentar establecer un gestor de tareas por GPU 
#ifdef PX_WINDOWS
	if (!sceneDesc->gpuDispatcher && cudaContextManager)
		sceneDesc->gpuDispatcher = cudaContextManager->getGpuDispatcher();
#endif
}

//--------------------------------------------------------

void SceneManager::setSceneVisualizationParameters()
{
	scene->setVisualizationParameter(PxVisualizationParameter::eSCALE, 2.0f);
	scene->setVisualizationParameter(PxVisualizationParameter::eACTOR_AXES, 2.0f);
	scene->setVisualizationParameter(PxVisualizationParameter::eCOLLISION_STATIC, 2.0f);
	scene->setVisualizationParameter(PxVisualizationParameter::eCOLLISION_SHAPES, 2.0f);
	scene->setVisualizationParameter(PxVisualizationParameter::eCOLLISION_DYNAMIC, 2.0f);
	scene->setVisualizationParameter(PxVisualizationParameter::eCOLLISION_EDGES, 2.0f);
	scene->setVisualizationParameter(PxVisualizationParameter::eCOLLISION_AABBS, 1.0f);
	scene->setVisualizationParameter(PxVisualizationParameter::eCOLLISION_COMPOUNDS, 2.0f);
}

//--------------------------------------------------------

SceneManager::~SceneManager()
{
	destroyScene();
	if (cudaContextManager)
	{
		cudaContextManager->release();
		cudaContextManager = nullptr;
	}
}

//--------------------------------------------------------

void SceneManager::destroyScene ()
{
	if (scene) {
		scene->release();
		scene = NULL;
	}

	if (cpuDispatcher) {
		cpuDispatcher->release();
		cpuDispatcher = NULL;
	}
}

//-------------------------------------------------------

bool SceneManager::simulatePhysic(unsigned int msecs)
{
	if(msecs == 0)
		return true;

	scene->simulate(msecs / 1000.0f);
	
	return scene->fetchResults(true);
}

//-------------------------------------------------------

void SceneManager::createCudaContextManager()
{
	PxCudaContextManagerDesc cudaContextManagerDesc;
	cudaContextManager = PxCreateCudaContextManager(*physXServer->_foundation, cudaContextManagerDesc, physXServer->_profileZoneManager);
	if( cudaContextManager && ! cudaContextManager->contextIsValid())
	{		
		cudaContextManager->release();
		cudaContextManager = nullptr;
	}
}

//-------------------------------------------------------

void SceneManager::removeFromScene(physx::PxActor *actor)
{
	scene->removeActor(*actor);
}

//-------------------------------------------------------

void SceneManager::addToScene(physx::PxActor *actor)
{
	scene->addActor(*actor);
}