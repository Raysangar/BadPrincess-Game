/**
@file IncrementsWavesCounter.h

Action that increment the waves counter and set the graphics text

@see IncrementsWavesCounter

@author Guidi Giacomo
@date March 2015

*/

#pragma once

#ifndef __AI_IncrementsWavesCounter_H
#define __AI_IncrementsWavesCounter_H

#include "AI/ActionNode.h"

namespace AI {


	class CIncrementsWavesCounter:public IActionNode
	{
	public:
		CIncrementsWavesCounter(int incrementNumber,int maxWaves):IActionNode("CIncrementsWavesCounter"),_maxWaves(maxWaves),_incrementWavesNumber(incrementNumber){};

		virtual ~CIncrementsWavesCounter(){};

		 virtual IActionStatus onStart();

	private:
		int _incrementWavesNumber;
		int _maxWaves;
	};
} // namespace AI

#endif __AI_IncrementsWavesCounterManager_H
