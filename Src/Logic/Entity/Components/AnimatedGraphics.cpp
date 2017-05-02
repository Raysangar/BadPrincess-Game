/**
@file AnimatedGraphics.cpp

Contiene la implementación del componente que controla la representación
gráfica de una entidad estática.
 
@see Logic::CAnimatedGraphics
@see Logic::IComponent

@author David Llansó
@date Agosto, 2010
*/

#include "AnimatedGraphics.h"

#include "Logic/AnimationMessages.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include "Graphics/Scene.h"

namespace Logic 
{
	IMP_FACTORY(CAnimatedGraphics);
	
	//---------------------------------------------------------

	Graphics::CMeshEntity* CAnimatedGraphics::createGraphicsEntity(const Map::CEntity *entityInfo)
	{
		_animatedGraphicsEntity = new Graphics::CAnimatedEntity(_entity->getName(),_model);
		
		if(entityInfo->hasAttribute("defaultAnimation"))
		{
			_defaultAnimation = entityInfo->getStringAttribute("defaultAnimation");			
		}

		float scale = 1.0;
		if (entityInfo->hasAttribute("scale"))
			scale = entityInfo->getFloatAttribute("scale");

		return _animatedGraphicsEntity;

	} // createGraphicsEntity
	void CAnimatedGraphics::awake()
	{
		if(!(_defaultAnimation.compare("")==0))
		{
			_animatedGraphicsEntity->setObserver(this);
			_currentAnimation=_defaultAnimation;
			_reset=true;
		}else
			_reset= false;
	}

	bool CAnimatedGraphics::activate()
	{
		if(!_isActive)
		{
			_isActive = true;
			if(!_scene->addEntity(_animatedGraphicsEntity))
				return 0;
			_animatedGraphicsEntity->reset();
			setGraphicsEntityAttributes();
		}
		return true;
	}

	bool CAnimatedGraphics::activateGraphics()
	{
		_isActive = true;
		if(!_scene->addEntity(_animatedGraphicsEntity))
			return 0;
		_animatedGraphicsEntity->reset();
		setGraphicsEntityAttributes();

		return true;
	}

	void CAnimatedGraphics::deactivate()
	{
		_scene->removeEntity(_animatedGraphicsEntity);
		_isActive = false;
	}

	void CAnimatedGraphics::restart()
	{
		_animatedGraphicsEntity->restartAnimation();
	}

	void CAnimatedGraphics::pause()
	{
		_animatedGraphicsEntity->pauseAnimation();
	}
	//---------------------------------------------------------
	
	bool CAnimatedGraphics::acceptN(const std::shared_ptr<NMessage> &message)
	{
		return CGraphics::acceptN(message) || (message->type.compare("SetAnimation") == 0)
			||  (message->type.compare("StopAnimation") == 0);
	} // acceptN
	
	//---------------------------------------------------------

	void CAnimatedGraphics::processN(const std::shared_ptr<NMessage> &message)
	{
		if (CGraphics::acceptN(message))
			CGraphics::processN(message);
		else
		{
			if (message->type.compare("SetAnimation") == 0)
			{
				std::shared_ptr<SetAnimation> m = std::static_pointer_cast<SetAnimation>(message);
				_animatedGraphicsEntity->stopAllAnimations();
				_animatedGraphicsEntity->setAnimation(m->name,m->cycle);
				_currentAnimation = m->name;
				_reset = m->reset;
			}
			else
			{
				std::shared_ptr<StopAnimation> m = std::static_pointer_cast<StopAnimation>(message);
				_animatedGraphicsEntity->stopAnimation(m->animationName);
				_reset = m->reset;
			}
		}
	} // processN
	
	//---------------------------------------------------------
	
	void CAnimatedGraphics::animationFinished(const std::string &animation, const bool blending)
	{
		// Si acaba una animación y tenemos una por defecto la ponemos
		auto endAnimation=std::make_shared<AnimationFinished>();
		endAnimation->animationName = animation;
		_entity->emitMessageN(endAnimation);

		if(_reset && !blending)
		{
			//Con el blending no quiero que ponga a false la animacion con la que hace blend
			//_animatedGraphicsEntity->stopAllAnimations();			
			_animatedGraphicsEntity->setAnimation(animation,false);
			_currentAnimation=animation;
		}
	}
	void CAnimatedGraphics::setGraphicsEntityAttributes()
	{
		_animatedGraphicsEntity->setScale(_currentScale);
		_animatedGraphicsEntity->setTransform(_entity->getTransform());
		_animatedGraphicsEntity->setAnimation(_currentAnimation,_reset);
	}
} // namespace Logic

