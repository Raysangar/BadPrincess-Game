/**
@file DefendToWaveAction.h

Action that lauch a wave and check if the wave finish

@see DefendToWaveAction

@author Guidi Giacomo
@date March 2015

*/

#pragma once

#ifndef __AI_DefendToWaveAction_H
#define __AI_DefendToWaveAction_H

#include "AI/ActionNode.h"

namespace AI {
	

	class CDefendToWaveAction: public IActionNode
	{
	public:
		CDefendToWaveAction(int waveId,long maxTime);

		virtual ~CDefendToWaveAction();

		virtual bool onCheck(unsigned int msecs);

		virtual IActionStatus onStart();

		virtual IActionStatus onEnd();

	private:
		
		int _waveId;
		long _maxTime;
		long _currentTime;

	};


} // namespace AI

#endif __AI_DefendToWaveAction_H
