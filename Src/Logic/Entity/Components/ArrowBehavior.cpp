/**
@file ArrowBehavior.cpp
*/

#include "ArrowBehavior.h"
#include "Logic\AllyUnitMessages.h"
#include "Logic/GraphicsMessages.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"
#include "Logic/Server.h"
#include "Logic\Entity\Entity.h"
#include "Logic\Entity\Components\Graphics.h"

#define MOV_SPEED 7.50f
#define HEIGHT_TO_SHOOT 2.5f

namespace Logic 
{
	IMP_FACTORY(CArrowBehavior);

	//---------------------------------------------------------

	bool CArrowBehavior::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		if(entityInfo->hasAttribute("particle_name"))
			_particleName = entityInfo->getStringAttribute("particle_name");

		return true;

	} 

	void CArrowBehavior::awake()
	{
		((CGraphics*)_entity->getComponent("CGraphics"))->setVisible(false);
	}

	void CArrowBehavior::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);

		if(_isFlying)
		{
			if(_entity->getPosition().positionCloses(_target->getPosition(),Ogre::Real(0.0004f)))
			{
				_isFlying = false;
				((CGraphics*)_entity->getComponent("CGraphics"))->setVisible(false);	
				_entity->setPositionN(_owner->getPosition() + Vector3(0.0f,-100.0f,0.0f));

				auto playParticleMessage = std::make_shared<StopParticle>();
				playParticleMessage->particleName = _particleName;
				_entity->emitMessageN(playParticleMessage);
			}
			else
			{
				_entity->setPositionN(_entity->getPosition() + _targetDirection * MOV_SPEED);
			}
		}
	}

	//---------------------------------------------------------

	bool CArrowBehavior::acceptN(const std::shared_ptr<NMessage> &message)
	{
		return message->type.compare("ShootArrow") == 0;
	}

	//---------------------------------------------------------

	void CArrowBehavior::processN(const std::shared_ptr<NMessage> &message)
	{
		if(message->type.compare("ShootArrow") == 0)
		{
			std::shared_ptr<ShootArrow> m=std::static_pointer_cast<ShootArrow>(message);			
			_target = m->target;		
				
			_entity->setPositionN(_owner->getPosition() + Vector3(0.0f,HEIGHT_TO_SHOOT,0.0f));
			
			Ogre::Vector3 targetPos = _target->getPosition();
			targetPos.y = _entity->getPosition().y * 1.5f;
				
			_targetDirection = targetPos - _entity->getPosition();
			
			_targetDirection.normalise();

			Ogre::Quaternion quat  = Ogre::Vector3::NEGATIVE_UNIT_Z.getRotationTo(_targetDirection);
			
			Ogre::Matrix3 rot;
			quat.ToRotationMatrix(rot);

			_entity->setOrientationN(rot);

			_isFlying = true;
			((CGraphics*)_entity->getComponent("CGraphics"))->setVisible(true);

			auto playParticleMessage = std::make_shared<PlayParticle>();
			playParticleMessage->particleName = _particleName;
			_entity->emitMessageN(playParticleMessage);
		}
	} 

}

