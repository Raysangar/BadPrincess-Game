/**
@file CampComponent.cpp
*/

#include "CampComponent.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Server.h"
#include "Logic/Maps/Map.h"
#include "Logic/AllyUnitMessages.h"
#include "Logic/GameControlMessages.h"
#include "Logic/Entity/Components/Discover.h"

#include "Map/MapEntity.h"

#include "Logic/GameControlMessages.h"

#include "AI/Server.h"
#include "AI/EntityManager.h"
#include "AI/GameManager.h"
#include "AI/Actions/DefendToWaveAction.h"


namespace Logic 
{
	IMP_FACTORY(CCampComponent);

	//---------------------------------------------------------
	
	CCampComponent::CCampComponent(std::string componentName) :IComponent(componentName),_isCampActived(false),_campRange(0.0f),_initiallyCampActived(false),_initiallyWave(0),_currentWaveMaxTime(0),_initiallyWaveMaxTime(0)
	{
		_waveActionList.clear();
	}
	//---------------------------------------------------------

	bool CCampComponent::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;				

		
		assert(entityInfo->hasAttribute("campRange"));
		_campRange = entityInfo->getFloatAttribute("campRange");

		assert(entityInfo->hasAttribute("campActived"));
		_initiallyCampActived = entityInfo->getBoolAttribute("campActived");

		_isCampActived=_initiallyCampActived;

		assert(entityInfo->hasAttribute("campWaveId"));
		_initiallyWave = entityInfo->getIntAttribute("campWaveId");


		assert(entityInfo->hasAttribute("campWaveMaxTime"));
		_initiallyWaveMaxTime = entityInfo->getIntAttribute("campWaveMaxTime");
		
		_currentWave = _initiallyWave;

		_currentWaveMaxTime = _initiallyWaveMaxTime;

		return true;
	} 

	//---------------------------------------------------------

	CCampComponent::~CCampComponent() 
	{
		for(AI::IActionNode* node : _waveActionList)
			delete node;
		_waveActionList.clear();
	} 

	//---------------------------------------------------------

	void CCampComponent::awake()
	{

		_isCampActived=_initiallyCampActived;

		_currentWave = _initiallyWave;

		_currentWaveMaxTime = _initiallyWaveMaxTime;

		_waveActionList.clear();
	
	}

	//---------------------------------------------------------
	
	bool CCampComponent::activate()
	{
	
		return true;
	}

	//---------------------------------------------------------

	void CCampComponent::deactivate()
	{
	
	}

	//---------------------------------------------------------

	bool CCampComponent::acceptN(const std::shared_ptr<NMessage> &message)
	{
		return (message->type.compare("ActivateCamp")==0);
	}

	//---------------------------------------------------------

	void CCampComponent::processN(const std::shared_ptr<NMessage> &message)
	{		
		if(message->type.compare("ActivateCamp")==0)
		{
			std::shared_ptr<ActivateCamp> m = std::static_pointer_cast<ActivateCamp>(message);
			_currentWave = m->waveId;
			_currentWaveMaxTime = m->maxWaveTime;
			_isCampActived = true;

		}
	}


	void CCampComponent::tick(unsigned int msecs) 
	{
		// Invocar al método de la clase padre (IMPORTANTE)
		IComponent::tick(msecs);

		if(_isCampActived)
		{
			std::set<Logic::CEntity*> *allyUnitList = AI::CServer::getSingletonPtr()->getEntityManager()->getAlliesList();
			std::set<Logic::CEntity*>::const_iterator it=allyUnitList->begin();
			bool res = false;
			for(;it!=allyUnitList->end();++it)
			{
				if(checkEntityDistance(*it))
				{
					res = true;
					break;
				}
			}

			if(!res)
			{
				//check the princess distance
				checkEntityDistance(*AI::CServer::getSingletonPtr()->getEntityManager()->getPrincessesList()->begin());
			}
		}
	}
	bool CCampComponent::checkEntityDistance(Logic::CEntity* e)
	{
		//if they are out of maps get the movement speed
		Logic::IComponent *iComponent = e->getComponent("CDiscover");
		assert(iComponent);
		Logic::CDiscover *discover = static_cast<Logic::CDiscover*>(iComponent);
		float dist=e->getPosition().distance(_entity->getPosition());
		if(dist-discover->getDiscoverRange()<_campRange)
		{
			std::cout<<"camp startWave"<<std::endl;
					//send start wave message
			AI::CDefendToWaveAction * action= new AI::CDefendToWaveAction(_currentWave,_currentWaveMaxTime);
			AI::CServer::getSingletonPtr()->getGameManager()->executeAction(action);
			_waveActionList.push_back(action);
			_isCampActived = false;
			return true;
		}
		return false;
	}
	

}








