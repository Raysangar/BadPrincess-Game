/**
@file ActorController.h

Contains the declaration of the ActorController. 

@author Rayco S�nchez Garc�a
@date February, 2015
*/

#ifndef __Physics_ActorController_H
#define __Physics_ActorController_H

#include "BaseSubsystems/Math.h"

namespace physx {
	class PxRigidActor;
	class PxRigidDynamic;
};


namespace Physics
{
	class ActorController
	{
	public:
		ActorController();

		~ActorController();

		/**
		Devuelve la posici�n y rotaci�n de una entidad f�sica.

		@param actor Actor f�sico del que se desea conocer la posici�n y orientaci�n.
		@return Matriz 4x4 con la posici�n y orientaci�n de la entidad.
		 */
		Matrix4 getActorTransform(const physx::PxRigidActor *actor);

		/**
		Mueve un actor cinem�tico.

		@param actor Actor cinem�tico que se desea mover.
		@param transform Nueva posici�n y orientaci�n. 
		 */
		void moveKinematicActor(physx::PxRigidDynamic *actor, const Matrix4 &transform);

		/**
		Aplica una traslaci�n a un actor cinem�tico.

		@param actor Actor cinem�tico que se desea mover.
		@param displ Desplazamiento a realizar
		@param msecs Tiempo transcurrido desde el �ltimo tick
		 */
		void moveKinematicActor(physx::PxRigidDynamic *actor, Vector3 displ, unsigned int msecs);

		/**
		Aplica una traslaci�n a un actor cinem�tico.

		@param actor Actor cinem�tico que se desea mover.
		@param displ Desplazamiento a realizar
		@param speed Velocidad de desplazamiento
		@param msecs Tiempo transcurrido desde el �ltimo tick
		 */
		void moveKinematicActor(physx::PxRigidDynamic *actor, Vector3 displ, float speed, unsigned int msecs);

		/**
		Set the global position of an actor

		@param actor Dynamic actor that is going to change it position
		@param newPosition The new position of the actor
		*/
		void setGlopalPosition(physx::PxRigidDynamic *actor, const Vector3 &newPosition);

		void setGlobalPose(physx::PxRigidDynamic *actor, const Matrix4 &matrixTransform);


		/**
		Indica si un actor din�mico es cinem�tico.
		 */
		bool isKinematic(const physx::PxRigidDynamic *actor);
	};
} //namespace Physics

#endif // __Physics_ActorController_H