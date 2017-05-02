/**
@file Visibility.h

Contiene la declaración del componente que controla la vida de una entidad.

@see Logic::CVisibility
@see Logic::IComponent

@author David Llansó
@date Octubre, 2010
*/
#ifndef __Logic_Visibility_H
#define __Logic_Visibility_H

#include "Logic/Entity/Component.h"
#include "Physics.h"

namespace Logic 
{
	class CAnimatedGraphics;

	class CLife;

	class CEntity;

	class CVisibility : public IComponent
	{

	DEC_FACTORY(CVisibility);

	public:	
		/**
		This constructor initialize the component with its component name 
		and the rest attributes with their default value.
		*/
		CVisibility(std::string componentName);

		/**
		Destructor (virtual); Liberamos los Manual Object recién creados
		*/
		virtual ~CVisibility();
		
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
		Este método se invoca en cada ciclo de la simulación y actualiza la posición del actor
		*/
		virtual void tick(unsigned int msecs);

		bool isVisible() {return (_observers > 0);}

		void sendChangeCollisionGroupMessage(bool entityIsTargetable);
		
	private:

		int _observers;

		CAnimatedGraphics* graphicsComponent;

		CLife* lifeComponent;
	}; 

	REG_FACTORY(CVisibility);

} // namespace Logic

#endif // __Logic_Visibility_H
