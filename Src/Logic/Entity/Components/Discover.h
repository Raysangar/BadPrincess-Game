/**
@file Discover.h

Contiene la declaraci�n del componente que controla la vida de una entidad.

@see Logic::CDiscover
@see Logic::IComponent

@author David Llans�
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

// Predeclaraci�n de tipos
namespace physx {	
	class PxRigidDynamic;
};

namespace Physics {
	class ActorFactory;
	class ActorController;
};

//declaraci�n de la clase
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
		Destructor (virtual); Liberamos los Manual Object reci�n creados
		*/
		virtual ~CDiscover();
		
		/**
		Method called once all enitties are loaded.
		*/
		virtual void awake();

		/**
		Inicializaci�n del componente, utilizando la informaci�n extra�da de
		la entidad le�da del mapa (Maps::CEntity). 

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
		M�todo virtual que elige que mensajes son aceptados.

		@param message Mensaje a chequear.
		@return true si el mensaje es aceptado.
		*/
		virtual bool acceptN(const std::shared_ptr<NMessage> &message);

		
		/**
		M�todo virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		virtual void processN(const std::shared_ptr<NMessage> &message);

		/**
		Se invoca cuando se produce una colisi�n entre una entidad f�sica y un trigger.
		Ver descripci�n paso por paso en el cpp.
		*/
		virtual void onTrigger (IPhysics *otherComponent, bool enter);

		/**
		Este m�todo se invoca en cada ciclo de la simulaci�n y actualiza la posici�n del actor
		*/
		virtual void tick(unsigned int msecs);

		float getDiscoverRange() { return _discoveryRange; }
		
	private:
		
		/**
		Crea el actor de PhysX que representa la entidad f�sica a partir de la
		informaci�n del mapa.
		*/
		physx::PxRigidDynamic*	createTriggerDetector(const Map::CEntity *entityInfo);

		// Physic Actor Factory
		Physics::ActorFactory* _actorFactory;

		// Actor que representa la entidad f�sica en PhysX
		physx::PxRigidDynamic* _triggerDetector;

		// Physic Actor Controller
		Physics::ActorController* _actorController;

		// Entity types that triggers the detector
		std::list<std::string> targetEntityTypes;

		// Radius of discovery sphere
		float _discoveryRange;

		// Entidad que es descubierta.
		CEntity* _discoveredEntity;

		// Variable que controla si la FOW est� activada.

		bool _enableFOW;

	}; 

	REG_FACTORY(CDiscover);

} // namespace Logic

#endif // __Logic_Discover_H
