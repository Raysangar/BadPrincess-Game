/**
@file Graphics.h

Contiene los smart objects que controlan la recolección de recursos y la interacción con los obreros

@see Logic::IComponent

@author Rayco Sánchez García
@date Enero, 2015
*/

#include "SmartObject.h"

#include "Logic/SmartObjectMessages.h"
#include "Logic/MovementMessages.h"
#include "Logic/EntityMessages.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"
#include "Physics\Server.h"
#include "Physics\PhysicPerceptionManager.h"

//DEBUG
#include "Graphics/MeshEntity.h"
//#include "Graphics\Scene.h"
//#include "Graphics\Server.h"
#include "Logic\Entity\Components\Graphics.h"

#define RAYCAST_TOLLERANCE 5.0f
namespace Logic 
{
	const Physics::CollisionGroup PHYSIC_RESOURCE_FILTER[]={Physics::CollisionGroup::eResource,Physics::CollisionGroup::eChest};

	IMP_FACTORY(CSmartObject);
	
	//---------------------------------------------------------

	CSmartObject::~CSmartObject() 
	{
	} // ~CGraphics
	
	//---------------------------------------------------------

	bool CSmartObject::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		getSmartPositions(entityInfo);

		assert(entityInfo->hasAttribute("animation_name"));
		collectAnimationName = entityInfo->getStringAttribute("animation_name");

		assert(entityInfo->hasAttribute("action_type"));
		actionType = entityInfo->getIntAttribute("action_type");

		return true;

	} // spawn	

	
	//---------------------------------------------------------

	void CSmartObject::awake(){
		//reset smart positions
		_isEnabled = true;
		_isAvailable = true;
		checkSmartPositionAvailability(nullptr);
		std::list<std::pair<SmartPosition, CEntityPtr>>::iterator it = smartPositions.begin();
		for(;it != smartPositions.end(); ++it)
		{
			it->second=nullptr;
		}
	}

	//---------------------------------------------------------

	void CSmartObject::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);
		
	}

	//---------------------------------------------------------
	//DEBUG
	bool CSmartObject::activate()
	{
		if(!isAccesible())
			drawSmartObjectAvailability(false);
		return true;
	}


	bool CSmartObject::acceptN(const std::shared_ptr<NMessage> &message)
	{
		return acceptedMessages.find(message->type) != acceptedMessages.end();
	} // acceptN

	void CSmartObject::processN(const std::shared_ptr<NMessage> &message)
	{
		(*this.*acceptedMessages[message->type])(message);
	} // processN

	void CSmartObject::handleSmartPositionRequestMessage(const std::shared_ptr<NMessage> &message)
	{
		std::shared_ptr<SmartDataPositionRequest> requestPositionMessage = std::static_pointer_cast<SmartDataPositionRequest>(message);
		if(!isAccesible())
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
//			std::cout<<_entity->getName()<<" reserve smartPos to "<<requestPositionMessage->entity->getName()<<std::endl;
			auto positionReceiverMessage = std::make_shared<SmartDataPositionReceiver>();
			positionReceiverMessage->position = p->first._position + _entity->getPosition();
			positionReceiverMessage->smartObject = _entity;
			requestPositionMessage->entity->emitMessageN(positionReceiverMessage,this);
		}else
		{
//			std::cout<<_entity->getName()<<" send smart pos full to "<<requestPositionMessage->entity->getName()<<std::endl;
			auto positionOccupedMessage = std::make_shared<SmartObjectFull>();
			positionOccupedMessage->sender = _entity;
			requestPositionMessage->entity->emitMessageN(positionOccupedMessage,this);
		}
	}

	void CSmartObject::handleSmartPositionReleaseMessage(const std::shared_ptr<NMessage> &message)
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

	void CSmartObject::handleAvatarReadyMessage(const std::shared_ptr<NMessage> &message)
	{
		std::shared_ptr<SmartDataAvatarReady> avatarReadyMessage = std::static_pointer_cast<SmartDataAvatarReady>(message);		
		if(!isAccesible())
		{
			sendDisabledMessage(avatarReadyMessage->entity);
			return;
		}
/*		std::list<std::pair<Vector3, CEntityPtr>>::iterator it = smartPositions.begin();
		//i'm ready into a smart position, check what smart position is
		for(;it != smartPositions.end(); ++it)
		{
			Vector3 spWorld=_entity->getPosition()+it->first;
			if(spWorld.positionEquals(avatarReadyMessage->position))
				break;
		}*/
		//check if the smart position it isn't already occuped
//		if(it!=smartPositions.end() &&  it->second==nullptr)
//		{
			auto smartActionMessage = std::make_shared<SmartDataAction>();
			smartActionMessage->animation = collectAnimationName;
			smartActionMessage->actionType = actionType;
			avatarReadyMessage->entity->emitMessageN(smartActionMessage, this);
/*		}else//tell to entity to request newly a position (it never should happen)
		{
			auto entityMoveTo = std::make_shared<AvatarMoveToEntity>();
			entityMoveTo->target = _entity;
			avatarReadyMessage->entity->emitMessageN(entityMoveTo, this);
		}*/
	}

	void CSmartObject::handleEnableMessage(const std::shared_ptr<NMessage> &message)
	{
		std::shared_ptr<ChangeSmartObjectState> enableMessage = std::static_pointer_cast<ChangeSmartObjectState>(message);		
		//DEBUG
		if(!_isEnabled && _isAvailable && enableMessage->isEnabled )
			drawSmartObjectAvailability(true);
		else if(_isEnabled && _isAvailable && !enableMessage->isEnabled )
			drawSmartObjectAvailability(false);
			//END DEBUG
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
	void CSmartObject::handleEntityDestoyedMessage(const std::shared_ptr<NMessage> &message)
	{
		std::shared_ptr<EntityDestroying> destroyMessage = std::static_pointer_cast<EntityDestroying>(message);		
		if(_isEnabled)
			checkSmartPositionAvailability(destroyMessage->entity);
	}

	
	std::pair<SmartPosition, CEntityPtr>* CSmartObject::getNearestSmartPosition(const Vector3 &entityPosition, const std::list<std::pair<SmartPosition, CEntityPtr>*> &freePositions)
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

	void CSmartObject::getSmartPositions(const Map::CEntity *entityInfo)
	{		
		std::stringstream smartPositionAttributeName;		
		for (int i = 1; i <= entityInfo->getIntAttribute("smart_positions_num"); ++i)
		{
			smartPositionAttributeName.str("");
			smartPositionAttributeName << "smart_position_" << (i);
			assert(entityInfo->hasAttribute(smartPositionAttributeName.str()));
			SmartPosition sp(entityInfo->getVector3Attribute(smartPositionAttributeName.str()),true);
			smartPositions.push_back(std::pair<SmartPosition, CEntityPtr> (sp, nullptr));

			//DEBUG
			//Graphics::CMeshEntity *e=new Graphics::CMeshEntity(_entity->getName()+"_sp_"+std::to_string(i),"esfera.mesh");
			//Graphics::CServer::getSingletonPtr()->getScene("map.txt")->addEntity(e);
			//Vector3 pos(entityInfo->getVector3Attribute(smartPositionAttributeName.str()));
			//pos+=entityInfo->getVector3Attribute("position");
			//e->setPosition(pos);
			//e->setScale(5);
			//spDebug[sp._position]=e;
		}
	} // getSmartPositions

	void CSmartObject::checkSmartPositionAvailability(CEntityPtr entityToAvoid)
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
			{	
				it->first._isActive = false;//in this case set the smartposition to disabled
				//DEBUG
//				spDebug[it->first._position]->changeEntityMaterial("esferaBlue");
//				std::cout<<_entity->getName()<<" disable sp "<<it->first._position<<std::endl;
			}else
			{
				it->first._isActive = true;//in this case set the smartposition to disabled
				++spAvailable;
				//DEBUG
//				spDebug[it->first._position]->changeEntityMaterial("01-Default");
			}
		}
		if(spAvailable>0)
		{
			//DEBUG if before is disabled remove disabled texture
			if(!_isAvailable)
				drawSmartObjectAvailability(true);
			//END DEBUG
			_isAvailable=true;

		}else
		{
			//DEBUG if before is enabled add disabled texture
			if(_isAvailable)
				drawSmartObjectAvailability(false);
			//END DEBUG
			_isAvailable=false;
		}
	}

	void CSmartObject::drawSmartObjectAvailability(bool enabled)
	{
		if(enabled)
		{
			CGraphics* component = (CGraphics*) _entity->getComponent("CGraphics");
			component->getGraphicsEntity()->removePass(-1);
		}else
		{
			CGraphics* component = (CGraphics*) _entity->getComponent("CGraphics");
			component->getGraphicsEntity()->addTexture("rojo.jpg");
		}
	}

	void CSmartObject::sendDisabledMessage(CEntityPtr recipient)
	{
//		std::cout<<"send disable message to"<<recipient->getName()<<std::endl;
		auto disabledMessage = std::make_shared<SmartObjectDisabled>();
		disabledMessage->entity = _entity;
		recipient->emitMessageN(disabledMessage);
	}

	Vector3 CSmartObject::getNearestSmartPositionAvailable(Vector3 entityPosition)
	{
		float minDist=std::numeric_limits<float>::max();
		std::pair<SmartPosition, CEntityPtr> *minItem = nullptr;
		for (std::list<std::pair<SmartPosition, CEntityPtr>>::iterator it = smartPositions.begin(); it != smartPositions.end(); ++it)
		{
			if(it->first._isActive)
			{
				float d=entityPosition.distance((*it).first._position +_entity->getPosition());
				if(d<minDist)
				{
					minDist=d;
					minItem=&(*it);
				}
			}
		}
		if(minItem!=nullptr)
			return minItem->first._position +_entity->getPosition();
		else
			return Vector3::ZERO;
	}

} // namespace Logic

