#include "LASetTrap.h"

#include "Application/BaseApplication.h"

#include "Logic\Entity\Entity.h"
#include "Logic\Maps\EntityFactory.h"
#include "Logic\Entity\Components\RangerBehaviour.h"
#include "Logic\RangerMessages.h"

namespace AI 
{


//////////////////////////////
//	Implementaci�n de CLASetTrap
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
	M�todo invocado al final de la ejecuci�n de la acci�n,
	para que se realicen las tareas que son �nicamente necesarias
	al final (y no durante toda la vida de la acci�n).

	En la mayor�a de los casos este m�todo no hace nada.
	*/
	void CLASetTrap::OnStop()
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
	M�todo invocado cuando la acci�n ha sido cancelada (el comportamiento
	al que pertenece se ha abortado por cualquier raz�n).

	La tarea puede en este momento realizar las acciones que
	considere oportunas para "salir de forma ordenada".

	@note <b>Importante:</b> el Abort <em>no</em> provoca la ejecuci�n
	de OnStop().
	*/
	CLatentAction::LAStatus CLASetTrap::OnAbort() 
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
	bool CLASetTrap::acceptN(const std::shared_ptr<NMessage> &message)
	{
		// La acci�n no acepta mensajes
		return false;
	}
	/**
	Procesa el mensaje recibido. El m�todo es invocado durante la
	ejecuci�n de la acci�n cuando se recibe el mensaje.

	@param msg Mensaje recibido.
	*/
	void CLASetTrap::processN(const std::shared_ptr<NMessage> &message)
	{		
		// La acci�n no procesa mensajes
		
	}


} //namespace AI 
