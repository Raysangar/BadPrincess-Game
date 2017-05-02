#include "LAFaint.h"

#include "Application/BaseApplication.h"

#include "Logic\Server.h"
#include "Logic\Maps\Map.h"
#include "Logic\Entity\Entity.h"

#include "Logic/Entity/Components/PrincessManager.h"

#include "Logic/PrincessMessages.h"
#include "Logic/RouteCalculationMessages.h"
#include "Logic/AllyUnitMessages.h"
#include "Logic/UnitMessages.h"
#include "Logic/AudioMessages.h"

#include "Logic/Entity/Components/Life.h"
#include "Logic/Entity/Components/CombatStats.h"



#include "GUI/PlayerController.h"
#include "GUI/Server.h"

namespace AI 
{


//////////////////////////////
//	Implementaci�n de CLAFaint
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
	CLatentAction::LAStatus CLAFaint::OnStart()
	{
		sendAnimationMessage("Death",false);
		
		auto m = std::make_shared<AvatarInterruptRoute>();
		_entity->emitMessageN(m);

		auto m2 = std::make_shared<ModifyWrath>();
		m2->reason = WrathModifiers::faint;
		_entity->emitMessageN(m2);

		_princessManagerComponent = static_cast<Logic::CPrincessManager*>(_entity->getComponent("CPrincessManager"));
		_lifeComponent = static_cast<Logic::CLife*>(_entity->getComponent("CLife"));
		_combatComponent = static_cast<Logic::CCombatStats*>(_entity->getComponent("CCombatStats"));

		_combatComponent->setCurrentEnemy(nullptr);

		_princessManagerComponent->setIsUnconscius(true);

		if(_princessManagerComponent->getRaptor() != NULL)
			_princessManagerComponent->setRaptor(NULL);

		_endingTime = Application::CBaseApplication::getSingletonPtr()->getAppTime() + _princessManagerComponent->getTimeToAwake();

		GUI::CServer::getSingletonPtr()->getPlayerController()->removeEntityFromSelectedUnits(_entity);
		
		auto m3 = std::make_shared<AvatarSelection>();
		m3->selected = false;
		_entity->emitMessageN(m3);

		auto m4 = std::make_shared<AvatarActivation>();
		m4->actived = false;
		_entity->emitMessageN(m3);

		GUI::CServer::getSingletonPtr()->getPlayerController()->setTypeIsClickable(1, "Princess", false);
		
		_entity->deactivate("CUnitSelected");

		return RUNNING;
	}

	/**
	M�todo invocado al final de la ejecuci�n de la acci�n,
	para que se realicen las tareas que son �nicamente necesarias
	al final (y no durante toda la vida de la acci�n).

	En la mayor�a de los casos este m�todo no hace nada.
	*/
	void CLAFaint::OnStop()
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
	CLatentAction::LAStatus CLAFaint::OnRun(unsigned int msecs) 
	{
		if(Application::CBaseApplication::getSingletonPtr()->getAppTime() >= _endingTime)
		{	
			auto m = std::make_shared<HealUnit>();
			m->healing = _lifeComponent->getMaxLife() / 2;
			_entity->emitMessageN(m);
			_entity->activate("CUnitSelected");
			GUI::CServer::getSingletonPtr()->getPlayerController()->setTypeIsClickable(1, "Princess", true);
			_princessManagerComponent->setIsUnconscius(false);

			Logic::CServer::getSingletonPtr()->getMap()->sendMessageToAll(std::make_shared<PrincessIsFree>());

			Logic::CEntity* gameManager = Logic::CServer::getSingletonPtr()->getMap()->getEntityByName("GameManager");
			
			auto backgroundVolumen = std::make_shared<SetParameterValue>();
			backgroundVolumen->eventName = "default";
			backgroundVolumen->parameterName = "Volumen";
			backgroundVolumen->value = 0;
			gameManager->emitMessageN(backgroundVolumen);

			auto stopDrums = std::make_shared<StopAudio>();
			stopDrums->eventName = "event:/Music/Drums";
			gameManager->emitMessageN(stopDrums);


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
	CLatentAction::LAStatus CLAFaint::OnAbort() 
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
	bool CLAFaint::acceptN(const std::shared_ptr<NMessage> &message)
	{
		return false;
	}
	/**
	Procesa el mensaje recibido. El m�todo es invocado durante la
	ejecuci�n de la acci�n cuando se recibe el mensaje.

	@param msg Mensaje recibido.
	*/
	void CLAFaint::processN(const std::shared_ptr<NMessage> &message)
	{		
	}


} //namespace AI 
