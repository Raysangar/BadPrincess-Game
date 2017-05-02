/**
@file KnightState.h

Contiene la implementación del componente que gestiona
los parametros unicos del asesino

@see Logic::IComponent

@author Rayco Sánchez García
@date March, 2015
*/

#ifndef __Logic_AssassinState_H
#define __Logic_AssassinState_H

#include "Logic/Entity/Component.h"

namespace Logic 
{
	class CCombatStats;
	class CEntity;
	class CAssassinCamouflage;

	class CAssassinState : public IComponent
	{

	DEC_FACTORY(CAssassinState);

	public:	
		/**
		This constructor initialize the component with its component name 
		and the rest attributes with their default value.
		*/
		CAssassinState(std::string componentName) : IComponent(componentName), combatComponent(nullptr), objetive(nullptr), 
			enemyList(nullptr), princess(nullptr), lastVictim(nullptr), assassinIsActive(false), attacking(false)
		{
			priorityEnemyTypeList.push_back("RangerGoblin");
			priorityEnemyTypeList.push_back("WorkmanGoblin");
			priorityEnemyTypeList.push_back("WarriorGoblin");
		}

		/**
		Destructor (virtual); Liberamos los Manual Object recién creados
		*/
		virtual ~CAssassinState();
		
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
		Método llamado en cada frame que actualiza el estado del componente.
		<p>
		Las clases hijas deberán sobreescribir este método con las 
		instrucciones que quieran realizar cada ciclo.

		@param msecs Milisegundos transcurridos desde el último tick.
		*/
		virtual void tick(unsigned int msecs);

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
		CEntity* getCurrentEnemy();

		/**
		Método para añadir enemigos

		@param enemy Enemigo a atacar.
		*/
		void addEnemy(CEntity *enemy);

		/**
		Método para quitar enemigos

		@param enemy Enemigo a atacar.
		*/
		void removeEnemy(CEntity *enemy);

	protected:
		bool princessHasFallen();

		void attack(CEntityPtr objective);

		CEntityPtr getClosestSuitableEnemy();

		void selectNewObjective();

		std::set<Logic::CEntity*>* enemyList;

		std::list<std::string> priorityEnemyTypeList;

		CCombatStats* combatComponent;

		CAssassinCamouflage* camouflageComponent;

		CEntityPtr objetive, lastVictim;

		CEntityPtr princess;

		bool assassinIsActive;

		bool attacking;

	}; 

	REG_FACTORY(CAssassinState);
} 

#endif