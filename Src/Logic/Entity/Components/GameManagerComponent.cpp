/**
@file GameManagerComponent.cpp

Contiene la implementación del componente que controla el movimiento 
de la entidad.
 
@see Logic::CGameManagerComponent
@see Logic::IComponent

@author Guidi Giacomo
@date March, 2015
*/

#include "GameManagerComponent.h"
#include "Map/MapEntity.h"
#include "Logic/Server.h"
#include "Logic/Maps/Map.h"
#include "PhysicEntity.h"
#include "Logic/AudioMessages.h"
#include "Logic/Entity/Entity.h"

#include "GUI/Server.h"
#include "GUI/PlayerController.h"
#include "GUI/ActiveUnitController.h"

#include "AI/Server.h"
#include "AI/WaveManager.h"
#include "AI/GameManager.h"
#include "AI/EntityManager.h"




namespace Logic 
{
	IMP_FACTORY(CGameManagerComponent);

	//--------------------------------------------------------

	CGameManagerComponent::CGameManagerComponent(std::string componentName) : IComponent(componentName),_topLimit(0.0f),_leftLimit(0.0f),_rightLimit(0.0f),_bottomLimit(0.0f)
	{			
	}
	
	//---------------------------------------------------------

	bool CGameManagerComponent::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;
		assert(entityInfo->hasAttribute("currentGameType"));
		std::string gameType = entityInfo->getStringAttribute("currentGameType");
		AI::CServer::getSingletonPtr()->getGameManager()->setupCurrentGame(gameType);

		if(gameType=="campaign")
		{
			assert(entityInfo->hasAttribute("currentGameLevel"));
			int gameLevel = entityInfo->getIntAttribute("currentGameLevel");
			AI::CServer::getSingletonPtr()->getGameManager()->setCurrentCampaignLevel(gameLevel);
		}
		
		assert(entityInfo->hasAttribute("loadWaveLevel"));
		std::string file = entityInfo->getStringAttribute("loadWaveLevel");
		AI::CServer::getSingletonPtr()->getWaveManager()->loadWaveLevel(file);

		assert(entityInfo->hasAttribute("loadGameLevel"));
		file = entityInfo->getStringAttribute("loadGameLevel");
		AI::CServer::getSingletonPtr()->getGameManager()->loadGameLevel(file);

		assert(entityInfo->hasAttribute("loadAnimationInventary"));
		file = entityInfo->getStringAttribute("loadAnimationInventary");
		AI::CServer::getSingletonPtr()->getGameManager()->loadScript(file);

		assert(entityInfo->hasAttribute("loadAnimationTrack"));
		file = entityInfo->getStringAttribute("loadAnimationTrack");
		AI::CServer::getSingletonPtr()->getGameManager()->loadScript(file);

		if(entityInfo->hasAttribute("worldTopLimit"))
			_topLimit = entityInfo->getFloatAttribute("worldTopLimit");

		if(entityInfo->hasAttribute("worldBottomLimit"))
			_bottomLimit = entityInfo->getFloatAttribute("worldBottomLimit");

		if(entityInfo->hasAttribute("worldRightLimit"))
			_rightLimit = entityInfo->getFloatAttribute("worldRightLimit");

		if(entityInfo->hasAttribute("worldLeftLimit"))
			_leftLimit = entityInfo->getFloatAttribute("worldLeftLimit");

		return true;
	} // spawn
	
	//---------------------------------------------------------

	void CGameManagerComponent::awake(){
		AI::CServer::getSingletonPtr()->getGameManager()->onAwake();
		AI::CServer::getSingletonPtr()->computeNavigationGraph();
		auto playBackground = std::make_shared<PlayAudio>();
		playBackground->eventName = "default";
		_entity->emitMessageN(playBackground);

		CPhysicEntity* physicComponent = (CPhysicEntity*) Logic::CServer::getSingletonPtr()->getMap()->getEntityByName("Ground")->getComponent("CPhysicEntity");
		Vector3 cameraLimits = physicComponent->getActorDimensions()/2;

		float topLimit = physicComponent->getActorDimensions().z * _topLimit;
		float bottomLimit = physicComponent->getActorDimensions().z * _bottomLimit;
		float rightLimit = physicComponent->getActorDimensions().x * _rightLimit;
		float leftLimit = physicComponent->getActorDimensions().x * _leftLimit;

		AI::CServer::getSingletonPtr()->getEntityManager()->setupWorldLimits(-cameraLimits.x+leftLimit,-cameraLimits.z+topLimit,cameraLimits.x-rightLimit,cameraLimits.z-bottomLimit);

	}

	//---------------------------------------------------------

	bool CGameManagerComponent::acceptN(const std::shared_ptr<NMessage> &message)
	{
		return AI::CServer::getSingletonPtr()->getWaveManager()->acceptN(message) || AI::CServer::getSingletonPtr()->getGameManager()->acceptN(message);
	} // accept
	//---------------------------------------------------------

	void CGameManagerComponent::processN(const std::shared_ptr<NMessage> &message)
	{
		//Accept here all message that could interest to wave manager		
		if(AI::CServer::getSingletonPtr()->getWaveManager()->acceptN(message))
		{
			message->sender = _entity;
			AI::CServer::getSingletonPtr()->getWaveManager()->processN(message);
		}
		else
			AI::CServer::getSingletonPtr()->getGameManager()->processN(message);
	} // process

	void CGameManagerComponent::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);
	}

} // namespace Logic

