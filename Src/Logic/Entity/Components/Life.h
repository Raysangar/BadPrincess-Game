/**
@file Life.h

Contiene la declaración del componente que controla la vida de una entidad.

@see Logic::CLife
@see Logic::IComponent

@author David Llansó
@date Octubre, 2010
*/
#ifndef __Logic_Life_H
#define __Logic_Life_H

#include "Logic/Entity/Component.h"

namespace Graphics
{
	class CBillboardSetEntity;
	class CScene;
}

//declaración de la clase
namespace Logic 
{
/**
	Este componente controla la vida de una entidad. Procesa mensajes de tipo 
	DAMAGED (indican que la entidad ha sido herida) y resta el daño a la vida de la
	entidad.
	<p>
	La vida de la entidad se especifica en el mapa con el atributo "life".

	@todo  Si la vida pasa a ser 0 que la entidad muera (poner animación de muerte?)
	y si es el jugador habrá que terminar el juego. Si la vida sigue siendo mayor 
	que 0 trás un golpe ¿poner la animación de herido?.
	
    @ingroup logicGroup

	@author David Llansó García
	@date Octubre, 2010
*/
	class CLife : public IComponent
	{
		DEC_FACTORY(CLife);
	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CLife(std::string componentName) : IComponent(componentName), _currentLife(1.f),_maxLife(1.f),_lifebarBg(nullptr),_lifebarFg(nullptr),
			_visualLife(1.f),_animationTime(0),_lifebarPosition(Vector3::ZERO),_isInvincible(false), _berserkerMode(false),_lifebarColorR(0.0f),_lifebarColorG(0.0f),_lifebarColorB(0.0f) {}
		
		/**
		Destructor (virtual); Quita de la escena y destruye la entidad gráfica.
		*/
		virtual ~CLife();
		
		/**
		Inicialización del componente usando la descripción de la entidad que hay en 
		el fichero de mapa.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		Método virtual que elige que mensajes son aceptados. Son válidos
		SELECTED.

		@param message Mensaje a chequear.
		@return true si el mensaje es aceptado.
		*/
		virtual bool acceptN(const std::shared_ptr<NMessage> &message);

		/**
		Método virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		virtual void processN(const std::shared_ptr<NMessage> &message);

		virtual bool activate();

		virtual void deactivate();

		virtual void tick(unsigned int msecs);

		virtual void awake();

		const float getMaxLife(){ return _maxLife; }

		void setMaxLife(float newMaxLife);

		const float getCurrentLife(){ return _currentLife; }

		bool getInvincible(){ return _isInvincible;}

		void setInvincible(bool invincible){ _isInvincible=invincible;}

		void setBerserkerMode(bool active){ _berserkerMode=active; }

		void setLifebarVisible(bool isVisible);

		void setLife(float life);

		float getLife(){return _currentLife;}

		float getLifebarColorR(){return _lifebarColorR;}

		float getLifebarColorG(){return _lifebarColorG;}

		float getLifebarColorB(){return _lifebarColorB;}

	protected:

		/**
		Vida de la entidad
		*/
		float _maxLife;
		float _currentLife;
		float _visualLife;

		/*Componente roja, verde y azul de la barra de vida de valores decimales entre 0 y 1*/
		float _lifebarColorR;
		float _lifebarColorG;
		float _lifebarColorB;
		/*
		Time variable used to make the life resize animation indipendent to framerate
		*/
		int _animationTime;

		/**
		Vector that define the billboard position respect to the entity position
		*/
		Vector3 _lifebarPosition;

		Graphics::CScene *_scene;
		/**
		Billbaord used to draw the background texture of life container
		*/
		Graphics::CBillboardSetEntity *_lifebarBg;
		/**
		Billbaord used to draw the life
		*/
		Graphics::CBillboardSetEntity *_lifebarFg;

		void updateLifebarDamage();

		void updateLifebarLife();

		bool _isInvincible;

		bool _berserkerMode;

	}; // class CLife

	REG_FACTORY(CLife);

} // namespace Logic

#endif // __Logic_Life_H
