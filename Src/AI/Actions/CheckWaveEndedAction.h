/**
@file CheckWaveEndedAction.h

Action that check if a certain quantity of action are created or destroyed (NOT FINAL VERSION)

@see CheckWaveEndedAction

@author Guidi Giacomo
@date March 2015

*/

#pragma once

#ifndef __AI_CheckWaveEndedAction_H
#define __AI_CheckWaveEndedAction_H

#include "AI/ActionNode.h"

namespace AI {
	

	class CCheckWaveEndedAction: public IActionNode
	{
	public:
		CCheckWaveEndedAction(int waveId);

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

		virtual ~CCheckWaveEndedAction(){};

		virtual bool onCheck(unsigned int msecs);

	private:
		int _waveId;
		bool _waveEnded;

	};


} // namespace AI

#endif __AI_CheckWaveEndedAction_H
