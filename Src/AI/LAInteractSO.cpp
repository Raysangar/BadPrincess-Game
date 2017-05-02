#include "LAInteractSO.h"
#include "Server.h"
#include "AI/EntityManager.h"
#include "Logic\Entity\Entity.h"
#include "Logic/Entity/Components/WorkmanState.h"
#include "Logic\UnitMessages.h"
#include "Logic\MovementMessages.h"
#include "Logic\AudioMessages.h"
#include "Logic\SmartObjectMessages.h"
#include "Logic\WorkmanMessages.h"
#include "Logic/RouteCalculationMessages.h"


namespace AI 
{
	bool CLAInteractSO::acceptN(const std::shared_ptr<NMessage> &message)
	{
		return (message->type.compare("AvatarMoveToEntity") == 0) || (message->type.compare("SmartDataAction") == 0) || (message->type.compare("ResourceAutoDestroy") == 0) || (message->type.compare("AvatarRouteTo") == 0) || (message->type.compare("AvatarFinishedRoute") == 0);
	}

	void CLAInteractSO::processN(const std::shared_ptr<NMessage> &message)
	{
		if(message->type.compare("AvatarMoveToEntity") == 0)
		{
			// Cuando el usuario indica que la unidad debe mover hacia una nueva smartEntity...
			std::shared_ptr<AvatarMoveToEntity> m = std::static_pointer_cast<AvatarMoveToEntity>(message);

			// Si la smartEntity no es la que actualmente se está utilizando...
			if(stateComponent != nullptr && m->target != stateComponent->getSmartEntity())
			{
				// Se elimina a la unidad como usuario de la smartEntity que se actualmente se está utilizando.	
				auto removeMessage = std::make_shared<removeFromWorkmanList>();
				removeMessage->sender = _entity;
				stateComponent->getSmartEntity()->emitMessageN(removeMessage);	

				// Se libera la smartPosition que se estaba utilizando.
				freeSmartPosition();

				// Se almacena que la nueva smartEntity es la que se está utilizando.
				stateComponent->setSmartEntity(m->target);

				currentTimeGather = 0;

				// La acción latente fracasa.
				this->abort();
			}
		}
		else if(message->type.compare("SmartDataAction") == 0)
		{
			// Cuando se recibe la forma de interacción...

			std::shared_ptr<SmartDataAction> m = std::static_pointer_cast<SmartDataAction>(message);

			// Se apunta el tipo de recurso que se estaba recolectando.
			ResourceType lastType = stateComponent->getResourceType();

			// Se actualiza la acción a realizar en el stateComponent.
			stateComponent->setAction(static_cast<Logic::CWorkmanState::WorkmanAction>(m->actionType));

			// Se guarda la animación que corresponde a la acción a realizar.
			animation = m->animation;

			if(stateComponent->getAction() != Logic::CWorkmanState::storeResource && stateComponent->getAction() != Logic::CWorkmanState::gatherFull)
			{
				// Se actualiza el tipo de recurso que se va a recolectar en el stateComponent.
				switch(stateComponent->getAction())
				{
				case  Logic::CWorkmanState::gatherWood:
					stateComponent->setResourceType(ResourceType::Wood);
					break;
				case  Logic::CWorkmanState::gatherIron:
					stateComponent->setResourceType(ResourceType::Iron);
					break;
				case  Logic::CWorkmanState::gatherStone:
					stateComponent->setResourceType(ResourceType::Stone);
					break;
				default:
					//stateComponent->setResourceType(ResourceType::none);
					break;
				}

				// Si el nuevo tipo de recurso a recolectar es distinto del tipo que ya se estaba recolectando, 
				// la unidad se deshace de los recursos que estaba cargando.
				if(lastType != stateComponent->getResourceType())
				{
						stateComponent->setCurrentCharge(0);
						// Se modifica la velocidad de movimiento por la velocidad por defecto.
						auto modidySpeedMessage= std::make_shared<ModifyMaxLinearSpeed>();
						modidySpeedMessage->speed = stateComponent->getSpeed();
						_entity->emitMessageN(modidySpeedMessage);
				}
			}
			
			// Se reanuda la acción latente.
			this->resume();
		}
		else if(message->type.compare("ResourceAutoDestroy") == 0)
		{
			// Cuando la smartEntity que se está utilizando resulta destruida...
			//std::cout << "DESTROYED" << std::endl;

			std::shared_ptr<ResourceAutoDestroy> m = std::static_pointer_cast<ResourceAutoDestroy>(message);

			// Si hay una smartEntity "siguiente", se almacena en el stateComponent de la unidad.
			if(stateComponent != nullptr && m->nextItem != nullptr)
			{
				stateComponent->setSmartEntity(m->nextItem);
				stateComponent->setLastResource(m->nextItem);
					
				// Si la unidad estaba recolectando, se aborta la acción latente para que continúe recolectando en el recurso más cercano.
				if(stateComponent->getAction() == Logic::CWorkmanState::gatherWood|| stateComponent->getAction() == Logic::CWorkmanState::gatherIron  || stateComponent->getAction() == Logic::CWorkmanState::gatherStone)
						this->abort();
			// Si NO hay una smartEntity "siguiente", se almacena "nullptr".
			}else if(stateComponent != nullptr && m->nextItem == nullptr)
			{
				stateComponent->setSmartEntity(nullptr);
				stateComponent->setLastResource(nullptr);

				// Si la unidad estaba recolectando, como no existe un recurso cercano,
				// la unidad considera que no puede recolectar más.
				if(stateComponent->getAction() == Logic::CWorkmanState::gatherWood|| stateComponent->getAction() == Logic::CWorkmanState::gatherIron  || stateComponent->getAction() == Logic::CWorkmanState::gatherStone)
				{
					stateComponent->setAction(Logic::CWorkmanState::gatherFull);
					this->resume();
				}
			}	
		}
		else if (message->type.compare("AvatarRouteTo") == 0)
		{
			// Cuando la unidad recibe un mensaje de ruta externo...

			// Se libera la smartPosition que la unidad estaba utilizando.
			freeSmartPosition();

			currentTimeGather = 0;

			// Se deja de utilizar la smartEntity actual.
			stateComponent->setSmartEntity(nullptr);
		}else if (message->type.compare("AvatarFinishedRoute") == 0)
		{
			sendAnimationMessage("Idle");
		}
	}

	CLatentAction::LAStatus CLAInteractSO::OnStart()
	{	
		// Si no se ha recuperado aún el stateComponent de la unidad, entonces se recupera.
		if(_entity != nullptr)
		{
			if(stateComponent == nullptr)
			{
				Logic::IComponent *iComponent = _entity->getComponent("CWorkmanState");

				if(iComponent != nullptr)
					stateComponent = static_cast<Logic::CWorkmanState*>(iComponent);
			}
		}

		// La unidad mira hacia la smartEntity.
		lookAtSmartObject();

		// Se le indica a la smartEntity que la unidad está en la smartPosition, lista para recibir la forma de interacción.
		if(stateComponent->getSmartEntity() != nullptr)
		{
			auto m = std::make_shared<SmartDataAvatarReady>();
			m->entity = _entity;
			stateComponent->getSmartEntity()->emitMessageN(m);

			auto playAudioMessage = std::make_shared<PlayAudio>();
			playAudioMessage->eventName = "default";
			stateComponent->getSmartEntity()->emitMessageN(playAudioMessage);
		}

		// se suspende la ejecución de la acción latente a la espera de que la smartEntity envíe la forma de interacción.
		return SUSPENDED;
	}

	CLatentAction::LAStatus CLAInteractSO::OnRun(unsigned int msecs) 
	{

		if(stateComponent->getCurrentCharge() > stateComponent->getGatherMax())
		{
			// El obrero nunca debe llevar recursos por encima de su carga máxima.
			stateComponent->setCurrentCharge(stateComponent->getGatherMax());
		}

		// Según sea la acción a realizar...

			switch (stateComponent->getAction())
			{
				// Si se trata de recolectar algún tipo de recurso:
				case Logic::CWorkmanState::gatherWood:
				case Logic::CWorkmanState::gatherIron:
				case Logic::CWorkmanState::gatherStone:
				{
					// Si el recurso fue destruido la acción latente fracasa.
					if(stateComponent->getSmartEntity() == nullptr)
						this->abort();

					// Si la unidad ha llegado a su capacidad máxima de carga, no podrá seguir recolectando.
					if(stateComponent->getCurrentCharge() >= stateComponent->getGatherMax())
					{
						stateComponent->setAction(Logic::CWorkmanState::gatherFull);
						currentTimeGather = 0;

						auto stopAudioMessage = std::make_shared<StopAudio>();
						stateComponent->getSmartEntity()->emitMessageN(stopAudioMessage);

						break;
					}

					// Se activa la animación que concuerda con la forma de interacción del recurso.
					sendAnimationMessage(animation);

					// Se incrementa el tiempo que la unidad lleva recolectando.
					currentTimeGather +=  stateComponent->getTime();
		
					// Cuando se llega al tiempo que tarda en recolectar la unidad.
					if(currentTimeGather >= stateComponent->getGatherTime())
					{
						if(stateComponent->getCurrentCharge() > 0) // Evita división por cero.
						{
							// Se modifica la velocidad de movimiento, para reducirla en función de la carga actual.
							auto modidySpeedMessage= std::make_shared<ModifyMaxLinearSpeed>();
							modidySpeedMessage->speed = stateComponent->getSpeed() - (stateComponent->getSpeed() * 0.07f /* Sacar a MAP!! */ * (stateComponent->getCurrentCharge() / stateComponent->getGatherUnit()));
							_entity->emitMessageN(modidySpeedMessage);
						}
						// Se aumenta la carga actual de la unidad en su stateComponent.
						stateComponent->setCurrentCharge(stateComponent->getCurrentCharge() + stateComponent->getGatherUnit());

						// Se reinicia la cuenta del tiempo que lleva recolectando.
						currentTimeGather = 0; //-= stateComponent->getGatherTime();

						// Se le restan recursos a la smartEntity que se está utilizando.
						auto m = std::make_shared<GatherMessage>();
						m->quantity = stateComponent->getGatherUnit();
						stateComponent->getSmartEntity()->emitMessageN(m);
					}
					break;
				}
				// Si se trata de almacenar algún tipo de recurso:
				case Logic::CWorkmanState::storeResource:
				{
					// Se libera la smarPosition que se estaba utilizando (la del cofre).
					freeSmartPosition();

					// Se suman los recursos del tipo recolectado al contador corresponiendte.
					AI::ResourceServer::getInstance()->addResource(stateComponent->getResourceType(),stateComponent->getCurrentCharge());

					// Se libera a la unidad de su carga, actualizando su stateComponent.
					stateComponent->setCurrentCharge(0);

					// Se modifica la velocidad de movimiento por la velocidad por defecto.
					auto modidySpeedMessage= std::make_shared<ModifyMaxLinearSpeed>();
					modidySpeedMessage->speed = stateComponent->getSpeed();
					_entity->emitMessageN(modidySpeedMessage);


					// Se recupera el último recurso utilizado, para regresar a él.
					stateComponent->setSmartEntity(stateComponent->getLastResource());

					// La acción latente finaliza con éxito.
					return SUCCESS;		
					break;
				}
				// Si se ha alcanzado el máximo de capacidad de la unidad.
				case Logic::CWorkmanState::gatherFull:
				{
					// Se libera la smarPosition que se estaba utilizando (la del recurso).
					freeSmartPosition();

					// Se busca le cofre más cercano.
					Logic::CEntity* closestChest = AI::CServer::getSingletonPtr()->getEntityManager()->getClosestEntity(AI::CEntityManager::EntityType::Chest,_entity->getPosition());
					
					// Se apunta cúal ha sido el último recurso (smartEntity) utilizado, para regresar más tarde.
					stateComponent->setLastResource(stateComponent->getSmartEntity());

					// Se pone como smartEntity en uso el cofre.
					stateComponent->setSmartEntity(closestChest);

					//La acción latente finaliza con éxito.
					return SUCCESS;
					break;
				}
				default:
					break;
				}

				// La acción latente se continúa ejecutando.
				return RUNNING;
	}

	CLatentAction::LAStatus CLAInteractSO::OnAbort() 
	{
		return FAIL;
	}

	void CLAInteractSO::lookAtSmartObject()
	{	
		auto rotationMessage = std::make_shared<AvatarControl>();
		rotationMessage->controlType = "turn";
		Vector3 direction = stateComponent->getSmartEntity()->getPosition() - _entity->getPosition();
		double rotation = AI::CServer::correctAngle(atan2(-direction.x, -direction.z) - _entity->getYaw());
		rotationMessage->attribute = (float) rotation;
		_entity->emitMessageN(rotationMessage);
	}

	void CLAInteractSO::freeSmartPosition()
	{
		auto freeMessage = std::make_shared<SmartDataFreePosition>();
		freeMessage->entity=_entity;
		freeMessage->position=stateComponent->getPosition();
		if(stateComponent->getSmartEntity() != nullptr)
		{
			stateComponent->getSmartEntity()->emitMessageN(freeMessage);
		}
	}
	
} //namespace AI 