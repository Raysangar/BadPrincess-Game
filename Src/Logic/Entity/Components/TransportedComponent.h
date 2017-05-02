/**
@file TransportedComponent.h

*/

#ifndef __Logic_TransportedComponent_H
#define __Logic_TransportedComponent_H

#include "Logic/Entity/Component.h"

namespace Logic 
{
	class CEntity;
	class CGraphics;
	
/**
	Componente que se encarga...
*/
	
	class CTransportedComponent : public IComponent
	{

	DEC_FACTORY(CTransportedComponent);

	public:	

		CTransportedComponent(std::string componentName) : IComponent(componentName),_transporterEntity(nullptr),_graphicsComponent(nullptr),_normalModelMesh(""),_transportedModelMesh(""){}

		virtual ~CTransportedComponent(){};
		
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		virtual void awake();

		virtual void tick(unsigned int msecs);

		virtual bool acceptN(const std::shared_ptr<NMessage> &message);

		virtual void processN(const std::shared_ptr<NMessage> &message);

		CEntity* getTransporterEntity(){return _transporterEntity;};

		bool isTransported(){return _transporterEntity!=nullptr;}

	protected:
		//Name of mesh model when the entity it's in normal state
		std::string _normalModelMesh;
		//Name of mesh model when the entity are carried
		std::string _transportedModelMesh;

		Logic::CEntity* _transporterEntity;
		Logic::CGraphics *_graphicsComponent;
	}; 

	REG_FACTORY(CTransportedComponent);
} 

#endif
