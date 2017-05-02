/**
@file CLatentAction.h

Clase que implementa las acciones latentes

@author Marco Antonio Gómez Martín
@date Diciembre 2010

*/

#pragma once

#ifndef __AI_LatentAction_H
#define __AI_LatentAction_H

#include <memory> 
#include "Logic\NMessage.h"

namespace Logic
{
	class Logic::CEntity;
};

namespace AI 
{

	/**
	Clase base que representa una acción latente del sistema;
	esta clase está pensada para ser extendida con clases hijas
	que son las que implementan las distintas acciones latentes
	del sistema.

	La existencia de esta clase permite un tratamiento general de
	todas ellas.

	Las acciones latentes después serán ejecutadas por una entidad
	externa (por ejemplo dentro de un árbol de comportamiento, o
	como función latente de un lenguaje de script).

	El responsable de esa ejecución simplemente llamará al método
	Tick() hasta que la acción latente termine. El Tick() es un
	template method, de forma que las clases hija deben (o mejor, pueden)
	implementar los siguientes métodos:

	- OnStart(): invocado cuando la acción latente comienza su
	ejecución.

	- OnRun(): invocado cíclicamente para que la acción latente
	continúe su ejecución.

	- OnStop(): cuando OnRun() notifica que la acción ha terminado,
	se invoca a este método por si se quieren liberar recursos.

	- OnAbort(): este método es llamado únicamente en situaciones
	especiales; en concreto cuando el comportamiento al que pertenece
	la acción latente es cancelado. Permite a la acción latente
	liberar los recursos que considere oportunos, desengancharse
	de observadores, etc.

	Una acción latente se encuentra siempre en uno de estos estados:

	- READY: la acción latente está lista para empezar a ser ejecutada.

	- RUNNING: la acción latente está ejecutándose.

	- SUSPENDED: la acción latente está suspendida a la espera de 
	algún evento externo y no necesita ciclos de CPU.

	- SUCCESS: la acción latente ha finalizado con éxito.

	- FAIL: la acción latente ha finalizado con fallo.

	En la mayoría de los casos la acción latente necesitará conocer
	cuál es la Logic::CEntity ejecutable de su ejecución. Para ello hay que 
	llamar al método setEntity

	@author Marco Antonio Gómez Martín
	*/
	class CLatentAction
	{
	public:
		enum LAStatus {
			/// Indica que la acción latente aún no se ha empezado
			/// a ejecutar
			READY,
			/// Indica que la acción latente está ejecutándose,
			/// y por lo tanto necesita ciclos de CPU
			RUNNING,
			/// Indica que la acción latente está suspendida a
			/// la espera de algún evento externo, y por tanto no
			/// necesita (al menos por el momento) ciclos de CPU.
			SUSPENDED,
			/// Indica que la acción latente ha terminado su
			/// tarea con éxito.
			SUCCESS,
			/// Indica que la acción latente ha terminado su
			/// tarea con fallo.
			FAIL,
			//Para acciones latentes dentro de SubMáquinas de estado
			//indica que puede salir de la máquina de estado con éxito.
			EXIT_SM_SUCCESS,
			//Para acciones latentes dentro de SubMáquinas de estado
			//indica que puede salir de la máquina de estado con fallo.
			EXIT_SM_FAIL
		};
		
		/**
		Constructor
		*/
		CLatentAction() : _status(READY), _stopping(false) {};

		virtual ~CLatentAction(){};
		/**
		Establece la entidad que ejecuta la acción.
		*/
		void setEntity(Logic::CEntity* entity) { _entity = entity; };

		/**
		Método llamado cíclicamente por el responsable de la ejecución
		de la acción latente.
		@return Devuelve el estado de la función latente: si ésta ha
		terminado su ejecución, si necesita que se siga invocando a
		la función Tick() cíclicamente o si, aún sin haber terminado,
		no necesita (de momento) la invocación a Tick().
		*/
		LAStatus tick(unsigned int msecs);
		/**
		Cancela la tarea que se está ejecutando; se entiende que este
		método es llamado cuando el comportamiento al que pertenece
		la tarea es anulado.
		*/
		void abort();
		/**
		Reinicia la acción, que queda en un estado listo para ser ejecutada
		de nuevo (READY). Dependiendo del estado de la acción en el momento
		de llamar a reset (si éste es RUNNING o SUSPENDED) se considera que 
		la acción ha sido abortada y se llama a OnAbort.
		*/
		void reset();
		/**
		Devuelve true si a la acción le interesa el tipo de mensaje
		enviado como parámetro.
		@param msg Mensaje que ha recibido la entidad.
		@return true Si la acción está en principio interesada
		por ese mensaje.
		*/
		virtual bool acceptN(const std::shared_ptr<NMessage> &message) = 0;
		/**
		Procesa el mensaje recibido. El método es invocado durante la
		ejecución de la acción cuando se recibe el mensaje.

		@param msg Mensaje recibido.
		*/
		virtual void processN(const std::shared_ptr<NMessage> &message) = 0;
		/**
		Devuelve el estado actual de la acción.
		*/
		LAStatus getStatus() {return _status; };

	private:
		/**
		Estado de la acción
		*/
		LAStatus _status;

	protected:
		/**
		Entidad que ejecuta la acción
		*/
		Logic::CEntity *_entity;
		/**
		Se pone a cierto cuando se solicita la finalización de la acción.
		*/
		bool _stopping;

		/**
		Método invocado al principio de la ejecución de la acción,
		para que se realicen las tareas que son únicamente necesarias
		al principio (y no durante toda la vida de la acción).
		@return Estado de la al que pasa la acción; si se indica que la
		acción a terminado (LatentAction::Completed), se invocará
		al OnStop().
		*/
		virtual LAStatus OnStart() { return READY;}

		/**
		Método invocado al final de la ejecución de la acción,
		para que se realicen las tareas que son únicamente necesarias
		al final (y no durante toda la vida de la acción).

		En la mayoría de los casos este método no hace nada.
		*/
		virtual void OnStop() {}

		/**
		Método invocado cíclicamente para que se continúe con la
		ejecución de la acción.

		@return Estado de la acción tras la ejecución del método;
		permite indicar si la acción ha terminado o se ha suspendido,
		o si sigue en ejecución.
		*/
		virtual LAStatus OnRun(unsigned int msecs) { return SUCCESS;}

		/**
		Método invocado cuando la acción ha sido cancelada (el comportamiento
		al que pertenece se ha abortado por cualquier razón).

		La tarea puede en este momento realizar las acciones que
		considere oportunas para "salir de forma ordenada".
		
		@return Estado de la acción tras la ejecución del método.

		@note <b>Importante:</b> el Abort <em>no</em> provoca la ejecución
		de OnStop().
		*/
		virtual LAStatus OnAbort() { return FAIL; }
		/**
		Solicita la finalización de la acción en el siguiente tick, 
		estableciendo el estado	a SUCCESS o FAIL según el valor del
		parámetro de entrada.
		*/
		virtual void finish(bool success);
		/**
		Continúa la ejecución de la acción cuando esta se encuentra suspendida
		Cuidado: únicamente realiza el cambio de estado de SUSPENDED a RUNNING.
		Si el estado original es otro, no tiene efecto
		*/
		void resume() {if (_status == SUSPENDED) _status = RUNNING; };

		/**
		Método invocado cuando queremos indicar a la submáquina de estado
		en la cual se encuentra el estado, que queremos salir de ella.

		@return Estado de la acción tras la ejecucuón del método;
		*/
		virtual void exitSM(bool success); 
		

		/**
		Envía un mensaje para cambiar la animación actual.

		@param animation Nueva animación.
		*/
		void sendAnimationMessage(std::string animation, bool loop = true,bool reset = true );

	}; // class CLatentAction	


} // namespace AI 

#endif // __AI_LatentAction_H
