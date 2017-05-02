/**
@file ActorFactory.h

Contains the declaration of the Actor Factory. 

@author Rayco Sánchez García
@date February, 2015
*/

#ifndef __Physics_ActorFactory_H
#define __Physics_ActorFactory_H

#include "BaseSubsystems/Math.h"

namespace Physics
{
	class SceneManager;
	class PhysXServer;
	class CCollisionManager;
};

namespace physx {
	class PxActor;
	class PxRigidStatic;
	class PxRigidActor;
	class PxRigidDynamic;
};

namespace Logic
{
	class IPhysics;
};


namespace Physics
{
	class ActorFactory
	{
	public:
		ActorFactory(PhysXServer* physXServer);

		~ActorFactory();

		/**
		Crea un plano estático en la escena.

		@param point Punto del plano.
		@param normal Vector normal al plano.
		@param group Grupo de colisión.
		@param component Componente lógico asociado a la entidad física.
		@return actor físico creado
		*/
		physx::PxRigidStatic* createPlane(const Vector3 &point, const Vector3 &normal, 
			                              int group, const Logic::IPhysics *component);

		/**
		Crea una caja estática en la escena.

		@param position Posición de la caja en coordenadas lógicas (el origen de coordenadas 
		       está en el centro de la cara inferior del cubo).
		@param dimensions Dimensiones de la caja divididas entre 2. Es decir, los lados de 
		       la caja medirán dimensions*2.
		@param trigger Indica si la entidad física representa un trigger.
		@param group Grupo de colisión.
		@param component Componente lógico asociado a la entidad física.
		@return actor físico creado
		*/
		physx::PxRigidStatic* createStaticBox(const Vector3 &position, const Vector3 &dimensions, 
			                                  bool trigger, int group, const Logic::IPhysics *component); 

		/**
		Crea una caja dinámica en la escena.

		@param position Posición de la caja en coordenadas lógicas (el origen de coordenadas 
		       está en el centro de la cara inferior del cubo).
		@param dimensions Dimensiones de la caja divididas entre 2. Es decir, los lados de 
		       la caja medirán dimensions*2.
		@param mass Masa distribuida uniformemente en el volumen de la entidad.
		@param kinematic Indica si la entidad es cinemática.
		@param trigger Indica si la entidad física representa un trigger.
		@param group Grupo de colisión.
		@param component Componente lógico asociado a la entidad física.
		@return actor físico creado
		*/
		physx::PxRigidDynamic* createDynamicBox(const Vector3 &position, const Vector3 &dimensions, 
			                                    float mass, bool kinematic, bool trigger, int group, 
												const Logic::IPhysics *component); 

		/**
		Crea una esfera en la escena.

		@param position Posición de la caja en coordenadas lógicas (el origen de coordenadas 
		       está en el centro de la cara inferior del cubo).
		@param radius Radio de la esfera.
		@param mass Masa distribuida uniformemente en el volumen de la entidad.
		@param kinematic Indica si la entidad es cinemática.
		@param trigger Indica si la entidad física representa un trigger.
		@param group Grupo de colisión.
		@param component Componente lógico asociado a la entidad física.
		@return actor físico creado
		*/
		physx::PxRigidDynamic* createDynamicSphere(const Vector3 &position, float radius, 
			                                    float mass, bool kinematic, bool trigger, int group, 
												const Logic::IPhysics *component); 

		/**
		Crea una entidad física en la escena a partir de un fichero RepX exportado con el 
		plug-in de PhysX para 3ds Max. Asume que el fichero contiene únicamente la 
		descripción de un actor.

		@param file Fichero generado con el plug-in de PhysX.
		@param group Grupo de colisión en el que debe ser incluida la entidad.
		@param component Componente lógico asociado a la entidad física.
		@return actor físico creado
		*/
		physx::PxRigidActor* createFromFile(const std::string &file, int group, 
			                                const Logic::IPhysics *component);

		/**
		Elimina una entidad física de la escena y libera los recursos que tenga asociados.
		NO DEBE USARSE CON CHARACTER CONTROLLERS.

		@param actor Actor físico asociado a la entidad.
		 */
		void destroyActor(physx::PxActor *actor);

	private:

		// Object wich encapsulate PhysX SDK and other usefull physic objects
		PhysXServer* physXServer;

		CCollisionManager *_collisionManager;
		
	};
} //namespace Physics

#endif // __Physics_ActorFactory_H