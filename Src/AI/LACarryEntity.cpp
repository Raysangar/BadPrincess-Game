#include "LACarryEntity.h"

#include "Logic\Entity\Entity.h"
#include "Logic\Entity\Components\CarrierState.h"
#include "Logic\Entity\Components\StatsModifier.h"
#include "Logic\CarrierMessages.h"
#include "Logic\SmartObjectMessages.h"
#include "Logic\UnitMessages.h"
#include "Logic/AudioMessages.h"


#define MAX_DISTANCE_TO_TAKE_CHEST 12
#define DISTANCE_TAKEN_CHEST 5

namespace AI 
{


//////////////////////////////
//	Implementación de CLACarryEntity
//////////////////////////////

	//---------------------------------------------------------------
	
	CLatentAction::LAStatus CLACarryEntity::OnStart()
	{	
		//std::cout << "LACArryEntity onStart" << std::endl;


		init();

		

		if(_carrierState->getCarriedEntity() == nullptr)
			return EXIT_SM_FAIL;
				
		_carrierState->setupAbilityIcon(_carrierState->getReleaseAbilityIcon());
		
		float dist = _entity->getPosition().distance(_carrierState->getCarriedEntity()->getPosition());

		if(dist <= MAX_DISTANCE_TO_TAKE_CHEST)
		{
			sendAnimationMessage(_carrierState->getCarrierIdleAnimationName());
	
//			std::cout << "START Carring" << std::endl;

			auto transportMessage = std::make_shared<TransportEntityStart>();
			transportMessage->entity=_entity;
			_carrierState->getCarriedEntity()->emitMessageN(transportMessage);

			auto disableSmartEntityMessage = std::make_shared<ChangeSmartObjectState>();
			disableSmartEntityMessage->isEnabled=false;
			_carrierState->getCarriedEntity()->emitMessageN(disableSmartEntityMessage);

			auto mMS = std::make_shared<ModifyStat>();
			mMS->modID = "CarryEntity";
			mMS->modType = Logic::ModificationType::speedMod;
			mMS->percentage = -50;
			mMS->durationMsecs = 0;
			_entity->emitMessageN(mMS);

			setupCarriedTransform();

			return SUSPENDED;
		}else
			return EXIT_SM_FAIL;
	}

	//---------------------------------------------------------------

	CLatentAction::LAStatus CLACarryEntity::OnRun(unsigned int msecs) 
	{		
		if(_carrierState->getCarriedEntity()==nullptr)
		{
			auto playSound = std::make_shared<StopAudio>();
			playSound->eventName = "event:/WorkmanGoblin/dragChest";
			_entity->emitMessageN(playSound);
			return EXIT_SM_FAIL;
		}

		setupCarriedTransform();

		return RUNNING;
		
	}

	void CLACarryEntity::setupCarriedTransform()
	{
		Vector3 orientation = Math::getDirection( _entity->getOrientation() );
		orientation.normalise();
		Vector3 newPos = _entity->getPosition()- (orientation*DISTANCE_TAKEN_CHEST);
		newPos.y = 1;
		
		_carrierState->getCarriedEntity()->setPositionN(newPos);
		_carrierState->getCarriedEntity()->setYawN(_entity->getYaw());
	}
	//---------------------------------------------------------------

	void CLACarryEntity::OnStop()
	{
		// Cuando se aborta se queda en estado terminado con fallo
		Vector3 pos = _carrierState->getCarriedEntity()->getPosition();
		pos.y = 0;

		_carrierState->getCarriedEntity()->setPositionN(pos);
		auto transportMessage = std::make_shared<TransportEntityEnd>();
		_carrierState->getCarriedEntity()->emitMessageN(transportMessage);

		auto enableSmartEntityMessage = std::make_shared<ChangeSmartObjectState>();
		enableSmartEntityMessage->isEnabled=true;
		_carrierState->getCarriedEntity()->emitMessageN(enableSmartEntityMessage);

		auto mMS = std::make_shared<RestoreStat>();
		mMS->modID = "CarryEntity";
		_entity->emitMessageN(mMS);

	}

	//---------------------------------------------------------------

	CLatentAction::LAStatus CLACarryEntity::OnAbort() 
	{
		// Cuando se aborta se queda en estado terminado con fallo
		Vector3 pos = _carrierState->getCarriedEntity()->getPosition();
		pos.y = 0;
		_carrierState->getCarriedEntity()->setPositionN(pos);
		auto transportMessage = std::make_shared<TransportEntityEnd>();
		_carrierState->getCarriedEntity()->emitMessageN(transportMessage);

		auto enableSmartEntityMessage = std::make_shared<ChangeSmartObjectState>();
		enableSmartEntityMessage->isEnabled=true;
		_carrierState->getCarriedEntity()->emitMessageN(enableSmartEntityMessage);

		auto mMS = std::make_shared<RestoreStat>();
		mMS->modID = "CarryEntity";
		_entity->emitMessageN(mMS);
		return FAIL;
	}

	//---------------------------------------------------------------

	bool CLACarryEntity::acceptN(const std::shared_ptr<NMessage> &message)
	{		
		return (message->type.compare("AvatarFinishedRoute") == 0) ||
			 (message->type.compare("AvatarRouteTo") == 0);
	}

	//---------------------------------------------------------------

	void CLACarryEntity::processN(const std::shared_ptr<NMessage> &message)
	{		
		if(message->type.compare("AvatarFinishedRoute") == 0)
		{
			//if this message is not the first message recived when we change the state			
			if(this->getStatus()!=READY)
			{
				//restart the stati attending an user input
				this->reset();
			}else{
				//watch in the other side in order to have a chest behind
				_entity->setYawN(_entity->getYaw()+Math::PI);
			}
		}
		
		else if(message->type.compare("AvatarRouteTo") == 0)
		{
			auto playSound = std::make_shared<PlayAudio>();
			playSound->eventName = "event:/WorkmanGoblin/dragChest";
			_entity->emitMessageN(playSound);
			init();
			sendAnimationMessage(_carrierState->getCarrierMoveAniamtionName());
			this->resume();
		}
		
	}
	void CLACarryEntity::init()
	{
		if(_carrierState==nullptr)
		{
			Logic::IComponent* component = _entity->getComponent("CCarrierState");
			assert(component);
			_carrierState = static_cast<Logic::CCarrierState*>(component);		
		}
	}

} //namespace AI 
