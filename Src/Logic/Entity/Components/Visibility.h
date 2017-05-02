/**
@file Visibility.h

Contiene la declaraci�n del componente que controla la vida de una entidad.

@see Logic::CVisibility
@see Logic::IComponent

@author David Llans�
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
		Destructor (virtual); Liberamos los Manual Object reci�n creados
		*/
		virtual ~CVisibility();
		
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
		Este m�todo se invoca en cada ciclo de la simulaci�n y actualiza la posici�n del actor
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
