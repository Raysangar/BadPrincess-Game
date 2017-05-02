/**
@file ChangeAttackAction.h

Accion de cambiar el daño que realiza una unidad

@see ChangeAttackAction

@author Jorge Luque
@date May 2015

*/

#pragma once

#ifndef __AI_ChangeAttackAction_H
#define __AI_ChangeAttackAction_H

#include "AI/ActionNode.h"


namespace AI {
	

	class CChangeAttackAction: public IActionNode
	{
	public:
		CChangeAttackAction(std::string name, float att):IActionNode("CBlockCameraAction"), _entityName(name), _attack(att) {};

		virtual ~CChangeAttackAction(){};

		virtual IActionStatus onStart();


	private:

		std::string _entityName;
		float _attack;
	};


} // namespace AI

#endif __AI_ChangeAttackAction_H