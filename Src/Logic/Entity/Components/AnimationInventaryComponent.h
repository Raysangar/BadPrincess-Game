/**
@file AnimationInventaryComponent.h

Implement the component that manage all items that we have to attach to the skeleton
for each animations

@see Logic::IComponent

@author Guidi Giacomo
@date Mayo, 2015
*/

#ifndef __Logic_AnimationInventaryComponent_H
#define __Logic_AnimationInventaryComponent_H

#include "Logic/Entity/Component.h"


namespace Graphics{
	class CMeshEntity;
	class CScene;
}

namespace Logic 
{
	class CAnimatedGraphics;

	class CAnimationInventaryComponent : public IComponent
	{

	DEC_FACTORY(CAnimationInventaryComponent);

	public:	
		/**
		This constructor initialize the component with its component name 
		and the rest attributes with their default value.
		*/
		CAnimationInventaryComponent(std::string componentName) : IComponent(componentName),_animationInventary(nullptr),_currentInventary(nullptr),_graphics(nullptr),_scene(nullptr),_luaAnimationTable("") {}

		/**
		Destructor (virtual); Liberamos los Manual Object recién creados
		*/
		virtual ~CAnimationInventaryComponent();
		
		virtual void awake();
		/**
		Inicialización del componente, utilizando la información extraída de
		la entidad leída del mapa (Maps::CEntity). Toma del mapa el tamaño de la 
		cápsula que representa la entidad física y usa su radio para asignárselo
		al círculo de selección.
		Después crea un ManualObject para dibujar el círculo y lo asigna a un
		SceneNode de Ogre también recién creado.

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa Lógico en el que se registrará el objeto.
		@param entityInfo Información de construcción del objeto leído del
			fichero de disco.
		@return Cierto si la inicialización ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		virtual bool activate();

		virtual void deactivate();
		/**
		Método virtual que elige que mensajes son aceptados. Son válidos
		EnemySelected.

		@param message Mensaje a chequear.
		@return true si el mensaje es aceptado.
		*/
		virtual bool acceptN(const std::shared_ptr<NMessage> &message);

		
		/**
		Método virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		virtual void processN(const std::shared_ptr<NMessage> &message);


	protected:
		/*
			Function that reed from lua all mesh object that we have to attach to all animation
		*/
		void initAnimationInventary();
		/*
			Function that free the memory
		*/
		void deleteAnimationInventary();

		typedef std::pair<std::string,Graphics::CMeshEntity*> AttachableObject;

		typedef std::list<AttachableObject*> AttachableObjectList;

		/*
		Map of all animation that require to attach one or more object
		*/
		std::map<std::string,AttachableObjectList*> *_animationInventary;
		/*
		current list of attached objects
		*/
		const AttachableObjectList *_currentInventary;

		/*
			function that attach all the object to respective bone
			@param listToActivate list of objects that we have to attach to a bone
		*/
		void attachInventary(const AttachableObjectList *listToActivate);
		/*
			function that detach all the current objects from respective bone
		*/
		void detachCurrentInventary();
		/*
			function that find for all animation that we are setting  all objects that we have to attach to the skeleton
			@param name name of animation
		*/
		void changeAnimation(std::string name);

		/*
		Name of lua table that contains all animation with each objects to attach
		*/
		std::string _luaAnimationTable;

		Logic::CAnimatedGraphics* _graphics;

		Graphics::CScene*_scene;
			
	}; 

	REG_FACTORY(CAnimationInventaryComponent);
} 

#endif
