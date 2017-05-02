/**
@file AvatarController.h

Contiene la declaración del componente que controla el movimiento 
de la entidad.

@see Logic::CAvatarController
@see Logic::IComponent

@author David Llansó
@date Agosto, 2010
*/
#ifndef __Logic_AvatarController_H
#define __Logic_AvatarController_H

#include "Logic/Entity/Component.h"

//declaración de la clase
namespace Logic 
{
/**
	Este componente es el encargado de mover a una entidad animada. Tiene
	diferentes métodos que permiten avances o giros. El uso de este componente
	es un poco atípico ya que se puede registrar en otro controlador externo
	(i.e. GUI::CPlayerController) que sea el que de las órdenes que se deben
	llevar a cabo mediante llamadas a métodos públicos del componente. Puede
	no obstante ampliarse este componente para aceptar mensajes tradicionales
	con las órdenes, sin embargo de momento así es suficiente.
	
    @ingroup logicGroup

	@author David Llansó García
	@date Agosto, 2010
*/
	class CAvatarController : public IComponent
	{
		DEC_FACTORY(CAvatarController);

	public:
		/**
		This constructor initialize the component with its component name 
		and the rest attributes with their default value.
		*/
		CAvatarController(std::string componentName);
		
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
		Método virtual que elige que mensajes son aceptados. Son válidos
		CONTROL.

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
		Provoca que la entidad avance.
		*/
		void walk();

		/**
		Provoca que la entidad retroceda.
		*/
		void walkBack();

		/**
		Provoca que la entidad cese el avance.
		*/
		void stopWalk();

		/**
		Provoca que la entidad se desplace lateralmente a la izquierda.
		*/
		void strafeLeft();

		/**
		Provoca que la entidad se desplace lateralmente a la derecha.
		*/
		void strafeRight();

		/**
		Provoca que la entidad cese el desplazamiento lateral.
		*/
		void stopStrafe();
		/**
		Acción especial para depuración.
		*/
		void specialAction();
		/**
		Provoca que la entidad gire. Números Positivos para	giro a 
		derechas, negativos para giro izquierdas.

		@param amount Cantidad de giro. Positivos giro a derechas,
		negativos a izquierdas.
		*/
		void turn(float amount);

	protected:

		/**
		Atributo para saber si la entidad está avanzando.
		*/
		bool _walking;

		/**
		Atributo para saber si la entidad está retrocediendo.
		*/
		bool _walkingBack;

		/**
		Atributo para saber si la entidad está moviendose lateralmente
		a la izquierda.
		*/
		bool _strafingLeft;

		/**
		Atributo para saber si la entidad está moviendose lateralmente
		a la derecha.
		*/
		bool _strafingRight;

		/**
		Atributo que indica la magnitud de velocidad de la entidad.
		*/
		float _speed;

		/**
		Pointer to a function which implements an Avatar Action.
		*/
		typedef void (CAvatarController::*AvatarAction)();

		/**
		Map that contains all available avatar actions identified by their action type in string format.
		*/
		std::map<std::string, AvatarAction> avatarActions;

	}; // class CAvatarController

	REG_FACTORY(CAvatarController);

} // namespace Logic

#endif // __Logic_AvatarController_H
