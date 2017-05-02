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
	Esta acci�n espera durante un periodo de tiempo indicado en el constructor.
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