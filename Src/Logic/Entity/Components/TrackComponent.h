/**
@file TrackComponent.h

Implement the component that manage the sounds associated to 
a specific time for each animations

@see Logic::IComponent

@author Guidi Giacomo
@date Junio, 2015
*/

#ifndef __Logic_TrackComponent_H
#define __Logic_TrackComponent_H

#include "Logic/Entity/Component.h"


namespace Logic 
{
	class CAnimatedGraphics;

	class CTrackComponent : public IComponent
	{

	DEC_FACTORY(CTrackComponent);

	public:	
		/**
		This constructor initialize the component with its component name 
		and the rest attributes with their default value.
		*/
		CTrackComponent(std::string componentName) : IComponent(componentName),_animationTrack(nullptr),_currentTrack(nullptr),_graphics(nullptr),_luaAnimationTable("") {}

		/**
		Destructor (virtual); Liberamos los Manual Object recién creados
		*/
		virtual ~CTrackComponent();
		
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

		virtual void tick(unsigned int msecs);

		void changeAnimationTrackSoundName(std::string animationName,std::string id,std::string newSoundName);

	protected:
		/*
			Function that reed from lua all the sounds that we have to reproduce for all animations
		*/
		void initAnimationTrack();
		/*
			Function that free the memory
		*/
		void deleteAnimationTrack();

		typedef struct TrackSound{

			TrackSound(float time,std::string soundName):_time(time),_soundName(soundName),_alreadyExecuted(false){}

			float _time;
			std::string _soundName;
			bool _alreadyExecuted;
		}TrackSound;

		typedef std::pair<std::string,TrackSound> TrackSoundEvent;

		typedef std::list<TrackSoundEvent*> TrackSoundEventList;

		/*
		Map of all animation that require to play a sound
		*/
		std::map<std::string,TrackSoundEventList*> *_animationTrack;
		/*
		current list of sounds
		*/
		const TrackSoundEventList *_currentTrack;

		/*
			function that activate a list of sounds
			@param listToActivate list of sound that we have to play for the current animation
		*/
		void activateTrack(const TrackSoundEventList *listToActivate);
		/*
			function that stop all sounds for the current animation
		*/
		void deactivateTrack();
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
			
	}; 

	REG_FACTORY(CTrackComponent);
} 

#endif
