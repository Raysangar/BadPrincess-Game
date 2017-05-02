/**
@file FollowEntityAction.h

Accion de asignar una entidad a seguir con la camara

@see FollowEntityAction

@author Jorge Luque
@date May 2015

*/

#pragma once

#ifndef __AI_FollowEntityAction_H
#define __AI_FollowEntityAction_H

#include "AI/ActionNode.h"


namespace AI {
	

	class CFollowEntityAction: public IActionNode
	{
	public:
		CFollowEntityAction(std::string name):IActionNode("CFollowEntityAction"), _entityName(name) {};

		virtual ~CFollowEntityAction(){};

		virtual IActionStatus onStart();


	private:

		std::string _entityName;
	};


} // namespace AI

#endif __AI_FollowEntityAction_H