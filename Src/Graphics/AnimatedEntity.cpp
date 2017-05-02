//---------------------------------------------------------------------------
// AnimatedEntity.cpp
//---------------------------------------------------------------------------

/**
@file AnimatedEntity.cpp

Contiene la implementación de la clase que representa una entidad gráfica 
con animaciones.

@see Graphics::CAnimatedEntity
@see Graphics::CEntity

@author David Llansó
@date Julio, 2010
*/

#include "AnimatedEntity.h"

#include <assert.h>

#include <OgreEntity.h>
#include <OgreAnimationState.h>

#define BLENDING_TIME 0

namespace Graphics 
{
	void CAnimatedEntity::reset()
	{
		_currentAnimation = nullptr;
		_targetAnimation = nullptr;
		_timeAcum = 0.0f;
		animationPaused = false;
	}
		
	bool CAnimatedEntity::setAnimation(const std::string &anim, bool loop)
	{
		if(!_entity->getAllAnimationStates()->hasAnimationState(anim))
			return false;
		
		if(_currentAnimation == nullptr)
		{
			_currentAnimation = _entity->getAnimationState(anim);
			_currentAnimation->setWeight(1.0);
			_currentAnimation->setEnabled(true);
			_currentAnimation->setLoop(loop);
			_currentAnimation->setTimePosition(0);

			_targetAnimation = nullptr;
			_timeAcum = 0.0f;
		}
		else if(_currentAnimation != _entity->getAnimationState(anim) && _targetAnimation != _entity->getAnimationState(anim))
		{
			if(_targetAnimation != nullptr)
				_targetAnimation->setEnabled(false);

			_currentAnimation->setWeight(1.0);

			_targetAnimation = _entity->getAnimationState(anim);
			_targetAnimation->setWeight(0.0);
			_targetAnimation->setEnabled(true);
			_targetAnimation->setLoop(loop);
			_targetAnimation->setTimePosition(0);

			_timeAcum = 0.0f;
		}
		else if(_currentAnimation == _entity->getAnimationState(anim))
		{
			_currentAnimation->setWeight(1.0);
			_currentAnimation->setEnabled(true);
			_currentAnimation->setLoop(loop);

			_targetAnimation = nullptr;
			_timeAcum = 0.0f;
		}
		
		return true;

	} // setAnimation

	//--------------------------------------------------------
		
	bool CAnimatedEntity::stopAnimation(const std::string &anim)
	{
		if(!_entity->getAllAnimationStates()->hasAnimationState(anim))
			return false;
		Ogre::AnimationState *animation = _entity->getAnimationState(anim);
		animation->setEnabled(false);
		// Si la animación a parar es la animación activa ya no lo estará.
		//if(animation == _currentAnimation)
			//_currentAnimation = 0;
		
		return true;

	} // stopAnimation

	//--------------------------------------------------------
		
	void CAnimatedEntity::stopAllAnimations()
	{
		if(_entity->getAllAnimationStates()->hasEnabledAnimationState())
		{
			Ogre::ConstEnabledAnimationStateIterator it = 
				_entity->getAllAnimationStates()->getEnabledAnimationStateIterator();
			Ogre::AnimationState *animation;

			//hay que recorrer con el iterador las animaciones una a una y pausarlas
			while(it.hasMoreElements())
			{
				animation = it.getNext();
				if(animation != _currentAnimation && animation != _targetAnimation)
					animation->setEnabled(false);
			}

			// Si había animación activa ya no lo está.
			//_currentAnimation = 0;
		}

	} // stopAllAnimations

	//--------------------------------------------------------
		
	void CAnimatedEntity::tick(float secs)
	{

		if(_targetAnimation != nullptr && !_currentAnimation->hasEnded())
		{
			blend(secs);

			if(_currentAnimation->hasEnded() && _targetAnimation != nullptr)
			{
				_targetAnimation->setWeight(1);
			
				_currentAnimation->setWeight(0);
				_currentAnimation->setEnabled(false);

				_currentAnimation = _targetAnimation;

				_targetAnimation = nullptr;
				_timeAcum = 0.0f;
			}
		}
		else
		{
			if(_currentAnimation && !_currentAnimation->hasEnded() && _currentAnimation->getEnabled())
			{
				_currentAnimation->addTime(animationPaused ? 0 : secs);
				bool blending = true;
				if(!_targetAnimation)
					blending = false;
				// Comprobamos si la animación ha terminado para avisar
				if(_observer && _currentAnimation->hasEnded())
					_observer->animationFinished
					(_currentAnimation->getAnimationName(), blending);
				
			}
			//Aqui solo entrara cuando haya una animacion finalizada sin loop y entre una nueva animacion (por ahora solo la princesa muerta)
			else if(_currentAnimation != nullptr && _currentAnimation->hasEnded() && _targetAnimation != nullptr)
			{
				_targetAnimation->setWeight(1);
			
				_currentAnimation->setWeight(0);
				_currentAnimation->setEnabled(false);

				_currentAnimation = _targetAnimation;

				_targetAnimation = nullptr;
				_timeAcum = 0.0f;
			}
		}
		
	} // tick

	//--------------------------------------------------------

	void CAnimatedEntity::blend(float secs)
	{
		_timeAcum += secs;
		if(_timeAcum > BLENDING_TIME || _currentAnimation->hasEnded())
		{
 			_targetAnimation->setWeight(1);
			
			_currentAnimation->setWeight(0);
			_currentAnimation->setEnabled(false);

			_currentAnimation = _targetAnimation;

			_targetAnimation = nullptr;
			_timeAcum = 0.0f;
		}
		else
		{
			//_targetAnimation->setWeight(_timeAcum/BLENDING_TIME);
			//_currentAnimation->setWeight(1 - _timeAcum/BLENDING_TIME);

			_currentAnimation->addTime(secs);
			_targetAnimation->addTime(secs);
		}
	} // blend

	//--------------------------------------------------------

	bool CAnimatedEntity::animationIsFinished()
	{
		if(_currentAnimation)
			return _currentAnimation->hasEnded();
		return false;
	}

	float CAnimatedEntity::getCurrentAnimationTime()
	{
		if(_currentAnimation)
			return _currentAnimation->getTimePosition();
		return 0.0f;
	}

} // namespace Graphics
