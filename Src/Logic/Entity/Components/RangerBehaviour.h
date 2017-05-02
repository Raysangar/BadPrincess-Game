/**
@file RangerBehaviour.h

Declaration of RangerBehaviour component.

@see Logic::IComponent

@author Rayco Sánchez García
@date February, 2015
*/

#ifndef __Logic_RangerBehaviour_H
#define __Logic_RangerBehaviour_H

#include "Logic/Entity/Component.h"

namespace Ogre
{
	class Vector3;
}

struct NextTrapInfo{
	CEntityPtr nextTrapGenerator;
	std::string newTrapName;
	Vector3 position;
	int trapTimeToSet;
};

namespace Logic 
{
	class CEntity;
	class CCombatStats;

	class CRangerBehaviour : public IComponent
	{
		DEC_FACTORY(CRangerBehaviour);
	public:
		/**
		This constructor initialize the component with its component name 
		and the rest attributes with their default value.
		*/
		CRangerBehaviour(std::string componentName) : IComponent(componentName), nextTrapName(""), 
			_nextTrapInfo(0), _trapNum(0), settingTrap(false), _arrowToShoot(0), _combatComponent(0)
		{
			messageHandlers["CastAbility"] = &CRangerBehaviour::castAbilityHandler;
			messageHandlers["InstantiateAbilityObject"] = &CRangerBehaviour::instantiateAbilityHandler;
			messageHandlers["SetTrap"] = &CRangerBehaviour::setTrapHandler;
			messageHandlers["TrapDestroyed"] = &CRangerBehaviour::trapDestroyedHandler;
			messageHandlers["Attacking"] = &CRangerBehaviour::shootArrow;
			messageHandlers["AvatarRouteTo"] = &CRangerBehaviour::setTrapCancelledHandler;
		}

		/**
		Destructor (virtual);
		*/
		virtual ~CRangerBehaviour();
		
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
		Method called once all enitties are loaded.
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

		const NextTrapInfo* getNextTrapInfo(){ return _nextTrapInfo; }
				
		std::string getNewTrapName();

		void trapSet(CEntityPtr trap);
		
	private:
		
		Ogre::Quaternion currentPitch;

		typedef void (CRangerBehaviour::*processMessage)(const std::shared_ptr<NMessage> &message);

		std::map<std::string,  processMessage> messageHandlers;

		std::map<std::string, std::pair<CEntityPtr, int>> availableTraps;

		std::map<std::string, CEntityPtr> helperTraps;

		// The first value is the key. The second one is the name of the trap assigned to it.
		std::map<std::string, std::string> trapsAssignedToKeys;

		std::string nextTrapName;

		void castAbilityHandler(const std::shared_ptr<NMessage> &message);

		CEntityPtr CRangerBehaviour::instantiateTrap();

		void setTrapHandler(const std::shared_ptr<NMessage> &message);

		void instantiateAbilityHandler(const std::shared_ptr<NMessage> &message);

		void trapDestroyedHandler(const std::shared_ptr<NMessage> &message);

		void shootArrow(const std::shared_ptr<NMessage> &message);

		void setTrapCancelledHandler(const std::shared_ptr<NMessage> &message);

		NextTrapInfo *_nextTrapInfo;

		bool settingTrap;

		int _trapNum;

		std::vector<Logic::CEntity*> _arrows;

		int _arrowToShoot;

		Logic::CCombatStats* _combatComponent;
	};

	REG_FACTORY(CRangerBehaviour);

} // namespace Logic

#endif // __Logic_RangerBehaviour_H
