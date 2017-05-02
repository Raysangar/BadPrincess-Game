/**
@file OscilationManager.cpp

Componente para crear una animación de oscilación en un objeto

@see Logic::IComponent

@author Iván León
@date June, 2015
*/
 
#ifndef __Logic_OscilationManager_H
#define __Logic_OscilationManager_H

#include "Logic/Entity/Component.h"

namespace Ogre{
	class Vector3;
	class AnimationState;
};

//declaración de la clase
namespace Logic 
{
/**
	Componente que recibe la textura y e frustum para dibujar l'area de efecto en suelo
	
    @ingroup logicGroup

	@author Guidi Giacomo
	@date March, 2015
*/
	class COscilationManager : public IComponent
	{
		DEC_FACTORY(COscilationManager);
	public:
		/**
		This constructor initialize the component with its component name 
		and the rest attributes with their default value.
		*/
		COscilationManager(std::string componentName) : IComponent(componentName)
		{
		}

		/**
		Destructor (virtual); Liberamos los Manual Object recién creados
		*/
		virtual ~COscilationManager();
		

		virtual void awake();

		virtual void tick(unsigned int msecs);

		/**
		Inicialización del componente, utilizando la información extraída de
		la entidad leída del mapa (Maps::CEntity). 

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa Lógico en el que se registrará el objeto.
		@param entityInfo Información de construcción del objeto leído del
			fichero de disco.
		@return Cierto si la inicialización ha sido satisfactoria.
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
		
	protected:
		//Ogre::AnimationState *animationTranslation;
		//Ogre::AnimationState *animationRotation;

		float _posYincrement;

		float maxYPos;
		float minYPos;
			
	}; // class CGraphics

	REG_FACTORY(COscilationManager);

} // namespace Logic

#endif // __Logic_Graphics_H
