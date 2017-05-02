/**
@file WellComponent.h

*/

#ifndef __Logic_WellComponent_H
#define __Logic_WellComponent_H

#include "Logic/Entity/Component.h"

namespace Logic 
{
	class CEntity;
	class CBuildComponent;
	
/**
	Componente que se encarga...
*/
	
	class CWellComponent : public IComponent
	{

	DEC_FACTORY(CWellComponent);

	public:	

		CWellComponent(std::string componentName) : IComponent(componentName),_buildComponent(nullptr){}

		virtual ~CWellComponent();
		
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		virtual void awake();

		// -- smartEntity methods --
		virtual bool acceptN(const std::shared_ptr<NMessage> &message);

		virtual void processN(const std::shared_ptr<NMessage> &message);
	
	


	protected:

		Logic::CBuildComponent* _buildComponent;
		void sendAbilityMessage(std::string charAbility,bool isEnabled);
	}; 

	REG_FACTORY(CWellComponent);
} 

#endif
