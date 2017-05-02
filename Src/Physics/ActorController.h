/**
@file ActorController.h

Contains the declaration of the ActorController. 

@author Rayco Sánchez García
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
		Devuelve la posición y rotación de una entidad física.

		@param actor Actor físico del que se desea conocer la posición y orientación.
		@return Matriz 4x4 con la posición y orientación de la entidad.
		 */
		Matrix4 getActorTransform(const physx::PxRigidActor *actor);

		/**
		Mueve un actor cinemático.

		@param actor Actor cinemático que se desea mover.
		@param transform Nueva posición y orientación. 
		 */
		void moveKinematicActor(physx::PxRigidDynamic *actor, const Matrix4 &transform);

		/**
		Aplica una traslación a un actor cinemático.

		@param actor Actor cinemático que se desea mover.
		@param displ Desplazamiento a realizar
		@param msecs Tiempo transcurrido desde el último tick
		 */
		void moveKinematicActor(physx::PxRigidDynamic *actor, Vector3 displ, unsigned int msecs);

		/**
		Aplica una traslación a un actor cinemático.

		@param actor Actor cinemático que se desea mover.
		@param displ Desplazamiento a realizar
		@param speed Velocidad de desplazamiento
		@param msecs Tiempo transcurrido desde el último tick
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
		Indica si un actor dinámico es cinemático.
		 */
		bool isKinematic(const physx::PxRigidDynamic *actor);
	};
} //namespace Physics

#endif // __Physics_ActorController_H