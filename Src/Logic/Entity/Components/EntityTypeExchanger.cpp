/**
@file EntityTypeExchanger.cpp
*/

#include "EntityTypeExchanger.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Server.h"
#include "Logic\AllyUnitMessages.h"
#include "Logic/GraphicsMessages.h"
#include "Logic/PhysicMessages.h"
#include "Logic/Entity/Components/GUIManager.h"
#include "Logic/Entity/Components/ChangeEntityTypeState.h"

#include "Map/MapEntity.h"

#include "GUI\Server.h"
#include "GUI\PlayerController.h"
#include "GUI\ActiveUnitController.h"

#include "Physics/Server.h"
#include "Physics/ActorFactory.h"
#include "Physics/SceneManager.h"
#include "Physics/ActorController.h"
#include <PxPhysicsAPI.h>

#define ENTITY_COST_PERCENT 0.5f

namespace Logic 
{
	IMP_FACTORY(CEntityTypeExchanger);

	//---------------------------------------------------------
	
	CEntityTypeExchanger::CEntityTypeExchanger(std::string componentName) :IPhysics(componentName), _triggerDetector(0),_exchangerRange(0.0f),_abilityQName(""),_abilityWName(""),_abilityEName(""),_abilityRName(""),_abilityQTooltip("")
		,_abilityWTooltip(""),_abilityETooltip(""),_abilityRTooltip(""),_changeTypeActivated(false),_exchangerAbilityQName(""),_exchangerAbilityWName(""),_exchangerAbilityEName(""),_exchangerAbilityRName(""),_exchangerAbilityQTooltip("")
		,_exchangerAbilityWTooltip(""),_exchangerAbilityETooltip(""),_exchangerAbilityRTooltip(""),_entityToChangePos(Vector3::ZERO),_entityToChange(nullptr),_entityToChangeLife(0),_particleName(""),
		_abilityQluaField(""),_abilityWluaField(""),_abilityEluaField(""),_abilityRluaField(""),_exchangerAbilityQcost(""),_exchangerAbilityWcost(""),_exchangerAbilityEcost(""),_exchangerAbilityRcost("")
	{
		_actorFactory = Physics::CServer::getSingletonPtr()->getActorFactory();
		_actorController = Physics::CServer::getSingletonPtr()->getActorController();

	}
	//---------------------------------------------------------

	bool CEntityTypeExchanger::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;				

		//normal well GUI
		if(entityInfo->hasAttribute("abilityQ_name"))
			_abilityQName =  entityInfo->getStringAttribute("abilityQ_name");
		if(entityInfo->hasAttribute("abilityQ_tooltip"))
			_abilityQTooltip =  entityInfo->getStringAttribute("abilityQ_tooltip");
		if(entityInfo->hasAttribute("abilityQ_luaField"))
			_abilityQluaField =  entityInfo->getStringAttribute("abilityQ_luaField");

		if(entityInfo->hasAttribute("abilityW_name"))
			_abilityWName =  entityInfo->getStringAttribute("abilityW_name");
		if(entityInfo->hasAttribute("abilityW_tooltip"))
			_abilityWTooltip =  entityInfo->getStringAttribute("abilityW_tooltip");
		if(entityInfo->hasAttribute("abilityW_luaField"))
			_abilityWluaField =  entityInfo->getStringAttribute("abilityW_luaField");

		if(entityInfo->hasAttribute("abilityE_name"))
			_abilityEName =  entityInfo->getStringAttribute("abilityE_name");
		if(entityInfo->hasAttribute("abilityE_tooltip"))
			_abilityETooltip =  entityInfo->getStringAttribute("abilityE_tooltip");
		if(entityInfo->hasAttribute("abilityE_luaField"))
			_abilityEluaField =  entityInfo->getStringAttribute("abilityE_luaField");

		if(entityInfo->hasAttribute("abilityR_name"))
			_abilityRName =  entityInfo->getStringAttribute("abilityR_name");
		if(entityInfo->hasAttribute("abilityR_tooltip"))
			_abilityRTooltip =  entityInfo->getStringAttribute("abilityR_tooltip");
		if(entityInfo->hasAttribute("abilityR_luaField"))
			_abilityRluaField =  entityInfo->getStringAttribute("abilityR_luaField");

		//exchange well GUI
		if(entityInfo->hasAttribute("exchangerAbilityQ_name"))
			_exchangerAbilityQName =  entityInfo->getStringAttribute("exchangerAbilityQ_name");
		if(entityInfo->hasAttribute("exchangerAbilityQ_tooltip"))
			_exchangerAbilityQTooltip =  entityInfo->getStringAttribute("exchangerAbilityQ_tooltip");
		if(entityInfo->hasAttribute("exchangerAbilityQ_luaField"))
			_exchangerAbilityQcost =  entityInfo->getStringAttribute("exchangerAbilityQ_luaField");

		if(entityInfo->hasAttribute("exchangerAbilityW_name"))
			_exchangerAbilityWName =  entityInfo->getStringAttribute("exchangerAbilityW_name");
		if(entityInfo->hasAttribute("exchangerAbilityW_tooltip"))
			_exchangerAbilityWTooltip =  entityInfo->getStringAttribute("exchangerAbilityW_tooltip");
		if(entityInfo->hasAttribute("exchangerAbilityW_luaField"))
			_exchangerAbilityWcost =  entityInfo->getStringAttribute("exchangerAbilityW_luaField");

		if(entityInfo->hasAttribute("exchangerAbilityE_name"))
			_exchangerAbilityEName =  entityInfo->getStringAttribute("exchangerAbilityE_name");
		if(entityInfo->hasAttribute("exchangerAbilityE_tooltip"))
			_exchangerAbilityETooltip =  entityInfo->getStringAttribute("exchangerAbilityE_tooltip");
		if(entityInfo->hasAttribute("exchangerAbilityE_luaField"))
			_exchangerAbilityEcost =  entityInfo->getStringAttribute("exchangerAbilityE_luaField");

		if(entityInfo->hasAttribute("exchangerAbilityR_name"))
			_exchangerAbilityRName =  entityInfo->getStringAttribute("exchangerAbilityR_name");
		if(entityInfo->hasAttribute("exchangerAbilityR_tooltip"))
			_exchangerAbilityRTooltip =  entityInfo->getStringAttribute("exchangerAbilityR_tooltip");
		if(entityInfo->hasAttribute("exchangerAbilityR_luaField"))
			_exchangerAbilityRcost =  entityInfo->getStringAttribute("exchangerAbilityR_luaField");

		if(entityInfo->hasAttribute("exchangeEntityCostPercent"))
			_entityCostPercents =  entityInfo->getFloatAttribute("exchangeEntityCostPercent");

		if(entityInfo->hasAttribute("exchangerParticleEffectName"))
			_particleName =  entityInfo->getStringAttribute("exchangerParticleEffectName");
		
		IComponent * component = _entity->getComponent("CGUIManager");
		assert(component);
		_guiManagerComponent = static_cast<Logic::CGUIManager*>(component);
		
		assert(entityInfo->hasAttribute("exchangerRange"));
		_exchangerRange = entityInfo->getFloatAttribute("exchangerRange");

		_triggerDetector = createTriggerDetector(entityInfo);

		return true;
	} 

	//---------------------------------------------------------

	CEntityTypeExchanger::~CEntityTypeExchanger() 
	{
		if (_triggerDetector) 
		{
			_actorFactory->destroyActor(_triggerDetector);
			_triggerDetector = NULL;
		}

		_actorFactory = nullptr;	
	} 

	//---------------------------------------------------------

	void CEntityTypeExchanger::awake()
	{

		_changeTypeActivated=false;
		//clean entity inside the trigger
		_triggerList.clear();
		//setup new trigger position
		if(_triggerDetector)
		{
			//movemos el detector
			Vector3 pos =  _entity->getPosition();
			pos.y -= _exchangerRange;
			_actorController->setGlopalPosition(_triggerDetector,pos);
		}
		//setup normal behaviour
		setNormalGUI();

	}

	//---------------------------------------------------------
	
	bool CEntityTypeExchanger::activate()
	{
		if (_triggerDetector)
			Physics::CServer::getSingletonPtr()->getSceneManager()->addToScene(_triggerDetector);
		return true;
	}

	//---------------------------------------------------------

	void CEntityTypeExchanger::deactivate()
	{
		if (_triggerDetector)
			Physics::CServer::getSingletonPtr()->getSceneManager()->removeFromScene(_triggerDetector);
	}

	//---------------------------------------------------------

	bool CEntityTypeExchanger::acceptN(const std::shared_ptr<NMessage> &message)
	{
		return (_changeTypeActivated && (message->type.compare("CastAbility")==0 ||  message->type.compare("EntityDying")==0));
	}

	//---------------------------------------------------------

	void CEntityTypeExchanger::processN(const std::shared_ptr<NMessage> &message)
	{		
		if(message->type.compare("CastAbility")==0)
		{
			std::shared_ptr<CastAbility> m = std::static_pointer_cast<CastAbility>(message);
			if(m->ability=="R")
			{

				//The generator entity have the same name of entity type
//				Logic::CEntity* e= Logic::CEntityFactory::getSingletonPtr()->instantiate(Logic::CServer::getSingletonPtr()->getMap()->getEntityByName(_entityToChange->getType()), "",_entityToChangePos);

				//IComponent* component = e->getComponent("CLife");
				//assert(component);
				//Logic::CLife *eLife = static_cast<Logic::CLife*>(component);
				//eLife->setLife(_entityToChangeLife);

				auto move = std::make_shared<SetPhysicControllerPosition>();
				move->position=_entityToChangePos;
				_entityToChange->emitMessageN(move);

				IComponent* component = _entityToChange->getComponent("CChangeEntityTypeState");
				assert(component);
				Logic::CChangeEntityTypeState *changeEntityState = static_cast<Logic::CChangeEntityTypeState*>(component);
				changeEntityState->awake();
				
				moveSelectionToChangedEntity(_entityToChange);

				endExchangerBehaviour();
			}else
			{
				Logic::CEntityFactory::getSingletonPtr()->deferredDeleteEntity(_entityToChange);
			}

		}else if(message->type.compare("EntityDying")==0)
		{
			//if the well was destroyed until it still change an entity this entity will destroy
			if(_entityToChange!=nullptr)
				Logic::CEntityFactory::getSingletonPtr()->deferredDeleteEntity(_entityToChange);

		}
	}


	void CEntityTypeExchanger::moveSelectionToChangedEntity(CEntityPtr entityToSelect)
	{
					//deselect the entity
			GUI::CServer::getSingletonPtr()->getPlayerController()->removeEntityFromSelectedUnits(_entity);

			auto m = std::make_shared<AvatarActivation>();
			m->actived = false;
			_entity->emitMessageN(m);

			auto m2 = std::make_shared<AvatarSelection>();
			m2->selected = false;
			_entity->emitMessageN(m2);

			GUI::CServer::getSingletonPtr()->getPlayerController()->selectEntity(entityToSelect);


	}


	void CEntityTypeExchanger::tick(unsigned int msecs) 
	{
		// Invocar al método de la clase padre (IMPORTANTE)
		IComponent::tick(msecs);

	}

	void CEntityTypeExchanger::setNormalGUI()
	{
			//needed because if you deselect the unit and reselect the workman the icon not correspond to the reality state
		_guiManagerComponent->setAbilityQName(_abilityQName);
		_guiManagerComponent->setAbilityQTooltip(_abilityQTooltip);
		_guiManagerComponent->setAbilityQLuaField(_abilityQluaField);

			//needed because if you deselect the unit and reselect the workman the icon not correspond to the reality state
		_guiManagerComponent->setAbilityWName(_abilityWName);
		_guiManagerComponent->setAbilityWTooltip(_abilityWTooltip);
		_guiManagerComponent->setAbilityWLuaField(_abilityWluaField);
			//needed because if you deselect the unit and reselect the workman the icon not correspond to the reality state
		_guiManagerComponent->setAbilityEName(_abilityEName);
		_guiManagerComponent->setAbilityETooltip(_abilityETooltip);
		_guiManagerComponent->setAbilityELuaField(_abilityEluaField);

		_guiManagerComponent->setAbilityRName(_abilityRName);
		_guiManagerComponent->setAbilityRTooltip(_abilityRTooltip);
		_guiManagerComponent->setAbilityRLuaField(_abilityRluaField);

		if(GUI::CServer::getSingletonPtr()->getActiveUnitController()->getActiveUnit()==_entity)
		{
			GUI::CServer::getSingletonPtr()->getActiveUnitController()->deactivateElements();
			GUI::CServer::getSingletonPtr()->getActiveUnitController()->activateElements();
		}
	}

	void CEntityTypeExchanger::setExchangerGUI()
	{
		//needed because if you deselect the unit and reselect the workman the icon not correspond to the reality state
		_guiManagerComponent->setAbilityQName(_entityToChange->getType().compare("WorkmanGoblin")!=0?_exchangerAbilityQName:"");
		_guiManagerComponent->setAbilityQTooltip(_entityToChange->getType().compare("WorkmanGoblin")!=0?_exchangerAbilityQTooltip:"");
		_guiManagerComponent->setAbilityQLuaField(_entityToChange->getType().compare("WorkmanGoblin")!=0?_exchangerAbilityQcost:"");

			//needed because if you deselect the unit and reselect the workman the icon not correspond to the reality state
		_guiManagerComponent->setAbilityWName(_entityToChange->getType().compare("WarriorGoblin")!=0?_exchangerAbilityWName:"");
		_guiManagerComponent->setAbilityWTooltip(_entityToChange->getType().compare("WarriorGoblin")!=0?_exchangerAbilityWTooltip:"");
		_guiManagerComponent->setAbilityWLuaField(_entityToChange->getType().compare("WarriorGoblin")!=0?_exchangerAbilityWcost:"");

		//needed because if you deselect the unit and reselect the workman the icon not correspond to the reality state
		_guiManagerComponent->setAbilityEName(_entityToChange->getType().compare("RangerGoblin")!=0?_exchangerAbilityEName:"");
		_guiManagerComponent->setAbilityETooltip(_entityToChange->getType().compare("RangerGoblin")!=0?_exchangerAbilityETooltip:"");
		_guiManagerComponent->setAbilityELuaField(_entityToChange->getType().compare("RangerGoblin")!=0?_exchangerAbilityEcost:"");

		//needed because if you deselect the unit and reselect the workman the icon not correspond to the reality state
		_guiManagerComponent->setAbilityRName(_exchangerAbilityRName);
		_guiManagerComponent->setAbilityRTooltip(_exchangerAbilityRTooltip);

		if(GUI::CServer::getSingletonPtr()->getActiveUnitController()->getActiveUnit()==_entity)
		{
			GUI::CServer::getSingletonPtr()->getActiveUnitController()->deactivateElements();
			GUI::CServer::getSingletonPtr()->getActiveUnitController()->activateElements();
		}
	}
	void CEntityTypeExchanger::startExchangerBehaviour(CEntityPtr exchangeEntity,float life,Vector3 pos)
	{
		_entityToChange = exchangeEntity;
		_entityToChangePos = pos;
		_entityToChangeLife = life;

		_changeTypeActivated=true;

		auto playParticleMessage = std::make_shared<PlayParticle>();
		playParticleMessage->particleName = _particleName;
		_entity->emitMessageN(playParticleMessage);

		setExchangerGUI();
	}

	void CEntityTypeExchanger::endExchangerBehaviour()
	{
		_entityToChange = nullptr;
		_entityToChangePos = Vector3::ZERO;
		_entityToChangeLife = 0.0f;

		_changeTypeActivated=false;

		auto stopParticleMessage = std::make_shared<StopParticle>();
		stopParticleMessage->particleName = _particleName;
		_entity->emitMessageN(stopParticleMessage);

		setNormalGUI();
	}

		//------------------------------------------------


	physx::PxRigidDynamic* CEntityTypeExchanger::createTriggerDetector(const Map::CEntity *entityInfo)
	{
		assert(entityInfo->hasAttribute("exchangerCollisionGroup"));
		int wellCollisionGroup = entityInfo->getIntAttribute("exchangerCollisionGroup");

		Vector3 pose = _entity->getPosition();
		pose.y -= _exchangerRange;

		return _actorFactory->createDynamicSphere(pose,_exchangerRange,1,true,true,wellCollisionGroup,this);
	}

	//--------------------------------------------

	void CEntityTypeExchanger::onTrigger(IPhysics *otherComponent, bool enter)
	{
		//if the trigger detect an entity
		if(enter)
		{
			_triggerList.push_back(otherComponent->getEntity());
			//std::cout<<"CEntityTypeExchanger trigger enter with "<< otherComponent->getEntity()->getName()<<std::endl;
			//check if it is the well where we have to go to change entity type
			auto enterTrigger = std::make_shared<EnterIntoExchangeTypeArea>();
			otherComponent->getEntity()->emitMessageN(enterTrigger,this);
		}else
		{
			//std::cout<<"CEntityTypeExchanger trigger exit with "<< otherComponent->getEntity()->getName()<<std::endl;
			std::list<CEntity*>::iterator it=std::find(_triggerList.begin(),_triggerList.end(),otherComponent->getEntity());
			if(it!=_triggerList.end())
				_triggerList.erase(it);
		}
	}

	bool CEntityTypeExchanger::isEntityInTrigger(Logic::CEntity* decisionEntity)
	{
							//check if i'm already near to the well 
		std::list<CEntity*>::iterator it=std::find(_triggerList.begin(),_triggerList.end(),decisionEntity);
		if(it!=_triggerList.end())
			return true;
		else
			return false;
	}
}








