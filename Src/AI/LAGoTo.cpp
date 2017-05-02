#include "LAGoTo.h"
#include "Server.h"

#include "Logic\Entity\Entity.h"
#include "Logic/Entity/Components/WorkmanState.h"
#include "Logic\MovementMessages.h"
#include "Logic\SmartObjectMessages.h"

namespace AI 
{
	bool CLAGoTo::acceptN(const std::shared_ptr<NMessage> &message)
	{
		return (message->type.compare("AvatarFinishedMove") == 0) || (message->type.compare("AvatarMoveToEntity") == 0);
	}
	

	void CLAGoTo::processN(const std::shared_ptr<NMessage> &message)
	{
		if (message->type.compare("AvatarFinishedMove") == 0)
		{
			std::shared_ptr<AvatarFinishedMove> m = std::static_pointer_cast<AvatarFinishedMove>(message);
		this->finish(m->finished);
		}else if(message->type.compare("AvatarMoveToEntity") == 0)
		{
			std::shared_ptr<SmartDataPositionReceiver> m = std::static_pointer_cast<SmartDataPositionReceiver>(message);
			//currentSmartEntity=m->smartObject;

			Logic::IComponent *iComponent = _entity->getComponent("CWorkmanState");

			if(iComponent != nullptr)
			{
				Logic::CWorkmanState* stateComponent = static_cast<Logic::CWorkmanState*>(iComponent);
				stateComponent->setSmartEntity(m->smartObject);
			}

		}
		
	}

	CLatentAction::LAStatus CLAGoTo::OnStart()
	{	
		//std::cout << "START" << std::endl;
		return RUNNING;
	}

	CLatentAction::LAStatus CLAGoTo::OnRun() 
	{

		if(_entity != nullptr)
		{
			Logic::IComponent *iComponent = _entity->getComponent("CWorkmanState");

			if(iComponent != nullptr)
			{
				Logic::CWorkmanState* stateComponent = static_cast<Logic::CWorkmanState*>(iComponent);

				if(stateComponent->getSmartEntity() != nullptr)
				{
					//std::cout << "SMART_ENTITY" << std::endl;
					//auto m2 = std::make_shared<SmartDataPositionRequest>();
					//m2->entity=_entity;
					//currentSmartEntity->emitMessageN(m2);
				}
			}
		}
		
			
		auto m = std::make_shared<AvatarMoveToPosition>();
		m->target=_target;
		m->moveType = 1;
		_entity->emitMessageN(m);
		
		//std::cout << "RUNNING" << std::endl;
		return SUSPENDED;
	}


} //namespace AI 