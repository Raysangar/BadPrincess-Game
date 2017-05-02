/**
@file KnightState.h

Contiene la implementaci�n del componente que gestiona
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
		Destructor (virtual); Liberamos los Manual Object reci�n creados
		*/
		virtual ~CKnightState();
		
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
		M�todo que desactiva el componente; invocado cuando se
		desactiva el mapa donde est� la entidad a la que pertenece el
		componente. Se invocar� siempre, independientemente de si estamos
		activados o no.
		<p>
		La implementaci�n eliminar� al componente de algunos observers en los 
		que pueda estar registrado (como el cron�metro del sistema, etc.).
		*/
		virtual void deactivate();

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