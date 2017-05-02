/**
@file CLAIdle.h

Represents the Idle state of any unit;

@author Iv�n Le�n
@date February 2014


*/

#pragma once

#ifndef __AI_LAIdle_H
#define __AI_LAIdle_H

#include "LatentAction.h"

namespace AI 
{

	/**
	Esta acci�n espera durante un periodo de tiempo indicado en el constructor.
	*/
	class CLAIdle : public CLatentAction
	{
	public:
		/**
		Constructor.
		
		@param entity Entity to which this state belongs
		*/
		CLAIdle(Logic::CEntity *entity) : CLatentAction()
		{
			_entity = entity;
		};
		/**
		Destructor
		*/
		~CLAIdle() {};

		/**
		M�todo invocado al principio de la ejecuci�n de la acci�n,
		para que se realicen las tareas que son �nicamente necesarias
		al principio (y no durante toda la vida de la acci�n).
		<p>
		Al comenzar establecemos la animaci�n Idle

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
		M�todo invocado c�clicamente para que se contin�e con la
		ejecuci�n de la acci�n.
		<p>
		En este caso no hace nada.

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
		Esta acci�n no acepta mensajes de ning�n tipo.

		@param msg Mensaje que ha recibido la entidad.
		@return true Si la acci�n est� en principio interesada
		por ese mensaje.
		*/
		virtual bool acceptN(const std::shared_ptr<NMessage> &message);
		/**
		Procesa el mensaje recibido. El m�todo es invocado durante la
		ejecuci�n de la acci�n cuando se recibe el mensaje.

		@param msg Mensaje recibido.
		*/
		virtual void processN(const std::shared_ptr<NMessage> &message);

	};

} //namespace AI 

#endif // __AI_LAIdle_H