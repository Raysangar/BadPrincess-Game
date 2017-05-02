/**
@file PhysXServer.h

Contains the declaration of the server that encapsulates PhysX SDK. 

@author Rayco Sánchez García
@date February, 2015
*/

#ifndef __Physics_PhysXServer_H
#define __Physics_PhysXServer_H

#include "BaseSubsystems/Math.h"

namespace Physics
{
	class CCollisionManager;
};

namespace physx {
	class PxErrorCallback;
	class PxAllocatorCallback;
	class PxFoundation;
	class PxProfileZoneManager;
	class PxCooking;
	class PxPhysics;
	class PxMaterial;
	class PxTolerancesScale;
};


namespace Physics
{

	class PhysXServer
	{
	public:
		PhysXServer();


		/**
		@author Rayco Sánchez García
		@date February, 2015
		
		Default destructor.
		*/
		~PhysXServer();

		// Gestor de errores
		physx::PxErrorCallback *_errorManager;

		// Gestor de memoria
		physx::PxAllocatorCallback *_allocator;

		// Distintos atributos relacionados con PhysX
		physx::PxFoundation *_foundation;
		physx::PxProfileZoneManager *_profileZoneManager;
		physx::PxCooking *_cooking;

		// SDK de PhysX
		physx::PxPhysics *_physics;

		// Material que se aplica por defecto a las nuevas entidades físicas
		physx::PxMaterial *_defaultMaterial;

		// Gestion de colisiones
		CCollisionManager *_collisionManager;
		
	private:
		/**
		@author Rayco Sánchez García
		@date February, 2015

		Function which creates a PxFoundation object that will
		allows to create other PhysX objects.
		*/
		void createPxFoundation();

		/**
		@author Rayco Sánchez García
		@date February, 2015

		Function which creates the object for 
		accessing to PhysX SDK.

		@param toleranceScale  Object that defines the scale at which simulation runs.
		*/
		void createPxPhysics(const physx::PxTolerancesScale &toleranceScale);

		/**
		@author Rayco Sánchez García
		@date February, 2015

		Function which inits the module that cooks mesh and
		deserializes physic actors from RepX files.

		@param toleranceScale  Object that defines the scale at which simulation runs.
		*/
		void initPxCookingModule(const physx::PxTolerancesScale &toleranceScale);

		/**
		@author Rayco Sánchez García
		@date February, 2015

		Function which creates a physic default material
		for physic entities.
		*/
		void createPhysicDefaultMaterial();
	};
} //namespace Physics

#endif // __Physics_PhysicControllerManager_H