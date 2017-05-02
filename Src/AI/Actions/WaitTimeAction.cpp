/**
@file ActionNode.cpp

Clase istancia de una oleada.

@see ActionNode

@author Guidi Giacomo 
@date March 2015

*/
#include "WaitTimeAction.h"


namespace AI {
	IActionStatus CWaitTimeAction::onStart(){
		_currentWaitTime=0;
		return IActionStatus::OnStart;
	};

	bool CWaitTimeAction::onCheck(unsigned int msecs){
		_currentWaitTime+=msecs;
		return _currentWaitTime>=_waitTime;
	};

}