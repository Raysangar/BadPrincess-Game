#include "LAThiefInteractSO.h"

#include "Logic/Entity/Components/ThiefEnemyState.h"
#include "Logic/Entity/Components/InteractSOState.h"
#include "Logic/SmartObjectMessages.h"
#include "Logic/RouteCalculationMessages.h"
#include "Logic/MovementMessages.h"
#include "Logic/Entity/Entity.h"
#include "Logic/UnitMessages.h"
#include "AI\GameManager.h"
#include "AI\EntityManager.h"
#include "AI\Server.h"
#include "GUI\EventsController.h"
#include "GUI/Server.h"
#include "GUI/MinimapController.h"

namespace AI 
{


//////////////////////////////
//	Implementación de CLAThiefInteractSO
//////////////////////////////

	//-------------------------------------------------------------------------

	CLatentAction::LAStatus CLAThiefInteractSO::OnStart()
	{	
		Logic::IComponent *component = _entity->getComponent("CThiefEnemyState");
		assert(component);
		_state = static_cast<Logic::CThiefEnemyState*>(component);

		component = _entity->getComponent("CInteractSOState");
		if(component != nullptr)
			_interactComponent = static_cast<Logic::CInteractSOState*>(component);
		
		if(_interactComponent->getSmartEntity()==nullptr)
			return EXIT_SM_FAIL;

		auto m = std::make_shared<SmartDataAvatarReady>();
		m->entity = _entity;
		m->position = _interactComponent->getSmartPosition();
		_interactComponent->getSmartEntity()->emitMessageN(m);

		GUI::CServer::getSingletonPtr()->getEventscontroller()->stealingResources(_entity);

		lookAtSmartObject();

		return SUSPENDED;
	}

	//-------------------------------------------------------------------------

	void CLAThiefInteractSO::OnStop()
	{
		_interactComponent->setIsInteracting(false);

	}

	//-------------------------------------------------------------------------

	CLatentAction::LAStatus CLAThiefInteractSO::OnRun(unsigned int msecs) 
	{
		if(_state->getResourceType()==AI::ResourceType::none)
		{
			this->startEscape();
			return EXIT_SM_SUCCESS;
		}
		_currentTime += msecs;
		if(_currentTime>= _state->getThiefTime())
		{
			int qta= std::min(AI::ResourceServer::getInstance()->getResource(_state->getResourceType()),_state->getThiefUnit());
			std::cout<<"stole qta= "<<qta<<std::endl;
			_state->setCurrentThiefQuantity(qta);

			AI::ResourceServer::getInstance()->addResource(_state->getResourceType(),-_state->getCurrentThiefQuantity());

			this->startEscape();
			
			return EXIT_SM_SUCCESS;
		}else
			return RUNNING;
	}

	void CLAThiefInteractSO::startEscape()
	{
		AI::CServer::getSingletonPtr()->getGameManager()->freeSmartPosition(_interactComponent->getSmartEntity(), _interactComponent->getSmartPosition(), _entity);

		Logic::CEntity* spawnPoint =  AI::CServer::getSingletonPtr()->getEntityManager()->getClosestEntity(CEntityManager::EntityType::SpawnPoint, _entity->getPosition());

		auto m2 = std::make_shared<AvatarMoveToEntity>();		
		m2->target = spawnPoint;
		_entity->emitMessageN(m2);

	}

	//-------------------------------------------------------------------------

	CLatentAction::LAStatus CLAThiefInteractSO::OnAbort() 
	{
		// Cuando se aborta se queda en estado terminado con fallo
		_interactComponent->setIsInteracting(false);

		return FAIL;
	}

	//-------------------------------------------------------------------------

	bool CLAThiefInteractSO::acceptN(const std::shared_ptr<NMessage> &message)
	{
		return (message->type.compare("SmartDataAction")==0); 
	}

	//-------------------------------------------------------------------------

	void CLAThiefInteractSO::processN(const std::shared_ptr<NMessage> &message)
	{	

		if(message->type.compare("SmartDataAction") == 0)
		{
			std::shared_ptr<SmartDataAction> m = std::static_pointer_cast<SmartDataAction>(message);

			AI::ResourceType type = findAvailableResource(_state->getThiefUnit());		

			if(type == AI::ResourceType::none)
				type = findAvailableResource(1);


			_state->setResourceType(type);

			sendAnimationMessage(m->animation);

			if(_interactComponent!=nullptr)
				_interactComponent->setIsInteracting(true);

			_currentTime = 0;
			this->resume();
			
		}

	}

	AI::ResourceType CLAThiefInteractSO::findAvailableResource(int quantity)
	{
		int res = rand() % 3;
		AI::ResourceType type = AI::ResourceType::none; 

		for(int i=0;i<3;++i)
			{
				if(ResourceServer::getInstance()->getResource(AI::ResourceType((res+i)%3))>= quantity)
				{
					type=AI::ResourceType((res+i)%3);
					break;
				}

			}
		return type;
	}

	void CLAThiefInteractSO::lookAtSmartObject()
	{	
		auto rotationMessage = std::make_shared<AvatarControl>();
		rotationMessage->controlType = "turn";
		Vector3 direction = _interactComponent->getSmartEntity()->getPosition() - _entity->getPosition();
		double rotation = AI::CServer::correctAngle(atan2(-direction.x, -direction.z) - _entity->getYaw());
		rotationMessage->attribute = (float) rotation;
		_entity->emitMessageN(rotationMessage);
	}

} //namespace AI 
