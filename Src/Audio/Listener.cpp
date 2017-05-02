#include "Listener.h"

#include "Audio\Server.h"

#include "fmod_studio.hpp"
#include "fmod.hpp"

namespace Audio
{
	CListener::CListener(Vector3 position, Vector3 forward, Vector3 movement, Vector3  up)
	{
		CServer::setFmodVectorWithLogicVector(listenerAttributes.position, position);
		CServer::setFmodVectorWithLogicVector(listenerAttributes.forward, forward);
		CServer::setFmodVectorWithLogicVector(listenerAttributes.velocity, movement);
		CServer::setFmodVectorWithLogicVector(listenerAttributes.up, up);
		studioSystem = CServer::getInstance()->getStudio();
	}

	//---------------------------------------------------------

	CListener::~CListener()
	{
	}

	//---------------------------------------------------------

	void CListener::load()
	{
	}

	//---------------------------------------------------------

	void CListener::unload()
	{
	}

	//---------------------------------------------------------

	void CListener::setPosition(const Vector3 &position)
	{
		CServer::setFmodVectorWithLogicVector(listenerAttributes.position, position);
		FMOD_RESULT result = studioSystem->setListenerAttributes(0, &listenerAttributes);
		assert(result == FMOD_OK);
	}

	//---------------------------------------------------------

	void CListener::setMovement(const Vector3 &movement)
	{
		CServer::setFmodVectorWithLogicVector(listenerAttributes.velocity, movement);
		studioSystem->setListenerAttributes(0, &listenerAttributes);
	}

	//---------------------------------------------------------

	void CListener::setOrientation(const Vector3 &forward)
	{
		CServer::setFmodVectorWithLogicVector(listenerAttributes.forward, forward);
		studioSystem->setListenerAttributes(0, &listenerAttributes);
	}

	//---------------------------------------------------------

	void CListener::setUpDirection(const Vector3 &up)
	{
		CServer::setFmodVectorWithLogicVector(listenerAttributes.up, up);
		studioSystem->setListenerAttributes(0, &listenerAttributes);
	}
} // namespace Audio