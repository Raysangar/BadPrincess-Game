/**
@file DeleteEntityAction.h

Accion de convertir a una unidad en immmortal y viceversa

@see DeleteEntityAction

@author Jorge Luque
@date May 2015

*/

#pragma once

#ifndef __AI_DeleteEntityAction_H
#define __AI_DeleteEntityAction_H

#include "AI/ActionNode.h"


namespace AI {
	

	class CDeleteEntityAction: public IActionNode
	{
	public:
		CDeleteEntityAction(std::string name):IActionNode("CFollowEntityAction"), _entityName(name){};

		virtual ~CDeleteEntityAction(){};

		virtual IActionStatus onStart();


	private:

		std::string _entityName;

	};


} // namespace AI

#endif __AI_DeleteEntityAction_H