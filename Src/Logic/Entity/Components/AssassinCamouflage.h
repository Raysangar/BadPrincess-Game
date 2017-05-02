/**
@file DamageTrap.h

Declaration of MudTrap component.

@see Logic::IComponent

@author Rayco Sánchez García
@date April, 2015
*/

#ifndef __Logic_AssassinCamouflage_H
#define __Logic_AssassinCamouflage_H

#include "Logic/Entity/Component.h"

namespace Physics 
{
	class CServer;
}

namespace Logic 
{
	class CAnimatedGraphics;

	class CVisibility;

	class CLife;

	class CAssassinCamouflage : public IComponent
	{
		DEC_FACTORY(CAssassinCamouflage);
	public:
		/**
		This constructor initialize the component with its component name 
		and the rest attributes with their default value.
		*/
		CAssassinCamouflage(std::string componentName) : IComponent(componentName), timeTillAppears(3), timeTillDisappears(3), isMuddy(false),
			timeSinceLastChange(0), isCamouflaged(false), visibilityComponent(nullptr), isAttacking(false),
			timeTillDisappearsAfterReceiveDamege(6), timeSinceLastReceivedAttack(0), attacked(false){}

		/**
		Destructor (virtual);
		*/
		virtual ~CAssassinCamouflage();
		
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

		bool assassinIsAttackin() { return isAttacking; }

		void setAssassinIsAttackin(bool attacking) { isAttacking = attacking; }

	private:
		void setVisibility(bool isVisible);

		void disappear();

		unsigned int timeTillDisappears;

		unsigned int timeTillAppears;

		unsigned int timeTillDisappearsAfterReceiveDamege;

		bool isMuddy;

		bool isAttacking;

		bool isCamouflaged;

		bool attacked;

		unsigned int timeSinceLastReceivedAttack;

		unsigned int timeSinceLastChange;


		CAnimatedGraphics* animatedGraphics;
		CVisibility* visibilityComponent;
		CLife* lifeComponent;

		Physics::CServer* physicServer;
	};

	REG_FACTORY(CAssassinCamouflage);

} // namespace Logic

#endif // __Logic_DamageTrap_H
