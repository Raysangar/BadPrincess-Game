/**
@file WorkmanState.h

*/

#ifndef __Logic_WorkmanState_H
#define __Logic_WorkmanState_H

#include "Logic/Entity/Component.h"
#include "AI/ResourceServer.h"

namespace Logic 
{
	class CEntity;
	
/**
	Componente que se encarga...
*/
	
	class CWorkmanState : public IComponent
	{

	DEC_FACTORY(CWorkmanState);

	public:	

		enum WorkmanAction
		{
			none,
			gatherWood,
			gatherIron,
			gatherStone,
			storeResource,
			gatherFull,
		};

		CWorkmanState(std::string componentName) : IComponent(componentName), resType(AI::ResourceType::none), currentGatherQuantity(0), deltaTime(0), buildingEntity(nullptr), lastResource(nullptr), action(none),buildAbilityChar(""),repairBuildingAbilityChar(""),repairBuildingAbilityActive(false){}

		virtual ~CWorkmanState();
		
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		virtual void awake();

		virtual void tick(unsigned int msecs);

		virtual bool acceptN(const std::shared_ptr<NMessage> &message);

		virtual void processN(const std::shared_ptr<NMessage> &message);

		// -- smartEntity methods --

		/**
		Método que asigna una smartEntity al estado del obrero.

		@param entity Entidad que se debe asignar. 
		*/
		void setBuildingEntity(CEntity *entity);

		/**
		Método que devuelve la smartEntity que el obrero está utilizando actualmente.

		@return la smartEntity.
		*/
		CEntity* getBuildingEntity();

		/**
		Método que devuelve si l'abilidad del obrero es activada.

		@return estado de l'abilidad.
		*/
		bool isRepairBuildingAbilityActive();
		/**
		Método que desabita l'abilitad de raparar.
		*/
		void disableRepairBuildingAbility();
		// -- lastResource methods --

		void setLastResource(CEntity *entity);

		CEntity* getLastResource();

		// position methods.

		void setPosition(const Vector3 &p);

		const Vector3& getPosition();

		// action methods.

		void setAction(WorkmanAction a);

		WorkmanAction getAction();

		// deltaTime methods.

		unsigned int getTime();

		// maxToGather methods.

		unsigned int getGatherMax();

		// gatherUnit methods.

		unsigned int getGatherUnit();

		// timeBetweenGather methods.

		unsigned int getGatherTime();

		// currentGatherQuantity methods.

		unsigned int getCurrentCharge();

		void setCurrentCharge(unsigned int q);

		// resType methods.

		AI::ResourceType getResourceType();

		void setResourceType(AI::ResourceType type);

		// speed methods.

		float getSpeed(){ return speed;};

		void manageSmartEntityDestroyed(Logic::CEntity *smartEntity);

		void removeRepairAbility(CEntity* decisionEntity);

	protected:
		CEntity* buildingEntity;
		CEntity* lastResource; 
		Vector3 position;
		WorkmanAction action;
		unsigned int deltaTime;
		unsigned int maxToGather;
		unsigned int gatherUnit;
		unsigned int timeBetweenGather;
		unsigned int currentGatherQuantity;
		float speed;
		AI::ResourceType resType;

		void findNextResourceClosest(Logic::CEntity*);

		std::string buildAbilityChar;
		std::string repairBuildingAbilityChar;
		bool repairBuildingAbilityActive;


	}; 

	REG_FACTORY(CWorkmanState);
} 

#endif
