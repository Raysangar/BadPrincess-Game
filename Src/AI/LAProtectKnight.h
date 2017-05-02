/**
@file LAProtectKnight.h

Representa el estado de proteger al caballero con la princesa

@author Jorge Luque
@date April 2014

*/

#pragma once

#ifndef __AI_LAProtectknight_H
#define __AI_LAProtectknight_H

#include "LatentAction.h"

namespace AI
{
	/**
	Esta acción espera durante un periodo de tiempo indicado en el constructor.
	*/
	class CLAProtectKnight : public CLatentAction
	{
	public:
		/**
		Constructor.
		
		@param time Tiempo de espera
		*/
		CLAProtectKnight(Logic::CEntity *entity) : CLatentAction()
		{
			_entity = entity;
		};
		/**
		Destructor
		*/
		~CLAProtectKnight() {};

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
		Enemigo asignado
		*/
		Logic::CEntity *_currentEnemy;

		/**
		Variable si es posicionado alrededor del caballero
		*/
		bool _positioned;

		/**
		Variable si ha recibido su posicion
		*/
		bool _positionAssigned;

		/**
		Vector relativo a la posicion del caballero
		*/
		Vector3 _relativePos;

		/**
		Vector spanwpoint
		*/
		Vector3 _spawnPoint;

		void sendRouteMessage(Vector3 target,bool recalculateRoute);
	};

} //namespace AI 

#endif // __AI_LAProtectknight_H