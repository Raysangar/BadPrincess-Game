/**
@file Particle.h

Contains the declaration of the Particle component

@see Logic::CParticle
@see Logic::IComponent

@author Rayco S�nchez Garc�a
@date March, 2015
*/
#ifndef __Logic_Particle_H
#define __Logic_Particle_H

#include "Logic/Entity/Component.h"

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
namespace Graphics 
{
	class CParticleEntity;
	class CScene;
}

//declaraci�n de la clase
namespace Logic 
{
/**
	Class that represent a particle system in the scene
	
    @ingroup logicGroup

	@author Rayco S�nchez Garc�a
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
		Destructor (virtual); Quita de la escena y destruye la entidad gr�fica.
		*/
		virtual ~CParticle();
		
		/**
		Inicializaci�n del componente, utilizando la informaci�n extra�da de
		la entidad le�da del mapa (Maps::CEntity). Toma del mapa el atributo
		model con el modelo que se deber� cargar e invoca al m�todo virtual
		createGraphicsEntity() para generar la entidad gr�fica.

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa L�gico en el que se registrar� el objeto.
		@param entityInfo Informaci�n de construcci�n del objeto le�do del
			fichero de disco.
		@return Cierto si la inicializaci�n ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		virtual bool activate();

		virtual void deactivate();

		virtual void awake();

		/**
		M�todo virtual que elige que mensajes son aceptados. Son v�lidos
		SET_TRANSFORM.

		@param message Mensaje a chequear.
		@return true si el mensaje es aceptado.
		*/
		virtual bool acceptN(const std::shared_ptr<NMessage> &message);

		/**
		M�todo virtual que procesa un mensaje.

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
		Escena gr�fica donde se encontrar�n las representaciones gr�ficas de
		las entidades. La guardamos para la destrucci�n de la entidad gr�fica.
		*/
		Graphics::CScene* _scene;

	}; // class CGraphics

	REG_FACTORY(CParticle);

} // namespace Logic

#endif // __Logic_Particle_H
