/**
@file PhysicControllerManager.cpp

Contains the implementation of the manager for physic controllers. 

@see Physics::CServer

@author Rayco Sánchez García
@date February, 2015
*/

#include "PhysicControllerManager.h"
#include "SceneManager.h"
#include "PhysXServer.h"
#include "Conversions.h"
#include "Physics\Server.h"
#include "CollisionManager.h"

#include <assert.h>

#include <PxPhysicsAPI.h>

#include <extensions\PxExtensionsAPI.h> 

using namespace Physics;
using namespace physx;

PhysicControllerManager::PhysicControllerManager(PhysXServer* physXServer, SceneManager* sceneManager) : physXServer(physXServer), sceneManager(sceneManager)
{
	// Crear PxControllerManager. Es necesario para crear character controllers
	controllerManager = PxCreateControllerManager(*sceneManager->getScene());
}

//--------------------------------------------------------

PhysicControllerManager::~PhysicControllerManager()
{
	if (controllerManager) {
		controllerManager->release();
		controllerManager = NULL;
	}
}

//--------------------------------------------------------

PxCapsuleController* PhysicControllerManager::createCapsuleController(const Vector3 &position, float radius, 
	                                                  float height,int group, const Logic::CPhysicController *component)
{
	// Nota: PhysX coloca el sistema de coordenadas local en el centro de la cápsula, mientras
	// que la lógica asume que el origen del sistema de coordenadas está en los piés del 
	// jugador. Para unificar necesitamos realizar una traslación en el eje Y.
	// Desafortunadamente, el descriptor que se usa para crear los controllers no permite
	// definir esta transformación local (que sí permite al crear un actor), por lo que
	// tendremos que realizar la traslación nosotros cada vez. 

	// Transformación entre el sistema de coordenadas lógico y el de PhysX
	float offsetY = height / 2.0f + radius;
	PxVec3 pos = Vector3ToPxVec3(position + Vector3(0, offsetY, 0));
	

	_collisionManager = physXServer->_collisionManager;
	// Crear descriptor del controller
	PxCapsuleControllerDesc desc;
	desc.position = PxExtendedVec3(pos.x, pos.y, pos.z);
	desc.height = height;
	desc.radius = radius;
	desc.material = physXServer->_defaultMaterial;
	//desc.climbingMode = PxCapsuleClimbingMode::eEASY; 
	desc.climbingMode = PxCapsuleClimbingMode::eCONSTRAINED;
	desc.stepOffset = 0.0f;
	desc.slopeLimit = 0.5f;
	desc.reportCallback = _collisionManager;   // Establecer gestor de colisiones
	desc.userData = (void *) component;  // Anotar el componente lógico asociado al controller
	
	PxCapsuleController *controller = (PxCapsuleController *) controllerManager->createController(desc);
	
	// Anotar el componente lógico asociado al actor dentro del controller (No es automático)
	controller->getActor()->userData = (void *) component;
	//PxSetGroup(*controller->getActor(),group);

	CServer::getSingletonPtr()->setupFiltering(controller->getActor(),CollisionGroup(group));

	//PARTE IMPORTANTE TANTO PARA QUE FUNCIONE EL TRIGGER, COMO LOS GRUPOS DE COLISION
	

	PxFilterData data;
	PxShape *shape;
	//obtengo la malla fisica del actor
 	controller->getActor()->getShapes(&shape, 1, 0);
	//Obtengo el filterData del actor
	data = shape->getSimulationFilterData();

	//Quito el trigger, porque sino el controller hace cosas raras y choca contra el. 
	CServer::getSingletonPtr()->UpdateGroup(data.word1, CollisionGroup::eTrigger, false);

	//Igualo a word0 al nuevo FilterData del capsuleController a los grupos de colision del player
	(_data).word0 = data.word1;

	return controller;
}

//--------------------------------------------------------

unsigned PhysicControllerManager::moveController(PxController *controller, const Vector3 &movement, unsigned int msecs)
{
	PxVec3 disp = Vector3ToPxVec3(movement);
	float minDist = 0.01f;
	float elapsedTime = msecs / 1000.0f;	

	PxControllerFilters filters(&_data, NULL, _collisionManager);

	return controller->move(disp, minDist, elapsedTime, filters, NULL);
}

//--------------------------------------------------------

Vector3 PhysicControllerManager::getControllerPosition(const PxCapsuleController *controller)
{
	// Antes de devolver la posición del controller debemos transformar entre el 
	// sistema de coordenadas de PhysX y el de la lógica
	float offsetY = controller->getHeight() / 2.0f + controller->getRadius();
	Vector3 pos = PxExtendedVec3ToVector3(controller->getPosition());
	return pos - Vector3(0, offsetY, 0);
}

//--------------------------------------------------------

void PhysicControllerManager::setControllerPosition(PxCapsuleController *controller, const Vector3 &position)
{
	float offsetY = controller->getHeight() / 2.0f + controller->getRadius();
	PxExtendedVec3 pos = Vector3ToPxExtendedVec3(position+Vector3(0,offsetY,0));
	controller->setPosition(pos);
}