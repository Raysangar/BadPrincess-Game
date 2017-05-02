/**
@file EntityAction.h

Action that check if a certain quantity of action are created or destroyed (NOT FINAL VERSION)

@see EntityAction

@author Guidi Giacomo
@date March 2015

*/

#pragma once

#ifndef __AI_EntityAction_H
#define __AI_EntityAction_H

#include "AI/ActionNode.h"

namespace AI {
	

	class CEntityAction: public IActionNode
	{
	public:
		CEntityAction(std::string entityType,int qta);

		/**
		Método virtual que elige que mensajes son aceptados.

		@param message Mensaje a chequear.
		@return true si el mensaje es aceptado.
		*/
		virtual bool acceptN(const std::shared_ptr<NMessage> &message);
		/**
		Método virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		virtual void processN(const std::shared_ptr<NMessage> &message);

		virtual ~CEntityAction(){};

		virtual bool onCheck(unsigned int msecs);

	private:
		std::string _entityType;
		int _unitQuantity;
		int _currentUnitQuantity;

	};


} // namespace AI

#endif __AI_EntityAction_H
