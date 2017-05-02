#include "LASetTrap.h"

#include "Application/BaseApplication.h"

#include "Logic\Entity\Entity.h"
#include "Logic\Maps\EntityFactory.h"
#include "Logic\Entity\Components\RangerBehaviour.h"
#include "Logic\RangerMessages.h"

namespace AI 
{


//////////////////////////////
//	Implementación de CLASetTrap
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
	CLatentAction::LAStatus CLASetTrap::OnStart()
	{	
		std::cout << "SETTING TRAP" << std::endl;
		iComponentRangerBehaviour = _entity->getComponent("CRangerBehaviour");
		assert(iComponentRangerBehaviour);
		RangerBehaviourComponent = static_cast<Logic::CRangerBehaviour*>(iComponentRangerBehaviour);

		_endingTime = RangerBehaviourComponent->getNextTrapInfo()->trapTimeToSet;
		_acumTime = 0;

		sendAnimationMessage("Build", false);

		return RUNNING;
	}

	/**
	Método invocado al final de la ejecución de la acción,
	para que se realicen las tareas que son únicamente necesarias
	al final (y no durante toda la vida de la acción).

	En la mayoría de los casos este método no hace nada.
	*/
	void CLASetTrap::OnStop()
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
	CLatentAction::LAStatus CLASetTrap::OnRun(unsigned int msecs) 
	{
		_acumTime += msecs;
		if(_acumTime >= _endingTime)
		{	
			const NextTrapInfo* nextTrapInfo = RangerBehaviourComponent->getNextTrapInfo(); 
			Ogre::Vector3 pos = nextTrapInfo->position + (Math::getDirection(_entity->getOrientation()).normalisedCopy() * 2);
			CEntityPtr trap = Logic::CEntityFactory::getSingletonPtr()->instantiate(nextTrapInfo->nextTrapGenerator, nextTrapInfo->newTrapName, pos);

			RangerBehaviourComponent->trapSet(trap);
			std::cout << "TRAP SET" << std::endl;
			return EXIT_SM_SUCCESS;			
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
	CLatentAction::LAStatus CLASetTrap::OnAbort() 
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
	bool CLASetTrap::acceptN(const std::shared_ptr<NMessage> &message)
	{
		// La acción no acepta mensajes
		return false;
	}
	/**
	Procesa el mensaje recibido. El método es invocado durante la
	ejecución de la acción cuando se recibe el mensaje.

	@param msg Mensaje recibido.
	*/
	void CLASetTrap::processN(const std::shared_ptr<NMessage> &message)
	{		
		// La acción no procesa mensajes
		
	}


} //namespace AI 
