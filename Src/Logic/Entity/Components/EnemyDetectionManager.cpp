/**
@file EnemyDetectionManager.cpp
*/

#include "EnemyDetectionManager.h"
#include "CombatStats.h"
#include "SteeringMovement.h"

#include "AI/EntityManager.h"
#include "AI/Server.h"

#include "Logic/EnemyMessages.h"
#include "Logic/PhysicMessages.h"
#include "Logic/CombatMessages.h"
#include "Logic/RouteCalculationMessages.h"


#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Components/Life.h"
#include "Logic/Entity/Components/BuildComponent.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include "Physics/Server.h"
#include "Physics/PhysicPerceptionManager.h"
#include "Physics/ActorFactory.h"
#include "Physics/SceneManager.h"
#include "Physics/ActorController.h"

//DEBUG to draw line and point
//#include "Graphics/DrawManager.h"
//#include <OgreSceneNode.h>
//#include "Graphics/ManualEntity.h"
//#include <OgreSceneManager.h>
//#include "Graphics/Scene.h"
//#include "Graphics/Server.h"

#include <PxPhysicsAPI.h>

#define OVERLAP_QUERIES_HEIGHT 20
#define CLOSENESS_TOLERANCE 1.5f

namespace Logic 
{
	IMP_FACTORY(CEnemyDetectionManager);

	//---------------------------------------------------------
	Physics::CollisionGroup PHYSIC_FILTER[]={Physics::CollisionGroup::eEnemy,Physics::CollisionGroup::ePlayer};
	
	CEnemyDetectionManager::CEnemyDetectionManager(std::string componentName) : IPhysics(componentName), _triggerDetector(0),
		_isDefendingPosition(false), _canDefendPosition(false),_defendingDistance(0),_movingDetection(false),_canKeepAttacking(false)
	{
		_actorFactory = Physics::CServer::getSingletonPtr()->getActorFactory();
		_actorController = Physics::CServer::getSingletonPtr()->getActorController();
	
	}
	//---------------------------------------------------------

	bool CEnemyDetectionManager::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;		

		_combatComponent = static_cast<Logic::CCombatStats*>(_entity->getComponent("CCombatStats"));
		_steeringComponent = static_cast<Logic::CSteeringMovement*>(_entity->getComponent("CSteeringMovement"));

		if(entityInfo->hasAttribute("is_aggressive"))
			_isAggressive = entityInfo->getBoolAttribute("is_aggressive");

		if(entityInfo->hasAttribute("moving_detection"))
			_movingDetection = entityInfo->getBoolAttribute("moving_detection");

		if(entityInfo->hasAttribute("can_defend_position"))
			_canDefendPosition = entityInfo->getBoolAttribute("can_defend_position");

		assert(entityInfo->hasAttribute("detection_range"));
		_detectionRange = entityInfo->getFloatAttribute("detection_range");

		if(entityInfo->hasAttribute("can_keep_attacking"))
			_canKeepAttacking = entityInfo->getBoolAttribute("can_keep_attacking");

		if(_canDefendPosition)
		{
			assert(entityInfo->hasAttribute("defending_distance"));
			_defendingDistance = entityInfo->getFloatAttribute("defending_distance");			
		}
		_triggerDetector = createTriggerDetector(entityInfo);
		//DEBUG		
		//debugEntity = new Graphics::CManualEntity("debugDefendPosition"+_entity->getName());
		//_entity->getMap()->getScene()->addEntity(debugEntity);

		return true;
	} 

	//---------------------------------------------------------

	CEnemyDetectionManager::~CEnemyDetectionManager() 
	{
		if (_triggerDetector) {
			_actorFactory->destroyActor(_triggerDetector);
			_triggerDetector = NULL;
		}

		_actorFactory = nullptr;		
	} 

	//---------------------------------------------------------

	void CEnemyDetectionManager::awake()
	{

	}

	//---------------------------------------------------------
	
	bool CEnemyDetectionManager::activate()
	{
		if (_triggerDetector)
			Physics::CServer::getSingletonPtr()->getSceneManager()->addToScene(_triggerDetector);
		return true;
	}

	//---------------------------------------------------------

	void CEnemyDetectionManager::deactivate()
	{
		if (_triggerDetector)
			Physics::CServer::getSingletonPtr()->getSceneManager()->removeFromScene(_triggerDetector);
	}

	//---------------------------------------------------------

	bool CEnemyDetectionManager::acceptN(const std::shared_ptr<NMessage> &message)
	{
	 return (message->type.compare("AvatarFinishedRoute") == 0) ||  (message->type.compare("AttackEnemy") == 0) ||  
		 (message->type.compare("AvatarRouteTo") == 0) ;
	}

	//---------------------------------------------------------

	void CEnemyDetectionManager::processN(const std::shared_ptr<NMessage> &message)
	{		
		if  (message->type.compare("AvatarFinishedRoute") == 0)
		{
			//Si está defendiendo una posición y la posición actual es muy cercana a la posición a defender
			if (_isDefendingPosition && _entity->getPosition().distance(_positionToDefend) <= CLOSENESS_TOLERANCE)
			{
				_isDefendingPosition = false;
			}	
		}
		else if  (message->type.compare("AvatarRouteTo") == 0)
		{
			if(_isDefendingPosition && message->sender != _entity)
			{
				_isDefendingPosition = false;							
			}
		}
		else if  (message->type.compare("AttackEnemy") == 0)
		{
			if (_isDefendingPosition && message->sender != _entity)
			{
				_isDefendingPosition = false;
			}				
		}
	}
	

	//------------------------------------------------


	physx::PxRigidDynamic* CEnemyDetectionManager::createTriggerDetector(const Map::CEntity *entityInfo)
	{
		assert(entityInfo->hasAttribute("detector_collision_group"));
		int detectorCollisionGroup = entityInfo->getIntAttribute("detector_collision_group");

		//DEBUG
		//drawDetector = _entity->getMap()->getScene()->getSceneMgr()->getRootSceneNode()->createChildSceneNode();
		//debugEntity->drawCircleXZ(detectionRange);

		Vector3 pose = _entity->getPosition();
		pose.y -= _detectionRange;

		return _actorFactory->createDynamicSphere(pose,_detectionRange,1,true,true,detectorCollisionGroup,this);
	}

	//--------------------------------------------

	void CEnemyDetectionManager::onTrigger(IPhysics *otherComponent, bool enter)
	{		

		if (enter) {
			//std::cout << "Entra: " << otherComponent->getEntity()->getName() << std::endl;
			//Si la unidad está parada, no está defendiendo ninguna posición, no tiene ningún enemigo asignado y se cumple que: la entidad es aliada y la otra entidad es enemiga o
			//la unidad es enemiga y la otra unidad es aliada
			if(_isAggressive && _combatComponent->getCurrentEnemy() == NULL && resolveMovingDetection() && otherIsFoe(otherComponent->getEntity()))
			{	
				//Asignamos el enemigo
				_combatComponent->setCurrentEnemy(otherComponent->getEntity());
				
				//Si la entidad puede defender la posición (parametro ajustable en el map), entonces se marca que lo está haciendo y se asigna la posición actual
				if(_canDefendPosition){
					_positionToDefend = _entity->getPosition();
					_isDefendingPosition = true;
				}

				//Mandamos el mensaje de atacar
				auto m2 = std::make_shared<AttackEnemy>();
				_entity->emitMessageN(m2,this);
			}
			Logic::CBuildComponent * buildComponent = static_cast<Logic::CBuildComponent*>(otherComponent->getEntity()->getComponent("CBuildComponent"));
			if(otherComponent->getEntity()->getType() == "Well" && _entity->isEnemy() && (buildComponent->isBuilded() && !buildComponent->isIndestructible()))
			{
				auto m2 = std::make_shared<PlayerSeen>();
				m2->ent = otherComponent->getEntity();
				_entity->emitMessageN(m2);
			}
			
		} else {
			//std::cout << "Sale: " << otherComponent->getEntity()->getName() << std::endl;
			//Si la otra endidad que sale del trigger es el actual enemigo asignado y se cumple que: la entidad es aliada y la otra entidad es enemiga o
			//la unidad es enemiga y la otra unidad es aliada
			if (otherComponent->getEntity() == _combatComponent->getCurrentEnemy() && otherIsFoe(otherComponent->getEntity()) && _entity->isPlayer())
			{			
				//Mandamos el mensaje de enemigo fuera de alcance
				auto message = std::make_shared<EnemyLost>();
				message->enemy = otherComponent->getEntity();
				_entity->emitMessageN(message, this);
			}
		}
	}

	
	//---------------------------------------------------------

	void CEnemyDetectionManager::tick(unsigned int msecs) 
	{
		// Invocar al método de la clase padre (IMPORTANTE)
		IComponent::tick(msecs);
		
		if(_triggerDetector)
		{
			//movemos el detector
			Vector3 pos =  _entity->getPosition();
			pos.y -= _detectionRange;
			_actorController->setGlopalPosition(_triggerDetector,pos);
		}
		
		//DEBUG
		//debugEntity->setPosition(_entity->getPosition());

	}

	//---------------------------------------------------------

	Logic::CEntity* CEnemyDetectionManager::getClosestEnemy(Logic::CEntity* enemyToAvoid)
	{
		//Lanzamos un volumen de colisión contra el grupo de colisión de los enemigos en un area alrededor de la entidad
		std::list<Logic::CEntity*>* enemiesAround = Physics::CServer::getSingletonPtr()->getPhysicPerceptionManager()->overlapQueries(_entity->getPosition(), _detectionRange, 
			&std::vector<Physics::CollisionGroup> (PHYSIC_FILTER, PHYSIC_FILTER + sizeof(PHYSIC_FILTER) / sizeof(PHYSIC_FILTER[0])));
		
		//Si se han encontrado enemigos
		if(enemiesAround->size() > 0)
		{
			std::list<Logic::CEntity*>::iterator it;
			std::set<Logic::CEntity*> enemySet;
			
			for(it = enemiesAround->begin(); it != enemiesAround->end(); ++it)
			{				
				if((*it)->isActivated())
				{
					//Insertamos en una lista los enemigos encontrados y que sean diferentes a "enemyToAvoid"
					if(_entity->isPlayer())
					{
						if((*it)->isEnemy() && (*it)!= enemyToAvoid)
							enemySet.insert(*it);
					}
					else if (_entity->isEnemy())
					{
						if((*it)->isPlayer() && (*it)!= enemyToAvoid)
							enemySet.insert(*it);
					}
				}
			}		
			delete enemiesAround;
			//Usamos el EntityManager para encontrar, de todos los enemigos dentro del área, el más cercano
			return AI::CServer::getSingletonPtr()->getEntityManager()->getClosestEntity(&enemySet,_entity->getPosition());
		}
		delete enemiesAround;
		return NULL;
	}
	

	//---------------------------------------------------------

	bool CEnemyDetectionManager::searchEnemies(Logic::CEntity* previousEnemy)
	{	

		Logic::CEntity* closestEnemy = getClosestEnemy(previousEnemy);		
		if(closestEnemy != NULL)
		{
			_combatComponent->setCurrentEnemy(closestEnemy);
			auto m2 = std::make_shared<AttackEnemy>();
			_entity->emitMessageN(m2,this);
			return true;
		}
		return false;
	}

		
	//---------------------------------------------------------

	bool CEnemyDetectionManager::otherIsFoe(Logic::CEntity *other)
	{
		return ((_entity->isPlayer() && other->isEnemy()) || (_entity->isEnemy() && other->isPlayer()));
	}

	//---------------------------------------------------------

	bool CEnemyDetectionManager::resolveMovingDetection()
	{
		if(!_movingDetection && _steeringComponent->getCurrentSpeed() != Vector3::ZERO)
			return false;
		return true;
	}

	//---------------------------------------------------------

	void CEnemyDetectionManager::setIsAggressive(bool isAggressive)
	{
		_isAggressive = isAggressive;
		if(_isAggressive)
		{
			searchEnemies(NULL);
		}
	}
}








