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

// Predeclaraci�n de tipos
namespace physx {	
	class PxRigidDynamic;
};

namespace Physics {
	class ActorFactory;
	//class ActorController;
};

//declaraci�n de la clase
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
		Destructor (virtual); Liberamos los Manual Object reci�n creados
		*/
		virtual ~CKitchenHeal();
		
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
		M�todo que se invoca en cada ciclo.
		*/
		virtual void tick(unsigned int msecs);

		/**
		M�todo que devuelve las dimensiones de la zona de curaci�n.
		*/
		Vector3 getHealZone() { return _healZone; }
		
	private:
		
		/**
		Crea el actor de PhysX que representa la entidad f�sica a partir de la
		informaci�n del mapa.
		*/
		physx::PxRigidDynamic*	createTriggerDetector(const Map::CEntity *entityInfo);

		// Physic Actor Factory.
		Physics::ActorFactory* _actorFactory;

		// Actor que representa la entidad f�sica en PhysX.
		physx::PxRigidDynamic* _triggerDetector;


		// Entity types that triggers the detector.
		std::list<std::string> targetEntityTypes;

		// Dimensiones de la zona de curaci�n.
		Vector3 _healZone;

		// Lista de entidades que deben ser curadas.
		std::list<CEntity*> _healList;

		Logic::CBuildComponent * _buildComponent;

	}; 

	REG_FACTORY(CKitchenHeal);

} // namespace Logic

#endif // __Logic_KitchenHeal_H
