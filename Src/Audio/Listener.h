#ifndef Listener_h
#define Listener_h

#include "BaseSubsystems\Math.h"

#include "../../dependencies/include/FMOD/fmod_studio.hpp"
#include "../../dependencies/include/FMOD/fmod.hpp"

namespace Audio
{
	class CListener
	{
	public:
		CListener(Vector3 position = Vector3::ZERO, Vector3 forward = Vector3(0, 0, -1), 
			Vector3 movement = Vector3::ZERO, Vector3  up = Vector3::UNIT_Y);

		~CListener();

		void load();

		void unload();

		void setPosition(const Vector3 &position);

		void setMovement(const Vector3 &movement);

		void setOrientation(const Vector3 &forward);

		void setUpDirection(const Vector3 &up);
	
	private:
		FMOD::Studio::System* studioSystem;

		FMOD_3D_ATTRIBUTES listenerAttributes;
	};
}

#endif