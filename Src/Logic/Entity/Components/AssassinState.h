/**
@file KnightState.h

Contiene la implementaci�n del componente que gestiona
los parametros unicos del asesino

@see Logic::IComponent

@author Rayco S�nchez Garc�a
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
		Destructor (virtual); Liberamos los Manual Object reci�n creados
		*/
		virtual ~CAssassinState();
		
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
		M�todo llamado en cada frame que actualiza el estado del componente.
		<p>
		Las clases hijas deber�n sobreescribir este m�todo con las 
		instrucciones que quieran realizar cada ciclo.

		@param msecs Milisegundos transcurridos desde el �ltimo tick.
		*/
		virtual void tick(unsigned int msecs);

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
		CEntity* getCurrentEnemy();

		/**
		M�todo para a�adir enemigos

		@param enemy Enemigo a atacar.
		*/
		void addEnemy(CEntity *enemy);

		/**
		M�todo para quitar enemigos

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