/**
@file Discover.h

Contiene la declaración del componente que controla la vida de una entidad.

@see Logic::CDiscover
@see Logic::IComponent

@author David Llansó
@date Octubre, 2010
*/
#ifndef __Logic_Discover_H
#define __Logic_Discover_H

#include "Logic/Entity/Component.h"
#include "Physics.h"

namespace Ogre
{
	class Vector3;
};

// Predeclaración de tipos
namespace physx {	
	class PxRigidDynamic;
};

namespace Physics {
	class ActorFactory;
	class ActorController;
};

//declaración de la clase
namespace Logic 
{
	class CEntity;

	class CDiscover : public IPhysics
	{

	DEC_FACTORY(CDiscover);

	public:	
		/**
		This constructor initialize the component with its component name 
		and the rest attributes with their default value.
		*/
		CDiscover(std::string componentName);

		/**
		Destructor (virtual); Liberamos los Manual Object recién creados
		*/
		virtual ~CDiscover();
		
		/**
		Method called once all enitties are loaded.
		*/
		virtual void awake();

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

		virtual bool activate();

		virtual void deactivate();

		/**
		Método virtual que elige que mensajes son aceptados.

		@param message Mensaje a chequear.
		@return true si el mensaje es aceptado.
		*/
		virtual bool acceptN(const std::shared_ptr<NMessage> &message);

		
		/**
		Método virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		virtual void processN(const std::shared_ptr<NMessage> &message);

		/**
		Se invoca cuando se produce una colisión entre una entidad física y un trigger.
		Ver descripción paso por paso en el cpp.
		*/
		virtual void onTrigger (IPhysics *otherComponent, bool enter);

		/**
		Este método se invoca en cada ciclo de la simulación y actualiza la posición del actor
		*/
		virtual void tick(unsigned int msecs);

		float getDiscoverRange() { return _discoveryRange; }
		
	private:
		
		/**
		Crea el actor de PhysX que representa la entidad física a partir de la
		información del mapa.
		*/
		physx::PxRigidDynamic*	createTriggerDetector(const Map::CEntity *entityInfo);

		// Physic Actor Factory
		Physics::ActorFactory* _actorFactory;

		// Actor que representa la entidad física en PhysX
		physx::PxRigidDynamic* _triggerDetector;

		// Physic Actor Controller
		Physics::ActorController* _actorController;

		// Entity types that triggers the detector
		std::list<std::string> targetEntityTypes;

		// Radius of discovery sphere
		float _discoveryRange;

		// Entidad que es descubierta.
		CEntity* _discoveredEntity;

		// Variable que controla si la FOW está activada.

		bool _enableFOW;

	}; 

	REG_FACTORY(CDiscover);

} // namespace Logic

#endif // __Logic_Discover_H
