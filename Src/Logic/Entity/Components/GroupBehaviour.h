/**
@file GroupBehaviour.h

Contiene la implementaci�n del componente que controla 
el comportamiento de una unidad al moverse en grupo

@see Logic::IComponent

@author Iv�n Le�n
@date Febrero, 2015
*/
#ifndef __Logic_GroupBehaviour_H
#define __Logic_GroupBehaviour_H

#include "Logic/Entity/Component.h"

namespace Ogre
{
	class Vector3;
}

//declaraci�n de la clase
namespace Logic 
{	
/**
	Componente que se encarga de gestionar si una entidad controlable est� seleccionada 
	para recibir acciones o no. En caso positivo, dibuja un c�rculo bajo la misma.
	
    @ingroup logicGroup

	@author Iv�n Le�n
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
		Destructor (virtual); Liberamos los Manual Object reci�n creados
		*/
		virtual ~CGroupBehaviour(){}

		
		/**
		Inicializaci�n del componente, utilizando la informaci�n extra�da de
		la entidad le�da del mapa (Maps::CEntity). Toma del mapa el tama�o de la 
		c�psula que representa la entidad f�sica y usa su radio para asign�rselo
		al c�rculo de selecci�n.
		Despu�s crea un ManualObject para dibujar el c�rculo y lo asigna a un
		SceneNode de Ogre tambi�n reci�n creado.

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

		void groupMoveToPosition(Ogre::Vector3 destination);

	}; // class CGraphics

	REG_FACTORY(CGroupBehaviour);

} // namespace Logic

#endif // __Logic_GroupBehaviour_H
