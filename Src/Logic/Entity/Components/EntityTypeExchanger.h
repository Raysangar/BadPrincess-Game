/**
@file EntityTypeExchanger.h

Contiene la declaración del componente que 
ontrola el cambio de entidades

@see Logic::CEntityTypeExchanger
@see Logic::IComponent

@author Guidi Giacomo
@date June, 2015
*/
#ifndef __Logic_EntityTypeExchanger_H
#define __Logic_EntityTypeExchanger_H

#include "Logic/Entity/Component.h"
#include "Physics.h"

// Predeclaración de tipos
namespace physx {	
	class PxRigidDynamic;
};

namespace Physics {
	class ActorFactory;
	class ActorController;
};
//declaración de la clase
namespace Logic 
{
	class CEntity;
	class CGUIManager;

	class CEntityTypeExchanger : public IPhysics
	{

	DEC_FACTORY(CEntityTypeExchanger);

	public:	
		/**
		This constructor initialize the component with its component name 
		and the rest attributes with their default value.
		*/
		CEntityTypeExchanger(std::string componentName);

		/**
		Destructor (virtual); Liberamos los Manual Object recién creados
		*/
		virtual ~CEntityTypeExchanger();
		
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

		/**
		Se invoca cuando se produce una colisión entre una entidad física y un trigger.
		Ver descripción paso por paso en el cpp.
		*/
		virtual void onTrigger (IPhysics *otherComponent, bool enter);

		bool isEntityTypeExchangerActivate(){return _changeTypeActivated;}

		float getEntityCostPercent(){return _entityCostPercents;}
		/*
		Function that find if the entity passed it's already into trigger
		@param decisionEntity entity to find
		@return true if the entity it's already into trigger
		*/
		bool CEntityTypeExchanger::isEntityInTrigger(Logic::CEntity* decisionEntity);

		void startExchangerBehaviour(CEntityPtr exchangeEntity,float life,Vector3 pos);

		void endExchangerBehaviour();

		void moveSelectionToChangedEntity(CEntityPtr entity);
	private:
			/**
		Crea el actor de PhysX que representa la entidad física a partir de la
		información del mapa.
		*/
		physx::PxRigidDynamic*	createTriggerDetector(const Map::CEntity *entityInfo);

		// Physic Actor Factory
		Physics::ActorFactory* _actorFactory;

		// Actor que representa la entidad física en PhysX
		physx::PxRigidDynamic* _triggerDetector;

		// Physic Actor Controller
		Physics::ActorController* _actorController;

		//list of entities that are inside the trigger
		//it used when the user send an avatarMoveToEntity message to a well closest the well
		std::list<CEntity*> _triggerList;

		// Radius of ChangeEntityTypeStatey sphere
		float _exchangerRange;

		Logic::CEntity* _entityToChange;
		Vector3 _entityToChangePos;
		float _entityToChangeLife;
		std::string _particleName;

		bool _changeTypeActivated;
		CGUIManager *_guiManagerComponent;

		std::string _abilityQName;
		std::string _abilityQTooltip;
		std::string _abilityQluaField;

		std::string _exchangerAbilityQName;
		std::string _exchangerAbilityQTooltip;
		std::string _exchangerAbilityQcost;

		std::string _abilityWName;
		std::string _abilityWTooltip;
		std::string _abilityWluaField;

		std::string _exchangerAbilityWName;
		std::string _exchangerAbilityWTooltip;
		std::string _exchangerAbilityWcost;

		std::string _abilityEName;
		std::string _abilityETooltip;
		std::string _abilityEluaField;

		std::string _exchangerAbilityEName;
		std::string _exchangerAbilityETooltip;
		std::string _exchangerAbilityEcost;

		std::string _abilityRName;
		std::string _abilityRTooltip;
		std::string _abilityRluaField;

		std::string _exchangerAbilityRName;
		std::string _exchangerAbilityRTooltip;
		std::string _exchangerAbilityRcost;

		void setNormalGUI();

		void setExchangerGUI();

		float _entityCostPercents;
	}; 

	REG_FACTORY(CEntityTypeExchanger);

} // namespace Logic

#endif // __Logic_EntityTypeExchanger_H
