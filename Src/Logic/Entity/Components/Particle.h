/**
@file Particle.h

Contains the declaration of the Particle component

@see Logic::CParticle
@see Logic::IComponent

@author Rayco Sánchez García
@date March, 2015
*/
#ifndef __Logic_Particle_H
#define __Logic_Particle_H

#include "Logic/Entity/Component.h"

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Graphics 
{
	class CParticleEntity;
	class CScene;
}

//declaración de la clase
namespace Logic 
{
/**
	Class that represent a particle system in the scene
	
    @ingroup logicGroup

	@author Rayco Sánchez García
	@date March, 2015
*/
	class CParticle : public IComponent
	{
		DEC_FACTORY(CParticle);
	public:
		/**
		This constructor initialize the component with its component name 
		and the rest attributes with their default value.
		*/
		CParticle(std::string componentName) : IComponent(componentName), _scene(nullptr), defaultParticleSystem("") {}

		/**
		Destructor (virtual); Quita de la escena y destruye la entidad gráfica.
		*/
		virtual ~CParticle();
		
		/**
		Inicialización del componente, utilizando la información extraída de
		la entidad leída del mapa (Maps::CEntity). Toma del mapa el atributo
		model con el modelo que se deberá cargar e invoca al método virtual
		createGraphicsEntity() para generar la entidad gráfica.

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa Lógico en el que se registrará el objeto.
		@param entityInfo Información de construcción del objeto leído del
			fichero de disco.
		@return Cierto si la inicialización ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		virtual bool activate();

		virtual void deactivate();

		virtual void awake();

		/**
		Método virtual que elige que mensajes son aceptados. Son válidos
		SET_TRANSFORM.

		@param message Mensaje a chequear.
		@return true si el mensaje es aceptado.
		*/
		virtual bool acceptN(const std::shared_ptr<NMessage> &message);

		/**
		Método virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		virtual void processN(const std::shared_ptr<NMessage> &message);

		Graphics::CParticleEntity* getParticleEntity(std::string particleSystemName){return _particleEntities[particleSystemName];};
	protected:

		void createParticleEntities(const Map::CEntity *entityInfo);

		
		std::map<std::string, Graphics::CParticleEntity*> _particleEntities;

		std::string defaultParticleSystem;

		std::map <std::string, float> _scales;

		/**
		Escena gráfica donde se encontrarán las representaciones gráficas de
		las entidades. La guardamos para la destrucción de la entidad gráfica.
		*/
		Graphics::CScene* _scene;

	}; // class CGraphics

	REG_FACTORY(CParticle);

} // namespace Logic

#endif // __Logic_Particle_H
