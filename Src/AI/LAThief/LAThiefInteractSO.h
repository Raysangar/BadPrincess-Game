/**
@file LAThiefInteractSO.h

Represents the moving state of any unit.

@author Giacomo Guidi
@date March 2014

*/

#pragma once

#ifndef __AI_LAThiefInteractSO_H
#define __AI_LAThiefInteractSO_H

#include "AI/LatentAction.h"
#include "AI/ResourceServer.h"

namespace Logic{
	class CThiefEnemyState;
	class CInteractSOState;
};


namespace AI 
{

	/**
	Esta acción espera durante un periodo de tiempo indicado en el constructor.
	*/
	class CLAThiefInteractSO : public CLatentAction
	{
	public:
		/**
		Constructor.

		@param entity Entity to which this state belongs
		*/
		CLAThiefInteractSO(Logic::CEntity *entity) : CLatentAction(),_state(nullptr),_currentTime(0)
		{
			_entity = entity;
		};
		/**
		Destructor
		*/
		~CLAThiefInteractSO() {};

		/**
		Método invocado al principio de la ejecución de la acción,
		para que se realicen las tareas que son únicamente necesarias
		al principio (y no durante toda la vida de la acción).
		<p>
		Cuando se introduzca el cálculo del movimiento, 
		pasará automáticamente al estado RUNNING.

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
		Realizará el cálculo de movimiento.

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

	private:
		Logic::CThiefEnemyState * _state;
		unsigned int _currentTime;
		Logic::CInteractSOState* _interactComponent;
		AI::ResourceType findAvailableResource(int quantity);
		void startEscape();
		void lookAtSmartObject();
	};

} //namespace AI 

#endif // __AI_SimpleLatentActions_H