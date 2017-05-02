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
//	Implementación de CLAFaint
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
	Método invocado al final de la ejecución de la acción,
	para que se realicen las tareas que son únicamente necesarias
	al final (y no durante toda la vida de la acción).

	En la mayoría de los casos este método no hace nada.
	*/
	void CLAFaint::OnStop()
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
	Método invocado cuando la acción ha sido cancelada (el comportamiento
	al que pertenece se ha abortado por cualquier razón).

	La tarea puede en este momento realizar las acciones que
	considere oportunas para "salir de forma ordenada".

	@note <b>Importante:</b> el Abort <em>no</em> provoca la ejecución
	de OnStop().
	*/
	CLatentAction::LAStatus CLAFaint::OnAbort() 
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
	bool CLAFaint::acceptN(const std::shared_ptr<NMessage> &message)
	{
		return false;
	}
	/**
	Procesa el mensaje recibido. El método es invocado durante la
	ejecución de la acción cuando se recibe el mensaje.

	@param msg Mensaje recibido.
	*/
	void CLAFaint::processN(const std::shared_ptr<NMessage> &message)
	{		
	}


} //namespace AI 
