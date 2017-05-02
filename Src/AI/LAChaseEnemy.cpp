#include "LAChaseEnemy.h"

#include "AI/Server.h"

#include "Logic\Entity\Entity.h"
#include "Logic/Entity/Components/CombatStats.h"
#include "Logic/Entity/Components/StateMachineExecutor.h"
#include "Logic/Entity/Components/SteeringMovement.h"
#include "Logic/Entity/Components/EnemyDetectionManager.h"

#include "Logic/EntityMessages.h"
#include "Logic/EnemyMessages.h"
#include "Logic/UnitMessages.h"
#include "Logic/RouteCalculationMessages.h"
#include "Logic/CombatMessages.h"

#define MAX_TIME_TRYING_TO_ATTACK 8000
#define MAX_RANGE_TRYING_TO_ATTACK 20

namespace AI 
{


//////////////////////////////
//	Implementación de CLAChaseEnemy
//////////////////////////////

	//---------------------------------------------------------------
	
	CLatentAction::LAStatus CLAChaseEnemy::OnStart()
	{	
		//std::cout << "START CHASING.." << std::endl;

		sendAnimationMessage("Walk");

		combatComponent = static_cast<Logic::CCombatStats*>(_entity->getComponent("CCombatStats"));		

		_currentEnemy = combatComponent->getCurrentEnemy();
		if(_currentEnemy == nullptr)
		{
			return EXIT_SM_SUCCESS;
		}
		//En caso de errores con las lista de enemigos, para que no pete
		if(_entity->isEnemy() && _currentEnemy->isEnemy())
		{
			auto mPK = std::make_shared<ProtectKnight>();
			_entity->emitMessageN(mPK);	
		}

		_attackRange = combatComponent->getAttackRange();		
		
		enemyDetectorComponent = static_cast<Logic::CEnemyDetectionManager*>(_entity->getComponent("CEnemyDetectionManager"));
		smExecutorComponent = static_cast<Logic::CStateMachineExecutor*>( _entity->getComponent("CStateMachineExecutor"));		
		enemySteeringComponent = static_cast<Logic::CSteeringMovement*>(_currentEnemy->getComponent("CSteeringMovement"));

		_timeTryingToAttack = 0;
		_tryingToAttack = false;
		_atRange = false; 	
		
		float dist = _entity->getPosition().distance(_currentEnemy->getPosition());
		if(dist <= _attackRange * 4/5)
		{
			return SUCCESS;
		}
		else
			//Destination: enemy
			sendRouteMessage( _currentEnemy->getPosition(), true, smExecutorComponent);

		return RUNNING;
	}

	//---------------------------------------------------------------

	CLatentAction::LAStatus CLAChaseEnemy::OnRun(unsigned int msecs) 
	{		
		//Para cmabiar de enemigo en medio del chase (EX: atacan al caballero cuando persige a la princesa)
		if(_currentEnemy != combatComponent->getCurrentEnemy())
		{
			//std::cout << "Cambiando: " <<_currentEnemy->getName() << " por " << combatComponent->getCurrentEnemy()->getName() << std::endl;
			_currentEnemy = combatComponent->getCurrentEnemy();

			//TODO check combatComponent->getCurrentEnemy() is not null
			if(_currentEnemy == nullptr)
			{
				return EXIT_SM_SUCCESS;
			}

			//Para cambiar de estado en medio de la persecucion de la princesa(si ha sido capturada)
			if(_entity->isEnemy() && _currentEnemy->isEnemy())
			{
				auto mPK = std::make_shared<ProtectKnight>();
				_entity->emitMessageN(mPK);	
			}
			else
				sendRouteMessage( _currentEnemy->getPosition(), true, smExecutorComponent);
		}

		Vector3 enemySpeed = Vector3::ZERO;

		if(enemySteeringComponent)
			enemySpeed = enemySteeringComponent->getCurrentSpeed();

		/* Expamea demasiado la consola
		if(_currentEnemy)
			std::cout << _currentEnemy->getName() << std::endl;
		*/

		//If the enemy has been asigned by the Enemy Detector, checks whether the entity has walked too far from the defending position. If so, the entity goes back
		if(enemyDetectorComponent && enemyDetectorComponent->isDefendingPosition() &&
			_entity->getPosition().distance(enemyDetectorComponent->getPositionToDefend()) > enemyDetectorComponent->getDefendingDistance())
		{	
			sendRouteMessage(enemyDetectorComponent->getPositionToDefend(),true,combatComponent);	
		}
		else
		{
			if( _currentEnemy!= nullptr)
			{
				//We calculate whether the position is within the attack range
				float dist = _entity->getPosition().distance(_currentEnemy->getPosition());
				if(dist <= _attackRange * 4/5)
				{
					//If so, the entity stops
					sendStopMessage();
					auto rotationMessage = std::make_shared<AvatarControl>();
					rotationMessage->controlType = "turn";
					Vector3 direction = _currentEnemy->getPosition() - _entity->getPosition();
					double rotation = AI::CServer::correctAngle(atan2(-direction.x, -direction.z) - _entity->getYaw());
					rotationMessage->attribute = (float)rotation;
					_entity->emitMessageN(rotationMessage);		
				}
				else
				{
					//If not, the entity keep chasing the enemy, only if it is not still
					if(enemySpeed != Vector3::ZERO)
					{
						sendRouteMessage( _currentEnemy->getPosition(), true, smExecutorComponent);
						_tryingToAttack = false;
						_timeTryingToAttack = 0;
					}
					else
					{
						if(_tryingToAttack && _timeTryingToAttack > MAX_TIME_TRYING_TO_ATTACK)
						{
							//If the entity is able to defend positions, it tries to reach another one different from the unreachable one
							if(!(enemyDetectorComponent && enemyDetectorComponent->searchEnemies(_currentEnemy))){
								//If not, the entity stops
								combatComponent->setCurrentEnemy(NULL);
								_currentEnemy = NULL;					
								sendStopMessage();
							}
						}
						else
						{
							//If the entity has get closed enough to the enemy and now is looking for some space,
							//we flag it so
							if( _entity->getPosition().distance(_currentEnemy->getPosition()) <= MAX_RANGE_TRYING_TO_ATTACK && !_tryingToAttack)
							{
								_tryingToAttack = true;
							}
							//If the entity is looking for space to attack, we count the how long has been doing it
							if(_tryingToAttack)
							{
								_timeTryingToAttack += msecs;
							}
						}
					}
					
				}
			}
		}
		return RUNNING;
	}

	//---------------------------------------------------------------

	void CLAChaseEnemy::OnStop()
	{
	}

	//---------------------------------------------------------------

	CLatentAction::LAStatus CLAChaseEnemy::OnAbort() 
	{
		// Cuando se aborta se queda en estado terminado con fallo
		return FAIL;
	}

	//---------------------------------------------------------------

	bool CLAChaseEnemy::acceptN(const std::shared_ptr<NMessage> &message)
	{		
		return (message->type.compare("EntityDestroying") == 0) || (message->type.compare("AvatarFinishedRoute") == 0) 
			|| (message->type.compare("EnemyLost") == 0) || (message->type.compare("AvatarRouteTo") == 0) || (message->type.compare("AttackEnemy") == 0);
	}

	//---------------------------------------------------------------

	void CLAChaseEnemy::processN(const std::shared_ptr<NMessage> &message)
	{		
		if(message->type.compare("EntityDestroying") == 0)
		{
			std::shared_ptr<EntityDestroying> m = std::static_pointer_cast<EntityDestroying>(message);
			
			if(_currentEnemy==m->entity)
			{
				//If the destroyed entity is the current target, this entity cancels the action and stops moving
				combatComponent->setCurrentEnemy(nullptr);	
				_currentEnemy = nullptr;
				sendStopMessage();				
			}			
		}
		else if(message->type.compare("AvatarFinishedRoute") == 0)
		{
//			std::cout<<"CHASE recive AvatarFinishedRoute"<<_routeFinished<<std::endl;
			//When the entity stops moving, the chase ends
			if(_currentEnemy != NULL && _entity->getPosition().distance(_currentEnemy->getPosition()) <= _attackRange)
			{
				//if the target is not destroyed and the entity ends its movement within the attack range, we move to the next state
				this->finish(true);
			}
			else
				//if the target has been destroyed, the entity leaves the current state LAExecuteSM with success
				this->exitSM(true);
		}
		else if((message->type.compare("EnemyLost") == 0) && (_currentEnemy->getType().compare("Princess") != 0))
		{
			auto m = std::static_pointer_cast<EnemyLost>(message);
			
			if(enemyDetectorComponent && enemyDetectorComponent->isDefendingPosition())
			{	
				sendRouteMessage(enemyDetectorComponent->getPositionToDefend(),true);	
			}
			else if(_currentEnemy==m->enemy)
			{
				combatComponent->setCurrentEnemy(nullptr);
				_currentEnemy = nullptr;
				sendStopMessage();			
			}
		}
		else if(message->type.compare("AvatarRouteTo") == 0)
		{
			//If the entity receives a new position, the entity cancels the current action and leaves the current state LAExecuteSM with failure
			combatComponent->setCurrentEnemy(nullptr);
			_currentEnemy = nullptr;
			this->exitSM(false);
		}
		else if(message->type.compare("AttackEnemy") == 0)
		{
			this->reset();
		}
	}

	void CLAChaseEnemy::sendRouteMessage(Vector3 target,bool recalculateRoute,Logic::IComponent *component)
	{		
		auto m = std::make_shared<AvatarRouteTo>();		
		m->target = target;
		m->sender = _entity;
		m->calculateNewRoute = recalculateRoute;
		_entity->emitMessageN(m,component);
//		std::cout<<"CHASE ENEMY send message AvatarRouteTo"<<std::endl;
	}

	void CLAChaseEnemy::sendStopMessage()
	{
		auto m = std::make_shared<AvatarInterruptRoute>();
		_entity->emitMessageN(m);
	}

} //namespace AI 
