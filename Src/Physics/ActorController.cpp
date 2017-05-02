/**
@file ActorController.cpp

Contains the implementation of the ActorController. 

@author Rayco Sánchez García
@date February, 2015
*/

#include "ActorController.h"
#include "Conversions.h"

#include <assert.h>

#include <PxPhysicsAPI.h>
#include <extensions\PxExtensionsAPI.h> 

using namespace Physics;
using namespace physx;


ActorController::ActorController() {}

//--------------------------------------------------------

ActorController::~ActorController() {}

//--------------------------------------------------------

Matrix4 ActorController::getActorTransform(const PxRigidActor *actor)
{
	assert(actor);

	// Devolver la posición y orientación en coordenadas lógicas
	return PxTransformToMatrix4(actor->getGlobalPose());
}

//--------------------------------------------------------

void ActorController::moveKinematicActor(physx::PxRigidDynamic *actor, const Matrix4 &transform)
{
	assert(actor);
	assert(isKinematic(actor));

	// Mover el actor tras transformar el destino a coordenadas lógicas
	actor->setKinematicTarget(Matrix4ToPxTransform(transform));
}

//--------------------------------------------------------

void ActorController::moveKinematicActor(physx::PxRigidDynamic *actor, Vector3 displ, unsigned int msecs)
{
	assert(actor);
	assert(isKinematic(actor));

	// Desplazar el actor	
	displ.normalise();
	
	displ *= (Ogre::Real) msecs/1000 * msecs;

	PxTransform transform = actor->getGlobalPose();
	transform.p += Vector3ToPxVec3(displ);
	actor->setKinematicTarget(transform);
}

//--------------------------------------------------------

void ActorController::setGlopalPosition(physx::PxRigidDynamic *actor, const Vector3 &newPosition)
{
	assert(actor);

	// Desplazar el actor
	PxTransform transform = actor->getGlobalPose();
	transform.p = Vector3ToPxVec3(newPosition);
	actor->setGlobalPose(transform);
}

//--------------------------------------------------------

void ActorController::setGlobalPose(physx::PxRigidDynamic *actor, const Matrix4 &matrixTransform)
{
	assert(actor);

	// Desplazar el actor
	PxTransform transform = Matrix4ToPxTransform(matrixTransform);	
	actor->setGlobalPose(transform);
	
}
//--------------------------------------------------------

bool ActorController::isKinematic(const PxRigidDynamic *actor)
{
	assert(actor);

	return actor->getRigidDynamicFlags() & PxRigidDynamicFlag::eKINEMATIC;
}