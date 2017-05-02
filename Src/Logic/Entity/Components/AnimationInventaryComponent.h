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
		Destructor (virtual); Liberamos los Manual Object reci�n creados
		*/
		virtual ~CAnimationInventaryComponent();
		
		virtual void awake();
		/**
		Inicializaci�n del componente, utilizando la informaci�n extra�da de
		la entidad le�da del mapa (Maps::CEntity). Toma del mapa el tama�o de la 
		c�psula que representa la entidad f�sica y usa su radio para asign�rselo
		al c�rculo de selecci�n.
		Despu�s crea un ManualObject para dibujar el c�rculo y lo asigna a un
		SceneNode de Ogre tambi�n reci�n creado.

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa L�gico en el que se registrar� el objeto.
		@param entityInfo Informaci�n de construcci�n del objeto le�do del
			fichero de disco.
		@return Cierto si la inicializaci�n ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		virtual bool activate();

		virtual void deactivate();
		/**
		M�todo virtual que elige que mensajes son aceptados. Son v�lidos
		EnemySelected.

		@param message Mensaje a chequear.
		@return true si el mensaje es aceptado.
		*/
		virtual bool acceptN(const std::shared_ptr<NMessage> &message);

		
		/**
		M�todo virtual que procesa un mensaje.

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
