/**
@file PhysicControllerManager.h

Contains the declaration of the manager for physic controllers. 

@author Rayco Sánchez García
@date February, 2015
*/

#ifndef __Physics_PhysicControllerManager_H
#define __Physics_PhysicControllerManager_H

#include "BaseSubsystems/Math.h"


#include <PxFiltering.h>

namespace Physics
{
	class PhysXServer;
	class SceneManager;
	class CCollisionManager;
};

namespace Logic {
	class CPhysicController;
};

namespace physx {
	class PxCapsuleController;
	class PxController;
	class PxControllerManager;	
};



namespace Physics
{

	class PhysicControllerManager
	{
	public:

		/**
		@author Rayco Sánchez García
		@date February, 2015

		@param sceneManager Pointer to the scene manager wich
		contains the physic scene.
		*/
		PhysicControllerManager(PhysXServer* physXServer, SceneManager *sceneManager);


		/**
		@author Rayco Sánchez García
		@date February, 2015
		
		Default destructor.
		*/
		~PhysicControllerManager();

		/**
		Crea un controller de tipo capsula en la escena.

		@param position Posición del controller en coordenadas lógicas (el origen de coordenadas 
		       está en los pies del controller).
		@param radius Radio de la cápsula.
		@param height Altura de la cápsula.
		@param component Componente lógico asociado a la entidad física.
		@return character controller creado
		*/
		physx::PxCapsuleController* createCapsuleController(const Vector3 &position, float radius, 
															float height, int group,
															const Logic::CPhysicController *component);


		/**
		Mueve el controller por la escena.

		@param controller Controller que se desea mover.
		@param movement Movimiento que se quiere realizar.
		@param msecs Millisegundos transcurridos desde la última actualización.
		@return Flags de colisión, un conjunto de physx::PxControllerFlag.
		*/
		unsigned moveController(physx::PxController *controller, const Vector3 &movement, 
			                             unsigned int msecs);

		/**
		Devuelve la posición del controller.

		@param controller Controller del que se desea conocer su posición.
		@return posición del controller en coordenadas lógicas.
		*/
		Vector3 getControllerPosition(const physx::PxCapsuleController *controller);

		/**
		Establece la posición del controller.

		@param controller Controller del que se desea conocer su posición.
		@param posición del controller en coordenadas lógicas.
		*/
		void setControllerPosition(physx::PxCapsuleController *controller, const Vector3 &position);
		
	private:
		//PhysX SDK server
		PhysXServer* physXServer;

		// Gestor de controller managers
		physx::PxControllerManager* controllerManager;

		SceneManager *sceneManager;

		physx::PxFilterData _data;

		CCollisionManager *_collisionManager;
		
	};
} //namespace Physics

#endif // __Physics_PhysicControllerManager_H