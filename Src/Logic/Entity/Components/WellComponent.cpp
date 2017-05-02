/**
@file WellComponent.cpp
*/

#include "WellComponent.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Logic\Maps\EntityFactory.h"
#include "Logic/Entity/Components/SpawnPointComponent.h"
#include "Logic/Entity/Components/EntityTypeExchanger.h"
#include "Logic/Entity/Components/BuildComponent.h"

#include "Logic/AllyUnitMessages.h"
#include "Logic/Server.h"

#include "AI/ResourceServer.h"
#include "AI/Server.h"
#include "AI/GameManager.h"
#include "AI/EntityManager.h"

#include "GUI/Server.h"
#include "GUI/StatisticsController.h"


#define MAX_UNITS 16



namespace Logic 
{
	IMP_FACTORY(CWellComponent);

	//---------------------------------------------------------

	CWellComponent::~CWellComponent() 
	{

	} 

	//---------------------------------------------------------

	bool CWellComponent::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		Logic::IComponent *ic =_entity->getComponent("CBuildComponent");
		assert(ic);
		_buildComponent = static_cast<Logic::CBuildComponent*>(ic);
		

		return true;

	} 
	bool CWellComponent::acceptN(const std::shared_ptr<NMessage> &message)
	{
		if(_buildComponent->isBuilded())
			return message->type.compare("CastAbility")==0 || message->type.compare("EntityDying")==0;
		else
			return false;
	}

	void CWellComponent::processN(const std::shared_ptr<NMessage> &message)
	{
		if (message->type.compare("CastAbility") == 0)
		{

			std::shared_ptr<CastAbility> m = std::static_pointer_cast<CastAbility>(message);

			Logic::IComponent *ic =_entity->getComponent("CEntityTypeExchanger");
			assert(ic);
			CEntityTypeExchanger *exchanger = static_cast<Logic::CEntityTypeExchanger*>(ic);

			char* unitName = "";
			if(m->ability == "Q")
			{
				unitName = exchanger->isEntityTypeExchangerActivate() ? "WorkmanGoblinExchange" : "WorkmanGoblin";	
			}
			if(m->ability == "W")
			{
				unitName = exchanger->isEntityTypeExchangerActivate() ? "WarriorGoblinExchange" : "WarriorGoblin";	
			}
			if(m->ability == "E")
			{
				unitName = exchanger->isEntityTypeExchangerActivate() ? "RangerGoblinExchange" : "RangerGoblin";	
			}
						
			if(unitName != "" && AI::ResourceServer::getInstance()->checkResourceAvailability(unitName,1.0f) && AI::CServer::getSingletonPtr()->getEntityManager()->getEntitiesNumber(AI::CEntityManager::Ally,"") < MAX_UNITS)
			{
				AI::ResourceServer::getInstance()->useResource(unitName,1.0f);
				Logic::CEntity *e = Logic::CServer::getSingletonPtr()->getMap()->getEntityByName(unitName);
				assert(e);
				Logic::IComponent *ic =_entity->getComponent("CSpawnPointComponent");
				assert(ic);
				CSpawnPointComponent *sp = static_cast<Logic::CSpawnPointComponent*>(ic);
				GUI::CServer::getSingletonPtr()->getStatisticsController()->setGoblinCounter(AI::CServer::getSingletonPtr()->getEntityManager()->getEntitiesNumber(AI::CEntityManager::Ally,"") + 1);
				Logic::CEntity* spawnedEntity = sp->createEntity(e);

				if(exchanger->isEntityTypeExchangerActivate())
				{
					exchanger->moveSelectionToChangedEntity(spawnedEntity);
					exchanger->endExchangerBehaviour();
				}
			}
		}else if (message->type.compare("EntityDying") == 0)
		{
			if(!_buildComponent->isIndestructible())
				Logic::CEntityFactory::getSingletonPtr()->deferredDeleteEntity(_entity);
		}
	}
	void CWellComponent::awake()
	{

	}



}

