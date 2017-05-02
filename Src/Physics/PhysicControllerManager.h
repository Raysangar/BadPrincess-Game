/**
@file PhysicControllerManager.h

Contains the declaration of the manager for physic controllers. 

@author Rayco S�nchez Garc�a
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
		@author Rayco S�nchez Garc�a
		@date February, 2015

		@param sceneManager Pointer to the scene manager wich
		contains the physic scene.
		*/
		PhysicControllerManager(PhysXServer* physXServer, SceneManager *sceneManager);


		/**
		@author Rayco S�nchez Garc�a
		@date February, 2015
		
		Default destructor.
		*/
		~PhysicControllerManager();

		/**
		Crea un controller de tipo capsula en la escena.

		@param position Posici�n del controller en coordenadas l�gicas (el origen de coordenadas 
		       est� en los pies del controller).
		@param radius Radio de la c�psula.
		@param height Altura de la c�psula.
		@param component Componente l�gico asociado a la entidad f�sica.
		@return character controller creado
		*/
		physx::PxCapsuleController* createCapsuleController(const Vector3 &position, float radius, 
															float height, int group,
															const Logic::CPhysicController *component);


		/**
		Mueve el controller por la escena.

		@param controller Controller que se desea mover.
		@param movement Movimiento que se quiere realizar.
		@param msecs Millisegundos transcurridos desde la �ltima actualizaci�n.
		@return Flags de colisi�n, un conjunto de physx::PxControllerFlag.
		*/
		unsigned moveController(physx::PxController *controller, const Vector3 &movement, 
			                             unsigned int msecs);

		/**
		Devuelve la posici�n del controller.

		@param controller Controller del que se desea conocer su posici�n.
		@return posici�n del controller en coordenadas l�gicas.
		*/
		Vector3 getControllerPosition(const physx::PxCapsuleController *controller);

		/**
		Establece la posici�n del controller.

		@param controller Controller del que se desea conocer su posici�n.
		@param posici�n del controller en coordenadas l�gicas.
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