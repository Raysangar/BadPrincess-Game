/**
@file CarrierState.h

*/

#ifndef __Logic_CarrierState_H
#define __Logic_CarrierState_H

#include "Logic/Entity/Component.h"

namespace Logic 
{
	class CEntity;
	class CGUIManager;
/**
	Componente que se encarga...
*/
	
	class CCarrierState : public IComponent
	{

	DEC_FACTORY(CCarrierState);

	public:	


		CCarrierState(std::string componentName) : IComponent(componentName),_carrierAbilityActive(false),_carrierAbilityChar(""),_carriedEntity(nullptr),_carrierIdleAnimationName(""),_carrierMoveAnimationName(""),_carrierAbilityIcon(""),_releaseAbilityIcon(""),_guiManagerComponent(nullptr){}

		virtual ~CCarrierState();
		
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		virtual void awake();

		virtual void tick(unsigned int msecs);

		virtual bool acceptN(const std::shared_ptr<NMessage> &message);

		virtual void processN(const std::shared_ptr<NMessage> &message);

		/**
		Método que devuelve la entidad que  estamos trasportando.

		@return la smartEntity.
		*/
		CEntity* getCarriedEntity(){return _carriedEntity;};

		void setCarriedEntity(CEntity* entity){ _carriedEntity=entity;};
	
		bool isCarrierAbilityActive(){return _carrierAbilityActive;}

		std::string getCarrierAbilityCharKey(){return _carrierAbilityChar;}

		std::string getCarrierIdleAnimationName(){return _carrierIdleAnimationName;}

		std::string getCarrierMoveAniamtionName(){return _carrierMoveAnimationName;}

		/* function that change the ability icon into GUI*/
		void setupAbilityIcon(std::string iconName);

		std::string getReleaseAbilityIcon() const { return _releaseAbilityIcon; }

	protected:


		bool _carrierAbilityActive;

		std::string _carrierAbilityChar;

		std::string _carrierAbilityIcon;

		std::string _releaseAbilityIcon;

		std::string _carrierIdleAnimationName;

		std::string _carrierMoveAnimationName;

		Logic::CEntity* _carriedEntity;
		

		CGUIManager *_guiManagerComponent;
	}; 

	REG_FACTORY(CCarrierState);
} 

#endif
