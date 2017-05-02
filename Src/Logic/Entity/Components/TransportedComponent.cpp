/**
@file TransportedEntityComponent.cpp
*/

#include "TransportedComponent.h"
#include "Logic\Entity\Entity.h"
#include "Map/MapEntity.h"

#include "Logic\CarrierMessages.h"
#include "Logic\Entity\Components\Graphics.h"

namespace Logic 
{
	IMP_FACTORY(CTransportedComponent);

	//---------------------------------------------------------


	//---------------------------------------------------------

	bool CTransportedComponent::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		assert(entityInfo->hasAttribute("model"));
		_normalModelMesh = entityInfo->getStringAttribute("model");

		assert(entityInfo->hasAttribute("transported_mesh_model"));
		_transportedModelMesh = entityInfo->getStringAttribute("transported_mesh_model");

		Logic::IComponent*component=_entity->getComponent("CGraphics");
		assert(component);
		_graphicsComponent = static_cast<Logic::CGraphics*>(component);

		return true;

	} 

	void CTransportedComponent::awake()
	{
		_transporterEntity=nullptr;
		_graphicsComponent->changeModel(_normalModelMesh,1.0f);
	}
	void CTransportedComponent::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);
	}

	//---------------------------------------------------------

	bool CTransportedComponent::acceptN(const std::shared_ptr<NMessage> &message)
	{
		return message->type.compare("TransportEntityEnd")==0 || message->type.compare("TransportEntityStart") == 0;
	}

	//---------------------------------------------------------

	void CTransportedComponent::processN(const std::shared_ptr<NMessage> &message)
	{
		if(message->type.compare("TransportEntityEnd") == 0)
		{
			_transporterEntity = nullptr;
			_graphicsComponent->changeModel(_normalModelMesh,1.0f);
//			_graphicsComponent->setVisible(true);
		}
		else if(message->type.compare("TransportEntityStart") == 0)
		{
			std::shared_ptr<TransportEntityStart> m=std::static_pointer_cast<TransportEntityStart>(message);			
			_transporterEntity=m->entity;

			_graphicsComponent->changeModel(_transportedModelMesh,1.0f);
//			_graphicsComponent->setVisible(false);
		}
	} 

}

