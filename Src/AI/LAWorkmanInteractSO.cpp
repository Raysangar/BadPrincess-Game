#include "LAWorkmanInteractSO.h"
#include "Server.h"
#include "AI/EntityManager.h"
#include "AI\GameManager.h"
#include "Logic\Entity\Entity.h"
#include "Logic/Entity/Components/WorkmanState.h"
#include "Logic/Entity/Components/InteractSOState.h"
#include "Logic/Entity/Components/TrackComponent.h"
#include "Logic\UnitMessages.h"
#include "Logic\MovementMessages.h"
#include "Logic\AudioMessages.h"
#include "Logic\SmartObjectMessages.h"
#include "Logic\WorkmanMessages.h"
#include "Logic/RouteCalculationMessages.h"

#define CHEST_GET_AWAY_DISTANCE 30.0f
#define CHEST_GET_AWAY_ANGLE 120
namespace AI 
{
	bool CLAWorkmanInteractSO::acceptN(const std::shared_ptr<NMessage> &message)
	{
		return (message->type.compare("SmartDataAction") == 0);
	}

	void CLAWorkmanInteractSO::processN(const std::shared_ptr<NMessage> &message)
	{	
		if(message->type.compare("SmartDataAction") == 0)
		{
			// Cuando se recibe la forma de interacción...
			std::shared_ptr<SmartDataAction> m = std::static_pointer_cast<SmartDataAction>(message);

			// Se apunta el tipo de recurso que se estaba recolectando.
			ResourceType lastType = stateComponent->getResourceType();

			// Se actualiza la acción a realizar en el stateComponent.
			stateComponent->setAction(static_cast<Logic::CWorkmanState::WorkmanAction>(m->actionType));

			// Se guarda la animación que corresponde a la acción a realizar.
			animation = m->animation;
			// Se activa la animación que concuerda con la forma de interacción del recurso.
			sendAnimationMessage(animation);

			if(stateComponent->getAction() != Logic::CWorkmanState::storeResource /*&& stateComponent->getAction() != Logic::CWorkmanState::gatherFull*/)
			{
				std::string currentSoundPlaying = "";
				// Se actualiza el tipo de recurso que se va a recolectar en el stateComponent.
				switch(stateComponent->getAction())
				{
				case  Logic::CWorkmanState::gatherWood:
					stateComponent->setResourceType(ResourceType::Wood);
					currentSoundPlaying = "event:/Resources/CutTree";
					break;
				case  Logic::CWorkmanState::gatherIron:
					stateComponent->setResourceType(ResourceType::Iron);
					currentSoundPlaying = "event:/Resources/HitMetal";
					break;
				case  Logic::CWorkmanState::gatherStone:
					stateComponent->setResourceType(ResourceType::Stone);
					currentSoundPlaying = "event:/Resources/HitStone";
					break;
				default:
					//stateComponent->setResourceType(ResourceType::none);
					break;
				}

				if (currentSoundPlaying != "")
				{
					Logic::IComponent* iComponent = _entity->getComponent("CTrackComponent");
					assert(iComponent);
					Logic::CTrackComponent *trackComponent = static_cast<Logic::CTrackComponent*>(iComponent);
					trackComponent->changeAnimationTrackSoundName(animation,"gather",currentSoundPlaying);
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
	}

	CLatentAction::LAStatus CLAWorkmanInteractSO::OnStart()
	{	
		// Si no se ha recuperado aún el stateComponent de la unidad, entonces se recupera.
		//std::cout<<_entity->getName()<<" interact onstart "<<std::endl;

		Logic::IComponent *iComponent = _entity->getComponent("CWorkmanState");
		if(iComponent != nullptr)
			stateComponent = static_cast<Logic::CWorkmanState*>(iComponent);
		
		iComponent = _entity->getComponent("CInteractSOState");
		if(iComponent != nullptr)
			interactComponent = static_cast<Logic::CInteractSOState*>(iComponent);
		
		if(interactComponent->getSmartEntity()==nullptr)
			return EXIT_SM_FAIL;

		// La unidad mira hacia la smartEntity.
		lookAtSmartObject();

		// Se le indica a la smartEntity que la unidad está en la smartPosition, lista para recibir la forma de interacción.
		if(interactComponent->getSmartEntity() != nullptr)
		{
			auto readyMessage = std::make_shared<SmartDataAvatarReady>();
			readyMessage->entity = _entity;
			readyMessage->position = interactComponent->getSmartPosition();
			interactComponent->getSmartEntity()->emitMessageN(readyMessage);
		}

		// se suspende la ejecución de la acción latente a la espera de que la smartEntity envíe la forma de interacción.
		return SUSPENDED;
	}

	CLatentAction::LAStatus CLAWorkmanInteractSO::OnRun(unsigned int msecs) 
	{
		
		if(interactComponent->getSmartEntity() == nullptr)
		{
//		std::cout<<_entity->getName()<<" interact onrun smart point null"<<std::endl;
			if(stateComponent->getCurrentCharge() > 0)
			{
				goToChest();
				return SUSPENDED;
			}
			else
				return EXIT_SM_FAIL;
		}

		

		// Según sea la acción a realizar...

			switch (stateComponent->getAction())
			{
				// Si se trata de recolectar algún tipo de recurso:
				case Logic::CWorkmanState::gatherWood:
				case Logic::CWorkmanState::gatherIron:
				case Logic::CWorkmanState::gatherStone:
				{
					// Si la unidad ha llegado a su capacidad máxima de carga, no podrá seguir recolectando.
					if(checkGatherMaxReached())
						break;

					// Se incrementa el tiempo que la unidad lleva recolectando.
					currentTimeGather +=  stateComponent->getTime();
		
					// Cuando se llega al tiempo que tarda en recolectar la unidad.
					if(currentTimeGather >= stateComponent->getGatherTime())
					{
//						std::cout<<_entity->getName()<<" gather current="<<stateComponent->getCurrentCharge()<<std::endl;
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
						interactComponent->getSmartEntity()->emitMessageN(m);

					}
					break;
				}
				// Si se trata de almacenar algún tipo de recurso:
				case Logic::CWorkmanState::storeResource:
				{

					// Se suman los recursos del tipo recolectado al contador corresponiendte.
					AI::ResourceServer::getInstance()->addResource(stateComponent->getResourceType(),stateComponent->getCurrentCharge());

					// Se libera a la unidad de su carga, actualizando su stateComponent.
					stateComponent->setCurrentCharge(0);

					// Se modifica la velocidad de movimiento por la velocidad por defecto.
					auto modidySpeedMessage= std::make_shared<ModifyMaxLinearSpeed>();
					modidySpeedMessage->speed = stateComponent->getSpeed();
					_entity->emitMessageN(modidySpeedMessage);


					// Se recupera el último recurso utilizado, para regresar a él.
					//stateComponent->setSmartEntity(stateComponent->getLastResource());

					if(stateComponent->getLastResource() != nullptr)
					{
						// Se pide a la entidad que mueva hacia el smart object.
						auto moveMessage = std::make_shared<AvatarMoveToEntity>();
						moveMessage->target = stateComponent->getLastResource();
						_entity->emitMessageN(moveMessage);
					}
					else
					{
						
						//move the entity little far from chest when they stop near it
//						std::cout<<_entity->getName()<<" send far to chest"<<std::endl;
						auto routeMessage = std::make_shared<AvatarRouteTo>();
						routeMessage->target = AI::CServer::getSingletonPtr()->getGameManager()->getPositionFarToEntity( interactComponent->getSmartEntity(),_entity,CHEST_GET_AWAY_DISTANCE,CHEST_GET_AWAY_ANGLE);
						_entity->emitMessageN(routeMessage);

						return SUSPENDED;
					}
					break;
				}
				// Si se ha alcanzado el máximo de capacidad de la unidad.
/*				case Logic::CWorkmanState::gatherFull:
				{
					goToChest();
					break;
				}*/
				default:
					break;
				}

				// La acción latente se continúa ejecutando.
				return RUNNING;
	}

	CLatentAction::LAStatus CLAWorkmanInteractSO::OnAbort() 
	{
//		AI::CServer::getSingletonPtr()->getGameManager()->freeSmartPosition(interactComponent->getSmartEntity(),interactComponent->getSmartPosition(),_entity);
//		std::cout<<_entity->getName()<<" onabort send freesmartpos to "<<interactComponent->getSmartEntity()->getName()<<std::endl;
		return FAIL;
	}

	void CLAWorkmanInteractSO::OnStop() 
	{
//		AI::CServer::getSingletonPtr()->getGameManager()->freeSmartPosition(interactComponent->getSmartEntity(),interactComponent->getSmartPosition(),_entity);
//		std::cout<<_entity->getName()<<" onstop send freesmartpos to "<<interactComponent->getSmartEntity()->getName()<<std::endl;
	}

	void CLAWorkmanInteractSO::lookAtSmartObject()
	{	
		auto rotationMessage = std::make_shared<AvatarControl>();
		rotationMessage->controlType = "turn";
		Vector3 direction = interactComponent->getSmartEntity()->getPosition() - _entity->getPosition();
		double rotation = AI::CServer::correctAngle(atan2(-direction.x, -direction.z) - _entity->getYaw());
		rotationMessage->attribute = (float) rotation;
		_entity->emitMessageN(rotationMessage);
	}


	bool CLAWorkmanInteractSO::checkGatherMaxReached()
	{
		if(stateComponent->getCurrentCharge() >= stateComponent->getGatherMax())
		{
//			std::cout<<_entity->getName()<<" is Full="<<stateComponent->getCurrentCharge()<<std::endl;

//			stateComponent->setAction(Logic::CWorkmanState::gatherFull);
			currentTimeGather = 0;
			goToChest();
			return true;
		}else
			return false;
	}

	void CLAWorkmanInteractSO::goToChest()
	{
		if(stateComponent->getCurrentCharge() > stateComponent->getGatherMax())
			// El obrero nunca debe llevar recursos por encima de su carga máxima.
			stateComponent->setCurrentCharge(stateComponent->getGatherMax());
		// Se busca le cofre más cercano.
		Logic::CEntity* closestChest = AI::CServer::getSingletonPtr()->getEntityManager()->getClosestEntity(AI::CEntityManager::EntityType::Chest,_entity->getPosition());
			
		if(interactComponent->getSmartEntity() != nullptr)
			// Se apunta cúal ha sido el último recurso (smartEntity) utilizado, para regresar más tarde.
			stateComponent->setLastResource(interactComponent->getSmartEntity());

		// Se pide a la entidad que mueva hacia el smart object.
		auto moveMessage = std::make_shared<AvatarMoveToEntity>();
		moveMessage->target = closestChest;
		_entity->emitMessageN(moveMessage);	
	}
	
} //namespace AI 