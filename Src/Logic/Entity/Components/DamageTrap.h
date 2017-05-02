/**
@file DamageTrap.h

Declaration of DamageTrap component.

@see Logic::IComponent

@author Rayco Sánchez García
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
