/**
@file CollisionManager.h

Contiene la declaraci�n del gestor de colisiones.

@see Physics::CCollisionManager

@author Antonio S�nchez Ruiz-Granados
@date Noviembre, 2012
*/

#ifndef __PHYSICS_COLLISION_MANAGER_H_
#define __PHYSICS_COLLISION_MANAGER_H_

#include <PxSimulationEventCallback.h>
#include <characterkinematic/PxController.h>

// Namespace que contiene las clases relacionadas con la parte f�sica. 
namespace Physics {

	/**
	Clase que gestiona las colisiones entre entidades f�sicas. Cuando se produce una
	colisi�n notifica a los componentes l�gicos correspondientes. 

	 @ingroup physicGroup

	 @author Antonio S�nchez Ruiz-Granados
	 @date Noviembre, 2012
	*/

	/*
	Enumerados que sirven para identificar los grupos de colision y sus m�scaras.
	hay varias normas IMPORTANTES:

	-CollisionGroup::Init debe ser siempre igual a 0
	-CollisionGroup::ePlanoColision debe ser siempre el �ltimo
	-CollisionGroup debe estar siempre con valores contiguos
	-Cada grupo de colision debe tener su m�scara de colision a partir del enumerado de colision
	-A ser posible que dichas m�scaras se elaboren en el mismo orden que los grupos de colision
	*/


	class CCollisionManager : public physx::PxSimulationEventCallback,
							  public physx::PxUserControllerHitReport,
							  public physx::PxControllerFilterCallback
						  
	{
	public:
		/**
		Constructor por defecto.
		*/
		CCollisionManager();

		/**
		Destructor.
		*/
		virtual ~CCollisionManager();

		/**
		M�todo invocado cuando se rompe un constraint (ver documentaci�n PhysX).
		*/
		void onConstraintBreak(physx::PxConstraintInfo *constraints, physx::PxU32 count); 
 
		/**
		M�todo invocado cuando se despiertan actores f�sicos (ver documentaci�n PhysX).
		*/
		void onWake(physx::PxActor **actors, physx::PxU32 count);
  
		/**
		M�todo invocado cuando se duermen actores f�sicos (ver documentaci�n PhysX).
		*/
		void onSleep(physx::PxActor **actors, physx::PxU32 count); 
   
		/**
		M�todo invocado cuando se produce un contacto entre dos shapes f�sicos 
		(ver documentaci�n PhysX).
		*/
		void onContact(const physx::PxContactPairHeader &pairHeader, 
			           const physx::PxContactPair *pairs, physx::PxU32 nbPairs); 
  
		/**
		M�todo invocado cuando una entidad f�sica entra o sale de un trigger 
		(ver documentaci�n PhysX).
		*/
		void onTrigger(physx::PxTriggerPair *pairs, physx::PxU32 count);

		/**
		M�todo invocado cuando un character controller golpea una shape 
		(ver documentaci�n PhysX).
		*/ 
		void onShapeHit(const physx::PxControllerShapeHit &hit);
  
		/**
		M�todo invocado cuando un character controller golpea otro character controller 
		(ver documentaci�n PhysX).
		*/ 
		void onControllerHit(const physx::PxControllersHit &hit); 
   
		/**
		M�todo invocado cuando un character controller golpea un obst�culo definido por 
		el usuario (ver documentaci�n PhysX).
		*/ 
		void onObstacleHit(const physx::PxControllerObstacleHit &hit);


		static physx::PxFilterFlags filterShader(
			physx::PxFilterObjectAttributes attributes0, 
			physx::PxFilterData filterData0, 
			physx::PxFilterObjectAttributes attributes1, 
			physx::PxFilterData filterData1,
			physx::PxPairFlags& pairFlags, 
			const void* constantBlock, 
			physx::PxU32 constantBlockSize);


   		/*
		M�todo invocado cuando dos CCTs (CapsuleController) se chocan. Si se devuelve true habr� colisi�n entre ellos
		false en caso contrario. si queremos hacer una distincion especial entre ellos habr� que pasar informaci�n a la hora
		de crearlos y luego leerla en este callback y ahcer nuestra l�gica

		@param a CCT que participa en la colsion
		@param b CCT que participa en la colsion
		@return True si los CCTs colisionan entre ellos, False en caso contrario
		*/
		bool filter(const physx::PxController& a, const physx::PxController& b); 

	}; // CCollisionManager

}; // namespace Physics

#endif // __PHYSICS_COLLISION_MANAGER_H_