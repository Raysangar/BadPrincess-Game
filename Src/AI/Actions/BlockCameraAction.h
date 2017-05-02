/**
@file BlockCameraAction.h

Accion de bloquear el contro de la camara al jugador

@see BlockCameraAction

@author Jorge Luque
@date May 2015

*/

#pragma once

#ifndef __AI_BlockCameraAction_H
#define __AI_BlockCameraAction_H

#include "AI/ActionNode.h"


namespace AI {
	

	class CBlockCameraAction: public IActionNode
	{
	public:
		CBlockCameraAction(bool bc):IActionNode("CBlockCameraAction"), _cameraBlocked(bc) {};

		virtual ~CBlockCameraAction(){};

		virtual IActionStatus onStart();


	private:

		bool _cameraBlocked;
	};


} // namespace AI

#endif __AI_BlockCameraAction_H