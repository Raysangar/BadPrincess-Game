#include "3DEvent.h"

#include "Server.h"
#include "BaseSubsystems\Math.h"

#include "fmod_studio.hpp"
#include "fmod.hpp"

namespace Audio
{
	C3DEvent::C3DEvent(const std::string &eventName, const Vector3 &position, const Vector3 &forward, 
		const Vector3 &up, const Vector3 &velocity) : CEvent(eventName)
	{
		FMOD_3D_ATTRIBUTES attributes;

		CServer::setFmodVectorWithLogicVector(attributes.position, position);
		CServer::setFmodVectorWithLogicVector(attributes.forward, forward);
		CServer::setFmodVectorWithLogicVector(attributes.up, up);
		CServer::setFmodVectorWithLogicVector(attributes.velocity, velocity);

		FMOD_RESULT result = eventInstance->set3DAttributes(&attributes);
		assert(result == FMOD_OK);
	}

	//---------------------------------------------------------

	C3DEvent::~C3DEvent()
	{
		Audio::CServer::getInstance()->removeFromPlayingSounds(this);
	}

	//---------------------------------------------------------

	void C3DEvent::setPosition(const Vector3 &position)
	{
		FMOD_3D_ATTRIBUTES attributes;
		eventInstance->get3DAttributes(&attributes);
		CServer::setFmodVectorWithLogicVector(attributes.position, position);
		eventInstance->set3DAttributes(&attributes);
	}

	//---------------------------------------------------------

	void C3DEvent::setMovement(const Vector3 &movement)
	{
		FMOD_3D_ATTRIBUTES attributes;
		eventInstance->get3DAttributes(&attributes);
		CServer::setFmodVectorWithLogicVector(attributes.velocity, movement);
		eventInstance->set3DAttributes(&attributes);
	}

	//---------------------------------------------------------

	void C3DEvent::setOrientation(const Vector3 &forward)
	{
		FMOD_3D_ATTRIBUTES attributes;
		eventInstance->get3DAttributes(&attributes);
		CServer::setFmodVectorWithLogicVector(attributes.forward, forward);
		eventInstance->set3DAttributes(&attributes);
	}

	//---------------------------------------------------------

	void C3DEvent::setUpDirection(const Vector3 &up)
	{
		FMOD_3D_ATTRIBUTES attributes;
		eventInstance->get3DAttributes(&attributes);
		CServer::setFmodVectorWithLogicVector(attributes.up, up);
		eventInstance->set3DAttributes(&attributes);
	}

	//---------------------------------------------------------

	void C3DEvent::load()
	{
		CServer::getInstance()->registerSFXObserver(this);
	}

	//---------------------------------------------------------

	void C3DEvent::unload()
	{
		CServer::getInstance()->unregisterSFXObserver(this);
	}

	//---------------------------------------------------------

	void C3DEvent::play()
	{
		if (Audio::CServer::getInstance()->addToPlayingSounds(this))
		{
			eventInstance->start();
			paused = false;
		}
	}

	//---------------------------------------------------------

	void C3DEvent::stop()
	{
		Audio::CServer::getInstance()->removeFromPlayingSounds(this);
		eventInstance->stop(FMOD_STUDIO_STOP_IMMEDIATE);
		paused = true;
	}

	//---------------------------------------------------------

	void C3DEvent::pause()
	{
		Audio::CServer::getInstance()->removeFromPlayingSounds(this);
		eventInstance->setPaused(true);
		paused = true;
	}

	//---------------------------------------------------------

	void C3DEvent::volumeChanged()
	{
		eventInstance->setVolume(volume * CServer::getInstance()->getSFXVolume());
	}

	//---------------------------------------------------------

	Vector3 C3DEvent::getPosition()
	{
		FMOD_3D_ATTRIBUTES attributes;
		eventInstance->get3DAttributes(&attributes);
		return Vector3(attributes.position.x, attributes.position.y, attributes.position.z);
	}

} // namespace Audio