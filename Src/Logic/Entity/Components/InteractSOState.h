/**
@file InteractSOState.h

*/

#ifndef __Logic_InteractSOState_H
#define __Logic_InteractSOState_H

#include "Logic/Entity/Component.h"

namespace Logic 
{
	class CEntity;
	
/**
	Componente que se encarga...
*/
	
	class CInteractSOState : public IComponent
	{

	DEC_FACTORY(CInteractSOState);

	public:	


		CInteractSOState(std::string componentName) : IComponent(componentName),_smartEntity(nullptr), _smartPosition(Vector3::NEGATIVE_UNIT_Y),_isInteracting(false){}

		virtual ~CInteractSOState();
		
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		virtual void awake();

		virtual void tick(unsigned int msecs);

		virtual bool acceptN(const std::shared_ptr<NMessage> &message);

		virtual void processN(const std::shared_ptr<NMessage> &message);

		// -- smartEntity methods --

		/**
		Método que asigna una smartEntity al estado del obrero.

		@param entity Entidad que se debe asignar. 
		*/
		void setSmartEntity(CEntity *entity);

		/**
		Método que devuelve la smartEntity que el obrero está utilizando actualmente.

		@return la smartEntity.
		*/
		CEntity* getSmartEntity();

		// position methods.

		void setSmartPosition(const Vector3 &p);

		const Vector3& getSmartPosition();

		bool isInteracting(){return _isInteracting;}

		void setIsInteracting(bool val){_isInteracting = val;}

	protected:
		CEntity* _smartEntity;
		Vector3 _smartPosition;
		bool _isInteracting;
	}; 

	REG_FACTORY(CInteractSOState);
} 

#endif
