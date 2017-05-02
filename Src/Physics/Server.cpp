/**
@file Server.cpp

Contiene la implementación del servidor de física. 

@see Physics::CServer

@author Antonio Sánchez Ruiz-Granados
@date Noviembre, 2012
*/

#include "Server.h"
#include "PhysXServer.h"
#include "SceneManager.h"
#include "PhysicControllerManager.h"
#include "ActorController.h"
#include "PhysicPerceptionManager.h"
#include "ActorFactory.h"
#include "Conversions.h"
#include "PhysXVisualDebuggerConnector.h"
#include "Logic/Entity/Components/Physics.h"

#include "DebugDraw.h"
#include "Graphics\Scene.h"
#include "Logic/Server.h"
#include "Logic/Maps/Map.h"

#include <assert.h>
#include <algorithm>
#include <iterator>

#include <PxPhysicsAPI.h>
#include <extensions\PxExtensionsAPI.h> 

using namespace Physics;
//using namespace Logic;
using namespace physx;


// Única instancia del servidor
CServer *CServer::_instance = NULL;

//--------------------------------------------------------

CServer::CServer()
{
	physXServer = new PhysXServer();

	sceneManager = new SceneManager(physXServer);

	physicControllerManager = new PhysicControllerManager(physXServer, sceneManager);

	actorController = new ActorController();

	actorFactory = new ActorFactory(physXServer);

	physicPerceptionManager = new PhysicPerceptionManager(sceneManager, physXServer);

	InicializarGruposColision();

#ifdef _DEBUG
	PhysXVisualDebuggerConnector::connectToPhysXVisualDebugger(physXServer->_physics);
#endif
} 

//--------------------------------------------------------

CServer::~CServer() 
{
	// Destruir objetos en orden inverso a como fueron creados
	if (physicPerceptionManager)
	{
		delete physicPerceptionManager;
		physicPerceptionManager = nullptr;
	}

	if (actorFactory)
	{
		delete actorFactory;
		actorFactory = nullptr;
	}
	if (actorController)
	{
		delete actorController;
		actorController = nullptr;
	}

	if (physicControllerManager)
	{
		delete physicControllerManager;
		physicControllerManager = nullptr;
	}

	if (sceneManager)
	{
		delete sceneManager;
		sceneManager = nullptr;
	}

	if (physXServer)
	{
		delete physXServer;
		physXServer = nullptr;
	}

#ifdef _DEBUG
	PhysXVisualDebuggerConnector::releaseConnection();
#endif
} 

//--------------------------------------------------------

bool CServer::Init() 
{
	if (!_instance) {
		_instance = new CServer();
	}

	return true;
} 

//--------------------------------------------------------

void CServer::Release()
{
	if(_instance) {
		delete _instance;
		_instance = NULL;
	}
} 

//--------------------------------------------------------

bool CServer::tick(unsigned int msecs) 
{
	// Empezar la simulación física. Actualmente usamos intervalos de tiempo variables,
	// debemos tener cuidado porque PhysX puede no comportarse bien si el tiempo 
	// transcurrido es demasiado grande.
	return sceneManager->simulatePhysic(msecs);
} 

//--------------------------------------------------------

void CServer::setGroupCollisions(int group1, int group2, bool enable)
{
	// Activar / desactivar colisiones entre grupos
	PxSetGroupCollisionFlag(group1, group2, enable);
}

//--------------------------------------------------------


void CServer::InicializarGruposColision()
{

	int nGroups = CollisionGroup::eUntargetable;

	unsigned long int mask = 0;

	for(int j = 0; j <= nGroups; ++j)
	{
		mask |= (1<<j);
	}
	FilterMask fMask = FilterMask(mask);

	for (int i = CollisionGroup::eWorldGround + 1; i <= nGroups ; ++i)
	{
		if(i != 12)
		{
			CollisionGroup g = CollisionGroup(i);
			_groupCollisionMap.insert(std::pair<CollisionGroup, FilterMask>(g, fMask));
		}
	}
	_groupCollisionMap.insert(std::pair<CollisionGroup, FilterMask>(CollisionGroup::eWorldGround, FilterMask(0)));
	_groupCollisionMap.insert(std::pair<CollisionGroup, FilterMask>(CollisionGroup::eWorld, FilterMask(0)));
	_groupCollisionMap.insert(std::pair<CollisionGroup, FilterMask>(CollisionGroup::eRemovableWorld, FilterMask(0)));
	
	mask = FilterMask::eEnemyMask | FilterMask::ePlayerMask | FilterMask::eUntargetableMask;
	_groupCollisionMap.insert(std::pair<CollisionGroup, FilterMask>(CollisionGroup::eTrigger, FilterMask(mask)));	
	
}//InicializarGruposColision

//---------------------------------------------

void CServer::setupFiltering(physx::PxRigidActor* actor, CollisionGroup collisionGroup)
{

		//Guardamos en filterGròup el grupo al que pertenece el actor
		PxU32 filterGroup = (1<<collisionGroup);

		//Obtenemos la mascara de colision para este grupo

		PxU32 filterMask = _groupCollisionMap[collisionGroup];


        PxFilterData filterData;
        filterData.word0 = filterGroup; //word0 = grupo del actor
        filterData.word1 = filterMask;  //word1 = grupos con el que puede chocar el actor

		//Obtenemos la malla fisica
        PxShape* shapes;
        actor->getShapes(&shapes, 1, 0);

		//Y asignamos los filtros de colision a la malla fisica del actor, de manera que sepa con quien tiene que simular la colision
        shapes->setSimulationFilterData(filterData);
		shapes->setQueryFilterData(filterData);

}//setupFiltering

//---------------------------------------------

void CServer::UpdateGroup(unsigned int &filterMask, CollisionGroup group, bool add)
{
	if(add)
	{
		filterMask |= (1<<group);
	}
	else
	{
		unsigned int mask = (1<<group);
		filterMask &= ~mask;
	}
}//UpdateGroup
