#include "LAReachSO.h"
#include "Logic/RouteCalculationMessages.h"
#include "Logic/Entity/Components/InteractSOState.h"
#include "Logic\Entity\Entity.h"
#include "Logic\SmartObjectMessages.h"

#include "Server.h"
#include "AI\GameManager.h"

#define MAX_DISTANCE_TO_REQUIRE_SMARTPOSITION 20
namespace AI 
{


//////////////////////////////
//	Implementación de CLAReachSO
//////////////////////////////

	//-------------------------------------------------------------------------

	CLatentAction::LAStatus CLAReachSO::OnStart()
	{	
//		std::cout << "STATE: REACH onStart" << std::endl;
		init();
		sendAnimationMessage("Walk");
		if(_interactComponent->getSmartEntity()!=nullptr)
		{
			if(_interactComponent->getSmartPosition()==Vector3::NEGATIVE_UNIT_Y)
			{
//				std::cout<<_entity->getName()<<" reach go to running "<<std::endl;
				return RUNNING;
			}else
				return SUSPENDED;
		}

		return SUSPENDED;
	}

	//-------------------------------------------------------------------------

	void CLAReachSO::OnStop()
	{

	}

	//-------------------------------------------------------------------------

	CLatentAction::LAStatus CLAReachSO::OnRun(unsigned int msecs) 
	{
		if(_interactComponent->getSmartEntity()==nullptr)
		{
//			std::cout<<"reach suspend onRun"<<std::endl;
			return SUSPENDED;
		}
		float d=_interactComponent->getSmartEntity()->getPosition().distance(_entity->getPosition());
		if(d<=MAX_DISTANCE_TO_REQUIRE_SMARTPOSITION)
		{
			auto requestMessage = std::make_shared<SmartDataPositionRequest>();
			requestMessage->entity = _entity;
			_interactComponent->getSmartEntity()->emitMessageN(requestMessage);
//			std::cout<<"reach request smart data position to"<<_interactComponent->getSmartEntity()->getName()<<std::endl;
			return SUSPENDED;
		}

		return RUNNING;
	}

	//-------------------------------------------------------------------------

	CLatentAction::LAStatus CLAReachSO::OnAbort() 
	{
		// Cuando se aborta se queda en estado terminado con fallo
		return FAIL;
	}

	//-------------------------------------------------------------------------

	bool CLAReachSO::acceptN(const std::shared_ptr<NMessage> &message)
	{
		// La acción no acepta mensajes
		return message->type.compare("AvatarRouteTo")==0; //cambiar por AvatarFinishedRoute cuando esté implementado
	}

	//-------------------------------------------------------------------------

	void CLAReachSO::processN(const std::shared_ptr<NMessage> &message)
	{	
		
		if( message->type.compare("AvatarRouteTo")==0)
		{
			init();
			std::shared_ptr<AvatarRouteTo> m = std::static_pointer_cast<AvatarRouteTo>(message);
			if(m->sender==nullptr)
			{
				//std::cout<<"Route to from user to a position into map"<<std::endl;
				AI::CServer::getSingletonPtr()->getGameManager()->freeSmartPosition(_interactComponent->getSmartEntity(),_interactComponent->getSmartPosition(),_entity);
				_interactComponent->setSmartEntity(nullptr);
				_interactComponent->setSmartPosition(Vector3::NEGATIVE_UNIT_Y);
				this->finish(false);
			}
		}
	}

	void CLAReachSO::init(){
		if(_interactComponent == nullptr)
		{
			Logic::IComponent *iComponent = _entity->getComponent("CInteractSOState");

			if(iComponent != nullptr)
				_interactComponent = static_cast<Logic::CInteractSOState*>(iComponent);
		}
	}
} //namespace AI 
