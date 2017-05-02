/**
@file LAExecuteSM.h

En este fichero se implementa una acción latente que
permite ejecutar una CFiniteStateMachine. De esta manera
(un poco rudimentaria) podemos crear máquinas de estado
jerárquicas.

@author Gonzalo Flórez
@date Enero 2013

*/

#pragma once
#ifndef __AI_LAExecuteSM_H
#define __AI_LAExecuteSM_H

#include "LatentAction.h"


namespace AI 
{

	template <class TNode>
	class CStateMachine;

	/**
	Esta acción ejecuta una máquina de estado de la clase CStateMachine
	*/
	class CLAExecuteSM : public CLatentAction
	{

	protected:
		/**	Máquina de estado que estamos ejecutando */
		CStateMachine<CLatentAction>* _stateMachine;
		/** Acción de la máquina de estado que se está ejecutando */
		CLatentAction* _currentAction;

	public:
		/**
		Constructor.
		
		@param stateMachine Máquina de estado
		*/
		CLAExecuteSM(CStateMachine<CLatentAction>* stateMachine) : CLatentAction(), _stateMachine(stateMachine), _currentAction(NULL) {};
		/**
		Destructor
		*/
		virtual ~CLAExecuteSM();

		/**
		Método invocado al principio de la ejecución de la acción,
		para que se realicen las tareas que son únicamente necesarias
		al principio (y no durante toda la vida de la acción).
		<p>
		Al comenzar se obtiene el tiempo actual y se calcula el 
		tiempo en el que terminará la acción mediante el atributo _time

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
		En cada paso de ejecución se obtiene el tiempo actual 
		y se comprueba si se debe acabar la acción.

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
		Esta acción no acepta mensajes de ningún tipo.

		@param msg Mensaje que ha recibido la entidad.
		@return true Si la acción está en principio interesada
		por ese mensaje.
		*/
		virtual bool acceptN(const std::shared_ptr<NMessage> &message);
		/**
		Procesa el mensaje recibido. El método es invocado durante la
		ejecución de la acción cuando se recibe el mensaje.

		@param msg Mensaje recibido.
		*/
		virtual void processN(const std::shared_ptr<NMessage> &message);

	};

} // namespace AI 

#endif // __AI_LAExecuteSM_H