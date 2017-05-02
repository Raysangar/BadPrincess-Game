/**
@file EntityTypeExchanger.h

Contiene la declaraci�n del componente que 
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

// Predeclaraci�n de tipos
namespace physx {	
	class PxRigidDynamic;
};

namespace Physics {
	class ActorFactory;
	class ActorController;
};
//declaraci�n de la clase
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
		Destructor (virtual); Liberamos los Manual Object reci�n creados
		*/
		virtual ~CEntityTypeExchanger();
		
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

		/**
		Se invoca cuando se produce una colisi�n entre una entidad f�sica y un trigger.
		Ver descripci�n paso por paso en el cpp.
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
		Crea el actor de PhysX que representa la entidad f�sica a partir de la
		informaci�n del mapa.
		*/
		physx::PxRigidDynamic*	createTriggerDetector(const Map::CEntity *entityInfo);

		// Physic Actor Factory
		Physics::ActorFactory* _actorFactory;

		// Actor que representa la entidad f�sica en PhysX
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
