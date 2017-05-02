#include "ChestTrap.h"

#include "Logic/SmartObjectMessages.h"
#include "Logic/MovementMessages.h"
#include "Logic/EntityMessages.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"
#include "Physics\Server.h"
#include "Physics\PhysicPerceptionManager.h"
#include "Logic/CombatMessages.h"
#include "Logic/RangerMessages.h"
#include "Logic/AudioMessages.h"
#include "Logic/AllyUnitMessages.h"
#include "Logic/Maps/EntityFactory.h"
#include "AI/Server.h"
#include "AI/EntityManager.h"

#include "GUI/Server.h"
#include "GUI/MinimapController.h"

#define RAYCAST_TOLLERANCE 5.0f
namespace Logic 
{
	const Physics::CollisionGroup PHYSIC_RESOURCE_FILTER[]={Physics::CollisionGroup::eResource,Physics::CollisionGroup::eChest};

	IMP_FACTORY(CChestTrap);
	
	//---------------------------------------------------------

	CChestTrap::~CChestTrap() {}
	
	//---------------------------------------------------------

	bool CChestTrap::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		getSmartPositions(entityInfo);

		return true;

	}

	//---------------------------------------------------------

	void CChestTrap::awake(){
		//reset smart positions
		_isEnabled = true;
		_isAvailable = true;
		checkSmartPositionAvailability(nullptr);
		std::list<std::pair<SmartPosition, CEntityPtr>>::iterator it = smartPositions.begin();
		for(;it != smartPositions.end(); ++it)
			it->second=nullptr;
		trapOwner=nullptr;
		activated=false;
		timeToDestroy=500;
	}

	//---------------------------------------------------------

	bool CChestTrap::activate()
	{
		AI::CServer::getSingletonPtr()->getEntityManager()->registerEntity(AI::CEntityManager::EntityType::TrapChest, _entity);

		auto createMessage = std::make_shared<ChestTrapCreated>();
		createMessage->trap = _entity;
		AI::CServer::getSingletonPtr()->getEntityManager()->sendMessageToList(AI::CEntityManager::Enemy,createMessage);
		return true;
	}

	//---------------------------------------------------------

	void CChestTrap::deactivate()
	{
		AI::CServer::getSingletonPtr()->getEntityManager()->unregisterEntity(AI::CEntityManager::EntityType::TrapChest, _entity);
	}

	//---------------------------------------------------------

	void CChestTrap::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);

		if (activated)
		{
			timeToDestroy -= msecs;
			if (timeToDestroy <= 0)
				Logic::CEntityFactory::getSingletonPtr()->deferredDeleteEntity(_entity);
		}
	}

	//---------------------------------------------------------

	bool CChestTrap::acceptN(const std::shared_ptr<NMessage> &message)
	{
		return acceptedMessages.find(message->type) != acceptedMessages.end();
	}

	void CChestTrap::processN(const std::shared_ptr<NMessage> &message)
	{
		(*this.*acceptedMessages[message->type])(message);
	}

	void CChestTrap::handleSmartPositionRequestMessage(const std::shared_ptr<NMessage> &message)
	{
		std::shared_ptr<SmartDataPositionRequest> requestPositionMessage = std::static_pointer_cast<SmartDataPositionRequest>(message);
		if(activated || !isAccesible())
		{
			sendDisabledMessage(requestPositionMessage->entity);
			return;
		}

		std::list<std::pair<SmartPosition, CEntityPtr>>::iterator it = smartPositions.begin();
		std::list<std::pair<SmartPosition, CEntityPtr>*> freePositions;
		//check if smartposition is free
		while (it != smartPositions.end())
		{
			if (it->first._isActive && it->second == nullptr)
				freePositions.push_back(&(*it));
			++it;
		}
		//if we have smart position free
		if(freePositions.size()>0)
		{
			//get the closest one
			std::pair<SmartPosition, CEntityPtr>* p=getNearestSmartPosition(requestPositionMessage->entity->getPosition(), freePositions);
			//send this smartPosition to entity and reserve it to him
			p->second=requestPositionMessage->entity;

			auto positionReceiverMessage = std::make_shared<SmartDataPositionReceiver>();
			positionReceiverMessage->position = p->first._position + _entity->getPosition();
			positionReceiverMessage->smartObject = _entity;
			requestPositionMessage->entity->emitMessageN(positionReceiverMessage,this);
		}else
		{
			auto positionOccupedMessage = std::make_shared<SmartObjectFull>();
			positionOccupedMessage->sender = _entity;
			requestPositionMessage->entity->emitMessageN(positionOccupedMessage,this);
		}
	}

	void CChestTrap::handleSmartPositionReleaseMessage(const std::shared_ptr<NMessage> &message)
	{
		std::shared_ptr<SmartDataFreePosition> releasePositionMessage = std::static_pointer_cast<SmartDataFreePosition>(message);
		std::list<std::pair<SmartPosition, CEntityPtr>>::iterator it = smartPositions.begin();
		//cycle all entitysmartPosition
		for(;it != smartPositions.end(); ++it)
		{
			if(it->second == releasePositionMessage->entity)
				it->second = nullptr;
		}
	}

	void CChestTrap::handleAvatarReadyMessage(const std::shared_ptr<NMessage> &message)
	{
		std::shared_ptr<SmartDataAvatarReady> avatarReadyMessage = std::static_pointer_cast<SmartDataAvatarReady>(message);		
		if(!isAccesible())
			sendDisabledMessage(avatarReadyMessage->entity);
		else
		{
			GUI::CServer::getSingletonPtr()->getMinimapController()->addMinimapEvent(_entity->getName(),GUI::EventType::trap,_entity->getPosition());
			auto damageMessage = std::make_shared<DamageMessage>();
			damageMessage->damage = std::numeric_limits<int>::max();
			avatarReadyMessage->entity->emitMessageN(damageMessage);

			auto trapDestroyedMessage = std::make_shared<TrapDestroyed>();
			trapDestroyedMessage->trapType = _entity->getType();
			trapOwner->emitMessageN(trapDestroyedMessage);
			//this message is used to notificate to thief's InteractSOState that this trap is destroyed
			auto destroyMessage = std::make_shared<EntityDestroying>();
			destroyMessage->entity = _entity;
			_entity->getMap()->sendMessageToAll(destroyMessage);

			_entity->activate("CParticle");
			_entity->deactivate("CGraphics");

			auto playAudioMessage = std::make_shared<PlayAudio>();
			playAudioMessage->eventName = "default";
			_entity->emitMessageN(playAudioMessage);

			activated = true;
		}
	}

	void CChestTrap::handleEnableMessage(const std::shared_ptr<NMessage> &message)
	{
		std::shared_ptr<ChangeSmartObjectState> enableMessage = std::static_pointer_cast<ChangeSmartObjectState>(message);		
		
		_isEnabled = enableMessage->isEnabled;
		if(_isEnabled)
			this->checkSmartPositionAvailability(nullptr);
		else{
			//send a message to all entity that still interact with it
			std::list<std::pair<SmartPosition, CEntityPtr>>::iterator it = smartPositions.begin();
			for(;it!=smartPositions.end();++it)
			{
				if(it->second!=nullptr)
				{
					sendDisabledMessage(it->second);
					it->second=nullptr;
				}
			}
		}
	}
	void CChestTrap::handleEntityDestoyedMessage(const std::shared_ptr<NMessage> &message)
	{
		std::shared_ptr<EntityDestroying> destroyMessage = std::static_pointer_cast<EntityDestroying>(message);		
		if(_isEnabled)
			checkSmartPositionAvailability(destroyMessage->entity);
	}

	void CChestTrap::handleSetOwnerMessage(const std::shared_ptr<NMessage> &message)
	{
		trapOwner = std::static_pointer_cast<SetOwner>(message)->owner;
	}
	
	std::pair<SmartPosition, CEntityPtr>* CChestTrap::getNearestSmartPosition(const Vector3 &entityPosition, const std::list<std::pair<SmartPosition, CEntityPtr>*> &freePositions)
	{
		float minDist=std::numeric_limits<float>::max();
		std::pair<SmartPosition, CEntityPtr>* minItem;
		for (std::list<std::pair<SmartPosition, CEntityPtr>*>::const_iterator it = freePositions.begin(); it != freePositions.end(); ++it)
		{
			float d=entityPosition.distance((*it)->first._position + _entity->getPosition());
			if(d<minDist)
			{
				minDist=d;
				minItem=*it;
			}
		}
		return minItem;
	}

	void CChestTrap::getSmartPositions(const Map::CEntity *entityInfo)
	{				
		for (int i = 1; i <= entityInfo->getIntAttribute("smart_positions_num"); ++i)
		{
			std::string smartPositionName = "smart_position_" + std::to_string(i);
			assert(entityInfo->hasAttribute(smartPositionName));
			SmartPosition sp(entityInfo->getVector3Attribute(smartPositionName),true);
			smartPositions.push_back(std::pair<SmartPosition, CEntityPtr> (sp, nullptr));
		}
	}

	void CChestTrap::checkSmartPositionAvailability(CEntityPtr entityToAvoid)
	{
		std::list<std::pair<SmartPosition, CEntityPtr>>::iterator it = smartPositions.begin();
		Vector3 collisionPoint;
		//cycle all entitysmartPosition
		short spAvailable=0;
		for(;it != smartPositions.end(); ++it)
		{
			Vector3 dir=it->first._position;
			float magnitude= dir.length();
			dir/=magnitude;
			//launch a ray to smart position and check if it collide with some static entity
			Logic::CEntity* e= Physics::CServer::getSingletonPtr()->getPhysicPerceptionManager()->raycastAdvanced(Ray(_entity->getPosition(),dir),magnitude+RAYCAST_TOLLERANCE,collisionPoint,&std::vector<Physics::CollisionGroup> (PHYSIC_RESOURCE_FILTER, PHYSIC_RESOURCE_FILTER + sizeof(PHYSIC_RESOURCE_FILTER) / sizeof(PHYSIC_RESOURCE_FILTER[0])),_entity->getEntityID());
			if(e!=nullptr && e!=entityToAvoid)
				it->first._isActive = false;//in this case set the smartposition to disabled
			else
			{
				it->first._isActive = true;//in this case set the smartposition to enabled
				++spAvailable;
			}
		}
		_isAvailable = (spAvailable>0);
	}

	void CChestTrap::sendDisabledMessage(CEntityPtr recipient)
	{
		auto disabledMessage = std::make_shared<SmartObjectDisabled>();
		disabledMessage->entity = _entity;
		recipient->emitMessageN(disabledMessage);
	}

}

