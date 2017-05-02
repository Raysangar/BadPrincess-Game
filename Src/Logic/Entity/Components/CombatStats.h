/**
@file CombatStats.h

Contiene la implementaci�n del componente que gestiona
los atributos �nicos en el Goblin Montaraz

@see Logic::IComponent

@author Iv�n Le�n
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
		Destructor (virtual); Liberamos los Manual Object reci�n creados
		*/
		virtual ~CCombatStats()
		{
			if(_currentEnemy != NULL) _currentEnemy = NULL;	
		}
		
		virtual void awake();
		/**
		Inicializaci�n del componente, utilizando la informaci�n extra�da de
		la entidad le�da del mapa (Maps::CEntity). Toma del mapa el tama�o de la 
		c�psula que representa la entidad f�sica y usa su radio para asign�rselo
		al c�rculo de selecci�n.
		Despu�s crea un ManualObject para dibujar el c�rculo y lo asigna a un
		SceneNode de Ogre tambi�n reci�n creado.

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa L�gico en el que se registrar� el objeto.
		@param entityInfo Informaci�n de construcci�n del objeto le�do del
			fichero de disco.
		@return Cierto si la inicializaci�n ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		M�todo virtual que elige que mensajes son aceptados. Son v�lidos
		EnemySelected.

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
		M�todo para devolver el enemigo actualmente asignado

		@return _currentEnemy.
		*/
		CEntity* getCurrentEnemy(){ return _currentEnemy; }

		/**
		M�todo para asignar el enemigo a atacar

		@param enemy Enemigo a atacar.
		*/
		void setCurrentEnemy(CEntity *enemy){ _currentEnemy = enemy; }

		/**
		M�todo para devolver el rango de ataque de la unidad

		@return _attackRange.
		*/
		const float getAttackRange(){ return _attackRange; }

		/**
		M�todo para devolver el da�o de ataque de la unidad

		@return _attackDamage.
		*/
		const int getAttackDamage(){ return _attackDamage; }

		/**
		M�todo para devolver el da�o de ataque de la unidad

		@return _attackDamage.
		*/
		void setAttackDamage(int attackDamage){ _attackDamage = attackDamage; }

		/**
		M�todo para devolver el tiempo entre ataques de la unidad

		@return _timeBetweenAttacks.
		*/
		const unsigned int getTimeBetweenAttacks(){ return _timeBetweenAttacks; }

		/**
		M�todo para devolver la animaci�n de ataque de la unidad

		@return _attackAnimation Animaci�n de ataque.
		*/
		const std::string getAttackAnimation(){ return _attackAnimation; }

		const unsigned int getTimeOfLastAttack() { return _timeOfLastAttack;}

		void setTimeOfLastAttack(unsigned int timeOfLastAttack) {_timeOfLastAttack = timeOfLastAttack;}

	protected:
		
		//Actual enemigo asignado
		CEntity *_currentEnemy;

		//Distancia m�xima de ataque
		float _attackRange;

		//Da�o por ataque
		int _attackDamage;

		//Nombre de la animaci�n para atacar de la entidad
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
