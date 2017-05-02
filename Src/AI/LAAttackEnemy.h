/**
@file CLAAttackEnemy.h

Represents the Idle state of any unit;

@author Iván León
@date February 2014


*/

#pragma once

#ifndef __AI_LAAttackEnemy_H
#define __AI_LAAttackEnemy_H

#include "LatentAction.h"

namespace Logic
{
	class CEnemyDetectionManager;
	class CCombatStats;
};

namespace AI 
{

	/**
	Esta acción espera durante un periodo de tiempo indicado en el constructor.
	*/
	class CLAAttackEnemy : public CLatentAction
	{
	public:
		/**
		Constructor.
		
		@param time Tiempo de espera
		*/
		CLAAttackEnemy(Logic::CEntity *entity) : CLatentAction(),combatComponent(0),enemyDetectorComponent(0)
		{
			_entity = entity;
		};
		/**
		Destructor
		*/
		~CLAAttackEnemy() {};

		/**
		Al comenzar, se guardan referencias a los componentes que se utilizarán
		más tarde.
		Inicializamos el tiempo para realizar un ataque.
		También se guardan las variables necesarias para el OnRun();

		@return Estado de la función; si se indica que la
		acción a terminado (LatentAction::Completed), se invocará
		al OnStop().
		*/
		virtual LAStatus OnStart();

		/**
		Método invocado al final de la ejecución de la acción,
		para que se realicen las tareas que son únicamente necesarias
		al final (y no durante toda la vida de la acción).

		En la mayoría de los casos este método no hace nada.
		*/
		virtual void OnStop();

		/**
		Primero giramos la entidad hacia el enemigo para asegurarnos que siempre está 
		mirando en la dirección correcta.
		Comprobamos si el enemigo está aún a rango de ataque, en cuyo caso contrario
		devolvemos FAIL y volvemos al estado LAChaseEnemy.
		Finalmente comprobamos si ha pasado el tiempo necesario para atacar, en cuyo caso
		mandamos un mensaje de daño al enemigo y actualizamos el tiempo.

		@return Estado de la acción tras la ejecución del método;
		permite indicar si la acción ha terminado o se ha suspendido.
		*/
		virtual LAStatus OnRun(unsigned int msecs) ;
		 
		/**
		Método invocado cuando la acción ha sido cancelada (el comportamiento
		al que pertenece se ha abortado por cualquier razón).

		La tarea puede en este momento realizar las acciones que
		considere oportunas para "salir de forma ordenada".

		@note <b>Importante:</b> el Abort <em>no</em> provoca la ejecución
		de OnStop().
		*/
		virtual LAStatus OnAbort() ;

		/**
		Devuelve true si a la acción le interesa el tipo de mensaje
		enviado como parámetro.
		<p>
		Aceptamos EnemyDismissed
		

		@param msg Mensaje que ha recibido la entidad.
		@return true Si la acción está en principio interesada
		por ese mensaje.
		*/
		virtual bool acceptN(const std::shared_ptr<NMessage> &message);
		/**
		Procesa el mensaje recibido. El método es invocado durante la
		ejecución de la acción cuando se recibe el mensaje.
		Si recibimos EnemyDismissed quiere decir que, o bien a muerto, o ha salido del 
		trigger de visión. Pasamos a estado Idle

		@param msg Mensaje recibido.
		*/
		virtual void processN(const std::shared_ptr<NMessage> &message);

	private:

		/**
		Rango de ataque de la unidad
		*/
		float _attackRange;

		/**
		Tiempo necesario para realizar un ataque
		*/
		int _timeBetweenAttacks;

		/**
		Tiempo en el que se realizará el siguiente ataque
		*/
		unsigned int _newAttackTime;

		/**
		Daño de ataque
		*/
		int _attackDamage;

		/**
		Enemigo asignado
		*/
		Logic::CEntity *_currentEnemy;

		/**
		Nombre de la animación para atacar
		*/
		std::string _attackAnimation;

		/*
		Guardamos referencias a componentes para acceder a sus métodos
		**/		
		Logic::CCombatStats* combatComponent;
		Logic::CEnemyDetectionManager* enemyDetectorComponent;
	};

} //namespace AI 

#endif // __AI_LAAttackEnemy_H