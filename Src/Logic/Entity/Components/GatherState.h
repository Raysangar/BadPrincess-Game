/**
@file GatherState.h

*/

#ifndef __Logic_GatherState_H
#define __Logic_GatherState_H

#include "Logic/Entity/Component.h"

namespace Logic 
{
	class CEntity;
	class CInteractSOState;
	class CCarrierState;
	class CWorkmanState;
	
/**
	Componente que se encarga...
*/
	
	class CGatherState : public IComponent
	{

	DEC_FACTORY(CGatherState);

	public:	


		CGatherState(std::string componentName) : IComponent(componentName){}

		virtual ~CGatherState();
		
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		virtual void awake();

		virtual void tick(unsigned int msecs);

		virtual bool acceptN(const std::shared_ptr<NMessage> &message);

		virtual void processN(const std::shared_ptr<NMessage> &message);

	private:
		void goNearestSmartObjectResource(Logic::CEntity* decisionEntity);
		void removeTransportAbility(Logic::CEntity* decisionEntity,Logic::CCarrierState* carrierComponent=nullptr);
	}; 

	REG_FACTORY(CGatherState);
} 

#endif
