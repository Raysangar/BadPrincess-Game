/**
@file CLAAttackEnemy.h

Represents the Idle state of any unit;

@author Iv�n Le�n
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
	Esta acci�n espera durante un periodo de tiempo indicado en el constructor.
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
		Al comenzar, se guardan referencias a los componentes que se utilizar�n
		m�s tarde.
		Inicializamos el tiempo para realizar un ataque.
		Tambi�n se guardan las variables necesarias para el OnRun();

		@return Estado de la funci�n; si se indica que la
		acci�n a terminado (LatentAction::Completed), se invocar�
		al OnStop().
		*/
		virtual LAStatus OnStart();

		/**
		M�todo invocado al final de la ejecuci�n de la acci�n,
		para que se realicen las tareas que son �nicamente necesarias
		al final (y no durante toda la vida de la acci�n).

		En la mayor�a de los casos este m�todo no hace nada.
		*/
		virtual void OnStop();

		/**
		Primero giramos la entidad hacia el enemigo para asegurarnos que siempre est� 
		mirando en la direcci�n correcta.
		Comprobamos si el enemigo est� a�n a rango de ataque, en cuyo caso contrario
		devolvemos FAIL y volvemos al estado LAChaseEnemy.
		Finalmente comprobamos si ha pasado el tiempo necesario para atacar, en cuyo caso
		mandamos un mensaje de da�o al enemigo y actualizamos el tiempo.

		@return Estado de la acci�n tras la ejecuci�n del m�todo;
		permite indicar si la acci�n ha terminado o se ha suspendido.
		*/
		virtual LAStatus OnRun(unsigned int msecs) ;
		 
		/**
		M�todo invocado cuando la acci�n ha sido cancelada (el comportamiento
		al que pertenece se ha abortado por cualquier raz�n).

		La tarea puede en este momento realizar las acciones que
		considere oportunas para "salir de forma ordenada".

		@note <b>Importante:</b> el Abort <em>no</em> provoca la ejecuci�n
		de OnStop().
		*/
		virtual LAStatus OnAbort() ;

		/**
		Devuelve true si a la acci�n le interesa el tipo de mensaje
		enviado como par�metro.
		<p>
		Aceptamos EnemyDismissed
		

		@param msg Mensaje que ha recibido la entidad.
		@return true Si la acci�n est� en principio interesada
		por ese mensaje.
		*/
		virtual bool acceptN(const std::shared_ptr<NMessage> &message);
		/**
		Procesa el mensaje recibido. El m�todo es invocado durante la
		ejecuci�n de la acci�n cuando se recibe el mensaje.
		Si recibimos EnemyDismissed quiere decir que, o bien a muerto, o ha salido del 
		trigger de visi�n. Pasamos a estado Idle

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
		Tiempo en el que se realizar� el siguiente ataque
		*/
		unsigned int _newAttackTime;

		/**
		Da�o de ataque
		*/
		int _attackDamage;

		/**
		Enemigo asignado
		*/
		Logic::CEntity *_currentEnemy;

		/**
		Nombre de la animaci�n para atacar
		*/
		std::string _attackAnimation;

		/*
		Guardamos referencias a componentes para acceder a sus m�todos
		**/		
		Logic::CCombatStats* combatComponent;
		Logic::CEnemyDetectionManager* enemyDetectorComponent;
	};

} //namespace AI 

#endif // __AI_LAAttackEnemy_H