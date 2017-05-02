#ifndef __Logic_ChestTrap_H
#define __Logic_ChestTrap_H

#include "Logic/Entity/Component.h"

namespace Logic 
{
	struct SmartPosition
	{
		SmartPosition(Vector3 pos,bool active)
		{
			_position=pos;
			_isActive=active;
		}
		Vector3 _position;
		bool _isActive;
	};

	class CChestTrap : public IComponent
	{
		DEC_FACTORY(CChestTrap);

	public:
		/**
		This constructor initialize the component with its component name 
		and the rest attributes with their default value.
		*/
		CChestTrap(std::string componentName) : IComponent(componentName), collectAnimationName(""),_isAvailable(true),_isEnabled(true),
			actionType(0), smartPositions(std::list<std::pair<SmartPosition, CEntityPtr>>())
		{
			acceptedMessages = std::map<std::string, MessageHandler>();
			acceptedMessages["SmartDataPositionRequest"] = &CChestTrap::handleSmartPositionRequestMessage;
			acceptedMessages["SmartDataFreePosition"] = &CChestTrap::handleSmartPositionReleaseMessage;
			acceptedMessages["SmartDataAvatarReady"] = &CChestTrap::handleAvatarReadyMessage;
			acceptedMessages["ChangeSmartObjectState"] = &CChestTrap::handleEnableMessage;
			acceptedMessages["EntityDestroying"] = &CChestTrap::handleEntityDestoyedMessage;
			acceptedMessages["SetOwner"] = &CChestTrap::handleSetOwnerMessage;
		}

		/**
		Destructor (virtual);
		*/
		virtual ~CChestTrap();
		
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
		@author Rayco Sánchez García
		@date February, 2015

		Method called after all components are created and added
		to the entity.
		*/
		virtual void awake();


		virtual bool activate();

		virtual void deactivate();

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

		const std::list<std::pair<SmartPosition, CEntityPtr>> *getSmartDataList(){return &smartPositions;};

		bool isAccesible(){return _isAvailable && _isEnabled;}

	private:
		/**
		Estructura de handlers para funciones que ejecutan acciones específicas para un mensaje en concreto
		*/
		typedef void (CChestTrap::*MessageHandler)(const std::shared_ptr<NMessage> &message);

		std::string collectAnimationName;
		int actionType;

		std::list<std::pair<SmartPosition, CEntityPtr>> smartPositions;

		std::map<std::string, MessageHandler> acceptedMessages;

		std::pair<SmartPosition, CEntityPtr>* CChestTrap::getNearestSmartPosition(const Vector3 &entityPosition, 
			const std::list<std::pair<SmartPosition, CEntityPtr>*> &freePositions);

		void CChestTrap::handleSmartPositionRequestMessage(const std::shared_ptr<NMessage> &message);
		void CChestTrap::handleSmartPositionReleaseMessage(const std::shared_ptr<NMessage> &message);
		void CChestTrap::handleAvatarReadyMessage(const std::shared_ptr<NMessage> &message);
		void CChestTrap::handleEnableMessage(const std::shared_ptr<NMessage> &message);
		void CChestTrap::handleEntityDestoyedMessage(const std::shared_ptr<NMessage> &message);
		void CChestTrap::handleSetOwnerMessage(const std::shared_ptr<NMessage> &message);

		void getSmartPositions(const Map::CEntity *entityInfo);

		/*variable that means if the smartObject is forced to be disabled with message*/
		bool _isEnabled;

		/*variable that means if the smartObject hasn't smartPosition available*/
		bool _isAvailable;

		void checkSmartPositionAvailability(CEntityPtr entityToAvoid);

		void sendDisabledMessage(CEntityPtr recipient);

		
		// The unit which set the trap
		CEntity* trapOwner;

		bool activated;
		int timeToDestroy;
	};

	REG_FACTORY(CChestTrap);

} // namespace Logic

#endif // __Logic_TrapChest_H
