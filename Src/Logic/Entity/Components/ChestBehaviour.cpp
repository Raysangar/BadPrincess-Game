/**
@file ChestBehaviour.cpp

Contains ChestBehaviour class implementation.
 
@see Logic::IComponent

@author Rayco Sánchez García
@date January, 2015
*/

#include "ChestBehaviour.h"

#include "Map/MapEntity.h"
#include "AI/ResourceServer.h"


namespace Logic 
{
	IMP_FACTORY(CChestBehaviour);
	
	//---------------------------------------------------------

	CChestBehaviour::~CChestBehaviour() 
	{
	}
	
	//---------------------------------------------------------

	bool CChestBehaviour::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;
		return true;

	}
	
	//---------------------------------------------------------

	void CChestBehaviour::awake(){}

	//---------------------------------------------------------

	void CChestBehaviour::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);
	}

	//--------------------------------------------------------

	bool CChestBehaviour::acceptN(const std::shared_ptr<NMessage> &message)
	{
		return false;
	}

	//-------------------------------------------------------

	void CChestBehaviour::processN(const std::shared_ptr<NMessage> &message)
	{
	
	}

	//-------------------------------------------------------
	
} // namespace Logic

