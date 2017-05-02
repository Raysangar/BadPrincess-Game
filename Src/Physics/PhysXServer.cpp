/**
@file PhysXServer.cpp

Contains the implementation of the server that encapsulates PhysX SDK. 

@author Rayco Sánchez García
@date February, 2015
*/

#include "PhysXServer.h"
#include "CollisionManager.h"
#include "ErrorManager.h"

#include <assert.h>

#include <PxPhysicsAPI.h>
#include <extensions\PxExtensionsAPI.h> 

using namespace Physics;
using namespace physx;


PhysXServer::PhysXServer()
{
	// Crear gestor de colisiones
	_collisionManager = new CCollisionManager();

	createPxFoundation();

	PxTolerancesScale toleranceScale;
	createPxPhysics(toleranceScale);

	initPxCookingModule(toleranceScale);

	// Inicializar extensiones
	if (!PxInitExtensions(*_physics))
		assert(false && "PxInitExtensions failed!");

	createPhysicDefaultMaterial();
}

//--------------------------------------------------------

void PhysXServer::createPhysicDefaultMaterial()
{
	float staticFriction = 0.5f;
	float dynamicFriction = 0.5f;
	float restitution = 0.1f;
	_defaultMaterial = _physics->createMaterial(staticFriction, dynamicFriction, restitution);
}

//--------------------------------------------------------

void PhysXServer::createPxFoundation()
{
	// Crear gestor de errores
	_errorManager = new CErrorManager();

	// Crear gestor de memoria
	_allocator = new PxDefaultAllocator();

	// Crear PxFoundation. Es necesario para instanciar el resto de objetos de PhysX
	_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, *_allocator, *_errorManager);
	assert(_foundation && "Error en PxCreateFoundation");
}

//--------------------------------------------------------

void PhysXServer::createPxPhysics(const PxTolerancesScale &toleranceScale)
{
	// Crear PxProfileZoneManager. Es necesario para habitiar algunas opciones de 
	// profiling de rendimiento en el PhysX Visual Debugger 
	_profileZoneManager = &PxProfileZoneManager::createProfileZoneManager(_foundation);
	assert(_profileZoneManager && "Error en PxProfileZoneManager::createProfileZoneManage");

	// Crear PxPhysics. Es el punto de entrada al SDK de PhysX
	bool recordMemoryAllocations = true;
	_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *_foundation, toleranceScale, 
		                       recordMemoryAllocations, _profileZoneManager);
	assert(_physics && "Error en PxCreatePhysics");
}

//--------------------------------------------------------

void PhysXServer::initPxCookingModule(const PxTolerancesScale &toleranceScale)
{
	PxCookingParams params(toleranceScale);
	_cooking = PxCreateCooking(PX_PHYSICS_VERSION, *_foundation, params);
}

//--------------------------------------------------------

PhysXServer::~PhysXServer()
{
	if (_defaultMaterial) {
		_defaultMaterial->release();
		_defaultMaterial = NULL;
	}

	PxCloseExtensions();

	if (_cooking) {
		_cooking->release();
		_cooking = NULL;
	}

	if (_physics) {
		_physics->release();
		_physics = NULL;
	}

	if (_profileZoneManager) { 
		_profileZoneManager->release();
		_profileZoneManager = NULL;
	}

	if (_foundation) {
		_foundation->release();
		_foundation = NULL;
	}

	if (_allocator) {
		delete _allocator;
		_allocator = NULL;
	}

	if (_errorManager) {
		delete _errorManager;
		_errorManager = NULL;
	}
	
	if (_collisionManager) {
		delete _collisionManager;
		_collisionManager = NULL;
	}
}