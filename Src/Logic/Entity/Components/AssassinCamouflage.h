/**
@file DamageTrap.h

Declaration of MudTrap component.

@see Logic::IComponent

@author Rayco S�nchez Garc�a
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
