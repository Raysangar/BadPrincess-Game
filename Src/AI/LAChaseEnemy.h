/**
@file CLAChaseEnemy.h

Represents the state of chasing an enemy

@author Iván León
@date February 2014


*/ 

#pragma once

#ifndef __AI_LAChaseEnemy_H
#define __AI_LAChaseEnemy_H

#include "LatentAction.h"

namespace Logic{

	class CCombatStats;
	class CStateMachineExecutor;
	class CSteeringMovement;
	class CEnemyDetectionManager;
	class IComponent;
};

namespace AI 
{

	/**
	Esta acción persigue a una entidad rival hasta que se encuentra en rango de ataque
	*/
	class CLAChaseEnemy : public CLatentAction
	{
	public:
		/**
		Constructor.
		
		@param entity Entidad dueña del estado
		*/
		CLAChaseEnemy(Logic::CEntity *entity) : CLatentAction(),combatComponent(0),enemyDetectorComponent(0),smExecutorComponent(0),enemySteeringComponent(0),_currentEnemy(nullptr)
		{
			_entity = entity;
		};
		/**
		Destructor
		*/
		~CLAChaseEnemy() {};

		/**
		Al comenzar, se guardan referencias a los componentes que se utilizarán
		más tarde.
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
		Método invocado cíclicamente para que se continúe con la
		ejecución de la acción.
		<p>
		En cada paso se comprueba si el enemigo asignado se encuentra en
		rango de ataque. Si es así, finalizamos este estado y nos aseguramos
		de que la entidad se encuentra mirando hacia el enemigo.
		En caso contrario, se asigna un destino en rango de ataque a la entidad.

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
		Aceptamos AvatarFinishedRoute y EnemyDismissed
		

		@param msg Mensaje que ha recibido la entidad.
		@return true Si la acción está en principio interesada
		por ese mensaje.
		*/
		virtual bool acceptN(const std::shared_ptr<NMessage> &message);

		/**
		Procesa el mensaje recibido. El método es invocado durante la
		ejecución de la acción cuando se recibe el mensaje.
		Si recibimos un AvatarFinishedRoute quiere decir que hemos llegado
		a un punto dentro del rango de ataque. Pasamos al siguiente estado.
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
		Enemigo asignado
		*/
		Logic::CEntity *_currentEnemy;

		/**
		Flag para saber si el enemigo está dentro del rango de ataque
		Sólo útil para unidades de ataque a distancia
		*/
		bool _atRange;		

		/**
		Flag para empezar a contar el tiempo que lleva la unidad buscando un hueco
		para poder atacar.
		Sólo útik para unidades cuerpo a cuerpo
		*/
		bool _tryingToAttack;

		/**
		*/
		unsigned int _timeTryingToAttack;
		
		/*
		Guardamos referencias a componentes para acceder a sus métodos
		**/
		Logic::CCombatStats* combatComponent;			
		Logic::CEnemyDetectionManager* enemyDetectorComponent;
		Logic::CStateMachineExecutor *smExecutorComponent;						
		Logic::CSteeringMovement *enemySteeringComponent;

		void sendRouteMessage(Vector3 target,bool recalculateRoute, Logic::IComponent *component = 0);

		void sendStopMessage();
	
	};

} //namespace AI 

#endif // __AI_LAChaseEnemy_H