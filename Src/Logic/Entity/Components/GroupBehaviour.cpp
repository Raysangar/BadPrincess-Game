/**
@file GroupBehaviour.cpp

Contiene la implementación del componente que controla 
el comportamiento de una unidad al moverse en grupo

@see Logic::IComponent

@author Iván León
@date Febrero, 2015
*/

#include "GroupBehaviour.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include "Logic/RouteCalculationMessages.h"

namespace Logic 
{
	IMP_FACTORY(CGroupBehaviour);

	//---------------------------------------------------------

	bool CGroupBehaviour::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		return true;

	} // spawn	

	//---------------------------------------------------------

	void CGroupBehaviour::awake()
	{
		
	}

	//---------------------------------------------------------

	void CGroupBehaviour::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);
	}

	//---------------------------------------------------------

	bool CGroupBehaviour::acceptN(const std::shared_ptr<NMessage> &message)
	{
		return (message->type.compare("AvatarMoveToPosition") == 0) || 
			(message->type.compare("AvatarFinishedMove") == 0);
	} // acceptN

	//---------------------------------------------------------

	void CGroupBehaviour::processN(const std::shared_ptr<NMessage> &message)
	{
		//Logic::CGroupLeader *component=static_cast<CGroupLeader*>(_groupLeader->getComponent("CGroupLeader"));
		if (message->type.compare("AvatarMoveToPosition") == 0)
		{
			//if(_inGroup)
			//	_inGroup = false;
		}

		else if (message->type.compare("AvatarFinishedMove") == 0)
		{			
/*			if(_inGroup && _entity->getPosition().distance(*_meetingPoint) > _meetingPointRadius)
			{				
				float dist = _entity->getPosition().distance(*_meetingPoint) - _meetingPointRadius;
				Vector3 meetingPoint = (*_meetingPoint) - _entity->getPosition();

				meetingPoint.normalise();
				meetingPoint *= dist;

				auto m2 = std::make_shared<AvatarMoveToPosition>();
				m2->target= _entity->getPosition() + meetingPoint;
				m2->moveType = 1;
				_entity->emitMessageN(m2);
			}	*/		
		}		
			
	} // processN

	//---------------------------------------------------------

	void CGroupBehaviour::groupMoveToPosition(Vector3 destination)
	{
		auto m2 = std::make_shared<AvatarRouteTo>();
		m2->target = destination;
		m2->calculateNewRoute = true;
		_entity->emitMessageN(m2);
	}


} // namespace Logic

