/**
@file ImmortalUnitAction.h

Accion de convertir a una unidad en immmortal y viceversa

@see ImmortalUnitAction

@author Jorge Luque
@date May 2015

*/

#pragma once

#ifndef __AI_ImmortalUnitAction_H
#define __AI_ImmortalUnitAction_H

#include "AI/ActionNode.h"


namespace AI {
	

	class CImmortalUnitAction: public IActionNode
	{
	public:
		CImmortalUnitAction(std::string name, bool i):IActionNode("CFollowEntityAction"), _entityName(name), _immortal(i) {};

		virtual ~CImmortalUnitAction(){};

		virtual IActionStatus onStart();


	private:

		std::string _entityName;
		bool _immortal;
	};


} // namespace AI

#endif __AI_ImmortalUnitAction_H