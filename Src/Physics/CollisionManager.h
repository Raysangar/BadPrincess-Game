/**
@file CollisionManager.h

Contiene la declaración del gestor de colisiones.

@see Physics::CCollisionManager

@author Antonio Sánchez Ruiz-Granados
@date Noviembre, 2012
*/

#ifndef __PHYSICS_COLLISION_MANAGER_H_
#define __PHYSICS_COLLISION_MANAGER_H_

#include <PxSimulationEventCallback.h>
#include <characterkinematic/PxController.h>

// Namespace que contiene las clases relacionadas con la parte física. 
namespace Physics {

	/**
	Clase que gestiona las colisiones entre entidades físicas. Cuando se produce una
	colisión notifica a los componentes lógicos correspondientes. 

	 @ingroup physicGroup

	 @author Antonio Sánchez Ruiz-Granados
	 @date Noviembre, 2012
	*/

	/*
	Enumerados que sirven para identificar los grupos de colision y sus máscaras.
	hay varias normas IMPORTANTES:

	-CollisionGroup::Init debe ser siempre igual a 0
	-CollisionGroup::ePlanoColision debe ser siempre el último
	-CollisionGroup debe estar siempre con valores contiguos
	-Cada grupo de colision debe tener su máscara de colision a partir del enumerado de colision
	-A ser posible que dichas máscaras se elaboren en el mismo orden que los grupos de colision
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
		Método invocado cuando se rompe un constraint (ver documentación PhysX).
		*/
		void onConstraintBreak(physx::PxConstraintInfo *constraints, physx::PxU32 count); 
 
		/**
		Método invocado cuando se despiertan actores físicos (ver documentación PhysX).
		*/
		void onWake(physx::PxActor **actors, physx::PxU32 count);
  
		/**
		Método invocado cuando se duermen actores físicos (ver documentación PhysX).
		*/
		void onSleep(physx::PxActor **actors, physx::PxU32 count); 
   
		/**
		Método invocado cuando se produce un contacto entre dos shapes físicos 
		(ver documentación PhysX).
		*/
		void onContact(const physx::PxContactPairHeader &pairHeader, 
			           const physx::PxContactPair *pairs, physx::PxU32 nbPairs); 
  
		/**
		Método invocado cuando una entidad física entra o sale de un trigger 
		(ver documentación PhysX).
		*/
		void onTrigger(physx::PxTriggerPair *pairs, physx::PxU32 count);

		/**
		Método invocado cuando un character controller golpea una shape 
		(ver documentación PhysX).
		*/ 
		void onShapeHit(const physx::PxControllerShapeHit &hit);
  
		/**
		Método invocado cuando un character controller golpea otro character controller 
		(ver documentación PhysX).
		*/ 
		void onControllerHit(const physx::PxControllersHit &hit); 
   
		/**
		Método invocado cuando un character controller golpea un obstáculo definido por 
		el usuario (ver documentación PhysX).
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
		Método invocado cuando dos CCTs (CapsuleController) se chocan. Si se devuelve true habrá colisión entre ellos
		false en caso contrario. si queremos hacer una distincion especial entre ellos habrá que pasar información a la hora
		de crearlos y luego leerla en este callback y ahcer nuestra lógica

		@param a CCT que participa en la colsion
		@param b CCT que participa en la colsion
		@return True si los CCTs colisionan entre ellos, False en caso contrario
		*/
		bool filter(const physx::PxController& a, const physx::PxController& b); 

	}; // CCollisionManager

}; // namespace Physics

#endif // __PHYSICS_COLLISION_MANAGER_H_