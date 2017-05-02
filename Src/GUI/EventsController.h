/**

*/
#ifndef __GUI_EventsController_H
#define __GUI_EventsController_H

#include "Graphics\Entity.h"

namespace Logic{
	class CWorldComponent;
	class CCamera;
	class CEntity;
}

// Declaraci�n de la clase
namespace GUI
{
	enum ResourceType
	{
		WOOD = 0,
		STONE = 1,
		IRON = 2
	};

	/**
	
	*/
	class CEventsController
	{
	public:

		/**
		Constructor.
		*/
		CEventsController();

		/**
		Destructor.
		*/
		~CEventsController();

		/**
		
		*/
		void tick(unsigned int msecs);

		/**
		
		*/
		void activate();

		/**
		
		*/
		void deactivate();

		/**
		M�todo para notificar al jugador cuando la princesa ha sido da�ada fuera de c�mara.
		*/
		void princessDamaged(Logic::CEntity* entity);

		/**
		M�todo para notificar al jugador cuando la princesa ha sido secuestrada.
		*/
		void princessKidnapped(Logic::CEntity* entity);

		/**
		M�todo para notificar al jugador cuando le roban recursos.
		*/
		void stealingResources(Logic::CEntity* entity);

		/**
		M�todo para notificar al jugador cuando le faltan recursos de alg�n tipo.
		*/
		void CEventsController::missingResources(ResourceType type);

		/**
		M�todo para notificar al jugador cuando le est�n atacando.
		*/
		void CEventsController::underAttack(Logic::CEntity* entity);



	protected:
		
		
	private :

		Logic::CCamera *_cameraComponent;
		float calculatePositionY();
		unsigned int _underAttackTime;
		unsigned int _missingResourcesTime;
		unsigned int _stealingResourcesTime;
		unsigned int _princessKidnappedTime;
		unsigned int _princessDamagedTime;
		unsigned int _missingWoodTime;
		unsigned int _missingIronTime;
		unsigned int _missingStoneTime;
		
	}; // class CEventsController

} // namespace GUI

#endif // __GUI_MapController_H
