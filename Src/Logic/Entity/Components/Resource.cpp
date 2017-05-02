/**
@file Resource.cpp

Implementation of Resource component

@see Logic::IComponent

@author Rayco Sánchez García
@date January, 2015
*/

#include "Resource.h"

#include "Logic/WorkmanMessages.h"
#include "Logic/EntityMessages.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Components/WorkmanState.h"
#include "Logic/Entity/Components/SmartObject.h"

#include "Map/MapEntity.h"

#include "Graphics/Scene.h"

#include "AI/Server.h"
#include "AI/EntityManager.h"

#include "Physics\Server.h"
#include "Physics\PhysicPerceptionManager.h"

#define OVERLAPS_X_LENGHT 30.0f
#define OVERLAPS_Y_LENGHT 10.0f
#define OVERLAPS_Z_LENGHT 30.0f
namespace Logic 
{
	const Physics::CollisionGroup PHYSIC_RESOURCE_FILTER[]={Physics::CollisionGroup::eResource,Physics::CollisionGroup::eChest};

	IMP_FACTORY(CResource);
	
	//---------------------------------------------------------

	CResource::~CResource() {}
	
	//---------------------------------------------------------

	bool CResource::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		assert(entityInfo->hasAttribute("max_resource_quantity"));
		maxResourceQuantity = entityInfo->getIntAttribute("max_resource_quantity");
		
		return true;
	}

	void CResource::awake()
	{
		currentResourceQuantity = maxResourceQuantity;
	}
	//---------------------------------------------------------

	void CResource::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);	
	}

	//---------------------------------------------------------

	bool CResource::acceptN(const std::shared_ptr<NMessage> &message)
	{
		if(message->type.compare("EntityDying")==0)
			return true;
		if (currentResourceQuantity >= 0)
			return acceptedMessages.find(message->type) != acceptedMessages.end();
		return false;
	}

	//---------------------------------------------------------

	void CResource::processN(const std::shared_ptr<NMessage> &message)
	{
		if(message->type.compare("EntityDying")==0)
			Logic::CEntityFactory::getSingletonPtr()->deferredDeleteEntity(_entity);
		else
		(*this.*acceptedMessages[message->type])(message);
	}

	//---------------------------------------------------------

	void CResource::handleGatherMessage(const std::shared_ptr<NMessage> &message)
	{
		std::shared_ptr<GatherMessage> gatherMessage = std::static_pointer_cast<GatherMessage>(message);
		currentResourceQuantity -= gatherMessage->quantity;
#ifdef _DEBUG
		//std::cout<<"handleGatherMessage q="<<currentResourceQuantity<<std::endl;
#endif
		if (currentResourceQuantity <= 0)
			this->resourceEnded();
	}

	//---------------------------------------------------------
	
	void CResource::resourceEnded()
	{		
		Logic::CSmartObject *component=static_cast<CSmartObject*>(_entity->getComponent("CSmartObject"));
		if(component)
		{
			
			Logic::CEntity * nextItem= _entity->getMap()->getEntityByName(nextItemName);
			auto dieMessage = std::make_shared<EntityDying>();
			dieMessage->entity = _entity;
			_entity->emitMessageN(dieMessage);

			auto destroyMessage = std::make_shared<EntityDestroying>();
			destroyMessage->entity = _entity;
			//send that i'm destroing to all of ally goblin
			std::set<Logic::CEntity*> *allylist= AI::CServer::getSingletonPtr()->getEntityManager()->getAlliesList();
			for(Logic::CEntity *it:*allylist)
			{
				(*it).emitMessageN(destroyMessage);
			}
			
			//check if there is some resource near this resource to send this message			
			//launch a overlaps and check if it collide with some static entity
			std::list<Logic::CEntity*> *list= Physics::CServer::getSingletonPtr()->getPhysicPerceptionManager()->overlapQueries(_entity->getPosition(),OVERLAPS_X_LENGHT,OVERLAPS_Z_LENGHT,OVERLAPS_Y_LENGHT,&std::vector<Physics::CollisionGroup> (PHYSIC_RESOURCE_FILTER, PHYSIC_RESOURCE_FILTER + sizeof(PHYSIC_RESOURCE_FILTER) / sizeof(PHYSIC_RESOURCE_FILTER[0])));
			for(Logic::CEntity*e : *list)
			{
				if(e!=_entity)
					e->emitMessageN(destroyMessage);
			}
			delete list;

		}
	}

	//---------------------------------------------------------

	void CResource::sendMessageToAll(const std::list<std::pair<Vector3, CEntityPtr>> * smartDataList,const std::shared_ptr<NMessage> &message){
			std::list<std::pair<Vector3, CEntityPtr>>::const_iterator it = smartDataList->begin();
			for (; it != smartDataList->end(); ++it)
			{
				if(it->second)
				{
					it->second->emitMessageN(message,this);
#ifdef _DEBUG
					std::cout<<"functionOverDataList send message move to entity"<<std::endl;
#endif // DEBUG
				}
			}
	}

} // namespace Logic