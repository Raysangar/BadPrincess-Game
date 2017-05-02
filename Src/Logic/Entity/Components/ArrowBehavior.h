/**
@file ArrowBehavior.h

*/

#ifndef __Logic_ArrowBehavior_H
#define __Logic_ArrowBehavior_H

#include "Logic/Entity/Component.h"

namespace Ogre
{
	class Vector3;
}

namespace Logic 
{
	class CEntity;

	class CArrowBehavior : public IComponent
	{

	DEC_FACTORY(CArrowBehavior);

	public:	

		CArrowBehavior(std::string componentName) : IComponent(componentName),_owner(nullptr),_target(nullptr),_isFlying(false){}

		virtual ~CArrowBehavior(){};
		
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		virtual void awake();

		virtual void tick(unsigned int msecs);

		virtual bool acceptN(const std::shared_ptr<NMessage> &message);

		virtual void processN(const std::shared_ptr<NMessage> &message);

		Logic::CEntity *getOwner() const { return _owner; }

		void setOwner(Logic::CEntity *owner) { _owner = owner; }

		Logic::CEntity *getTargetPosition() const { return _target; }

		void setTarget(Logic::CEntity *target) { _target = target; }

	protected:

		Logic::CEntity *_owner;

		Logic::CEntity *_target;

		Ogre::Vector3 _targetDirection;

		std::string _particleName;

		bool _isFlying;

	}; 

	REG_FACTORY(CArrowBehavior);
} 

#endif
