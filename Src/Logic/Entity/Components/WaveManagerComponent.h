/**
@file WaveManagerComponent.h

Contiene la declaración del componente que controla el movimiento 
de la entidad enemiga.

@see Logic::CWaveManagerComponent
@see Logic::IComponent

@author Guidi Giacomo
@date March, 2015
*/
#ifndef __Logic_WaveManagerComponent_H
#define __Logic_WaveManagerComponent_H

#include "Logic/Entity/Component.h"

//declaración de la clase
namespace Logic 
{
/**
	Este componente es el encargado de inicializar el waveManager pasandole el fichero de lua por la carga y
	es encargado de ejecutar el tick del WaveManager y pasar al WaveManager los mensaje
	
    @ingroup logicGroup

	@author Guidi Giacomo
	@date March, 2015
*/
	class CWaveManagerComponent : public IComponent
	{
		DEC_FACTORY(CWaveManagerComponent);

	public:
		/**
		This constructor initialize the component with its component name 
		and the rest attributes with their default value.
		*/
		CWaveManagerComponent(std::string componentName);
		
		/**
		Inicialización del componente, utilizando la información extraída de
		la entidad leída del mapa (Maps::CEntity). Toma del mapa el atributo
		speed que indica a la velocidad a la que se moverá el jugador.

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
		Método que activa el componente; invocado cuando se activa
		el mapa donde está la entidad a la que pertenece el componente.
		<p>
		Si el componente pertenece a la entidad del jugador, el componente
		se registra así mismo en el controlador del GUI para que las ordenes 
		se reciban a partir de los eventos de teclado y ratón.

		@return true si todo ha ido correctamente.
		*/
		virtual bool activate();
		
		/**
		Método que desactiva el componente; invocado cuando se
		desactiva el mapa donde está la entidad a la que pertenece el
		componente. Se invocará siempre, independientemente de si estamos
		activados o no.
		<p>
		Si el componente pertenece a la entidad del jugador, el componente
		se deregistra así mismo en el controlador del GUI para dejar de
		recibir las ordenes dadas a partir de los eventos de teclado y ratón.
		*/
		virtual void deactivate();

		/**
		Método llamado en cada frame que actualiza el estado del componente.
		<p>
		Se encarga de mover la entidad en cada vuelta de ciclo cuando es
		necesario (cuando está andando o desplazándose lateralmente).

		@param msecs Milisegundos transcurridos desde el último tick.
		*/
		virtual void tick(unsigned int msecs);

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

	protected:


	
	}; // class CWaveManagerComponent

	REG_FACTORY(CWaveManagerComponent);

} // namespace Logic

#endif // __Logic_WaveManagerComponent_H
