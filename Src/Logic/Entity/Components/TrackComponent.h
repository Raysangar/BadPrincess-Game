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
		Destructor (virtual); Liberamos los Manual Object reci�n creados
		*/
		virtual ~CTrackComponent();
		
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
