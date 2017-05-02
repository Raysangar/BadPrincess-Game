#include "LAAttackEnemy.h"

#include "Application/BaseApplication.h"

#include "Logic\Entity\Entity.h"
#include "Logic/Entity/Components/CombatStats.h"
#include "Logic/Entity/Components/EnemyDetectionManager.h"
#include "Logic/Entity/Components/Life.h"
#include "Logic/EnemyMessages.h"
#include "Logic/PrincessMessages.h"
#include "Logic/CombatMessages.h"
#include "Logic/EntityMessages.h"
#include "Logic/MovementMessages.h"
#include "AI/Server.h"
#include "GameManager.h"
#include "Logic/UnitMessages.h"

#define MIN_ATTACK_DISTANCE_MULTIPLICATOR 1.5f

namespace AI 
{


//////////////////////////////
//	Implementación de CLAAttackEnemy
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
	CLatentAction::LAStatus CLAAttackEnemy::OnStart()
	{	


		auto interruptMessage = std::make_shared<AvatarInterruptMovement>();		
		_entity->emitMessageN(interruptMessage);

		combatComponent = static_cast<Logic::CCombatStats*>(_entity->getComponent("CCombatStats"));
		enemyDetectorComponent = static_cast<Logic::CEnemyDetectionManager*>(_entity->getComponent("CEnemyDetectionManager"));

		_attackDamage = combatComponent->getAttackDamage();
		_currentEnemy = combatComponent->getCurrentEnemy();
		_attackRange = combatComponent->getAttackRange();
		_timeBetweenAttacks = combatComponent->getTimeBetweenAttacks();		
		_attackAnimation = combatComponent->getAttackAnimation();
		if(_currentEnemy)
		{
			//En caso de errores con las lista de enemigos, para que no pete
			if(_entity->isEnemy() && _currentEnemy->isEnemy())
			{
				auto mPK = std::make_shared<ProtectKnight>();
				_entity->emitMessageN(mPK);	
			}

			_newAttackTime = combatComponent->getTimeOfLastAttack() + _timeBetweenAttacks;
			unsigned int currentTime = Application::CBaseApplication::getSingletonPtr()->getAppTime(); 
			if (currentTime >= _newAttackTime)
			{
				_newAttackTime = currentTime + _timeBetweenAttacks;
				combatComponent->setTimeOfLastAttack(currentTime);
				auto m = std::make_shared<DamageMessage>();
				m->damage = _attackDamage;
				_currentEnemy->emitMessageN(m, combatComponent);
			}
		
			sendAnimationMessage(_attackAnimation);

			auto attackingMessage = std::make_shared<Attacking>();
			attackingMessage->attacking = true;
			_entity->emitMessageN(attackingMessage);
		}
		return RUNNING;
	}

	/**
	Método invocado al final de la ejecución de la acción,
	para que se realicen las tareas que son únicamente necesarias
	al final (y no durante toda la vida de la acción).

	En la mayoría de los casos este método no hace nada.
	*/
	void CLAAttackEnemy::OnStop()
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
	CLatentAction::LAStatus CLAAttackEnemy::OnRun(unsigned int msecs) 
	{
		if(_currentEnemy!=nullptr)
		{	
			_attackDamage = combatComponent->getAttackDamage();
			//Para que el caballero se quede quieto y no siga atacando a la princesa mientras se implementa el secuestro
			if(static_cast<Logic::CLife*>(_currentEnemy->getComponent("CLife"))->getCurrentLife() <= 0 && _currentEnemy->getType().compare("Princess") == 0)
			{
				if(_entity->getType() == "Knight")
				{
					//Para evitar que 2 caballeros secuestren a la misma princesa en el mismo tick, solo el primero en ejecutar esto se la lleva
					if(AI::CServer::getSingletonPtr()->getGameManager()->registerPrincessKidnapped(_currentEnemy, _entity))
						return EXIT_SM_FAIL;
					else
						return EXIT_SM_SUCCESS;
				}
				
			}

			if(_currentEnemy != combatComponent->getCurrentEnemy() && _entity->isEnemy()  && _currentEnemy->getType() == "Well")
			{
				return EXIT_SM_SUCCESS;
			}

			auto rotationMessage = std::make_shared<AvatarControl>();
			rotationMessage->controlType = "turn";
			Vector3 direction = _currentEnemy->getPosition() - _entity->getPosition();
			double rotation = AI::CServer::correctAngle(atan2(-direction.x, -direction.z) - _entity->getYaw());
			rotationMessage->attribute = (float)rotation;
			_entity->emitMessageN(rotationMessage);

			if(_entity->getPosition().distance(_currentEnemy->getPosition()) > _attackRange)
			{
				//std::cout << "ATTACK FAILED !! p="<<_entity->getPosition()<<" ep="<<_currentEnemy->getPosition()<<" d=" <<_entity->getPosition().distance(_currentEnemy->getPosition())<< std::endl;
 				return FAIL;
			}

			if(Application::CBaseApplication::getSingletonPtr()->getAppTime() >= _newAttackTime)
			{
				_newAttackTime = Application::CBaseApplication::getSingletonPtr()->getAppTime() + _timeBetweenAttacks;
				combatComponent->setTimeOfLastAttack(Application::CBaseApplication::getSingletonPtr()->getAppTime());
				//std::cout << "ATTACK!!" << std::endl;
				auto m = std::make_shared<DamageMessage>();
				m->damage = _attackDamage;
				_currentEnemy->emitMessageN(m, combatComponent);

				auto attackingMessage = std::make_shared<Attacking>();
				attackingMessage->attacking = true;
				_entity->emitMessageN(attackingMessage);

				if(_entity->getType() == "Princess")
				{
					auto m2 = std::make_shared<ModifyWrath>();
					m2->reason = 0;
					_entity->emitMessageN(m2);
				}
			}
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
	CLatentAction::LAStatus CLAAttackEnemy::OnAbort() 
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
	bool CLAAttackEnemy::acceptN(const std::shared_ptr<NMessage> &message)
	{
		return (message->type.compare("EntityDestroying") == 0) || (message->type.compare("AvatarRouteTo") == 0);
	}
	/**
	Procesa el mensaje recibido. El método es invocado durante la
	ejecución de la acción cuando se recibe el mensaje.

	@param msg Mensaje recibido.
	*/
	void CLAAttackEnemy::processN(const std::shared_ptr<NMessage> &message)
	{		
		if(message->type.compare("EntityDestroying") == 0)
		{
			std::shared_ptr<EntityDestroying> m = std::static_pointer_cast<EntityDestroying>(message);
			//std::cout<<"EntityDestroying dese="<<m->entity<<" cure="<<_currentEnemy<<std::endl;
			if(_currentEnemy==m->entity)
			{
				//We try to search any close enemy if the entity can defend position
				if(!(enemyDetectorComponent && enemyDetectorComponent->getCanKeepAttacking() && enemyDetectorComponent->searchEnemies(_currentEnemy)) && _entity->isPlayer())
				{
					//If there is no more enemies around, we leave the SM with success
					combatComponent->setCurrentEnemy(nullptr);
					_currentEnemy= nullptr;
					this->exitSM(true);
				}

				if(_entity->getType() == "Knight")
					this->exitSM(true);
				//combatComponent->setCurrentEnemy(nullptr);
			}
		}
		else if (message->type.compare("AvatarRouteTo") == 0)
		{			
			//If the entity receives a new position, the entity cancels the current action and leaves the current state LAExecuteSM with failure
			combatComponent->setCurrentEnemy(nullptr);
			_currentEnemy = nullptr;
			this->exitSM(false);
		}
		auto attackingMessage = std::make_shared<Attacking>();
		attackingMessage->attacking = false;
		_entity->emitMessageN(attackingMessage);

	}


} //namespace AI 
