/**
@file EnemyDetectionManager.h

Contiene la implementación del componente que gestiona
la detección automática de unidades enemigas

@see Logic::IComponent

@author Iván León
@date Marzo, 2015
*/

#ifndef __Logic_EnemyDetectionManager_H
#define __Logic_EnemyDetectionManager_H

#include "Logic/Entity/Component.h"
#include "Physics.h"

namespace Ogre
{
	class Vector3;
};

//DEBUG
//namespace Ogre{
//	class SceneNode;
//}
//namespace Graphics
//{
//	class CManualEntity;
//}

// Predeclaración de tipos
namespace physx {	
	class PxRigidDynamic;
};

namespace Physics {
	class ActorFactory;
	class ActorController;
};

namespace Logic 
{
	class CEntity;
	class CCombatStats;
	class CSteeringMovement;

	class CEnemyDetectionManager : public IPhysics
	{

	DEC_FACTORY(CEnemyDetectionManager);

	public:	
		/**
		This constructor initialize the component with its component name 
		and the rest attributes with their default value.
		*/
		CEnemyDetectionManager(std::string componentName);

		/**
		Destructor (virtual); Liberamos los Manual Object recién creados
		*/
		virtual ~CEnemyDetectionManager();
		
		/**
		Method called once all enitties are loaded.
		*/
		virtual void awake();

		/**
		Inicialización del componente, utilizando la información extraída de
		la entidad leída del mapa (Maps::CEntity). 

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa Lógico en el que se registrará el objeto.
		@param entityInfo Información de construcción del objeto leído del
			fichero de disco.
		@return Cierto si la inicialización ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		virtual bool activate();

		virtual void deactivate();

		/**
		Método virtual que elige que mensajes son aceptados.

		@param message Mensaje a chequear.
		@return true si el mensaje es aceptado.
		*/
		virtual bool acceptN(const std::shared_ptr<NMessage> &message);

		
		/**
		Método virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		virtual void processN(const std::shared_ptr<NMessage> &message);

		/**
		Se invoca cuando se produce una colisión entre una entidad física y un trigger.
		Ver descripción paso por paso en el cpp.
		*/
		virtual void onTrigger (IPhysics *otherComponent, bool enter);

		/**
		Este método se invoca en cada ciclo de la simulación y actualiza la posición del actor
		*/
		virtual void tick(unsigned int msecs);

		/**
		Método que devuelve el enemigo más cercano a la unidad en un rango determinado

		@return _positionToDefend.
		*/
		Logic::CEntity* getClosestEnemy(Logic::CEntity* enemyToAvoid);

		/**
		Método para comprobar si una unidad está persiguiendo a un enemigo que 
		ha entrado en su trigger.

		@return _isDefendingPosition.
		*/
	    bool isDefendingPosition() const{ return _isDefendingPosition; }
				
		/**
		Método para cambiar la condición de defender una posición

		@param set nueva condición.
		*/
		void setIsDefendingPosition(bool set) { _isDefendingPosition = set;}

		/**
		Método que devuelve la posición a defender

		@return _positionToDefend.
		*/
		Vector3 getPositionToDefend() const { return _positionToDefend; }

		/**
		Método que devuelve la distancia máxima que la entidad se puede alejar mientras defiende una posición

		@return _defendingDistance.
		*/
		float getDefendingDistance() const { return _defendingDistance; }	

		/**
		Método que busca enemigos al rededor de la entidad. En caso de encontrar alguno, se asigna como objetivo y devuelve true
		*/
		bool searchEnemies(Logic::CEntity* previousEnemy);

		bool getCanKeepAttacking() const{ return _canKeepAttacking; }

		bool getIsAggressive() const { return _isAggressive; }

		void setIsAggressive(bool isAggressive);
		
	private:
		
		/**
		Crea el actor de PhysX que representa la entidad física a partir de la
		información del mapa.
		*/
		physx::PxRigidDynamic*	createTriggerDetector(const Map::CEntity *entityInfo);

		/**
		Comprueba si la entidad que entra en el trigger es un adversario
		*/
		bool otherIsFoe(Logic::CEntity* other);

		/**
		Comprueba que si la unidad no puede detectar en movimiento, esté quieta en el momento de la detección
		*/
		bool resolveMovingDetection();

		// Physic Actor Factory
		Physics::ActorFactory* _actorFactory;

		// Actor que representa la entidad física en PhysX
		physx::PxRigidDynamic* _triggerDetector;

		// Physic Actor Controller
		Physics::ActorController* _actorController;

		// Entity types that triggers the detector
		std::list<std::string> targetEntityTypes;

		//Radius of detection sphere
		float _detectionRange;

		//Flag para asignar si la entidad persigue automáticamente a enemigos que atraviesen el trigger
		bool _isAggressive;

		//Flag para asignar si puede detectar enemigos cuando está en movimiento o no
		bool _movingDetection;

		//Flag para asignar si la entidad volverá a la posición a defender tras perseguir
		bool _canDefendPosition;

		//Flag para asignar si la entidad puede seguir atacando a otro enemigo cercano tras 
		//vencer al enemigo actual
		bool _canKeepAttacking;

		Vector3 _positionToDefend;

		bool _isDefendingPosition;

		float _defendingDistance;

		Logic::CCombatStats* _combatComponent;	
		Logic::CSteeringMovement* _steeringComponent;

		//DEBUG to draw line and point 
		//Ogre::SceneNode* drawDetector;
		//Graphics::CManualEntity *debugEntity;

	}; 

	REG_FACTORY(CEnemyDetectionManager);
} 

#endif
