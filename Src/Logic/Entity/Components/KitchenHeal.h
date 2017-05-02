/**
	This component is relative to the object that recover life,
	it has a trigger and when it's builded, send a message to all the entities into the trigger
	that they entered or exited	to the recover area
*/

#ifndef __Logic_KitchenHeal_H
#define __Logic_KitchenHeal_H

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
	//class ActorController;
};

//declaración de la clase
namespace Logic 
{
	class CEntity;
	class CBuildComponent;

	class CKitchenHeal : public IPhysics
	{

	DEC_FACTORY(CKitchenHeal);

	public:	
		/**
		This constructor initialize the component with its component name 
		and the rest attributes with their default value.
		*/
		CKitchenHeal(std::string componentName);

		/**
		Destructor (virtual); Liberamos los Manual Object recién creados
		*/
		virtual ~CKitchenHeal();
		
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
		Método que se invoca en cada ciclo.
		*/
		virtual void tick(unsigned int msecs);

		/**
		Método que devuelve las dimensiones de la zona de curación.
		*/
		Vector3 getHealZone() { return _healZone; }
		
	private:
		
		/**
		Crea el actor de PhysX que representa la entidad física a partir de la
		información del mapa.
		*/
		physx::PxRigidDynamic*	createTriggerDetector(const Map::CEntity *entityInfo);

		// Physic Actor Factory.
		Physics::ActorFactory* _actorFactory;

		// Actor que representa la entidad física en PhysX.
		physx::PxRigidDynamic* _triggerDetector;


		// Entity types that triggers the detector.
		std::list<std::string> targetEntityTypes;

		// Dimensiones de la zona de curación.
		Vector3 _healZone;

		// Lista de entidades que deben ser curadas.
		std::list<CEntity*> _healList;

		Logic::CBuildComponent * _buildComponent;

	}; 

	REG_FACTORY(CKitchenHeal);

} // namespace Logic

#endif // __Logic_KitchenHeal_H
