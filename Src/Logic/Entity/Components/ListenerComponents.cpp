#include "ListenerComponent.h"

#include "Logic/CameraMessages.h"

#include "Logic/MovementMessages.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Components/Camera.h"
#include "Audio/Server.h"
#include "Audio/Listener.h"

namespace Logic
{
	IMP_FACTORY(CListenerComponent);

	CListenerComponent::CListenerComponent(std::string componentName) : IComponent(componentName), listener(nullptr) {}

	//---------------------------------------------------------

	CListenerComponent::~CListenerComponent() {}

	//---------------------------------------------------------

	bool CListenerComponent::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		return true;
	}

	//---------------------------------------------------------

	void CListenerComponent::awake()
	{
		camera = (CCamera*) _entity->getComponent("CCamera");
	}

	//---------------------------------------------------------

	bool CListenerComponent::activate()
	{
		listener = Audio::CServer::getInstance()->addListenerOwner(this);
		if (listener)
			listener->setPosition(_entity->getPosition());
		return listener != nullptr;
	}

	//---------------------------------------------------------

	void CListenerComponent::deactivate()
	{
		Audio::CServer::getInstance()->removeListenerOwner(this);
		listener = nullptr;
	}

	//---------------------------------------------------------

	void CListenerComponent::tick(unsigned int msecs) 
	{
		IComponent::tick(msecs);
	}

	//---------------------------------------------------------

	bool CListenerComponent::acceptN(const std::shared_ptr<NMessage> &message)
	{
		return (message->type == "TransformMessage");
	}

	//---------------------------------------------------------

	void CListenerComponent::processN(const std::shared_ptr<NMessage> &message) 
	{
		listener->setPosition(std::static_pointer_cast<TransformMessage>(message)->transform.getTrans());
	}
}