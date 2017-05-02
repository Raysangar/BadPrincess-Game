#ifndef Logic_ListenerComponent_h
#define Logic_ListenerComponent_h

#include "Logic/Entity/Component.h"

namespace Audio
{
	class CListener;
}

namespace Logic
{
	class CCamera;

	class CListenerComponent : IComponent
	{

		DEC_FACTORY(CListenerComponent);

	public:

		CListenerComponent(std::string componentName);

		virtual ~CListenerComponent();
		
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		virtual void awake();

		virtual bool activate();

		virtual void deactivate();

		virtual void tick(unsigned int msecs);

		virtual bool acceptN(const std::shared_ptr<NMessage> &message);

		virtual void processN(const std::shared_ptr<NMessage> &message);

	private:

		Audio::CListener* listener;

		CCamera* camera;
	};

	REG_FACTORY(CListenerComponent);
}

#endif