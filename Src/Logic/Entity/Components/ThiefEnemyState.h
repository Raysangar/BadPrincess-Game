/**
@file ThiefEnemyState.h

*/

#ifndef __Logic_ThiefEnemyState_H
#define __Logic_ThiefEnemyState_H

#include "Logic/Entity/Component.h"
#include "AI/ResourceServer.h"

namespace Logic 
{
	class CEntity;
	
/**
	Componente que se encarga...
*/
	
	class CThiefEnemyState : public IComponent
	{

	DEC_FACTORY(CThiefEnemyState);

	public:	



		CThiefEnemyState(std::string componentName) : IComponent(componentName), _resType(AI::ResourceType::none), _currentThiefQuantity(0)
			,_thiefUnit(0),_thiefTime(0),_escapeSpawnPoint(nullptr),_thiefAnimation(""){}

		virtual ~CThiefEnemyState();

		virtual void awake();
		
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		virtual void tick(unsigned int msecs);

		virtual bool acceptN(const std::shared_ptr<NMessage> &message);

		virtual void processN(const std::shared_ptr<NMessage> &message);

		unsigned int getThiefTime(){return _thiefTime;};
		

		int getThiefUnit(){return _thiefUnit;};

		// maxToGather methods.

		int getCurrentThiefQuantity(){ return _currentThiefQuantity;};
		
		// currentGatherQuantity methods.

		void setCurrentThiefQuantity(unsigned int q){_currentThiefQuantity = q;};

		// resType methods.

		AI::ResourceType getResourceType(){return _resType;};

		void setResourceType(AI::ResourceType type){_resType = type;};

		std::string getThiefAnimation(){return _thiefAnimation;};

		void manageSmartEntityDestroyed(Logic::CEntity *smartEntity);


	protected:
		int _thiefUnit;
		unsigned int _thiefTime;
		int _currentThiefQuantity;
		AI::ResourceType _resType;
		std::string _thiefAnimation;
		CEntityPtr _escapeSpawnPoint;
		/*
		Function that find a chest (prioritizing the trap chest) to attack
		and send an AvatarMoveToEntity message 
		@param entityToAvoid pointer to an entity that we want to discard from the comparison
		*/
		void attackChest(Logic::CEntity* entityToAvoid=nullptr);
		/*
			Function that find the closest spawnPoint and send a message to go to that 
		*/
		void escape();
	}; 

	REG_FACTORY(CThiefEnemyState);
} 

#endif
