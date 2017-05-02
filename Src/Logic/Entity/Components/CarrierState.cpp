/**
@file CarrierState.cpp
*/

#include "CarrierState.h"

#include "Logic/Entity/Entity.h"
#include "Logic/AllyUnitMessages.h"
#include "Logic/CarrierMessages.h"
#include "Logic/RouteCalculationMessages.h"
#include "Logic/Entity/Components/GUIManager.h"

#include "GUI\Server.h"
#include "GUI\PlayerController.h"
#include "GUI\ActiveUnitController.h"

#include "Map/MapEntity.h"

namespace Logic 
{
	IMP_FACTORY(CCarrierState);

	//---------------------------------------------------------

	CCarrierState::~CCarrierState() 
	{

	} 

	//---------------------------------------------------------

	bool CCarrierState::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;
		

		assert(entityInfo->hasAttribute("carrierAbilityChar"));
		_carrierAbilityChar = entityInfo->getStringAttribute("carrierAbilityChar");

		assert(entityInfo->hasAttribute("carrierIdleAnimationName"));
		_carrierIdleAnimationName = entityInfo->getStringAttribute("carrierIdleAnimationName");

		assert(entityInfo->hasAttribute("carrierMoveAnimationName"));
		_carrierMoveAnimationName = entityInfo->getStringAttribute("carrierMoveAnimationName");

		assert(entityInfo->hasAttribute("ability"+_carrierAbilityChar+"_name"));
		_carrierAbilityIcon = entityInfo->getStringAttribute("ability"+_carrierAbilityChar+"_name");

		assert(entityInfo->hasAttribute("releaseAbility_name"));
		_releaseAbilityIcon = entityInfo->getStringAttribute("releaseAbility_name");

		IComponent * component = _entity->getComponent("CGUIManager");
		assert(component);
		_guiManagerComponent = static_cast<Logic::CGUIManager*>(component);
		
		return true;

	} 

	void CCarrierState::awake()
	{
		_carrierAbilityActive=false;
		_carriedEntity = nullptr;
		//reset the icon 
		setupAbilityIcon(_carrierAbilityIcon);
	}
	void CCarrierState::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);
	}

	//---------------------------------------------------------

	bool CCarrierState::acceptN(const std::shared_ptr<NMessage> &message)
	{
		return message->type.compare("CastAbility")==0 || message->type.compare("InterruptTransport")==0 ||
			(message->type.compare("AvatarActivation") == 0) || (message->type.compare("AvatarRouteTo") == 0);
	}

	//---------------------------------------------------------

	void CCarrierState::processN(const std::shared_ptr<NMessage> &message)
	{
		if(message->type.compare("CastAbility") == 0)
		{
			std::shared_ptr<CastAbility> m=std::static_pointer_cast<CastAbility>(message);			
			if(m->ability.compare(_carrierAbilityChar)==0 && !GUI::CServer::getSingletonPtr()->getPlayerController()->hasAttachedEntity())
			{
				_carrierAbilityActive= !_carrierAbilityActive;
				//if i disable transport ability interrupt the movement
				if(!_carrierAbilityActive && _carriedEntity != NULL)
				{
					auto interruptTransport = std::make_shared<InterruptTransport>();
					_entity->emitMessageN(interruptTransport);
					setupAbilityIcon(_carrierAbilityIcon);
					GUI::CServer::getSingletonPtr()->getActiveUnitController()->restoreMouseCursor();
				}
				else
				{
					GUI::CServer::getSingletonPtr()->getActiveUnitController()->setMouseCursor("BadPrincessHud/CarryChestCursor");					
				}
			}
		}
		else if(message->type.compare("InterruptTransport") == 0)
		{
			//in case that entity still reach the entity to transport or he still transport it somewhere
			auto m = std::make_shared<AvatarInterruptRoute>();
			_entity->emitMessageN(m);

			//not set carried entity to nullptr because the LACarryEntity need it into onAbort
//			_carriedEntity= nullptr;
			_carrierAbilityActive=false;
			setupAbilityIcon(_carrierAbilityIcon);
		}
		else if( message->type.compare("EntityDestroying") == 0)
		{
			//TODO if we will transport another destructible entity

		}
		else if (message->type.compare("AvatarActivation") == 0)
		{
			std::shared_ptr<AvatarActivation> m = std::static_pointer_cast<AvatarActivation>(message);

			if(!m->actived)
			{
				if(_carriedEntity == nullptr)
					_carrierAbilityActive = false;
				GUI::CServer::getSingletonPtr()->getActiveUnitController()->restoreMouseCursor();
			}
		}		
		else if (message->type.compare("AvatarRouteTo") == 0)
		{		
			auto m = std::static_pointer_cast<AvatarRouteTo>(message);
			GUI::CServer::getSingletonPtr()->getActiveUnitController()->restoreMouseCursor();
			if(m->sender != _entity && _carriedEntity == nullptr)
			{	
				_carrierAbilityActive = false;
			}
		}
	} 

	void CCarrierState::setupAbilityIcon(std::string iconName)
	{
		if(_carrierAbilityChar.compare("Q")==0)
		{			
			//needed because if you deselect the unit and reselect the workman the icon not correspond to the reality state
			_guiManagerComponent->setAbilityQName(iconName);
			if(GUI::CServer::getSingletonPtr()->getActiveUnitController()->getActiveUnit()==_entity)
				GUI::CServer::getSingletonPtr()->getActiveUnitController()->setNameAbilityQ(iconName);
		}else if(_carrierAbilityChar.compare("W")==0)
		{			
			//needed because if you deselect the unit and reselect the workman the icon not correspond to the reality state
			_guiManagerComponent->setAbilityWName(iconName);
			if(GUI::CServer::getSingletonPtr()->getActiveUnitController()->getActiveUnit()==_entity)
				GUI::CServer::getSingletonPtr()->getActiveUnitController()->setNameAbilityW(iconName);
		}else if(_carrierAbilityChar.compare("E")==0)
		{			
			//needed because if you deselect the unit and reselect the workman the icon not correspond to the reality state
			_guiManagerComponent->setAbilityEName(iconName);
			if(GUI::CServer::getSingletonPtr()->getActiveUnitController()->getActiveUnit()==_entity)
				GUI::CServer::getSingletonPtr()->getActiveUnitController()->setNameAbilityE(iconName);
		}else if(_carrierAbilityChar.compare("R")==0)
		{			
			//needed because if you deselect the unit and reselect the workman the icon not correspond to the reality state
			_guiManagerComponent->setAbilityRName(iconName);
			if(GUI::CServer::getSingletonPtr()->getActiveUnitController()->getActiveUnit()==_entity)
				GUI::CServer::getSingletonPtr()->getActiveUnitController()->setNameAbilityR(iconName);
		}
	}
}


