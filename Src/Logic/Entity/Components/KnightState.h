/**
@file KnightState.h

Contiene la implementación del componente que gestiona
los parametros unicos del caballero

@see Logic::IComponent

@author Jorge Luque
@date March, 2015
*/

#ifndef __Logic_KnightState_H
#define __Logic_KnightState_H

#include "Logic/Entity/Component.h"

namespace Logic 
{
	class CCombatStats;
	class CEntity;

	class CKnightState : public IComponent
	{

	DEC_FACTORY(CKnightState);

	public:	
		/**
		This constructor initialize the component with its component name 
		and the rest attributes with their default value.
		*/
		CKnightState(std::string componentName) : IComponent(componentName), _combatComponent(nullptr), _objetive(nullptr), _princess(nullptr), 
			_knight(nullptr) {};

		/**
		Destructor (virtual); Liberamos los Manual Object recién creados
		*/
		virtual ~CKnightState();
		
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
		Método que desactiva el componente; invocado cuando se
		desactiva el mapa donde está la entidad a la que pertenece el
		componente. Se invocará siempre, independientemente de si estamos
		activados o no.
		<p>
		La implementación eliminará al componente de algunos observers en los 
		que pueda estar registrado (como el cronómetro del sistema, etc.).
		*/
		virtual void deactivate();

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
		
		//Lista de enemigos
		std::list <Logic::CEntity*> _enemyList;

		//Lista de enemigos
		std::list <Logic::CEntity*> _enemiesAttacking;

		//Lista de pozos cercanos
		std::list <Logic::CEntity*> _wellList;

		//Componente de combate
		CCombatStats* _combatComponent;

		//Objetivo Actual, variable para controlar que el CombatStats esta actualizado
		CEntity* _objetive;

		//Princessa si se al esta llevando
		CEntity* _princess;

		//Caballero al que que proteger
		CEntity* _knight;
	}; 

	REG_FACTORY(CKnightState);
} 

#endif