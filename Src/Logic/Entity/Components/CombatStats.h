/**
@file CombatStats.h

Contiene la implementación del componente que gestiona
los atributos únicos en el Goblin Montaraz

@see Logic::IComponent

@author Iván León
@date Febrero, 2015
*/

#ifndef __Logic_CombatStats_H
#define __Logic_CombatStats_H

#include "Logic/Entity/Component.h"

namespace Ogre
{
	class Vector3;
}

namespace Logic 
{
	class CEntity;	
	class CSteeringMovement;

	class CCombatStats : public IComponent
	{

	DEC_FACTORY(CCombatStats);

	public:	
		/**
		This constructor initialize the component with its component name 
		and the rest attributes with their default value.
		*/
		CCombatStats(std::string componentName) : IComponent(componentName), _currentEnemy(0), _timeOfLastAttack(0) {}

		/**
		Destructor (virtual); Liberamos los Manual Object recién creados
		*/
		virtual ~CCombatStats()
		{
			if(_currentEnemy != NULL) _currentEnemy = NULL;	
		}
		
		virtual void awake();
		/**
		Inicialización del componente, utilizando la información extraída de
		la entidad leída del mapa (Maps::CEntity). Toma del mapa el tamaño de la 
		cápsula que representa la entidad física y usa su radio para asignárselo
		al círculo de selección.
		Después crea un ManualObject para dibujar el círculo y lo asigna a un
		SceneNode de Ogre también recién creado.

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa Lógico en el que se registrará el objeto.
		@param entityInfo Información de construcción del objeto leído del
			fichero de disco.
		@return Cierto si la inicialización ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		Método virtual que elige que mensajes son aceptados. Son válidos
		EnemySelected.

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
		Método para devolver el enemigo actualmente asignado

		@return _currentEnemy.
		*/
		CEntity* getCurrentEnemy(){ return _currentEnemy; }

		/**
		Método para asignar el enemigo a atacar

		@param enemy Enemigo a atacar.
		*/
		void setCurrentEnemy(CEntity *enemy){ _currentEnemy = enemy; }

		/**
		Método para devolver el rango de ataque de la unidad

		@return _attackRange.
		*/
		const float getAttackRange(){ return _attackRange; }

		/**
		Método para devolver el daño de ataque de la unidad

		@return _attackDamage.
		*/
		const int getAttackDamage(){ return _attackDamage; }

		/**
		Método para devolver el daño de ataque de la unidad

		@return _attackDamage.
		*/
		void setAttackDamage(int attackDamage){ _attackDamage = attackDamage; }

		/**
		Método para devolver el tiempo entre ataques de la unidad

		@return _timeBetweenAttacks.
		*/
		const unsigned int getTimeBetweenAttacks(){ return _timeBetweenAttacks; }

		/**
		Método para devolver la animación de ataque de la unidad

		@return _attackAnimation Animación de ataque.
		*/
		const std::string getAttackAnimation(){ return _attackAnimation; }

		const unsigned int getTimeOfLastAttack() { return _timeOfLastAttack;}

		void setTimeOfLastAttack(unsigned int timeOfLastAttack) {_timeOfLastAttack = timeOfLastAttack;}

	protected:
		
		//Actual enemigo asignado
		CEntity *_currentEnemy;

		//Distancia máxima de ataque
		float _attackRange;

		//Daño por ataque
		int _attackDamage;

		//Nombre de la animación para atacar de la entidad
		std::string _attackAnimation;

		//Tiempo necesario para realizar un nuevo ataque
		unsigned int _timeBetweenAttacks;

		unsigned int _timeOfLastAttack;

		//Referencia al componente steering movement
		CSteeringMovement* _steeringComponent;

	}; 

	REG_FACTORY(CCombatStats);
} 

#endif
