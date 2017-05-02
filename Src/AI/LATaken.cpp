#include "LATaken.h"

#include "Application/BaseApplication.h"

#include "Logic\Server.h"
#include "Logic\Maps\Map.h"
#include "Logic\Entity\Entity.h"

#include "Logic\Entity\Components\PrincessManager.h"
#include "Logic\Entity\Components\SteeringMovement.h"
#include "Logic\RouteCalculationMessages.h"
#include "Logic/UnitMessages.h"
#include "Logic/AudioMessages.h"
#include "GUI\EventsController.h"
#include "GUI/Server.h"
#include "GUI/MinimapController.h"

namespace AI 
{


//////////////////////////////
//	Implementaci�n de CLATaken
//////////////////////////////

	/**
	M�todo invocado al principio de la ejecuci�n de la acci�n,
	para que se realicen las tareas que son �nicamente necesarias
	al principio (y no durante toda la vida de la acci�n).
	<p>
	Al comenzar se obtiene el tiempo actual y se calcula el 
	tiempo en el que terminar� la acci�n mediante el atributo _time

	@return Estado de la funci�n; si se indica que la
	acci�n a terminado (LatentAction::Completed), se invocar�
	al OnStop().
	*/
	CLatentAction::LAStatus CLATaken::OnStart()
	{	
		Logic::CEntity* gameManager = Logic::CServer::getSingletonPtr()->getMap()->getEntityByName("GameManager");

		auto backgroundVolumen = std::make_shared<SetParameterValue>();
		backgroundVolumen->eventName = "default";
		backgroundVolumen->parameterName = "Volumen";
		backgroundVolumen->value = 1.1f;
		gameManager->emitMessageN(backgroundVolumen);

		auto playDrums = std::make_shared<PlayAudio>();
		playDrums->eventName = "event:/Music/Drums";
		gameManager->emitMessageN(playDrums);
		

		_princessManagerComponent = static_cast<Logic::CPrincessManager*>(_entity->getComponent("CPrincessManager"));
		_raptorSteeringComponent = static_cast<Logic::CSteeringMovement*>(_princessManagerComponent->getRaptor()->getComponent("CSteeringMovement"));
		sendAnimationMessage("Walk");

		return RUNNING;
	}

	/**
	M�todo invocado al final de la ejecuci�n de la acci�n,
	para que se realicen las tareas que son �nicamente necesarias
	al final (y no durante toda la vida de la acci�n).

	En la mayor�a de los casos este m�todo no hace nada.
	*/
	void CLATaken::OnStop()
	{
	}

	/**
	M�todo invocado c�clicamente para que se contin�e con la
	ejecuci�n de la acci�n.
	<p>
	En cada paso de ejecuci�n se obtiene el tiempo actual 
	y se comprueba si se debe acabar la acci�n.

	@return Estado de la acci�n tras la ejecuci�n del m�todo;
	permite indicar si la acci�n ha terminado o se ha suspendido.
	*/
	CLatentAction::LAStatus CLATaken::OnRun(unsigned int msecs) 
	{
		GUI::CServer::getSingletonPtr()->getEventscontroller()->princessKidnapped(_entity);

		/**Todo esto se borrar� cuando la animaci�n de secuestro est� hecha**/
		if(_raptorSteeringComponent->getCurrentSpeed() != Vector3::ZERO)
		{
			auto m = std::make_shared<AvatarRouteTo>();
			m->target = _princessManagerComponent->getRaptor()->getPosition();
			m->sender = _entity;
			m->calculateNewRoute = false;
			_entity->emitMessageN(m);
		}

		return RUNNING;
	}

	/**
	M�todo invocado cuando la acci�n ha sido cancelada (el comportamiento
	al que pertenece se ha abortado por cualquier raz�n).

	La tarea puede en este momento realizar las acciones que
	considere oportunas para "salir de forma ordenada".

	@note <b>Importante:</b> el Abort <em>no</em> provoca la ejecuci�n
	de OnStop().
	*/
	CLatentAction::LAStatus CLATaken::OnAbort() 
	{
		// Cuando se aborta se queda en estado terminado con fallo
		return FAIL;
	}
	/**
	Devuelve true si a la acci�n le interesa el tipo de mensaje
	enviado como par�metro.
	<p>
	Esta acci�n no acepta mensajes de ning�n tipo.

	@param msg Mensaje que ha recibido la entidad.
	@return true Si la acci�n est� en principio interesada
	por ese mensaje.
	*/
	bool CLATaken::acceptN(const std::shared_ptr<NMessage> &message)
	{
		// La acci�n no acepta mensajes
		return false;
	}
	/**
	Procesa el mensaje recibido. El m�todo es invocado durante la
	ejecuci�n de la acci�n cuando se recibe el mensaje.

	@param msg Mensaje recibido.
	*/
	void CLATaken::processN(const std::shared_ptr<NMessage> &message)
	{				
	}


} //namespace AI 
