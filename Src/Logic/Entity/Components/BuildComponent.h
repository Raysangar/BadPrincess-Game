/**
@file BuildComponent.h

*/

#ifndef __Logic_BuildComponent_H
#define __Logic_BuildComponent_H

#include "Logic/Entity/Component.h"

namespace Logic 
{
	class CEntity;
	
/**
	Componente que se encarga...
*/
	
	class CBuildComponent : public IComponent
	{

	DEC_FACTORY(CBuildComponent);

	public:	

		CBuildComponent(std::string componentName) : IComponent(componentName),_timeBuild(0),_currentBuildTime(0),_buildedModelName(""),_buildedModelScale(1.0f),_isBuilded(false),_isIndestructible(false),_healtReparation(0),_isInitiallyBuilded(false){}

		virtual ~CBuildComponent();
		
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		virtual void awake();

		// -- smartEntity methods --
		virtual bool acceptN(const std::shared_ptr<NMessage> &message);

		virtual void processN(const std::shared_ptr<NMessage> &message);
	
		//get time to build .
		unsigned int getTimeBuild(){return _timeBuild;};
		//get current time to build .
		unsigned int getCurrentTimeBuild(){return _currentBuildTime;};
		//set current time to build  methods.
		void setCurrentTimeBuild(unsigned int val){ _currentBuildTime = val;};
		//get graphics model for builded 
		std::string getBuildedModelName(){return _buildedModelName;};
		//get graphics model scale for builded 
		float getBuildedModelScale(){return _buildedModelScale;};

		bool isBuilded(){return _isBuilded;};

		bool isIndestructible(){return _isIndestructible;};
		//get health that the entity recive when the reparation is ended .
		float getHealtReparation(){return _healtReparation;};
		//set health that the entity recive when the reparation is ended .
		void setHealtReparation(float val){_healtReparation=val;}

		void setBuilded();
	protected:
		unsigned int _currentBuildTime;

		unsigned int _timeBuild;

		std::string _buildedModelName;

		float _buildedModelScale;

		bool _isBuilded;

		bool _isInitiallyBuilded;

		bool _isIndestructible;

		float _healtReparation;

		void sendAbilityMessage(std::string charAbility,bool isEnabled);
	}; 

	REG_FACTORY(CBuildComponent);
} 

#endif
