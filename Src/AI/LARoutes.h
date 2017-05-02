/**
@file LARoutes.h

En este fichero se implementan algunas acciones 
latentes relacionadas con el seguimiento de rutas.

@author Gonzalo Flórez
@date Enero 2013

*/

#pragma once
#ifndef __AI_LARoutes_H
#define __AI_LARoutes_H

#include "LatentAction.h"

namespace AI 
{

	/**
	Acción latente que sigue una ruta desde el punto actual
	hasta un punto aleatorio del mapa de Galeon.
	<p>
	Utiliza el componente CRouteTo para calcular la ruta y 
	moverse.
	<p>
	Acepta mensajes de tipo FAILED_ROUTE y FINISHED_ROUTE, 
	que indican si debe terminar con fallo o con éxito.
	*/
	class CLARouteToRandom : public CLatentAction
	{
	public:
		/**
		Constructor.
		*/
		CLARouteToRandom(Logic::CEntity* entity) : CLatentAction() {this->setEntity(entity);};
		/**
		Destructor.
		*/
		~CLARouteToRandom() {};

		/**
		Devuelve true si a la acción le interesa el tipo de mensaje
		enviado como parámetro.
		<p>
		Esta acción acepta mensajes del tipo FAILED_ROUTE y FINISHED_ROUTE

		@param msg Mensaje que ha recibido la entidad.
		@return true Si la acción está en principio interesada
		por ese mensaje.
		*/
		virtual bool acceptN(const std::shared_ptr<NMessage> &message);
		/**
		Procesa el mensaje recibido. El método es invocado durante la
		ejecución de la acción cuando se recibe el mensaje.
		<p>
		Si recibe FINISHED_ROUTE la acción finaliza con éxito. Si recibe
		FAILED_ROUTE finaliza con fallo.

		@param msg Mensaje recibido.
		*/
		virtual void processN(const std::shared_ptr<NMessage> &message);

	protected:
		/**
		Método invocado al principio de la ejecución de la acción,
		para que se realicen las tareas que son únicamente necesarias
		al principio (y no durante toda la vida de la acción).
		<p>
		En este caso, se debe enviar un mensaje al componente 
		CRouteTo y cambiar al estado SUSPENDED.

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
		En este caso no hace nada.

		@return Estado de la acción tras la ejecución del método;
		permite indicar si la acción ha terminado o se ha suspendido.
		*/
		virtual LAStatus OnRun() ;

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
		Envía un mensaje de tipo ROUTE_TO.
		*/
		void sendMoveMessage(bool walk = false, Vector3 target = Vector3::ZERO);

	};

	/**
	Acción latente que sigue una ruta desde el punto actual
	hasta un punto aleatorio del mapa de Galeon.
	<p>
	Utiliza el componente CRouteTo para calcular la ruta y 
	moverse.
	<p>
	Acepta mensajes de tipo FAILED_ROUTE y FINISHED_ROUTE, 
	que indican si debe terminar con fallo o con éxito.
	*/
	class CLARouteTo : public CLARouteToRandom
	{
	private:
		Vector3 _target;
	public:
		/**
		Constructor.
		*/
		CLARouteTo(Logic::CEntity* entity, Vector3 &target) : CLARouteToRandom(entity), _target(target) {};
		/**
		Destructor.
		*/
		~CLARouteTo() {};

	protected:
		/**
		Método invocado al principio de la ejecución de la acción,
		para que se realicen las tareas que son únicamente necesarias
		al principio (y no durante toda la vida de la acción).
		<p>
		En este caso, se debe enviar un mensaje al componente 
		CRouteTo y cambiar al estado SUSPENDED.

		@return Estado de la función; si se indica que la
		acción a terminado (LatentAction::Completed), se invocará
		al OnStop().
		*/
		virtual LAStatus OnStart();

	};


} // namespace AI

#endif // __AI_LARoutes_H