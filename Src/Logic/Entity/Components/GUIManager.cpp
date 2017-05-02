/**
@file GUIManager.cpp
*/
#include "GUIManager.h"

#include "GUI/Server.h"
#include "GUI/ActiveUnitController.h"

#include "Logic/GUIMessages.h"
#include "Logic/AllyUnitMessages.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

namespace Logic 
{
	IMP_FACTORY(CGUIManager);

	//---------------------------------------------------------	

	bool CGUIManager::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		if(entityInfo->hasAttribute("portrait_name"))
			_portraitName =  entityInfo->getStringAttribute("portrait_name");

		if(entityInfo->hasAttribute("abilityQ_name"))
			_abilityQName =  entityInfo->getStringAttribute("abilityQ_name");
		if(entityInfo->hasAttribute("abilityQ_tooltip"))
			_abilityQTooltip =  entityInfo->getStringAttribute("abilityQ_tooltip");
		if(entityInfo->hasAttribute("abilityQ_luaField"))
			_abilityQluaField =  entityInfo->getStringAttribute("abilityQ_luaField");

		if(entityInfo->hasAttribute("abilityW_name"))
			_abilityWName =  entityInfo->getStringAttribute("abilityW_name");
		if(entityInfo->hasAttribute("abilityW_tooltip"))
			_abilityWTooltip =  entityInfo->getStringAttribute("abilityW_tooltip");
		if(entityInfo->hasAttribute("abilityW_luaField"))
			_abilityWluaField =  entityInfo->getStringAttribute("abilityW_luaField");

		if(entityInfo->hasAttribute("abilityE_name"))
			_abilityEName =  entityInfo->getStringAttribute("abilityE_name");
		if(entityInfo->hasAttribute("abilityE_tooltip"))
			_abilityETooltip =  entityInfo->getStringAttribute("abilityE_tooltip");
		if(entityInfo->hasAttribute("abilityE_luaField"))
			_abilityEluaField =  entityInfo->getStringAttribute("abilityE_luaField");

		if(entityInfo->hasAttribute("abilityR_name"))
			_abilityRName =  entityInfo->getStringAttribute("abilityR_name");
		if(entityInfo->hasAttribute("abilityR_tooltip"))
			_abilityRTooltip =  entityInfo->getStringAttribute("abilityR_tooltip");
		if(entityInfo->hasAttribute("abilityR_luaField"))
			_abilityRluaField =  entityInfo->getStringAttribute("abilityR_luaField");

		return true;
	} 

	void CGUIManager::awake()
	{

	}
	//---------------------------------------------------------

	bool CGUIManager::activate()
	{

		return true;
	}

	//---------------------------------------------------------

	void CGUIManager::deactivate()
	{
		if(GUI::CServer::getSingletonPtr()->getActiveUnitController()->getActiveUnit() == _entity)
			GUI::CServer::getSingletonPtr()->getActiveUnitController()->deactivateElements();
	} 

	//---------------------------------------------------------

	bool CGUIManager::acceptN(const std::shared_ptr<NMessage> &message)
	{
		return (message->type.compare("AvatarActivation") == 0);
	}

	//---------------------------------------------------------

	void CGUIManager::processN(const std::shared_ptr<NMessage> &message)
	{		

		if (message->type.compare("AvatarActivation") == 0)
		{
			std::shared_ptr<AvatarActivation> m = std::static_pointer_cast<AvatarActivation>(message);

			if(m->actived)
			{
				GUI::CServer::getSingletonPtr()->getActiveUnitController()->setActiveUnit(_entity);
				GUI::CServer::getSingletonPtr()->getActiveUnitController()->activateElements();		
			}
			else
			{
				if(GUI::CServer::getSingletonPtr()->getActiveUnitController()->getActiveUnit() == _entity)
				{
					GUI::CServer::getSingletonPtr()->getActiveUnitController()->setActiveUnit(NULL);
					GUI::CServer::getSingletonPtr()->getActiveUnitController()->deactivateElements();
				}				
			}
		}

	}	
}


