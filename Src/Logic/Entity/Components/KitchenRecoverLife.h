/**
	This component belong to all entities that could be recover them life
	when enter into a recover area.

*/

#ifndef __Logic_KitchenRecoverLife_H
#define __Logic_KitchenRecoverLife_H

#include "Logic/Entity/Component.h"

//declaración de la clase
namespace Logic 
{
	class CEntity;

	class CKitchenRecoverLife : public IComponent
	{

	DEC_FACTORY(CKitchenRecoverLife);

	public:	
		/**
		This constructor initialize the component with its component name 
		and the rest attributes with their default value.
		*/
		CKitchenRecoverLife(std::string componentName);

		/**
		Destructor (virtual); Liberamos los Manual Object recién creados
		*/
		virtual ~CKitchenRecoverLife();
		
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
		Método que se invoca en cada ciclo.
		*/
		virtual void tick(unsigned int msecs);
		
	private:
		//indica que estamos dendro de l'area de la cocina
		bool _recoveryStarted;
		// Cantidad de vida recuperada por cada tick.
		float _healQuantity;

		// Tiempo entre cada envio de vida recuperada
		unsigned int _healMaxTime;
		// Tiempo actual de recuperacion de vida
		unsigned int _healCurrentTime;
		// Heal particle effect name
		std::string _healParticleName;
	}; 

	REG_FACTORY(CKitchenRecoverLife);

} // namespace Logic

#endif // __Logic_KitchenRecoverLife_H
