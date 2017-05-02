/**

*/
#include <iostream>
#include <cassert>
#include "EventsController.h"
#include "MessagesController.h"
#include "GUI\Server.h"
#include "BaseSubsystems/Server.h"
#include "Graphics\Entity.h"
#include "EventsTexts.h"
#include "Logic\Server.h"
#include "Logic/Maps/Map.h"
#include "Logic/Entity/Entity.h"
#include "Logic\Entity\Components\Camera.h"
#include "GUI/Server.h"
#include "GUI/MinimapController.h"

#define MESSAGE_POSITION_Y 0.60f
#define MESSAGE_POSITION_X 0.01f
#define WAIT_STANDARD_TIME 6000
#define WAIT_SHORT_TIME 4000

namespace GUI {
	CEventsController::CEventsController(): _cameraComponent(nullptr),  _underAttackTime(0), 
		_stealingResourcesTime(0), _princessKidnappedTime(0), _princessDamagedTime(0), _missingWoodTime(0), _missingStoneTime(0), _missingIronTime(0)
	{
		
	}
	// CEventsController

	//--------------------------------------------------------
	
	CEventsController::~CEventsController()
	{
		deactivate();
	} // ~CEventsController

	//--------------------------------------------------------

	void CEventsController::activate()
	{		
		_cameraComponent = static_cast<Logic::CCamera*>(Logic::CServer::getSingletonPtr()->getMap()->getEntityByName("Camera")->getComponent("CCamera"));
	} // activate

	//--------------------------------------------------------
	void CEventsController::deactivate()
	{
		
	} // deactivate

	//--------------------------------------------------------
	void CEventsController::princessDamaged(Logic::CEntity* entity)
	{
		if(_princessDamagedTime >= WAIT_STANDARD_TIME && !_cameraComponent->isEntityOnCamera(entity->getPosition()))
		{
			// TODO: Seleccionar el texto a mostrar. (Cabecera externa).
			GUI::CServer::getSingletonPtr()->getMessagescontroller()->showTemporalErrorMessage("event_princess_damaged", EN_PrincessDamaged, MESSAGE_POSITION_X, calculatePositionY(), "",3000);
			// TODO: Reproducir sonido.
			GUI::CServer::getSingletonPtr()->getMinimapController()->addMinimapEvent(entity->getName(), GUI::EventType::princess, entity->getPosition());
			_princessDamagedTime = 0;
		}
	} // princessDamaged

	//--------------------------------------------------------
	void CEventsController::princessKidnapped(Logic::CEntity* entity)
	{
		if(_princessKidnappedTime >= WAIT_STANDARD_TIME && !_cameraComponent->isEntityOnCamera(entity->getPosition()))
		{
			// TODO: Seleccionar el texto a mostrar. (Cabecera externa).
			GUI::CServer::getSingletonPtr()->getMessagescontroller()->showTemporalErrorMessage("event_princess_kidnapped", EN_PrincessKidnapped, MESSAGE_POSITION_X, calculatePositionY(), "",3000);
			// TODO: Reproducir sonido.
			GUI::CServer::getSingletonPtr()->getMinimapController()->addMinimapEvent(entity->getName(), GUI::EventType::princess, entity->getPosition());
			_princessKidnappedTime = 0;
		}
	} // princessKidnapped

	//--------------------------------------------------------
	void CEventsController::stealingResources(Logic::CEntity* entity)
	{
		if( _stealingResourcesTime >= WAIT_STANDARD_TIME && !_cameraComponent->isEntityOnCamera(entity->getPosition()))
		{
			// TODO: Seleccionar el texto a mostrar según el tipo de recurso. (Cabecera externa).
			GUI::CServer::getSingletonPtr()->getMessagescontroller()->showTemporalErrorMessage("event_stealing_resources", EN_StolenResoruces, MESSAGE_POSITION_X, calculatePositionY(), "", 3000);
			// TODO: Reproducir sonido.
			GUI::CServer::getSingletonPtr()->getMinimapController()->addMinimapEvent(entity->getName(), GUI::EventType::chest, entity->getPosition());
			_stealingResourcesTime = 0;
		}
	} // stealingResources

	//--------------------------------------------------------
	void CEventsController::missingResources(ResourceType type)
	{
		
			switch (type)
			{
			case GUI::ResourceType::WOOD:
				if(_missingWoodTime >= WAIT_SHORT_TIME)
				{
					GUI::CServer::getSingletonPtr()->getMessagescontroller()->showTemporalErrorMessage("event_missing_wood", EN_MissingResourcesWood, MESSAGE_POSITION_X, calculatePositionY(), "", 3000);
					_missingWoodTime = 0;
				}
				break;
			case GUI::ResourceType::STONE:
				if(_missingStoneTime >= WAIT_SHORT_TIME)
				{
					GUI::CServer::getSingletonPtr()->getMessagescontroller()->showTemporalErrorMessage("event_missing_stone", EN_MissingResourcesStone, MESSAGE_POSITION_X, calculatePositionY(), "", 3000);
					_missingStoneTime = 0;
				}
				break;
			case GUI::ResourceType::IRON:
				if(_missingIronTime >= WAIT_SHORT_TIME)
				{
					GUI::CServer::getSingletonPtr()->getMessagescontroller()->showTemporalErrorMessage("event_missing_iron", EN_MissingResourcesIron, MESSAGE_POSITION_X, calculatePositionY(), "", 3000);
					_missingIronTime = 0;
				}
				break;
			default:
				break;
					
				// TODO: Reproducir sonido.
		}
	} // missingResources

	//--------------------------------------------------------
	void CEventsController::underAttack(Logic::CEntity* entity)
	{
		if(_underAttackTime >= WAIT_STANDARD_TIME && !_cameraComponent->isEntityOnCamera(entity->getPosition()))
		{
			GUI::CServer::getSingletonPtr()->getMessagescontroller()->showTemporalErrorMessage("event_under_attack" +  entity->getName(), EN_UnderAttack, MESSAGE_POSITION_X, calculatePositionY(), "", 3000);
			// TODO: Reproducir sonido.
			GUI::CServer::getSingletonPtr()->getMinimapController()->addMinimapEvent(entity->getName(), GUI::EventType::battle, entity->getPosition());
			_underAttackTime = 0;
		}
	} // underAttack

	//--------------------------------------------------------
	float CEventsController::calculatePositionY()
	{
		int otherMessages = GUI::CServer::getSingletonPtr()->getMessagescontroller()->countEventMessages(); //- 1;
		if(otherMessages < 0)
			otherMessages *= -1;
		//std::cout << "otherMessages: " << otherMessages << std::endl;
		float positionY = MESSAGE_POSITION_Y - (otherMessages * 0.08f);
		//std::cout << "PositionY: " << positionY << std::endl;
		return positionY;
	}

	//--------------------------------------------------------
	void CEventsController::tick(unsigned int msecs)
	{
		_underAttackTime += msecs;
		_stealingResourcesTime += msecs;
		_princessKidnappedTime += msecs;
		_princessDamagedTime += msecs;
		_missingWoodTime += msecs;
		_missingIronTime += msecs;
		_missingStoneTime += msecs;
	}

} // namespace GUI
