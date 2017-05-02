/**
@file ChaseCarriedEntityState.h

*/

#ifndef __Logic_ChaseCarriedEntityState_H
#define __Logic_ChaseCarriedEntityState_H

#include "Logic/Entity/Component.h"

namespace Logic 
{
	class CEntity;
	
/**
	Componente que se encarga...
*/
	
	class CChaseCarriedEntityState : public IComponent
	{

	DEC_FACTORY(CChaseCarriedEntityState);

	public:	


		CChaseCarriedEntityState(std::string componentName) : IComponent(componentName),_carriedEntity(nullptr){}

		virtual ~CChaseCarriedEntityState();
		
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		virtual void awake();

		virtual void tick(unsigned int msecs);

		virtual bool acceptN(const std::shared_ptr<NMessage> &message);

		virtual void processN(const std::shared_ptr<NMessage> &message);

		/**
		Método que devuelve la entidad que  estamos trasportando.

		@return la smartEntity.
		*/
		CEntity* getCarriedEntity(){return _carriedEntity;};

		void setCarriedEntity(CEntity* entity){ _carriedEntity=entity;};
	
	protected:

		Logic::CEntity* _carriedEntity;

	}; 

	REG_FACTORY(CChaseCarriedEntityState);
} 

#endif
