#include "LARequestSP.h"
#include "Server.h"

#include "Logic\Entity\Entity.h"
#include "Logic/Entity/Components/WorkmanState.h"
#include "Logic/SmartObjectMessages.h"
#include "Logic/MovementMessages.h"
#include "Logic/RouteCalculationMessages.h"
#include "Logic/WorkmanMessages.h"

namespace AI 
{
	bool CLARequestSP::acceptN(const std::shared_ptr<NMessage> &message)
	{
		return (message->type.compare("SmartDataPositionReceiver") == 0) || (message->type.compare("AvatarMoveToEntity") == 0) || (message->type.compare("ResourceAutoDestroy") == 0) || (message->type.compare("AvatarFinishedRoute") == 0);
	}
	
	void CLARequestSP::processN(const std::shared_ptr<NMessage> &message)
	{
		if(message->type.compare("AvatarMoveToEntity") == 0 )
		{
			// Cuando el usuario indica que la unidad debe mover hacia una nueva smartEntity...
			std::shared_ptr<AvatarMoveToEntity> m = std::static_pointer_cast<AvatarMoveToEntity>(message);

			// Si la smartEntity no es la que actualmente se está utilizando...
			if(stateComponent != nullptr && m->target != stateComponent->getSmartEntity())
			{
				if(stateComponent->getSmartEntity() != nullptr )
				{
					// Se elimina a la unidad como usuario de la smartEntity que se actualmente se está utilizando.	
					auto removeMessage = std::make_shared<removeFromWorkmanList>();
					removeMessage->sender = _entity;
					stateComponent->getSmartEntity()->emitMessageN(removeMessage);	
				}
				
				// Se almacena que la nueva smartEntity es la que se está utilizando.
				stateComponent->setSmartEntity(m->target);

				// Se reanuda la ejecución de la acción latente.
				this->resume();
			}
		}
		else if (message->type.compare("SmartDataPositionReceiver") == 0)
		{
			// Cuando se recibe una smartPosition como respuesta a una solicitud previa...
			std::shared_ptr<SmartDataPositionReceiver> m = std::static_pointer_cast<SmartDataPositionReceiver>(message);

			if(stateComponent != nullptr)
			{
				// Se almacena la smartPosition recibida en el stateComponent de la unidad.
				stateComponent->setPosition(m->position);

				// La acción latente finaliza con éxito.
				this->finish(true);
			}
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
				this->reset();
			// Si NO hay una smartEntity "siguiente", se almacena "nullptr".
			}else if(stateComponent != nullptr && m->nextItem == nullptr)
			{
				stateComponent->setSmartEntity(nullptr);
				stateComponent->setLastResource(nullptr);
				this->reset();
			}	
			// Se reinicia la acción latente, para que la unidad pueda solicitar otra smartPosition.
		}
		else if(message->type.compare("AvatarFinishedRoute") == 0)
		{
			this->reset();
		}
	}

	CLatentAction::LAStatus CLARequestSP::OnStart()
	{	
		sendAnimationMessage("Idle");
		return RUNNING;
	}

	CLatentAction::LAStatus CLARequestSP::OnRun(unsigned int msecs) 
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

			// Se recupera la smartEntity que la unidad está utilizando, desde su componente de estado.
			smartEntity = stateComponent->getSmartEntity();

			if(smartEntity != nullptr)
			{
				// Se añade a la unidad como usuario de la smartEntity.
				auto addMessage = std::make_shared<addToWorkmanList>();
				addMessage->sender = _entity;
				smartEntity->emitMessageN(addMessage);

				// Se le solicita una smartPosition a la smartEntity.
				auto m = std::make_shared<SmartDataPositionRequest>();
				m->entity=_entity;
				smartEntity->emitMessageN(m);
			}
		}		
		return SUSPENDED;
	}

	CLatentAction::LAStatus CLARequestSP::OnAbort() 
	{
		return FAIL;
	}
} //namespace AI 