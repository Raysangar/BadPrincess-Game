/**
@file GroupBehaviour.h

Contiene la implementación del componente que controla 
el comportamiento de una unidad al moverse en grupo

@see Logic::IComponent

@author Iván León
@date Febrero, 2015
*/
#ifndef __Logic_GroupBehaviour_H
#define __Logic_GroupBehaviour_H

#include "Logic/Entity/Component.h"

namespace Ogre
{
	class Vector3;
}

//declaración de la clase
namespace Logic 
{	
/**
	Componente que se encarga de gestionar si una entidad controlable está seleccionada 
	para recibir acciones o no. En caso positivo, dibuja un círculo bajo la misma.
	
    @ingroup logicGroup

	@author Iván León
	@date Enero, 2015
*/
	class CGroupBehaviour : public IComponent
	{
		DEC_FACTORY(CGroupBehaviour);
	public:
		/**
		This constructor initialize the component with its component name 
		and the rest attributes with their default value.
		*/
		CGroupBehaviour(std::string componentName) : IComponent(componentName){}

		/**
		Destructor (virtual); Liberamos los Manual Object recién creados
		*/
		virtual ~CGroupBehaviour(){}

		
		/**
		Inicialización del componente, utilizando la información extraída de
		la entidad leída del mapa (Maps::CEntity). Toma del mapa el tamaño de la 
		cápsula que representa la entidad física y usa su radio para asignárselo
		al círculo de selección.
		Después crea un ManualObject para dibujar el círculo y lo asigna a un
		SceneNode de Ogre también recién creado.

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

		void groupMoveToPosition(Ogre::Vector3 destination);

	}; // class CGraphics

	REG_FACTORY(CGroupBehaviour);

} // namespace Logic

#endif // __Logic_GroupBehaviour_H
