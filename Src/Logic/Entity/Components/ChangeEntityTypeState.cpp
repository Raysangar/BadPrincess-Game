/**
@file ChangeEntityTypeState.cpp
*/

#include "ChangeEntityTypeState.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"
#include "Logic/MovementMessages.h"
#include "Logic\Entity\Components\WorkmanState.h"
#include "Logic\Entity\Components\BuildComponent.h"
#include "Logic\Entity\Components\EntityTypeExchanger.h"
#include "Logic\RouteCalculationMessages.h"
#include "Logic\WorkmanMessages.h"
#include "Logic\AllyUnitMessages.h"
#include "Logic/GraphicsMessages.h"
#include "Logic/PhysicMessages.h"
#include "Logic/Entity/Components/Life.h"
#include "Logic/Entity/Components/AnimatedGraphics.h"

#include "GUI\Server.h"
#include "GUI\PlayerController.h"

#include "Map/MapEntity.h"

namespace Logic 
{
	IMP_FACTORY(CChangeEntityTypeState);

	//---------------------------------------------------------
	
	CChangeEntityTypeState::CChangeEntityTypeState(std::string componentName) : IComponent(componentName),_well(nullptr),_changeTypeActivated(false),_timeToDestroy(0),_changeTypeEntityParticleName("")
	{
	
	}
	//---------------------------------------------------------

	bool CChangeEntityTypeState::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;		

		assert(entityInfo->hasAttribute("material_name"));
		_materialName = entityInfo->getStringAttribute("material_name");

		assert(entityInfo->hasAttribute("changeTypeParticleEffectName"));
		_changeTypeEntityParticleName = entityInfo->getStringAttribute("changeTypeParticleEffectName");



		return true;
	} 

	//---------------------------------------------------------

	CChangeEntityTypeState::~CChangeEntityTypeState() 
	{
	
	} 

	//---------------------------------------------------------

	void CChangeEntityTypeState::awake()
	{
		_well = nullptr;
		_changeTypeActivated=false;
		_timeToDestroy=1500;

		((Logic::CGraphics*)_entity->getComponent("CAnimatedGraphics"))->setMaterialTransparency(_materialName,0,0,0,0.0f);

		((Logic::CGraphics*)_entity->getComponent("CAnimatedGraphics"))->setMaterialTransparency(_materialName,0,0,0,1.0f,true,250);

		auto m3= std::make_shared<PlayParticle>();
		m3->particleName = "summon";
		_entity->emitMessageN(m3);		
	}

	//---------------------------------------------------------
	
	bool CChangeEntityTypeState::activate()
	{
		return true;
	}

	//---------------------------------------------------------

	void CChangeEntityTypeState::deactivate()
	{
	}

	//---------------------------------------------------------

	bool CChangeEntityTypeState::acceptN(const std::shared_ptr<NMessage> &message)
	{
		return (!_changeTypeActivated && message->type.compare("AvatarMoveToEntity")==0)|| message->type.compare("EnterIntoExchangeTypeArea")==0 || message->type.compare("AvatarRouteTo")==0;
	}

	//---------------------------------------------------------

	void CChangeEntityTypeState::processN(const std::shared_ptr<NMessage> &message)
	{		
		if(message->type.compare("AvatarMoveToEntity")==0)
		{
			auto moveTo = std::static_pointer_cast<AvatarMoveToEntity>(message);
			Logic::CEntity* decisionEntity = moveTo->target;
			if(decisionEntity->getType().compare("Well")==0)
			{
				Logic::IComponent *iComponent = decisionEntity->getComponent("CBuildComponent");
				assert(iComponent);
				Logic::CBuildComponent *buildComponent = static_cast<Logic::CBuildComponent*>(iComponent);

				if(_entity->getType().compare("WorkmanGoblin")==0)
				{
					//check if the carrier ability is enabled and disable it if necessary

					iComponent = _entity->getComponent("CWorkmanState");
					assert(iComponent);
					Logic::CWorkmanState *workmanState = static_cast<Logic::CWorkmanState*>(iComponent);
					//if i have to build  the well or the repair ability are selected 
					if(!buildComponent->isBuilded() || (buildComponent->isBuilded() && workmanState->isRepairBuildingAbilityActive()))
					{
//						std::cout<<"change entity type buildwell"<<std::endl;
						//the wokman state manage that, we don't activate a change entity behaviour
						//set the variable that save the well where i go to change entity to null
						_well = nullptr;
						//send a message to disable the ability in order to don't use it for other well
						//PAY ATTENTION! send this message here because if we disable the ability into workman state this component could be
						//called after the CWorkmanState and we lost the information o repair ability and we could activate
						//the change entity type behaviour wrongly
						auto disableRepair = std::make_shared<DisableRepairAbility>();
						_entity->emitMessageN(disableRepair);

						return;
					}else{
						auto disableRepair = std::make_shared<DisableRepairAbility>();
						_entity->emitMessageN(disableRepair);

					}
//					std::cout<<"change entity type change"<<std::endl;

				}

				//if the well is builded and the repair ability it isn't activate go to well for changing entity	
				if(buildComponent->isBuilded())
				{
					_well=decisionEntity;

					iComponent = _well->getComponent("CEntityTypeExchanger");
					assert(iComponent);
					Logic::CEntityTypeExchanger *exchanger = static_cast<Logic::CEntityTypeExchanger*>(iComponent);
					//if it is already into destination well trigger 
					if(exchanger->isEntityInTrigger(_entity))
					{
						changeEntityType();
						//std::cout<<"changeEntyity type already in trigger"<<std::endl;
					}else
					{
						//send a message to change the state machine state in order to reach the well
						auto changeTypeMessage = std::make_shared<ChangeEntityTypeIntoWell>();
						_entity->emitMessageN(changeTypeMessage);

						auto routeMessage = std::make_shared<AvatarRouteTo>();
						routeMessage->target = decisionEntity->getPosition();
						routeMessage->sender = _entity;
						_entity->emitMessageN(routeMessage);
					}
				}else
					_well = nullptr;//used if i'm going already to a well and before that i reach it click over another well not builded 
			}
			else
				_well=nullptr;
		}else if( message->type.compare("EnterIntoExchangeTypeArea")==0)
		{
			//the well send me that i'm entering into a well trigger
			auto enter = std::static_pointer_cast<EnterIntoExchangeTypeArea>(message);
			//check if it's my correct destination
//			std::cout<<"changeEntityType well reached s="<<enter->sender->getName()<<" w="<<((_well==nullptr)?"":_well->getName())<<std::endl;
			if(enter->sender==_well)
			{
//				std::cout<<"changeEntityType start function"<<std::endl;
				changeEntityType();
			}
		}else if( message->type.compare("AvatarRouteTo")==0)
		{
			auto route = std::static_pointer_cast<AvatarRouteTo>(message);
			//if the player send command to go to a specific position reset the behaviour
			if(route->sender==nullptr)
			{
				//std::cout<<"cancel changeType due to route to"<<std::endl;
				_changeTypeActivated=false;
				_well=nullptr;
			}
		}
	}


	void CChangeEntityTypeState::changeEntityType()
	{

		auto moveInterrupt = std::make_shared<AvatarInterruptRoute>();
		_entity->emitMessageN(moveInterrupt);

		Logic::IComponent* iComponent = _well->getComponent("CEntityTypeExchanger");
		assert(iComponent);
		Logic::CEntityTypeExchanger *exchanger = static_cast<Logic::CEntityTypeExchanger*>(iComponent);

		//if the well it isn't in a changeState for another entity
		if(!exchanger->isEntityTypeExchangerActivate())
		{
//			std::cout<<"changeEntityType function activated"<<std::endl;
			//activate the behaviour			
			
			_changeTypeActivated = true;

			//deselect the entity
			GUI::CServer::getSingletonPtr()->getPlayerController()->removeEntityFromSelectedUnits(_entity);

			auto m = std::make_shared<AvatarActivation>();
			m->actived = false;
			_entity->emitMessageN(m);

			auto m2 = std::make_shared<AvatarSelection>();
			m2->selected = false;
			_entity->emitMessageN(m2);
			
			auto playParticleMessage = std::make_shared<PlayParticle>();
			playParticleMessage->particleName = _changeTypeEntityParticleName;
			_entity->emitMessageN(playParticleMessage);

			IComponent * component = _entity->getComponent("CLife");
			assert(component);
			Logic::CLife *lifeComponent = static_cast<Logic::CLife*>(component);
			
			//access to the component directly in order to block the behaviour for himself and avoid that other entity
			//could go to the well anyway
			exchanger->startExchangerBehaviour(_entity,lifeComponent->getCurrentLife(),_entity->getPosition());

		}
		else//the well is aleady occuped from some other entity
		{
			_well = nullptr;
			_changeTypeActivated = false;
//			std::cout<<_entity->getName()<<" changeEntityType well is aleady occuped from some other entity"<<std::endl;

		}

		//auto playAudioMessage = std::make_shared<PlayAudio>();
		//playAudioMessage->eventName = "default";
		//_entity->emitMessageN(playAudioMessage);

		

	}


	
	//---------------------------------------------------------

	void CChangeEntityTypeState::tick(unsigned int msecs) 
	{
		// Invocar al método de la clase padre (IMPORTANTE)
		IComponent::tick(msecs);

		if (_changeTypeActivated)
		{
			_timeToDestroy -= msecs;
			
			if(_timeToDestroy <= 500)
				((Logic::CGraphics*)_entity->getComponent("CAnimatedGraphics"))->setMaterialTransparency(_materialName,0,0,0,0.0f,true,250);

			if (_timeToDestroy <= 0)
			{
//				std::cout<<"ChangeEntityTypeState destroy"<<std::endl;

				GUI::CServer::getSingletonPtr()->getPlayerController()->selectEntity(_well);

				//deactivate all this in order to make the entity unselectable during particle effect
				//_entity->activate("CAnimatedGraphics");
				//_entity->activate("CLife");
				//_entity->activate("CPhysicController");
				//_entity->activate("CAvatarController");

				auto playParticleMessage = std::make_shared<StopParticle>();
				playParticleMessage->particleName = _changeTypeEntityParticleName;
				_entity->emitMessageN(playParticleMessage);
				
//				Logic::CEntityFactory::getSingletonPtr()->deferredDeleteEntity(_entity);
				auto move = std::make_shared<SetPhysicControllerPosition>();
				move->position=_entity->getPosition()+Vector3(0,-1000,0);
				_entity->emitMessageN(move);

				_well = nullptr;
				_changeTypeActivated=false;
				_timeToDestroy=1500;
			}
		}

		//if(_triggerDetector)
		//{
		//	//movemos el detector
		//	Vector3 pos =  _entity->getPosition();
		//	pos.y -= _changeEntityTypeStateRange;
		//	_actorController->setGlopalPosition(_triggerDetector,pos);
		//}
	}
}








