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
//	Implementación de CLATaken
//////////////////////////////

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
	Método invocado al final de la ejecución de la acción,
	para que se realicen las tareas que son únicamente necesarias
	al final (y no durante toda la vida de la acción).

	En la mayoría de los casos este método no hace nada.
	*/
	void CLATaken::OnStop()
	{
	}

	/**
	Método invocado cíclicamente para que se continúe con la
	ejecución de la acción.
	<p>
	En cada paso de ejecución se obtiene el tiempo actual 
	y se comprueba si se debe acabar la acción.

	@return Estado de la acción tras la ejecución del método;
	permite indicar si la acción ha terminado o se ha suspendido.
	*/
	CLatentAction::LAStatus CLATaken::OnRun(unsigned int msecs) 
	{
		GUI::CServer::getSingletonPtr()->getEventscontroller()->princessKidnapped(_entity);

		/**Todo esto se borrará cuando la animación de secuestro esté hecha**/
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
	Método invocado cuando la acción ha sido cancelada (el comportamiento
	al que pertenece se ha abortado por cualquier razón).

	La tarea puede en este momento realizar las acciones que
	considere oportunas para "salir de forma ordenada".

	@note <b>Importante:</b> el Abort <em>no</em> provoca la ejecución
	de OnStop().
	*/
	CLatentAction::LAStatus CLATaken::OnAbort() 
	{
		// Cuando se aborta se queda en estado terminado con fallo
		return FAIL;
	}
	/**
	Devuelve true si a la acción le interesa el tipo de mensaje
	enviado como parámetro.
	<p>
	Esta acción no acepta mensajes de ningún tipo.

	@param msg Mensaje que ha recibido la entidad.
	@return true Si la acción está en principio interesada
	por ese mensaje.
	*/
	bool CLATaken::acceptN(const std::shared_ptr<NMessage> &message)
	{
		// La acción no acepta mensajes
		return false;
	}
	/**
	Procesa el mensaje recibido. El método es invocado durante la
	ejecución de la acción cuando se recibe el mensaje.

	@param msg Mensaje recibido.
	*/
	void CLATaken::processN(const std::shared_ptr<NMessage> &message)
	{				
	}


} //namespace AI 
