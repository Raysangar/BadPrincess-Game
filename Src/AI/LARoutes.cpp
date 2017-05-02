#include "LARoutes.h"

#include "Logic/Entity/Entity.h"
#include "Logic/RouteCalculationMessages.h"
#include "WaypointGraph.h"
#include "Server.h"

namespace AI 
{
	/**
	M�todo invocado al principio de la ejecuci�n de la acci�n,
	para que se realicen las tareas que son �nicamente necesarias
	al principio (y no durante toda la vida de la acci�n).
	<p>
	En este caso, se debe enviar un mensaje al componente 
	CRouteTo y cambiar al estado SUSPENDED.

	@return Estado de la funci�n; si se indica que la
	acci�n a terminado (LatentAction::Completed), se invocar�
	al OnStop().
	*/
	CLatentAction::LAStatus CLARouteToRandom::OnStart()
	{
		// Al comenzar la ejecuci�n, la acci�n env�a un mensaje al 
		// componente que calcula las rutas. Mientras este componente
		// se ejecuta, la acci�n se queda suspendida, esperando recibir
		// el mensaje de que ha llegado a destino o no se ha podido 
		// calcular la ruta.

		// 1. Obtenemos el grafo de navegaci�n del server
		AI::CWaypointGraph* wpg = AI::CServer::getSingletonPtr()->getNavigationGraph();

		// 2. Sacamos un punto aleatorio del grafo
		int random = rand() % wpg->getWaypointCount();
		Vector3 position = wpg->getWaypoint(random);

		// 3. Enviamos un mensaje al componente que se ocupa de calcular rutas
		sendMoveMessage(true,position);

		// 4. Devolver el estado en el que queda la acci�n latente (suspendida)
		return SUSPENDED;
	}

	/**
	M�todo invocado al final de la ejecuci�n de la acci�n,
	para que se realicen las tareas que son �nicamente necesarias
	al final (y no durante toda la vida de la acci�n).

	En la mayor�a de los casos este m�todo no hace nada.
	*/
	void CLARouteToRandom::OnStop()
	{
		// Enviamos un mensaje al componente de rutas para que pare.
		// En teor�a no deber�a ser necesario porque si el seguimiento
		// de la ruta termina en condiciones normales el componente sabe
		// que tiene que parar y si no, al llamar a OnAbort se detiene;
		// pero tampoco est� de m�s.
		sendMoveMessage();
	}

	/**
	M�todo invocado c�clicamente para que se contin�e con la
	ejecuci�n de la acci�n.
	<p>
	En este caso no hace nada.

	@return Estado de la acci�n tras la ejecuci�n del m�todo;
	permite indicar si la acci�n ha terminado o se ha suspendido.
	*/
	CLatentAction::LAStatus CLARouteToRandom::OnRun() {
		// Si el flujo de ejecuci�n llega a entrar en este m�todo
		// significa que hay algo que no va bien. Se supone que 
		// la acci�n, nada m�s iniciarse se queda suspendida (por 
		// lo que no se llega a llamar a este m�todo) y cuando 
		// recibe los mensajes pasa directamente a terminar con
		// �xito o con fallo.
		return RUNNING;
	}

	/**
	Env�a un mensaje de tipo ROUTE_TO.
	*/
	void CLARouteToRandom::sendMoveMessage(bool walk, Vector3 target)
	{
		// Env�a un mensaje para calcular una ruta
		auto message = std::make_shared<AvatarRouteTo>();
		// El bool indica si el receptor debe calcular la ruta 
		// o debe dejar de recorrer la ruta actual y parar
		message->calculateNewRoute = walk;
		// Destino de la ruta. Si el par�metro anterior es false
		// este valor no se utiliza
		message->target = target;
		_entity->emitMessageN(message);		
	}

	/**
	M�todo invocado cuando la acci�n ha sido cancelada (el comportamiento
	al que pertenece se ha abortado por cualquier raz�n).

	La tarea puede en este momento realizar las acciones que
	considere oportunas para "salir de forma ordenada".

	@note <b>Importante:</b> el Abort <em>no</em> provoca la ejecuci�n
	de OnStop().
	*/
	CLatentAction::LAStatus CLARouteToRandom::OnAbort() 
	{
		// Cuando se interrumpe una acci�n de seguimiento de una ruta
		// hay que enviar un mensaje al componente de seguimiento de rutas
		// para que se detenga si hay una ruta a medioseguir.

		// Enviamos un mensaje al componente de enrutamiento para que pare.
		sendMoveMessage();

		return FAIL;
	}

	/**
	Devuelve true si a la acci�n le interesa el tipo de mensaje
	enviado como par�metro.
	<p>
	Esta acci�n acepta mensajes del tipo FAILED_ROUTE y FINISHED_ROUTE

	@param msg Mensaje que ha recibido la entidad.
	@return true Si la acci�n est� en principio interesada
	por ese mensaje.
	*/

	bool CLARouteToRandom::acceptN(const std::shared_ptr<NMessage> &message)
	{
		// Esta acci�n acepta mensajes del tipo FINISHED_ROUTE
		return (message->type.compare("AvatarFinishedRoute") == 0);
	}
	/**
	Procesa el mensaje recibido. El m�todo es invocado durante la
	ejecuci�n de la acci�n cuando se recibe el mensaje.
	<p>
	Si recibe FINISHED_ROUTE la acci�n finaliza con �xito. Si recibe
	FAILED_ROUTE finaliza con fallo.

	@param msg Mensaje recibido.
	*/
	void CLARouteToRandom::processN(const std::shared_ptr<NMessage> &message)
	{
		// Si se recibe un mensaje de fallo de la ruta hay que terminar con fallo.
		// Si es de finalizaci�n de la ruta hay que terminar con �xito.
		// Para terminar una acci�n latente usamos el m�todo finish (ver LatentAction.h)
		std::shared_ptr<AvatarFinishedRoute> m = std::static_pointer_cast<AvatarFinishedRoute>(message);
		
	}

	
	/**
	M�todo invocado al principio de la ejecuci�n de la acci�n,
	para que se realicen las tareas que son �nicamente necesarias
	al principio (y no durante toda la vida de la acci�n).
	<p>
	En este caso, se debe enviar un mensaje al componente 
	CRouteTo y cambiar al estado SUSPENDED.

	@return Estado de la funci�n; si se indica que la
	acci�n a terminado (LatentAction::Completed), se invocar�
	al OnStop().
	*/
	CLatentAction::LAStatus CLARouteTo::OnStart()
	{	
		// Al comenzar la ejecuci�n, la acci�n env�a un mensaje al 
		// componente que calcula las rutas. Mientras este componente
		// se ejecuta, la acci�n se queda suspendida, esperando recibir
		// el mensaje de que ha llegado a destino o no se ha podido 
		// calcular la ruta.
		// 1. Enviar un mensaje a la entidad que se ocupa de calcular rutas 
		// con el destino (_target)
		// 2. Devolver el estado en el que queda la acci�n latente
		sendMoveMessage(true,_target);
		return SUSPENDED;
	}


} //namespace AI 