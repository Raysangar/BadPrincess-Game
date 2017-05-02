#include "LAWaitBuild.h"

#include "Logic\Entity\Entity.h"
#include "Logic\Entity\Components\WorkmanState.h"
#include "Logic\Entity\Components\PrincessManager.h"

#include "AI\Server.h"
#include "AI\EntityManager.h"

namespace AI 
{


//////////////////////////////
//	Implementación de CLAWaitBuild
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
	CLatentAction::LAStatus CLAWaitBuild::OnStart()
	{	
		std::cout << "WAIT TO BUILD" << std::endl;
		Logic::IComponent *ic = _entity->getComponent("CWorkmanState");
		assert(ic);
		_state = static_cast<Logic::CWorkmanState*>(ic);

		_princess= AI::CServer::getSingletonPtr()->getEntityManager()->getClosestEntity(AI::CEntityManager::EntityType::Princess,_entity->getPosition());
		ic =_princess->getComponent("CPrincessManager");
		assert(ic);
		_princessState = static_cast<Logic::CPrincessManager*>(ic);
		sendAnimationMessage("Idle");

		return RUNNING;
	}

	/**
	Método invocado al final de la ejecución de la acción,
	para que se realicen las tareas que son únicamente necesarias
	al final (y no durante toda la vida de la acción).

	En la mayoría de los casos este método no hace nada.
	*/
	void CLAWaitBuild::OnStop()
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
	CLatentAction::LAStatus CLAWaitBuild::OnRun(unsigned int msecs) 
	{
		float d = _state->getBuildingEntity()->getPosition().distance(_princess->getPosition());
		if(!_princessNeeded || _princessNeeded && d<=_princessState->getWrathAreaRadius())
		{
			return SUCCESS;
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
	CLatentAction::LAStatus CLAWaitBuild::OnAbort() 
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
	bool CLAWaitBuild::acceptN(const std::shared_ptr<NMessage> &message)
	{
		// La acción no acepta mensajes
		return false;
	}
	/**
	Procesa el mensaje recibido. El método es invocado durante la
	ejecución de la acción cuando se recibe el mensaje.

	@param msg Mensaje recibido.
	*/
	void CLAWaitBuild::processN(const std::shared_ptr<NMessage> &message)
	{		
		
	}


} //namespace AI 
