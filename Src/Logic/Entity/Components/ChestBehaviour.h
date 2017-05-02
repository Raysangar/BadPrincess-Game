/**
@file ChestBehaviour.h

File which contains ChestBehaviour class declaration.

@see Logic::IComponent

@author Rayco S�nchez Garc�a
@date January, 2015
*/
#ifndef __Logic_ChestBehaviour_H
#define __Logic_ChestBehaviour_H

#include "Logic/Entity/Component.h"
#include "Logic/WorkmanMessages.h"

//declaraci�n de la clase
namespace Logic 
{
	class CEntity;
	
/**
	Component that receives collected resources and update the server with all player resources.
	
    @ingroup logicGroup

	@author Rayco S�nchez Garc�a
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
		Destructor (virtual); Quita de la escena y destruye la entidad gr�fica.
		*/
		virtual ~CChestBehaviour();
		
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

		/**
		@author Rayco S�nchez Garc�a
		@date February, 2015

		Method called after all components are created and added
		to the entity.
		*/
		virtual void awake();

		/**
		M�todo llamado en cada frame que actualiza el estado del componente.
		<p>
		Se encarga de mover la circunferencia bajo la unidad cuando esta cambia de posici�n.

		@param msecs Milisegundos transcurridos desde el �ltimo tick.
		*/
		virtual void tick(unsigned int msecs);


		/**
		M�todo virtual que elige que mensajes son aceptados. Son v�lidos
		SELECTED.

		@param message Mensaje a chequear.
		@return true si el mensaje es aceptado.
		*/
		virtual bool acceptN(const std::shared_ptr<NMessage> &message);

		/**
		M�todo virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		virtual void processN(const std::shared_ptr<NMessage> &message);

	private:

	}; // class ChestBehaviour

	REG_FACTORY(CChestBehaviour);

} // namespace Logic

#endif // __Logic_ChestBehaviour_H
