#include "LABuild.h"

#include "Logic\Entity\Entity.h"
#include "Logic\Maps\EntityFactory.h"
#include "Logic\Entity\Components\WorkmanState.h"
#include "Logic\Entity\Components\BuildComponent.h"
#include "Logic\Entity\Components\PrincessManager.h"
#include "Logic\Entity\Components\Life.h"
#include "Logic\MovementMessages.h"
#include "Logic\RouteCalculationMessages.h"
#include "Logic\UnitMessages.h"

#include "AI\Server.h"
#include "AI\EntityManager.h"
#include "AI\ResourceServer.h"
#include "Logic/AudioMessages.h"

#define BUILD_DIST 15

namespace AI 
{


//////////////////////////////
//	Implementaci�n de CLABuild
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
	CLatentAction::LAStatus CLABuild::OnStart()
	{	
		//std::cout << "START SETTING WELL" << std::endl;
		Logic::IComponent *ic = _entity->getComponent("CWorkmanState");
		assert(ic);
		_state = static_cast<Logic::CWorkmanState*>(ic);

		ic = _state->getBuildingEntity()->getComponent("CBuildComponent");
		assert(ic);
		_buildState = static_cast<Logic::CBuildComponent*>(ic);

		_princess= AI::CServer::getSingletonPtr()->getEntityManager()->getClosestEntity(AI::CEntityManager::EntityType::Princess,_entity->getPosition());
		ic =_princess->getComponent("CPrincessManager");
		assert(ic);
		_princessState = static_cast<Logic::CPrincessManager*>(ic);

		_buildLife=nullptr;
		ic = _state->getBuildingEntity()->getComponent("CLife");
		if(ic!=nullptr)
			_buildLife = static_cast<Logic::CLife*>(ic);

		float d=_entity->getPosition().distance(_state->getBuildingEntity()->getPosition());
		if(d>BUILD_DIST)
		{
			_state->setBuildingEntity(nullptr);
			return EXIT_SM_FAIL;
		}
		//if is builded check if we hace available resource to repair
		if(_buildState->isBuilded())
		{
			//if is indestructible don't need to repair it
			if(_buildState->isIndestructible())
			{
				_state->setBuildingEntity(nullptr);
				return EXIT_SM_SUCCESS;
			}
			//if we not already pay to repair and we haven't any resource to repair
			if(_buildState->getHealtReparation()==0 && !initRepair())
			{
				//exit
				_state->setBuildingEntity(nullptr);
				return EXIT_SM_FAIL;
			}
		}

		auto playSound = std::make_shared<PlayAudio>();
		playSound->eventName = "event:/WorkmanGoblin/repairing";
		_entity->emitMessageN(playSound);

		sendAnimationMessage("Build", true);
		return RUNNING;
	}

	/**
	M�todo invocado al final de la ejecuci�n de la acci�n,
	para que se realicen las tareas que son �nicamente necesarias
	al final (y no durante toda la vida de la acci�n).

	En la mayor�a de los casos este m�todo no hace nada.
	*/
	void CLABuild::OnStop()
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
	CLatentAction::LAStatus CLABuild::OnRun(unsigned int msecs) 
	{
		//if it already builded
		if(_buildState->isBuilded())
		{
			//repair build
			_buildState->setCurrentTimeBuild(_buildState->getCurrentTimeBuild()+msecs);
//			std::cout << "repair.."<< _buildState->getCurrentTimeBuildbuild()<< std::endl;
			if(_buildState->getCurrentTimeBuild()>=_buildState->getTimeBuild())
			{	
				float healtLife = _buildState->getHealtReparation();
				_buildState->setHealtReparation(0);
				auto m = std::make_shared<HealUnit>();
				m->healing = healtLife;
				_buildState->getEntity()->emitMessageN(m);
				//std::cout << "reparation ended" << std::endl;
				//PAY ATTENTION if the entity hasn't a life we don't repair that
				//if build don't need more repair action
				if(_buildLife==nullptr || _buildLife!=nullptr && _buildLife->getCurrentLife()+healtLife>=_buildLife->getMaxLife())
				{
					_state->setBuildingEntity(nullptr);
					return EXIT_SM_SUCCESS;
				}else//restart the state to continue the raparation
				{
					std::cout << "continue to repair" << std::endl;
					this->reset();
					return READY;
				}
			}
		}else
		{
			//else build build if the princess is near
			float d = _state->getBuildingEntity()->getPosition().distance(_princess->getPosition());
			if(!_princessNeeded || _princessNeeded && d<=_princessState->getWrathAreaRadius())
			{
				_buildState->setCurrentTimeBuild(_buildState->getCurrentTimeBuild()+msecs);
//					std::cout << "setting.."<< _wellState->getCurrentTimeBuildWell()<< std::endl;
				if(_buildState->getCurrentTimeBuild()>=_buildState->getTimeBuild())
				{	
					_buildState->setBuilded();
					_state->setBuildingEntity(nullptr);
					std::cout << "WELL SET COMPLETED" << std::endl;
					return EXIT_SM_SUCCESS;			
				}

			}else
				return FAIL;
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
	CLatentAction::LAStatus CLABuild::OnAbort() 
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
	bool CLABuild::acceptN(const std::shared_ptr<NMessage> &message)
	{
		// La acci�n no acepta mensajes
		return false;
	}
	/**
	Procesa el mensaje recibido. El m�todo es invocado durante la
	ejecuci�n de la acci�n cuando se recibe el mensaje.

	@param msg Mensaje recibido.
	*/
	void CLABuild::processN(const std::shared_ptr<NMessage> &message)
	{		

	}

	bool CLABuild::initRepair()
	{
		//check how much life miss
		float lifePercentage=  (float)_buildLife->getCurrentLife()/_buildLife->getMaxLife();
		float reparationCost = 0.5;
		//if we haven't enough resource
		if(!AI::ResourceServer::getInstance()->checkResourceAvailability(_state->getBuildingEntity()->getType().c_str(),reparationCost))
		{
			std::cout<<"initRepair res not available lp="<<reparationCost<<std::endl;
			return false;
		}else
		{
			//consume the needed resources
			AI::ResourceServer::getInstance()->useResource(_state->getBuildingEntity()->getType().c_str(),reparationCost);
			_buildState->setHealtReparation(_buildLife->getMaxLife()-_buildLife->getCurrentLife());
			_buildState->setCurrentTimeBuild((unsigned int)(_buildState->getTimeBuild()*lifePercentage));
			std::cout<<"initRepair ok lp="<<lifePercentage<<std::endl;
			return true;
		}
	}


} //namespace AI 
