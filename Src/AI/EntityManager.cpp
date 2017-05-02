/**
@file EntityManager.cpp


@author Guidi Giacomo
@date March, 2015
*/
#include "EntityManager.h"
#include "AI\Server.h"
#include "AI\GameManager.h"

#include "GUI\Server.h"
#include "GUI\StatisticsController.h"

#include "Physics/Server.h"
#include "Physics/PhysicPerceptionManager.h"
#include "Logic\Entity\Entity.h"
#include "Logic\RouteCalculationMessages.h"
#include "Logic\Entity\Components\SteeringMovement.h"

#define MAX_DIST 10000

namespace AI {
	
	/**
	Constructor de la clase.
	*/
	CEntityManager::CEntityManager():_worldLimitsTL(Vector2::ZERO),_worldLimitsBR(Vector2::ZERO)
	{
	}
	/////////////////////////////////////////
	/**
	Destructor de la clase.
	*/
	CEntityManager::~CEntityManager(void)
	{
	}

	void CEntityManager::tick(unsigned int msecs){
		if(_worldLimitsTL!=Vector2::ZERO && _worldLimitsBR !=Vector2::ZERO)
		{
			verifyEntityListIsInsideMap(&_allyUnitList);

			if(!AI::CServer::getSingletonPtr()->getGameManager()->isPrincessKidnapped())
				verifyEntityListIsInsideMap(&_princessList);
		}
	}
	void CEntityManager::verifyEntityListIsInsideMap(std::set<Logic::CEntity*> * list)
	{
				//check for all unit it they are into visible map area
		std::set<Logic::CEntity*>::const_iterator it=list->begin(); 
		for(;it!=list->end();++it)
		{
			if(!checkEntityIsInsideMap(*it))
			{
				//if they are out of maps get the movement speed
				Logic::IComponent *iComponent = (*it)->getComponent("CSteeringMovement");
				assert(iComponent);
				Logic::CSteeringMovement *steering = static_cast<Logic::CSteeringMovement*>(iComponent);
				Vector3 dir=steering->getCurrentSpeed();
				//if the movement is torward out of visible map area send a message to interrupt the movement
				if( (*it)->getPosition().x<=_worldLimitsTL.x && dir.x<0 || (*it)->getPosition().x>=_worldLimitsBR.x && dir.x>0 ||
					(*it)->getPosition().z<=_worldLimitsTL.y && dir.z<0 || (*it)->getPosition().z>=_worldLimitsBR.y && dir.z>0)
				{

					auto interruptMovement = std::make_shared<AvatarInterruptRoute>();
					(*it)->emitMessageN(interruptMovement);
				}
			}
		}
	}

	bool CEntityManager::checkEntityIsInsideMap(Logic::CEntity*e)
	{
		if(e->getPosition().x>=_worldLimitsTL.x && e->getPosition().x<=_worldLimitsBR.x
			&& e->getPosition().z>=_worldLimitsTL.y && e->getPosition().z<=_worldLimitsBR.y)
			return true;
		else
			return false;
	}

	void CEntityManager::setupWorldLimits(float left,float top,float right,float bottom){
		_worldLimitsTL.x = left;
		_worldLimitsTL.y = top;
		_worldLimitsBR.x = right;
		_worldLimitsBR.y = bottom;
	}


	Logic::CEntity * CEntityManager::getClosestEntity(EntityType type, Vector3 position,Logic::CEntity*entityToAvoid){
		Logic::CEntity *e= nullptr;
		switch(type)
		{
		case EntityType::Ally:
			e=this->getClosestEntity(&_allyUnitList,position,entityToAvoid);
			break;
		case EntityType::Enemy:
			e=this->getClosestEntity(&_enemyUnitList,position,entityToAvoid);
			break;
		case EntityType::Chest:
			e=this->getClosestEntity(&_chestList,position,entityToAvoid);
			break;
		case EntityType::TrapChest:
			e=this->getClosestEntity(&_trapChestList,position,entityToAvoid);
			break;
		case EntityType::Resource:
			e=this->getClosestEntity(&_resourceList,position,entityToAvoid);
			break;
		case EntityType::Princess:
			e=this->getClosestEntity(&_princessList, position,entityToAvoid);
			break;
		case EntityType::SpawnPoint:
			e=this->getClosestEntity(&_spawnPointList, position,entityToAvoid);
			break;
		default:
			break;
		}
		return e;
	}

	Logic::CEntity* CEntityManager::getClosestEntity(std::set<Logic::CEntity*> * list,Vector3 position,Logic::CEntity*entityToAvoid)
	{
		if(list->size()>0)
		{
			
			std::set<Logic::CEntity*>::const_iterator iterator=list->begin();
			Logic::CEntity *min=nullptr;
			float minDist= MAX_DIST;
			for (; iterator != list->end(); ++iterator) {
				//check if the entity that we still compute de distance it isn't the entity to avoid
				if(!(entityToAvoid!=nullptr && (*iterator)==entityToAvoid))
				{
					float d=position.distance((*iterator)->getPosition());
					if(d<minDist)
					{
						minDist=d;
						min=(*iterator);
					}
				}
			}
			return min;
		}
		return nullptr;
	}



	CEntityManager::EntityType CEntityManager::getEntityType(std::string entityType, Logic::CEntity *e)
	{
		if(e->isEnemy())
			return EntityType::Enemy;
		else if(e->isPlayer() && entityType.compare("Princess") != 0)
			return EntityType::Ally;
		else if(entityType.compare("Tree")==0 || entityType.compare("Iron")==0 || entityType.compare("Rock")==0 )
			return EntityType::Resource;
		else if(entityType.compare("Chest")==0)
			return EntityType::Chest;
		else if(entityType.compare("Trap")==0)
			return EntityType::TrapChest;
		else if(entityType.compare("Princess")==0)
			return EntityType::Princess;
		else if(entityType.compare("SpawnPoint")==0)
			return EntityType::SpawnPoint;
		else if(entityType.compare("Well")==0)
			return EntityType::Well;
		else 
			return EntityType::Other;
	}

	bool CEntityManager::registerEntity(Logic::CEntity * e)
	{
		return registerEntity(getEntityType(e->getType(), e),e);
	}


	bool CEntityManager::unregisterEntity(Logic::CEntity * e)
	{
		return unregisterEntity(getEntityType(e->getType(), e),e);
	}

	bool CEntityManager::registerEntity(EntityType type,Logic::CEntity*e)
	{
		switch(type)
		{
		case EntityType::Ally:			
			return this->registerEntity(&_allyUnitList, e);
			break;
		case EntityType::Enemy:
			return this->registerEntity(&_enemyUnitList, e);
			break;
		case EntityType::Chest:
			return this->registerEntity(&_chestList, e);
			break;
		case EntityType::TrapChest:
			return this->registerEntity(&_trapChestList,e);
			break;
		case EntityType::Resource:
			return this->registerEntity(&_resourceList, e);
			break;
		case EntityType::Princess:
			return this->registerEntity(&_princessList, e);
			break;
		case EntityType::SpawnPoint:
			return this->registerEntity(&_spawnPointList, e);
			break;
		case EntityType::Other:
			return this->registerEntity(&_otherList,e);
			break;
		case EntityType::Well:
			return this->registerEntity(&_wellList,e);
			break;
		default:
			break;
		}
		return false;
	}

	bool CEntityManager::registerEntity(std::set<Logic::CEntity*> * activeList, Logic::CEntity*e)
	{
		activeList->insert(e);
		return true;
	}


	bool CEntityManager::unregisterEntity(EntityType type, Logic::CEntity* e)
	{
		switch(type)
		{
		case EntityType::Ally:	
			GUI::CServer::getSingletonPtr()->getStatisticsController()->setGoblinCounter(this->getEntitiesNumber(EntityType::Ally,"") - 1);
			return this->unregisterEntity(&_allyUnitList, e);
			break;
		case EntityType::Enemy:
			return this->unregisterEntity(&_enemyUnitList, e);
			break;
		case EntityType::Chest:
			return this->unregisterEntity(&_chestList,e);
			break;
		case EntityType::TrapChest:
			return this->unregisterEntity(&_trapChestList, e);
			break;
		case EntityType::Resource:
			return this->unregisterEntity(&_resourceList,e);
			break;
		case EntityType::Princess:
			return this->unregisterEntity(&_princessList, e);
			break;
		case EntityType::SpawnPoint:
			return this->unregisterEntity(&_spawnPointList, e);
			break;
		case EntityType::Well:
			return this->unregisterEntity(&_wellList,e);
			break;
		case EntityType::Other:
			return this->unregisterEntity(&_otherList,e);
		}
		return false;
	}

	bool CEntityManager::unregisterEntity(std::set<Logic::CEntity*> * activeList, Logic::CEntity*e)
	{
//		std::cout<<"delete enetity definetly"<<e->getName()<<std::endl;
		std::set<Logic::CEntity*>::iterator it = activeList->find(e);
		if(it != activeList->end())
			activeList->erase(it);

		GUI::CServer::getSingletonPtr()->getStatisticsController()->addEntityCounter(e->getType(),-1);

		return true;
	}

	int CEntityManager::getEntitiesNumber(EntityType listType, std::string entityType){
		int e=0;
		switch(listType)
		{
		case EntityType::Ally:
			e=this->getEntitiesNumber(&_allyUnitList,entityType);
			break;
		case EntityType::Enemy:
			e=this->getEntitiesNumber(&_enemyUnitList,entityType);
			break;
		case EntityType::Chest:
			e=this->getEntitiesNumber(&_chestList,entityType);
			break;
		case EntityType::TrapChest:
			e=this->getEntitiesNumber(&_trapChestList,entityType);
			break;
		case EntityType::Resource:
			e=this->getEntitiesNumber(&_resourceList,entityType);
			break;
		case EntityType::Princess:
			e=this->getEntitiesNumber(&_princessList, entityType);
			break;
		case EntityType::Well:
			e = this->getEntitiesNumber(&_wellList,entityType);
			break;
		default:
			break;
		}
		return e;
	}
	int CEntityManager::getEntitiesNumber(std::set<Logic::CEntity*> * list,std::string entityType)
	{
		if(entityType.compare("")==0)
			return list->size();
		int n=0;
		if(list->size()>0)
		{			
			std::set<Logic::CEntity*>::const_iterator iterator=list->begin();
			for (; iterator != list->end(); ++iterator) {
					if(entityType.compare((*iterator)->getType())==0)
						++n;
			}
		}
		return n;
	}
	Logic::CEntity* CEntityManager::getClosestEntityTypeWithOverlaps(Vector3 overlapsPosition,std::string entityType,float overlapRadius,Vector3 targetPosition,int idAvoid)
	{
		std::list<Logic::CEntity*>* entities = Physics::CServer::getSingletonPtr()->getPhysicPerceptionManager()->overlapQueries(overlapsPosition, overlapRadius, NULL);
		Logic::CEntity * e = nullptr;
		float minDist=MAX_DIST;
		std::list<Logic::CEntity*>::const_iterator it=entities->begin();
		for(;it!=entities->end();++it)
		{
			//if the type entity match with the entity type that we need 
			if((*it)->getEntityID()!=idAvoid && (*it)->getType().compare(entityType)==0)
			{
				//calculate the distance from this enity to the target position(NOT THE POSITION WHERE WE LAUNCH THE OVERLAPS)
				float d=(*it)->getPosition().distance(targetPosition);
				if(d<minDist)
				{
					minDist=d;
					e=(*it);
				}
			}
		}
		delete entities;
		return e;
	}
	Logic::CEntity* CEntityManager::getClosestEntityTypeWithOverlaps(Logic::CEntity*entityOverlaps,float overlapRadius,Vector3 targetPosition)
	{
		return this->getClosestEntityTypeWithOverlaps(entityOverlaps->getPosition(),entityOverlaps->getType(),overlapRadius,targetPosition,entityOverlaps->getEntityID());
	}

	void CEntityManager::reset()
	{
		_chestList.clear();

		_trapChestList.clear();

		_allyUnitList.clear();

		_enemyUnitList.clear();

		_resourceList.clear();

		_otherList.clear();

		_princessList.clear();

		_spawnPointList.clear();

		_wellList.clear();

		_worldLimitsBR = Vector2::ZERO;

		_worldLimitsTL = Vector2::ZERO;
	}

	void CEntityManager::sendMessageToList(EntityType type,const std::shared_ptr<NMessage> &message, Logic::IComponent* emitter)
	{
				switch(type)
		{
		case EntityType::Ally:			
			return this->sendMessageToList(&_allyUnitList, message,emitter);
			break;
		case EntityType::Enemy:
			return this->sendMessageToList(&_enemyUnitList, message,emitter);
			break;
		case EntityType::Chest:
			return this->sendMessageToList(&_chestList, message,emitter);
			break;
		case EntityType::TrapChest:
			return this->sendMessageToList(&_trapChestList,message,emitter);
			break;
		case EntityType::Resource:
			return this->sendMessageToList(&_resourceList, message,emitter);
			break;
		case EntityType::Princess:
			return this->sendMessageToList(&_princessList, message,emitter);
			break;
		case EntityType::SpawnPoint:
			return this->sendMessageToList(&_spawnPointList, message,emitter);
			break;
		case EntityType::Other:
			return this->sendMessageToList(&_otherList,message,emitter);
			break;
		case EntityType::Well:
			return this->sendMessageToList(&_wellList,message,emitter);
			break;
		default:
			break;
		}
	}

	void CEntityManager::sendMessageToList(std::set<Logic::CEntity*> * activeList,const std::shared_ptr<NMessage> &message, Logic::IComponent* emitter)
	{
		std::set<Logic::CEntity*>::const_iterator it=activeList->begin();
		for(;it!=activeList->end();++it)
		{
			(*it)->emitMessageN(message,emitter);
		}
	}


} // namespace AI
