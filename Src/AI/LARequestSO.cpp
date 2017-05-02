#include "LARequestSO.h"
#include "Logic\Entity\Entity.h"
#include "Logic/Entity/Components/InteractSOState.h"
#include "Logic\SmartObjectMessages.h"
#include "Logic\RouteCalculationMessages.h"
#include "Logic\MovementMessages.h"
#include "Application\BaseApplication.h"
#include "AI\Server.h"
#include "AI\GameManager.h"
#include "AI\EntityManager.h"

#define RADIUS_OVERLAPS 30
#define TOLERANCE 7
#define TIME_BEETWEEN_ATTEMPT 4000
#define MAX_NUMBER_ATTEMPT 4
namespace AI 
{
	//////////////////////////////
	//	Implementación de CLARequestSO
	//////////////////////////////

	//-------------------------------------------------------------------------

	CLatentAction::LAStatus CLARequestSO::OnStart()
	{	
		if(_interactComponent == nullptr)
		{
			Logic::IComponent *iComponent = _entity->getComponent("CInteractSOState");

			if(iComponent != nullptr)
				_interactComponent = static_cast<Logic::CInteractSOState*>(iComponent);
		}
		//if this is null the user sended a workman to a specific position on map
		if(_interactComponent->getSmartEntity()==nullptr)
		{
//			std::cout<<_entity->getName()<<" request exit fail with smartentity null"<<std::endl;
			_interactComponent->setSmartEntity(nullptr);
			return EXIT_SM_FAIL;
		}

		Vector3 smartPosition = _interactComponent->getSmartPosition();
		//if i already recived the smart position
		if(smartPosition != Vector3::NEGATIVE_UNIT_Y)
		{
			//check if the distance from smartposition is little 
//			std::cout<<"request smart pos different to negative Y"<<std::endl;
			float distance = smartPosition.distance(_entity->getPosition());
			if(smartPosition.positionEquals(_entity->getPosition(),TOLERANCE))
			{
				//send the message to go to interact state
				auto readyMessage = std::make_shared<SmartDataAvatarReady>();
				readyMessage->entity = _entity;
				_entity->emitMessageN(readyMessage);
//				std::cout<<"request send avatar ready"<<std::endl;

			}else
			{
				//probably some problem to reach the smart position are occurred, exit to idle state
				
//				std::cout<<_entity->getName()<<"request smartposition so far from "<<_interactComponent->getSmartEntity()->getName()<<" d="<<distance<<std::endl;
				//AI::CServer::getSingletonPtr()->getGameManager()->freeSmartPosition(_interactComponent->getSmartEntity(),_interactComponent->getSmartPosition(),_entity);
				//_interactComponent->setSmartEntity(nullptr);
				//_interactComponent->setSmartPosition(Vector3::NEGATIVE_UNIT_Y);
				//return EXIT_SM_FAIL;

				//try to reach the smart position again
				return tryToAttemptRouteTo(_interactComponent->getSmartPosition());
			}
		}
		else
		{
			//try to reach the smartentity again until wait the smartposition
//			std::cout<<_entity->getName()<<"request waiting for smartposition"<<std::endl;
			return tryToAttemptRouteTo(_interactComponent->getSmartEntity()->getPosition());
		}

		return SUSPENDED;
	}

	//-------------------------------------------------------------------------

	void CLARequestSO::OnStop()
	{

	}

	//-------------------------------------------------------------------------

	CLatentAction::LAStatus CLARequestSO::OnRun(unsigned int msecs) 
	{
		return SUSPENDED;
	}

	//-------------------------------------------------------------------------

	CLatentAction::LAStatus CLARequestSO::OnAbort() 
	{
		// Cuando se aborta se queda en estado terminado con fallo
		return FAIL;
	}

	//-------------------------------------------------------------------------

	bool CLARequestSO::acceptN(const std::shared_ptr<NMessage> &message)
	{
		return false; 
	}

	//-------------------------------------------------------------------------

	void CLARequestSO::processN(const std::shared_ptr<NMessage> &message)
	{	
	}

	AI::CLatentAction::LAStatus CLARequestSO::tryToAttemptRouteTo(Vector3 position)
	{
		unsigned int now=Application::CBaseApplication::getSingletonPtr()->getAppTime();
		if(now-_lastRouteToAttempt < TIME_BEETWEEN_ATTEMPT)
		{
			++_numberOfAttempt;
			//if it's multiple times that we attempt to go to a position but we can't reach it
			if(_numberOfAttempt >= MAX_NUMBER_ATTEMPT)
			{
				//find a new entity of this type closest
				if(_interactComponent->getSmartEntity()!=nullptr)
				{
					Logic::CEntity*next = AI::CServer::getSingletonPtr()->getEntityManager()->getClosestEntityTypeWithOverlaps(_interactComponent->getSmartEntity(),RADIUS_OVERLAPS,_entity->getPosition());
					if(next!=nullptr)
					{
//						std::cout<<_entity->getName()<<" tryToAttemptRouteTo limit reached move to "<<next->getName()<<std::endl;
						auto moveMessage = std::make_shared<AvatarMoveToEntity>();
						moveMessage->target = next;
						_entity->emitMessageN(moveMessage);
						return SUSPENDED;
					}
				}
				//ATTENTION!! smart entity shouldn't be nullptr but it could be, check on reciving message side if it isn't null
				auto limitReachedMessage = std::make_shared<AttemptSmartObjectLimitReached>();
				limitReachedMessage->entity=_interactComponent->getSmartEntity();
				_entity->emitMessageN(limitReachedMessage);
				//if there isn't an other resource closest stop it
//				std::cout<<_entity->getName()<<" tryToAttemptRouteTo limit reached exit"<<std::endl;
				AI::CServer::getSingletonPtr()->getGameManager()->freeSmartPosition(_interactComponent->getSmartEntity(),_interactComponent->getSmartPosition(),_entity);
				_interactComponent->setSmartEntity(nullptr);
				_interactComponent->setSmartPosition(Vector3::NEGATIVE_UNIT_Y);


				return SUSPENDED;
			}
		}else
		{
			_lastRouteToAttempt=now;
			_numberOfAttempt = 1;
		}
//		std::cout<<_entity->getName()<<" tryToAttemptRouteTo n="<<_numberOfAttempt<<std::endl;
		//try to reach the position again
		auto moveMessage = std::make_shared<AvatarRouteTo>();
		moveMessage->target = position;
		moveMessage->sender = _entity;
		_entity->emitMessageN(moveMessage);
		return SUSPENDED;
	}
} //namespace AI 
