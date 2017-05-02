/**
@file DamageTrap.h

Declaration of DamageTrap component.

@see Logic::IComponent

@author Rayco S�nchez Garc�a
@date February, 2015
*/

#ifndef __Logic_DamageTrap_H
#define __Logic_DamageTrap_H

#include "Logic/Entity/Component.h"




namespace Logic 
{
	class CDamageTrap : public IComponent
	{
		DEC_FACTORY(CDamageTrap);
	public:
		/**
		This constructor initialize the component with its component name 
		and the rest attributes with their default value.
		*/
		CDamageTrap(std::string componentName) : IComponent(componentName), trapOwner(nullptr), activated(false), timeToDestroy(500)
		{}

		/**
		Destructor (virtual);
		*/
		virtual ~CDamageTrap();
		
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

		virtual void awake();

	private:
		// The unit which set the trap
		CEntity* trapOwner;

		// Damage dealt to enemies if trap is activated
		int trapDamage;

		bool activated;
		int timeToDestroy;
		float radiusOfEffect;
	};

	REG_FACTORY(CDamageTrap);

} // namespace Logic

#endif // __Logic_DamageTrap_H
