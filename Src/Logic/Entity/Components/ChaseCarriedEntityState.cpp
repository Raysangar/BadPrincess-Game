/**
@file ChaseCarriedEntityState.cpp
*/

#include "ChaseCarriedEntityState.h"

#include "Map/MapEntity.h"

namespace Logic 
{
	IMP_FACTORY(CChaseCarriedEntityState);

	//---------------------------------------------------------

	CChaseCarriedEntityState::~CChaseCarriedEntityState() 
	{

	} 

	//---------------------------------------------------------

	bool CChaseCarriedEntityState::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		return true;

	} 

	void CChaseCarriedEntityState::awake()
	{
		_carriedEntity = nullptr;
	}
	void CChaseCarriedEntityState::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);
	}

	//---------------------------------------------------------

	bool CChaseCarriedEntityState::acceptN(const std::shared_ptr<NMessage> &message)
	{
		return  false;
	}

	//---------------------------------------------------------

	void CChaseCarriedEntityState::processN(const std::shared_ptr<NMessage> &message)
	{
		
	} 
}

