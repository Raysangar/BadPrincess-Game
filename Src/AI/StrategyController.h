/**
@file StrategyController.h

@author Guidi Giacomo
@date January, 2015
*/
#pragma once

#ifndef __AI_StrategyController_H
#define __AI_StrategyController_H

#include <list>
#include "BaseSubsystems/Math.h"

namespace Logic{
	class CEntity;
}

namespace AI {


/**
Controller that manage the group movement
*/
class CStrategyController
{
	public:

		/**
		Constructor de la clase.
		*/
		CStrategyController();

		/**
		Destructor de la clase.
		*/
		~CStrategyController();
		/**
		Función invocada en cada vuelta para actualización.

		@param secs Segundos desde que se renderizó el último frame.
		@return Valor booleano indicando si todo fue bien.
		*/
		void tick(unsigned int secs);

		void moveGroup(std::list<Logic::CEntity*>* units,Vector3 destination);

		/*
		Reset all variable referred to a single instance of game
		*/
		void reset(){};
	private:


		Vector3 centroid(std::list<Logic::CEntity*>* units);

}; // class CStrategyController

} // namespace AI
#endif