/**
@file EnemyDetectionManager.h

Contiene la implementaci�n del componente que gestiona
la detecci�n autom�tica de unidades enemigas

@see Logic::IComponent

@author Iv�n Le�n
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

// Predeclaraci�n de tipos
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
		Destructor (virtual); Liberamos los Manual Object reci�n creados
		*/
		virtual ~CEnemyDetectionManager();
		
		/**
		Method called once all enitties are loaded.
		*/
		virtual void awake();

		/**
		Inicializaci�n del componente, utilizando la informaci�n extra�da de
		la entidad le�da del mapa (Maps::CEntity). 

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa L�gico en el que se registrar� el objeto.
		@param entityInfo Informaci�n de construcci�n del objeto le�do del
			fichero de disco.
		@return Cierto si la inicializaci�n ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		virtual bool activate();

		virtual void deactivate();

		/**
		M�todo virtual que elige que mensajes son aceptados.

		@param message Mensaje a chequear.
		@return true si el mensaje es aceptado.
		*/
		virtual bool acceptN(const std::shared_ptr<NMessage> &message);

		
		/**
		M�todo virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		virtual void processN(const std::shared_ptr<NMessage> &message);

		/**
		Se invoca cuando se produce una colisi�n entre una entidad f�sica y un trigger.
		Ver descripci�n paso por paso en el cpp.
		*/
		virtual void onTrigger (IPhysics *otherComponent, bool enter);

		/**
		Este m�todo se invoca en cada ciclo de la simulaci�n y actualiza la posici�n del actor
		*/
		virtual void tick(unsigned int msecs);

		/**
		M�todo que devuelve el enemigo m�s cercano a la unidad en un rango determinado

		@return _positionToDefend.
		*/
		Logic::CEntity* getClosestEnemy(Logic::CEntity* enemyToAvoid);

		/**
		M�todo para comprobar si una unidad est� persiguiendo a un enemigo que 
		ha entrado en su trigger.

		@return _isDefendingPosition.
		*/
	    bool isDefendingPosition() const{ return _isDefendingPosition; }
				
		/**
		M�todo para cambiar la condici�n de defender una posici�n

		@param set nueva condici�n.
		*/
		void setIsDefendingPosition(bool set) { _isDefendingPosition = set;}

		/**
		M�todo que devuelve la posici�n a defender

		@return _positionToDefend.
		*/
		Vector3 getPositionToDefend() const { return _positionToDefend; }

		/**
		M�todo que devuelve la distancia m�xima que la entidad se puede alejar mientras defiende una posici�n

		@return _defendingDistance.
		*/
		float getDefendingDistance() const { return _defendingDistance; }	

		/**
		M�todo que busca enemigos al rededor de la entidad. En caso de encontrar alguno, se asigna como objetivo y devuelve true
		*/
		bool searchEnemies(Logic::CEntity* previousEnemy);

		bool getCanKeepAttacking() const{ return _canKeepAttacking; }

		bool getIsAggressive() const { return _isAggressive; }

		void setIsAggressive(bool isAggressive);
		
	private:
		
		/**
		Crea el actor de PhysX que representa la entidad f�sica a partir de la
		informaci�n del mapa.
		*/
		physx::PxRigidDynamic*	createTriggerDetector(const Map::CEntity *entityInfo);

		/**
		Comprueba si la entidad que entra en el trigger es un adversario
		*/
		bool otherIsFoe(Logic::CEntity* other);

		/**
		Comprueba que si la unidad no puede detectar en movimiento, est� quieta en el momento de la detecci�n
		*/
		bool resolveMovingDetection();

		// Physic Actor Factory
		Physics::ActorFactory* _actorFactory;

		// Actor que representa la entidad f�sica en PhysX
		physx::PxRigidDynamic* _triggerDetector;

		// Physic Actor Controller
		Physics::ActorController* _actorController;

		// Entity types that triggers the detector
		std::list<std::string> targetEntityTypes;

		//Radius of detection sphere
		float _detectionRange;

		//Flag para asignar si la entidad persigue autom�ticamente a enemigos que atraviesen el trigger
		bool _isAggressive;

		//Flag para asignar si puede detectar enemigos cuando est� en movimiento o no
		bool _movingDetection;

		//Flag para asignar si la entidad volver� a la posici�n a defender tras perseguir
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
