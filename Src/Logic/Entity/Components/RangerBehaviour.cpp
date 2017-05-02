/**
@file RangerBehaviour.cpp

Implementation of RangerBehaviour component

@see Logic::IComponent

@author Rayco Sánchez García
@date February, 2015
*/

#include "RangerBehaviour.h"
#include "ArrowBehavior.h"

#include "Logic/MovementMessages.h"
#include "Logic/RangerMessages.h"
#include "Logic/AllyUnitMessages.h"
#include "Logic/RouteCalculationMessages.h"
#include "Logic/UnitMessages.h"

#include "CombatStats.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"
#include "Logic/Server.h"
#include "Map/MapEntity.h"
#include "GUI\Server.h"
#include "GUI\PlayerController.h"
#include "StateMachineExecutor.h"

#include "Physics/Server.h"
#include "Physics/PhysicPerceptionManager.h"
#include "Graphics/Server.h"

#define MAX_RAY_DISTANCE 1000

namespace Logic 
{
	IMP_FACTORY(CRangerBehaviour);

	const Physics::CollisionGroup PHYSIC_WORLD_FILTER []={Physics::CollisionGroup::eWorldGround};
		
	//---------------------------------------------------------

	CRangerBehaviour::~CRangerBehaviour() 
	{
		if(_nextTrapInfo != NULL)
			delete _nextTrapInfo;
	}
	
	//---------------------------------------------------------

	bool CRangerBehaviour::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		for(int i = 0; i < entityInfo->getIntAttribute("trap_name_count"); ++i)
		{
			std::string trapName = entityInfo->getStringAttribute("trap_name_"+std::to_string(i));
			availableTraps[trapName] = std::pair<CEntityPtr, int>(nullptr, entityInfo->getIntAttribute("max_number_traps_"+std::to_string(i)));		
			trapsAssignedToKeys[entityInfo->getStringAttribute("trap_key_"+std::to_string(i))] = trapName;		
		}	

		return true;

	}

	//---------------------------------------------------------

	void CRangerBehaviour::awake()
	{
		nextTrapName=""; 
		if(_nextTrapInfo!=nullptr)
			delete _nextTrapInfo;
		_nextTrapInfo = new NextTrapInfo();
		_nextTrapInfo->position = Vector3::ZERO;
		_nextTrapInfo->nextTrapGenerator = nullptr;
		_nextTrapInfo->trapTimeToSet = 1000;
		_trapNum=0;
		settingTrap=false;

		Logic::CEntity *e = Logic::CServer::getSingletonPtr()->getMap()->getEntityByName("Arrow");

		_arrows.push_back(Logic::CEntityFactory::getSingletonPtr()->instantiate(e, "",_entity->getPosition()));		
		_arrows.push_back(Logic::CEntityFactory::getSingletonPtr()->instantiate(e, "",_entity->getPosition()));
		((CArrowBehavior*)_arrows.at(0)->getComponent("CArrowBehavior"))->setOwner(_entity);
		((CArrowBehavior*)_arrows.at(1)->getComponent("CArrowBehavior"))->setOwner(_entity);

		_combatComponent = (CCombatStats*)_entity->getComponent("CCombatStats");

		for(std::map<std::string, std::pair<CEntityPtr, int>>::iterator it = availableTraps.begin(); it != availableTraps.end(); ++it)
		{
			it->second.first = CServer::getSingletonPtr()->getMap()->getEntityByName(it->first);
			helperTraps[it->first] = CServer::getSingletonPtr()->getMap()->getEntityByName(it->first + "Helper");
		}

		currentPitch = Ogre::Vector3::UNIT_Y.getRotationTo(Ogre::Vector3::UNIT_Y);
	}

	//---------------------------------------------------------

	void CRangerBehaviour::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);	
	}

	//---------------------------------------------------------

	bool CRangerBehaviour::acceptN(const std::shared_ptr<NMessage> &message)
	{
		return messageHandlers.find(message->type) != messageHandlers.end();
	}

	//---------------------------------------------------------

	void CRangerBehaviour::processN(const std::shared_ptr<NMessage> &message)
	{
		(*this.*messageHandlers[message->type])(message);
	}

	//---------------------------------------------------------

	void CRangerBehaviour::castAbilityHandler(const std::shared_ptr<NMessage> &message)
	{
		nextTrapName = trapsAssignedToKeys[std::static_pointer_cast<CastAbility>(message)->ability];
		assert(nextTrapName != "");

		if (availableTraps[nextTrapName].second > 0 && !settingTrap)
			GUI::CServer::getSingletonPtr()->getPlayerController()->attachEntityToMouse(instantiateTrap());
	}

	//---------------------------------------------------------

	void CRangerBehaviour::setTrapCancelledHandler(const std::shared_ptr<NMessage> &message)
	{
		settingTrap = false;
	}

	//---------------------------------------------------------

	CEntityPtr CRangerBehaviour::instantiateTrap()
	{		
		CEntityPtr trap = Logic::CEntityFactory::getSingletonPtr()->instantiate(helperTraps[nextTrapName], "SetTrapHelper",
			GUI::CServer::getSingletonPtr()->getPlayerController()->getActualMouseWorldPosition());
		return trap;
	}

	//---------------------------------------------------------

	std::string CRangerBehaviour::getNewTrapName()
	{		
		return _entity->getName() + nextTrapName + std::to_string(_trapNum++);
	}

	//---------------------------------------------------------

	void CRangerBehaviour::setTrapHandler(const std::shared_ptr<NMessage> &message)
	{
		settingTrap = true;

		auto trapMessage = std::static_pointer_cast<SetTrap>(message);
		_nextTrapInfo->position = trapMessage->position;
		_nextTrapInfo->nextTrapGenerator = availableTraps[nextTrapName].first;
		_nextTrapInfo->newTrapName = getNewTrapName();
		
		auto moveMessage = std::make_shared<AvatarRouteTo>();
		moveMessage->calculateNewRoute = true;
		moveMessage->target = trapMessage->position;

		IComponent* iComponentExecutor = _entity->getComponent("CStateMachineExecutor");
		CStateMachineExecutor* executor = static_cast<CStateMachineExecutor*>(iComponentExecutor);

		/*auto setTrapMessage = std::make_shared<SetTrap>();
		_entity->emitMessageN(setTrapMessage);
		*/
		_entity->emitMessageN(moveMessage, executor);
	}
	
	//---------------------------------------------------------

	void CRangerBehaviour::instantiateAbilityHandler(const std::shared_ptr<NMessage> &message)
	{
		auto instantiateMessage = std::static_pointer_cast<InstantiateAbilityObject>(message);
		
		auto setTrapMessage = std::make_shared<SetTrap>();
		setTrapMessage->position=instantiateMessage->position;
		_entity->emitMessageN(setTrapMessage);
		
	}
	//---------------------------------------------------------

	void CRangerBehaviour::trapDestroyedHandler(const std::shared_ptr<NMessage> &message)
	{
		auto trapDestroyedMessage = std::static_pointer_cast<TrapDestroyed>(message);
		++availableTraps[trapDestroyedMessage->trapType].second;
	}

	//---------------------------------------------------------

	void CRangerBehaviour::trapSet(CEntityPtr trap)
	{
		

		Ogre::Ray ray = Ogre::Ray(trap->getPosition() + Ogre::Vector3(0,100,0),Ogre::Vector3::NEGATIVE_UNIT_Y);
		
		Vector3 point = Vector3::ZERO;
		Vector3 normal = Vector3::ZERO;
		Logic::CEntity* e =	Physics::CServer::getSingletonPtr()->getPhysicPerceptionManager()->raycastAdvanced(ray,MAX_RAY_DISTANCE,point,normal,
			&std::vector<Physics::CollisionGroup> (PHYSIC_WORLD_FILTER, PHYSIC_WORLD_FILTER + sizeof(PHYSIC_WORLD_FILTER) / sizeof(PHYSIC_WORLD_FILTER[0])));


		Ogre::Quaternion q = Ogre::Vector3::UNIT_Y.getRotationTo(normal);
		if(currentPitch != q)
		{
			currentPitch = q;
			Ogre::Quaternion r = Ogre::Quaternion((Ogre::Radian)_entity->getYaw(), Ogre::Vector3::UNIT_Y);
			q = q * r;
			Ogre::Matrix3 m;
			q.ToRotationMatrix(m);		

			trap->setOrientationN(m);		
		}
		


		auto setOwnerMessage = std::make_shared<SetOwner>();
		setOwnerMessage->owner = _entity;
		trap->emitMessageN(setOwnerMessage);

		_nextTrapInfo->nextTrapGenerator = nullptr;
		_nextTrapInfo->position = Vector3::ZERO;
		settingTrap = false;
	}

	//---------------------------------------------------------

	void CRangerBehaviour::shootArrow(const std::shared_ptr<NMessage> &message)
	{
		auto attackingMessage = std::static_pointer_cast<Attacking>(message);

		if(attackingMessage->attacking)
		{
			auto m = std::make_shared<ShootArrow>();
			m->target = _combatComponent->getCurrentEnemy();
			_arrows.at(_arrowToShoot)->emitMessageN(m);
			_arrowToShoot = ++_arrowToShoot % 2;
		}
	}

} // namespace Logic
