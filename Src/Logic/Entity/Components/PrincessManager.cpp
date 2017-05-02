/**
@file PrincessManager.cpp

Contiene la implementación del componente que controla la condición
de selección de una entidad

@see Logic::IComponent

@author Iván León
@date Febrero, 2015
*/

#include "PrincessManager.h"

#include <limits>

#include "Logic/GraphicsMessages.h"
#include "Logic/AllyUnitMessages.h"
#include "Logic/UnitMessages.h"
#include "Logic/CombatMessages.h"
#include "Logic/PrincessMessages.h"
#include "Logic/EntityMessages.h"
#include "Logic/MovementMessages.h"
#include "Logic/AudioMessages.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Components/DirectionalLight.h"
#include "Logic/Entity/Components/WorldComponent.h"
#include "Logic/Server.h"
#include "Logic/Maps/Map.h"

#include "AI/Server.h"
#include "AI/EntityManager.h"

#include "Map/MapEntity.h"

#include "Graphics/Scene.h"
#include "Graphics/FrustumEntity.h"

#include "GUI/Server.h"
#include "GUI/WrathSystemController.h"
#include "GUI/MinimapController.h"
#include "GUI/PlayerController.h"

// Habilidad para aturdir enemigos dentro de un área.
#include "Physics\Server.h"
#include "Physics\PhysicPerceptionManager.h"

#define MAX_PROJECTION_HEIGHT 70
#define MIN_PROJECTION_HEIGHT 20

#define MAX_WRATH 100
#define DECAL_HEIGHT_DISTANCE_PROP 0.5f

namespace Logic 
{
	IMP_FACTORY(CPrincessManager);

	const Physics::CollisionGroup PHYSIC_ENEMY_FILTER[]={Physics::CollisionGroup::eEnemy, Physics::CollisionGroup::eUntargetable};
	const Physics::CollisionGroup PHYSIC_UNITS[]={Physics::CollisionGroup::ePlayer, Physics::CollisionGroup::eEnemy, Physics::CollisionGroup::eUntargetable};
	
	//---------------------------------------------------------

	CPrincessManager::~CPrincessManager() 
	{
		if(_node)
		{	
			_scene->removeEntity(_node);
			delete _node;
			_node = 0;
		}
	} // ~CGraphics
	 
	//---------------------------------------------------------

	bool CPrincessManager::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		_scene = _entity->getMap()->getScene();

		_node = new Graphics::CFrustumEntity(_entity->getName() + "_aura");
		if(!_scene->addEntity(_node))
			return false;
			
		//Variables para el aura
		if(entityInfo->hasAttribute("aura_texture_width"))
			_size.x = entityInfo->getFloatAttribute("aura_texture_width");

		if(entityInfo->hasAttribute("aura_texture_height"))
			_size.y = entityInfo->getFloatAttribute("aura_texture_height");
		
		if(entityInfo->hasAttribute("aura_projection_height"))
			_projectionHeight = entityInfo->getFloatAttribute("aura_projection_height");
		
		if(entityInfo->hasAttribute("aura_material"))
			 _materialToProject = entityInfo->getStringAttribute("aura_material");

		//Variables para la ira
		if(entityInfo->hasAttribute("wrath_per_casualty"))
			 _wrathPerCasualty = entityInfo->getIntAttribute("wrath_per_casualty");

		if(entityInfo->hasAttribute("wrath_per_damage"))
			_wrathPerDamage = entityInfo->getIntAttribute("wrath_per_damage");		

		if(entityInfo->hasAttribute("wrath_per_attack"))
			_wrathPerAttack = entityInfo->getIntAttribute("wrath_per_attack");		

		if(entityInfo->hasAttribute("time_to_decrease"))
			_timeToDecreaseWrath = entityInfo->getFloatAttribute("time_to_decrease");		

		//Tiempo que tarda en despertar tras caer abatida
		if(entityInfo->hasAttribute("time_to_awake"))
			_timeToAwake = entityInfo->getIntAttribute("time_to_awake");		

		//Variables para la habilidad de curar en área
		if(entityInfo->hasAttribute("aoe_heal_quantity"))
			_aoeHealQuantity = entityInfo->getFloatAttribute("aoe_heal_quantity");	

		if(entityInfo->hasAttribute("aoe_heal_cost"))
			_aoeHealCost = entityInfo->getIntAttribute("aoe_heal_cost");	

		if(entityInfo->hasAttribute("aoe_heal_particle"))
			_aoeHealParticle = entityInfo->getStringAttribute("aoe_heal_particle");	


		//Variables para la habilidad de mejorar aliados en area		
		if(entityInfo->hasAttribute("aoe_buff_duration"))
			_aoeBuffDuration = entityInfo->getIntAttribute("aoe_buff_duration");	

		if(entityInfo->hasAttribute("aoe_buff_percentage"))
			_aoeBuffPercentage = entityInfo->getFloatAttribute("aoe_buff_percentage");	

		if(entityInfo->hasAttribute("aoe_buff_cost"))
			_aoeBuffCost = entityInfo->getIntAttribute("aoe_buff_cost");	

		if(entityInfo->hasAttribute("aoe_buff_particle"))
			_aoeBuffParticle = entityInfo->getStringAttribute("aoe_buff_particle");	

		//Variables para la habilidad de empujar enemigos en area
		if(entityInfo->hasAttribute("aoe_stun_time"))
			_aoeStunTime = entityInfo->getFloatAttribute("aoe_stun_time");	

		if(entityInfo->hasAttribute("aoe_stun_distance"))
			_aoeStunDistance = entityInfo->getFloatAttribute("aoe_stun_distance");	

		if(entityInfo->hasAttribute("aoe_stun_cost"))
			_aoeStunCost = entityInfo->getIntAttribute("aoe_stun_cost");	

		if(entityInfo->hasAttribute("aoe_stun_particle"))
			_aoeStunParticle = entityInfo->getStringAttribute("aoe_stun_particle");	

		//Variables para la habilidad de inmortalidad
		if(entityInfo->hasAttribute("berserkerModeDuration"))
			_berserkerModeDuration = entityInfo->getIntAttribute("berserkerModeDuration");	

		if(entityInfo->hasAttribute("berserkerModeAttackModifier"))
			_berserkerModeAttackModifier = entityInfo->getFloatAttribute("berserkerModeAttackModifier");	

		if(entityInfo->hasAttribute("berserkerModeCost"))
			_berserkerModeCost = entityInfo->getIntAttribute("berserkerModeCost");

		if(entityInfo->hasAttribute("berserker_particle"))
			_berserkerParticle = entityInfo->getStringAttribute("berserker_particle");	

		if(entityInfo->hasAttribute("limitTimeOnDangerousWrathLevel"))
			_limitTimeOnDangerousWrathLevel = entityInfo->getIntAttribute("limitTimeOnDangerousWrathLevel");

		if(entityInfo->hasAttribute("currentTimeOnDangerousWrathLevel"))
			_currentTimeOnDangerousWrathLevel = entityInfo->getIntAttribute("currentTimeOnDangerousWrathLevel");

		if(entityInfo->hasAttribute("wrathOverflowParticleName"))
			wrathOverflowParticleName = entityInfo->getStringAttribute("wrathOverflowParticleName");

		if(entityInfo->hasAttribute("wrathOverflowSoundName"))
			wrathOverflowSoundName = entityInfo->getStringAttribute("wrathOverflowSoundName");

		if(entityInfo->hasAttribute("wrathOverflowActive"))
			_wrathOverflowActive = entityInfo->getBoolAttribute("wrathOverflowActive");

		if(entityInfo->hasAttribute("wrathOverflowDamage"))
			_wrathOverflowDamage = entityInfo->getIntAttribute("wrathOverflowDamage");


		initDecal();
		
		return true;

	} // spawn	

	//---------------------------------------------------------

	void CPrincessManager::awake(){
		if(_materialToProject !="")
		{
			auto m= std::make_shared<AreaOfEffect>();
			m->entityName = _entity->getName();
			m->materialToProject=_materialToProject;
			m->frustum = _node;
			_entity->getMap()->sendMessageToAll(m);
		}
		_berserkerModeActivated = false;
		_berserkerModeTimer = 0;
		_currentTimeOnDangerousWrathLevel = 0;

		GUI::CServer::getSingletonPtr()->getPlayerController()->setTypeIsClickable(1, "Princess", true);
	}

	//---------------------------------------------------------

	void CPrincessManager::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);

		Vector3 p=_entity->getPosition();
		
		_node->setPosition(p);
		
		_node->setOrthoWindowHeight(_projectionHeight); 

		_node->setFarClipDistance(_projectionHeight); 
		
		_timer += msecs;

		if(_timer > _timeToDecreaseWrath)
		{
			if(_wrath > 0)
			{
				--_wrath;

				if(_wrath < MAX_WRATH * 3/4)
				{
					auto stopParticleMessage = std::make_shared<StopParticle>();
					stopParticleMessage->particleName = "blackHole";
					_entity->emitMessageN(stopParticleMessage);
				}

				GUI::CServer::getSingletonPtr()->getWrathSystemController()->setWrathLevel(_wrath);
			}
			_timer = 0;
//			std::cout << "Wrath: " << _wrath << std::endl;
		}

		_projectionHeight = MIN_PROJECTION_HEIGHT + _wrath/2.0f;

		if(_berserkerModeActivated)
		{
			_berserkerModeTimer += msecs;
			if(_berserkerModeTimer >= _berserkerModeDuration)
			{
				auto startParticleMessage = std::make_shared<StopParticle>();
				startParticleMessage->particleName = "lightningBolt";
				_entity->emitMessageN(startParticleMessage);
				_berserkerModeActivated = false;
				_berserkerModeTimer = 0;
			}
		}

		if (_wrath >= MAX_WRATH * 0.95 && _wrathOverflowActive)
		{
			_currentTimeOnDangerousWrathLevel += msecs;
			if (_currentTimeOnDangerousWrathLevel >= _limitTimeOnDangerousWrathLevel)
				wrathOverflow();
		}
		else
			_currentTimeOnDangerousWrathLevel = 0;
		
	}

	//---------------------------------------------------------

	bool CPrincessManager::acceptN(const std::shared_ptr<NMessage> &message)
	{
		return  (message->type.compare("DamageMessage") == 0) ||  (message->type.compare("ModifyWrath") == 0) ||  (message->type.compare("EntityDestroying") == 0) 
			|| (message->type.compare("PrincessTaken") == 0) || (message->type.compare("CastAbility") == 0) ;
	} // acceptN

	//---------------------------------------------------------

	void CPrincessManager::processN(const std::shared_ptr<NMessage> &message)
	{
		if  (message->type.compare("DamageMessage") == 0)
		{			
			auto m = std::static_pointer_cast<DamageMessage>(message);
			modifyWrath(WrathModifiers::damaged);
		}
		else if  (message->type.compare("ModifyWrath") == 0)
		{
			auto m = std::static_pointer_cast<ModifyWrath>(message);
			modifyWrath(m->reason);
		}
		else if  (message->type.compare("EntityDestroying") == 0)
		{
			auto m = std::static_pointer_cast<EntityDestroying>(message);
			if(m->entity->isPlayer() && m->entity->getPosition().distance(_entity->getPosition()) < getWrathAreaRadius())
			{
				modifyWrath(WrathModifiers::casualty);
			}
		}
		else if (message->type.compare("PrincessTaken") == 0)
		{
			auto m = std::static_pointer_cast<PrincessTaken>(message);
			_raptor = m->raptor;
			auto mPT = std::make_shared<PrincessToTakenState>();
			
			_entity->emitMessageN(mPT);
		}
		else if(message->type.compare("CastAbility") == 0)
		{
			auto m = std::static_pointer_cast<CastAbility>(message);
			if(m->ability == "Q")
			{
				if(_wrath >= _aoeStunCost)
					aoeStun();
			}
			else if (m->ability == "W")
			{
				if(_wrath >= _aoeBuffCost)
					aoeBuff();
			}
			else if (m->ability == "E")
			{
				if(_wrath >= _aoeHealCost)
					aoeHealing();
			}
			else if (m->ability == "R")
			{
				if(_wrath >= _berserkerModeCost)
					activateBerserkerMode();
			}
		}
	} // processN
 
	//---------------------------------------------------------

	float CPrincessManager::getWrathAreaRadius()
	{
		return _projectionHeight * DECAL_HEIGHT_DISTANCE_PROP;
	}

	//---------------------------------------------------------

	void CPrincessManager::initDecal()
    {
		_heightIncrease = 0.02f;
		_node->setProjectionType(Ogre::PT_ORTHOGRAPHIC);
        _node->setOrientation(Ogre::Quaternion(Ogre::Degree(90),Ogre::Vector3::UNIT_X));
 
	  // update aspect ratio
		_node->setAspectRatio(_size.x/_size.y);
 
        // update height
		_node->setOrthoWindowHeight(_projectionHeight); 

		_node->setFarClipDistance(_projectionHeight); 
    
		_node->setDebugDisplayEnabled(true);	

    }

	//---------------------------------------------------------

	void CPrincessManager::modifyWrath(int reason)
	{
		if(!_isUnconscius)
		{
			switch (reason)
			{
				case WrathModifiers::attack:
					_wrath += _wrathPerAttack;
					break;

				case WrathModifiers::faint:
					_wrath = 0;
					break;

				case WrathModifiers::casualty:
					_wrath += _wrathPerCasualty;
					break;

				case WrathModifiers::damaged:
					_wrath += _wrathPerDamage;
					break;

				case WrathModifiers::aoeHeal:
					_wrath -= _aoeHealCost;
					break;

				case WrathModifiers::aoeBuff:
					_wrath -= _aoeBuffCost;
					break;

				case WrathModifiers::aoeStun:
					_wrath -= _aoeStunCost;
					break;

				case WrathModifiers::berserk:
					_wrath -= _berserkerModeCost;
					break;
			}
		}

		if(_wrath > MAX_WRATH * 4/5)
		{
			auto startParticleMessage = std::make_shared<PlayParticle>();
			startParticleMessage->particleName = "blackHole";
			_entity->emitMessageN(startParticleMessage);
		}
		else if(_wrath < MAX_WRATH * 3/4)
		{
			auto stopParticleMessage = std::make_shared<StopParticle>();
			stopParticleMessage->particleName = "blackHole";
			_entity->emitMessageN(stopParticleMessage);
		}

		if(_wrath > MAX_WRATH)
			_wrath = MAX_WRATH;

		GUI::CServer::getSingletonPtr()->getWrathSystemController()->setWrathLevel(_wrath);
		_projectionHeight = MIN_PROJECTION_HEIGHT + _wrath/2.0f;
	}

	//---------------------------------------------------------

	void CPrincessManager::aoeHealing()
	{
		auto soundMessage = std::make_shared<PlayAudio>();
		soundMessage->eventName = "event:/Princess/SpellE";
		_entity->emitMessageN(soundMessage);

		auto m = std::make_shared<HealUnit>();
		m->healing = _aoeHealQuantity;

		auto startParticleMessage = std::make_shared<PlayParticle>();
		startParticleMessage->particleName = _aoeHealParticle;		

		for(std::set<Logic::CEntity*>::const_iterator itAllies = AI::CServer::getSingletonPtr()->getEntityManager()->getAlliesList()->begin();
			itAllies != AI::CServer::getSingletonPtr()->getEntityManager()->getAlliesList()->end() ; ++itAllies)
		{	
			if ((*itAllies)->getPosition().distance(_entity->getPosition()) <= getWrathAreaRadius())
			{
				(*itAllies)->emitMessageN(m);
				(*itAllies)->emitMessageN(startParticleMessage);
			}
		}	
		_entity->emitMessageN(startParticleMessage);
		_entity->emitMessageN(m);//Quitar si la princesa no se cura a sí misma con el área
		modifyWrath(WrathModifiers::aoeHeal);

	}

	//---------------------------------------------------------

	void CPrincessManager::aoeBuff()
	{
		auto soundMessage = std::make_shared<PlayAudio>();
		soundMessage->eventName = "event:/Princess/SpellW";
		_entity->emitMessageN(soundMessage);

		auto mAttack = std::make_shared<ModifyStat>();
		mAttack->modID = "PrincessAttackBuff";
		mAttack->modType = 0;
		mAttack->durationMsecs = _aoeBuffDuration;
		mAttack->percentage = _aoeBuffPercentage;

		auto mLife = std::make_shared<ModifyStat>();
		mLife->modID = "PrincessLifeBuff";
		mLife->modType = 1;
		mLife->durationMsecs = _aoeBuffDuration;
		mLife->percentage = _aoeBuffPercentage;

		auto mSpeed = std::make_shared<ModifyStat>();
		mSpeed->modID = "PrincessSpeedBuff";
		mSpeed->modType = 2;
		mSpeed->durationMsecs = _aoeBuffDuration;
		mSpeed->percentage = _aoeBuffPercentage;

		auto startParticleMessage = std::make_shared<PlayParticle>();
		startParticleMessage->particleName = _aoeBuffParticle;		

		for(std::set<Logic::CEntity*>::const_iterator itAllies = AI::CServer::getSingletonPtr()->getEntityManager()->getAlliesList()->begin();
			itAllies != AI::CServer::getSingletonPtr()->getEntityManager()->getAlliesList()->end() ; ++itAllies)
		{	
			if ((*itAllies)->getPosition().distance(_entity->getPosition()) <= getWrathAreaRadius())
			{
				(*itAllies)->emitMessageN(mAttack);
				(*itAllies)->emitMessageN(mLife);
				(*itAllies)->emitMessageN(mSpeed);
				(*itAllies)->emitMessageN(startParticleMessage);
			}
		}	

		modifyWrath(WrathModifiers::aoeBuff);
	}

	//---------------------------------------------------------

	void CPrincessManager::aoeStun()
	{
		auto soundMessage = std::make_shared<PlayAudio>();
		soundMessage->eventName = "event:/Princess/SpellQ";
		_entity->emitMessageN(soundMessage);

		auto scaleParticleMessage = std::make_shared<ScaleParticle>();
		scaleParticleMessage->particleName = _aoeStunParticle;
		scaleParticleMessage->scale = getWrathAreaRadius()/100;
		_entity->emitMessageN(scaleParticleMessage);

		auto startParticleMessage = std::make_shared<PlayParticle>();
		startParticleMessage->particleName = _aoeStunParticle;
		_entity->emitMessageN(startParticleMessage);

		
		
		auto stunMessage = std::make_shared<StunHability>();
		stunMessage->origin = _entity->getPosition();
		stunMessage->distance = _aoeStunDistance * getWrathAreaRadius();
		stunMessage->time = _aoeStunTime;

		std::list<Logic::CEntity*> *list= Physics::CServer::getSingletonPtr()->getPhysicPerceptionManager()->overlapQueries(_entity->getPosition(), getWrathAreaRadius(), 
			&std::vector<Physics::CollisionGroup> (PHYSIC_ENEMY_FILTER, PHYSIC_ENEMY_FILTER + sizeof(PHYSIC_ENEMY_FILTER) / sizeof(PHYSIC_ENEMY_FILTER[0])));
		//std::cout << "Detectados: " << (*list).size() << std::endl;

		for(Logic::CEntity*e : *list)
		{
			if(e!=_entity)
			{
				e->emitMessageN(stunMessage);				
				//std::cout << "Nombre: " << e->getName() << std::endl;
			}
		}
		delete list;

		modifyWrath(WrathModifiers::aoeStun);
	}

	//---------------------------------------------------------

	void CPrincessManager::activateBerserkerMode()
	{
		if(!_berserkerModeActivated)
		{
			auto soundMessage = std::make_shared<PlayAudio>();
			soundMessage->eventName = "event:/Princess/SpellR";
			_entity->emitMessageN(soundMessage);

			auto startParticleMessage = std::make_shared<PlayParticle>();
			startParticleMessage->particleName = _berserkerParticle;
			_entity->emitMessageN(startParticleMessage);

			auto attackModifierMessage = std::make_shared<ModifyStat>();
			attackModifierMessage->modID = "PrincessManagerAttack";
			attackModifierMessage->modType = 0;
			attackModifierMessage->durationMsecs = _berserkerModeDuration;
			attackModifierMessage->percentage = _berserkerModeAttackModifier;
			_entity->emitMessageN(attackModifierMessage);

			auto berserkerModifierMessage = std::make_shared<ModifyStat>();
			berserkerModifierMessage->modID = "PrincessManagerBerserker";
			berserkerModifierMessage->modType = 3;
			berserkerModifierMessage->durationMsecs = _berserkerModeDuration;
			_entity->emitMessageN(berserkerModifierMessage);

			_berserkerModeActivated = true;

			modifyWrath(WrathModifiers::berserk);
		}
	}

	//---------------------------------------------------------

	void CPrincessManager::wrathOverflow()
	{
		std::list<Logic::CEntity*>* unitsInRadiusOfEffect = Physics::CServer::getSingletonPtr()->getPhysicPerceptionManager()->overlapQueries(_entity->getPosition(), 
					this->getWrathAreaRadius(), &std::vector<Physics::CollisionGroup> (PHYSIC_UNITS, PHYSIC_UNITS + sizeof(PHYSIC_UNITS) / sizeof(PHYSIC_UNITS[0])));
				
		auto damageMessage = std::make_shared<DamageMessage>();
		damageMessage->damage = _wrathOverflowDamage;

		for (std::list<Logic::CEntity*>::iterator it = unitsInRadiusOfEffect->begin(); it != unitsInRadiusOfEffect->end(); ++it)
		{
			if ((*it)->getType() != "Princess")
				(*it)->emitMessageN(damageMessage);
		}
		//_entity->emitMessageN(damageMessage);

		auto startParticleMessage = std::make_shared<PlayParticle>();
		startParticleMessage->particleName = wrathOverflowParticleName;
		_entity->emitMessageN(startParticleMessage);

		auto playOverflowSoundMessage = std::make_shared<PlayAudio>();
		playOverflowSoundMessage->eventName = wrathOverflowSoundName;
		_entity->emitMessageN(playOverflowSoundMessage);

		//_wrath = 0;
		modifyWrath(WrathModifiers::faint);
		_currentTimeOnDangerousWrathLevel = 0;

		delete unitsInRadiusOfEffect;
	}

}// namespace Logic





