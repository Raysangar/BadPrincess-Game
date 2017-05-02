
#ifndef Audio3DEvent_h
#define Audio3DEvent_h

#include "Event.h"

#include "BaseSubsystems\Math.h"

namespace Audio
{
	class C3DEvent : public CEvent
	{
	public:
		C3DEvent(const std::string &eventName, const Vector3 &position, const Vector3 &forward = Vector3(0,0,1), 
			const Vector3 &up = Vector3(0, 1, 0), const Vector3 &velocity = Vector3(0, 0, 0));

		virtual ~C3DEvent();

		void setPosition(const Vector3 &position);

		Vector3 getPosition();

		void setMovement(const Vector3 &movement);

		void setOrientation(const Vector3 &forward);

		void setUpDirection(const Vector3 &up);

		virtual void load();

		virtual void unload();

		virtual void play();

		virtual void stop();

		virtual void pause();

		virtual void volumeChanged();
	};
}

#endif
