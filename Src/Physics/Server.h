/**
@file Server.h

Contiene la declaraci�n del servidor de f�sica. 

@see Physics::CServer

@author Antonio S�nchez Ruiz-Granados
@date Noviembre, 2012
*/

#ifndef __Physics_Server_H
#define __Physics_Server_H

#include "BaseSubsystems/Math.h"
#include <map>

// Predeclaraci�n de tipos
namespace Logic {
	class CEntity;
	class CPhysicController;
	class CPhysicEntity;
	class IPhysics;
};

namespace Physics {
	class PhysXServer;
	class SceneManager;
	class PhysicControllerManager;
	class ActorController;
	class ActorFactory;
	class PhysicPerceptionManager;
};

namespace physx {
	class PxActor;
	class PxAllocatorCallback;
	class PxCapsuleController;
	class PxController;
	class PxControllerManager;
	class PxCooking;
	class PxDefaultAllocator;
	class PxDefaultCpuDispatcher;
	class PxErrorCallback;
	class PxFoundation;
	class PxMaterial;
	class PxPhysics;
	class PxProfileZoneManager;
	class PxRigidActor;
	class PxRigidDynamic;
	class PxRigidStatic;
	class PxTolerancesScale;

	namespace debugger {
		namespace comm {
			class PvdConnection;
		};
	};
};



// Namespace que contiene las clases relacionadas con la parte f�sica. 
namespace Physics {

	enum CollisionGroup
	{
		INIT = 7,	
		eWorldGround = 8,
		ePlayer = 9,		
		eEnemy = 10,
		eResource = 11,
		eTrigger = 12,
		eChest = 13,
		eUntargetable = 14,
		eWorld = 15,
		eRemovableWorld = 16
	};

	enum FilterMask
	{		
		eResourceMask = (1<<eResource),
		eChestMask = (1<<eChest),
		eEnemyMask = (1<<eEnemy),		
		eTriggerMask = (1<<eTrigger),
		eWorldGroundMask = (1<<eWorldGround),
		ePlayerMask = (1<<ePlayer),
		eUntargetableMask = (1<<eUntargetable),
		eWorldMask = (1<<eWorld),
		eRemovableWorldMask = (1<<eRemovableWorld)
	};

	/**

	@ingroup physicGroup

	@author Antonio S�nchez Ruiz-Granados
	@date Noviembre, 2012
	*/
	class CServer 
	{
	public:

		/**
		Devuelve la �nica instancia de la clase.

		@return Puntero al servidor f�sico.
		*/
		static CServer *getSingletonPtr() { return _instance; }
		
		/**
		Inicializa el servidor f�sico. Esta operaci�n s�lo es necesario realizarla
		una vez durante la inicializaci�n de la aplicaci�n. 

		@return Devuelve false si no se ha podido inicializar.
		*/
		static bool Init();

		/**
		Libera el servidor f�sico. Debe llamarse al finalizar la aplicaci�n.
		*/
		static void Release();

		/**
		Realiza la simulaci�n f�sica y actualiza la posici�n de todas las 
		entidades f�sicas. 

		@param secs Millisegundos transcurridos desde la �ltima actualizaci�n.
		@return Valor booleano indicando si todo fue bien.
		*/
		bool tick(unsigned int msecs);


		/**
		Establece si debe haber colisiones entre dos grupos de colisi�n. Al crear los objetos
		f�sicos se establece el grupo de colisi�n al que pertenecen. Usando este m�todo es
		posible activar / desactivar las colisiones entre grupos.

		@param group1 Primer grupo de colisi�n.
		@param group2 Segundo grupo de colisi�n
		@param enable Activar o desactivar las colisiones entre los dos grupos anteriores.
		*/
		void setGroupCollisions(int group1, int group2, bool enable);
		

		#ifdef _DEBUG
			bool getIsDebugEnabled(){return _isDebugEnabled;}

			void setDebugEnabled(bool set);
		#endif

			SceneManager* const getSceneManager() { return sceneManager; }

			ActorController* const getActorController() { return actorController; }

			ActorFactory* const getActorFactory() { return actorFactory; }

			PhysicPerceptionManager* const getPhysicPerceptionManager() { return physicPerceptionManager; }

			PhysicControllerManager* const getPhysicControllerManager() { return physicControllerManager; }

					/*
		Establece con que grupos de colision debe haber colisiones. 

		@param actor Actor a los que se le indicara los grupos con los que colisionara
		@param collisionGroup grupo del actor
		*/
		void setupFiltering(physx::PxRigidActor* actor, CollisionGroup collisionGroup);
		

		/*
		Metodo que obtiene la m�scara de colisi�n a partir de un grupo

		@param group grupo por el cual busco la m�scara de colision

		@return Mascara de colision (FilterMask) del grupo.
		*/
		FilterMask getMaskGroupCollision(CollisionGroup group){ return _groupCollisionMap[group];}

		/*
		M�todo que habilita o deshabilita un grupo de colisi�n en una m�scara en tiempo de ejecuci�n.
		Este m�todo no modifica la memoria de mascaras del servidor.

		@param filterMask M�scara a modificar (parametro In/out)
		@param group Grupo que se quiere habilitar/deshabilitar en la m�scara
		@param add Flag que indica si habilitamos/deshabilitamos el grupo en la m�scara
		*/
		void static UpdateGroup(unsigned int &filterMask, CollisionGroup group, bool add);

		void InicializarGruposColision();

	protected:
		#ifdef _DEBUG
			float _tickCount;	
			bool _isDebugEnabled;
		#endif
	private:

		/**
		Constructor de la clase.
		*/
		CServer();

		/**
		Destructor de la clase.
		*/
		virtual ~CServer();

		// Instancia �nica de la clase.
		static CServer *_instance;

		PhysXServer* physXServer;

		SceneManager* sceneManager;

		PhysicControllerManager* physicControllerManager;

		ActorController* actorController;

		ActorFactory* actorFactory;

		PhysicPerceptionManager* physicPerceptionManager;
		
		/*
		Map cuya clave es el grupo de colision y su valor la m�scara de colisiones del mismo
		*/
		std::map<CollisionGroup, FilterMask> _groupCollisionMap;
		
	}; // class CServer

}; // namespace Physics

#endif // __Physics_Server_H
