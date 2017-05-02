/**
@file GameManagerComponent.h

Contiene la declaraci�n del componente que controla el movimiento 
de la entidad enemiga.

@see Logic::CGameManagerComponent
@see Logic::IComponent

@author Guidi Giacomo
@date March, 2015
*/
#ifndef __Logic_GameManagerComponent_H
#define __Logic_GameManagerComponent_H

#include "Logic/Entity/Component.h"

//declaraci�n de la clase
namespace Logic 
{
/**
	Este componente es el encargado de inicializar el waveManager pasandole el fichero de lua por la carga y
	es encargado de ejecutar el tick del WaveManager y pasar al WaveManager los mensaje
	
    @ingroup logicGroup

	@author Guidi Giacomo
	@date March, 2015
*/
	class CGameManagerComponent : public IComponent
	{
		DEC_FACTORY(CGameManagerComponent);

	public:
		/**
		This constructor initialize the component with its component name 
		and the rest attributes with their default value.
		*/
		CGameManagerComponent(std::string componentName);
		
		/**
		Inicializaci�n del componente, utilizando la informaci�n extra�da de
		la entidad le�da del mapa (Maps::CEntity). Toma del mapa el atributo
		speed que indica a la velocidad a la que se mover� el jugador.

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
		M�todo que activa el componente; invocado cuando se activa
		el mapa donde est� la entidad a la que pertenece el componente.
		<p>
		Si el componente pertenece a la entidad del jugador, el componente
		se registra as� mismo en el controlador del GUI para que las ordenes 
		se reciban a partir de los eventos de teclado y rat�n.

		@return true si todo ha ido correctamente.
		*/

		virtual void tick(unsigned int msecs);

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
	private:
		/**
		Percentages of the world, that not entry into the camera
		*/
		float _topLimit;
		float _bottomLimit;
		float _rightLimit;
		float _leftLimit;
	
	}; // class CGameManagerComponent

	REG_FACTORY(CGameManagerComponent);

} // namespace Logic

#endif // __Logic_GameManagerComponent_H
