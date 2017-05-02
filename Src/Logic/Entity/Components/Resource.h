/**
@file Resource.h

Class which contains resource information and interaction messages for recollection

@see Logic::IComponent

@author Rayco Sánchez García
@date January, 2015
*/

#ifndef __Logic_Resource_H
#define __Logic_Resource_H

#include "Logic/Entity/Component.h"




namespace Logic 
{
	class CResource : public IComponent
	{
		DEC_FACTORY(CResource);
	public:
		/**
		This constructor initialize the component with its component name 
		and the rest attributes with their default value.
		*/
		CResource(std::string componentName) : IComponent(componentName), maxResourceQuantity(0), currentResourceQuantity(0), nextItemName("") 
		{
			acceptedMessages = std::map<std::string, MessageHandler>();
			acceptedMessages["GatherMessage"] = &CResource::handleGatherMessage;
		}

		/**
		Destructor (virtual);
		*/
		virtual ~CResource();
		
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

		/**
		Método llamado en cada frame que actualiza el estado del componente.
		<p>
		Se encarga de mover la circunferencia bajo la unidad cuando esta cambia de posición.

		@param msecs Milisegundos transcurridos desde el último tick.
		*/
		virtual void tick(unsigned int msecs);

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

		virtual void awake();
	private:

		/**
		Pointer to method that manage a specific messages
		*/
		typedef void (CResource::*MessageHandler)(const std::shared_ptr<NMessage> &message);
		
		/**
		Maximun resource quantity available on the resource entity
		*/
		int maxResourceQuantity;

		/**
		Current resource quantity available on the resource entity
		*/
		int currentResourceQuantity;

		/**
		Name to a resource entity near to these.
		*/
		std::string nextItemName;

		/**
		Map with all accepted messages and their function handler.
		*/
		std::map<std::string, MessageHandler> acceptedMessages;

		/**
		Function that is invoked when a GatherMessage is received.

		@param Message received.
		*/
		void handleGatherMessage(const std::shared_ptr<NMessage> &message);
		/*
		Method called when the currentResourceQuantity is lower or equal to 0
		<p>
		It get all the entities that occupy the smartObject and send a message to all of that entities that it dies
		Before it sends a message, it check if there is an entity linked to itself and it pass this entity into the message
		*/
		/**
		Function called when resources are run out.
		*/
		void resourceEnded();
		/*
		Method that cycle the smartDataList and send the message to all entities that occupy the smartDataPosition
		@param smartDataList list of smartDataPosition and the mathcing entity that occupy that position.
		@param message Message that we have to send.
		*/

		void sendMessageToAll(const std::list<std::pair<Vector3, CEntityPtr>> * smartDataList,const std::shared_ptr<NMessage> &message);

		/*
		*/
		
		std::list<CEntityPtr> workmanList;

		/*
		*/
		const std::list<CEntityPtr> *getWorkmanList(){return &workmanList;}
	};

	REG_FACTORY(CResource);

} // namespace Logic

#endif // __Logic_Resource_H
