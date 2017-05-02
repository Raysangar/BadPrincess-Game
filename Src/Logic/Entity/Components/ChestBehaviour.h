/**
@file ChestBehaviour.h

File which contains ChestBehaviour class declaration.

@see Logic::IComponent

@author Rayco Sánchez García
@date January, 2015
*/
#ifndef __Logic_ChestBehaviour_H
#define __Logic_ChestBehaviour_H

#include "Logic/Entity/Component.h"
#include "Logic/WorkmanMessages.h"

//declaración de la clase
namespace Logic 
{
	class CEntity;
	
/**
	Component that receives collected resources and update the server with all player resources.
	
    @ingroup logicGroup

	@author Rayco Sánchez García
	@date January, 2015
*/
	class CChestBehaviour : public IComponent
	{
		DEC_FACTORY(CChestBehaviour);
	public:
		/**
		This constructor initialize the component with its component name 
		and the rest attributes with their default value.
		*/
		CChestBehaviour(std::string componentName) : IComponent(componentName) {}

		/**
		Destructor (virtual); Quita de la escena y destruye la entidad gráfica.
		*/
		virtual ~CChestBehaviour();
		
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
		@author Rayco Sánchez García
		@date February, 2015

		Method called after all components are created and added
		to the entity.
		*/
		virtual void awake();

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

	private:

	}; // class ChestBehaviour

	REG_FACTORY(CChestBehaviour);

} // namespace Logic

#endif // __Logic_ChestBehaviour_H
