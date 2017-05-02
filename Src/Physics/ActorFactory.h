/**
@file ActorFactory.h

Contains the declaration of the Actor Factory. 

@author Rayco S�nchez Garc�a
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
		Crea un plano est�tico en la escena.

		@param point Punto del plano.
		@param normal Vector normal al plano.
		@param group Grupo de colisi�n.
		@param component Componente l�gico asociado a la entidad f�sica.
		@return actor f�sico creado
		*/
		physx::PxRigidStatic* createPlane(const Vector3 &point, const Vector3 &normal, 
			                              int group, const Logic::IPhysics *component);

		/**
		Crea una caja est�tica en la escena.

		@param position Posici�n de la caja en coordenadas l�gicas (el origen de coordenadas 
		       est� en el centro de la cara inferior del cubo).
		@param dimensions Dimensiones de la caja divididas entre 2. Es decir, los lados de 
		       la caja medir�n dimensions*2.
		@param trigger Indica si la entidad f�sica representa un trigger.
		@param group Grupo de colisi�n.
		@param component Componente l�gico asociado a la entidad f�sica.
		@return actor f�sico creado
		*/
		physx::PxRigidStatic* createStaticBox(const Vector3 &position, const Vector3 &dimensions, 
			                                  bool trigger, int group, const Logic::IPhysics *component); 

		/**
		Crea una caja din�mica en la escena.

		@param position Posici�n de la caja en coordenadas l�gicas (el origen de coordenadas 
		       est� en el centro de la cara inferior del cubo).
		@param dimensions Dimensiones de la caja divididas entre 2. Es decir, los lados de 
		       la caja medir�n dimensions*2.
		@param mass Masa distribuida uniformemente en el volumen de la entidad.
		@param kinematic Indica si la entidad es cinem�tica.
		@param trigger Indica si la entidad f�sica representa un trigger.
		@param group Grupo de colisi�n.
		@param component Componente l�gico asociado a la entidad f�sica.
		@return actor f�sico creado
		*/
		physx::PxRigidDynamic* createDynamicBox(const Vector3 &position, const Vector3 &dimensions, 
			                                    float mass, bool kinematic, bool trigger, int group, 
												const Logic::IPhysics *component); 

		/**
		Crea una esfera en la escena.

		@param position Posici�n de la caja en coordenadas l�gicas (el origen de coordenadas 
		       est� en el centro de la cara inferior del cubo).
		@param radius Radio de la esfera.
		@param mass Masa distribuida uniformemente en el volumen de la entidad.
		@param kinematic Indica si la entidad es cinem�tica.
		@param trigger Indica si la entidad f�sica representa un trigger.
		@param group Grupo de colisi�n.
		@param component Componente l�gico asociado a la entidad f�sica.
		@return actor f�sico creado
		*/
		physx::PxRigidDynamic* createDynamicSphere(const Vector3 &position, float radius, 
			                                    float mass, bool kinematic, bool trigger, int group, 
												const Logic::IPhysics *component); 

		/**
		Crea una entidad f�sica en la escena a partir de un fichero RepX exportado con el 
		plug-in de PhysX para 3ds Max. Asume que el fichero contiene �nicamente la 
		descripci�n de un actor.

		@param file Fichero generado con el plug-in de PhysX.
		@param group Grupo de colisi�n en el que debe ser incluida la entidad.
		@param component Componente l�gico asociado a la entidad f�sica.
		@return actor f�sico creado
		*/
		physx::PxRigidActor* createFromFile(const std::string &file, int group, 
			                                const Logic::IPhysics *component);

		/**
		Elimina una entidad f�sica de la escena y libera los recursos que tenga asociados.
		NO DEBE USARSE CON CHARACTER CONTROLLERS.

		@param actor Actor f�sico asociado a la entidad.
		 */
		void destroyActor(physx::PxActor *actor);

	private:

		// Object wich encapsulate PhysX SDK and other usefull physic objects
		PhysXServer* physXServer;

		CCollisionManager *_collisionManager;
		
	};
} //namespace Physics

#endif // __Physics_ActorFactory_H