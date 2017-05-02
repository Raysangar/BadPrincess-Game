/**
@file ActionNode.cpp

Clase istancia de una oleada.

@see ActionNode

@author Guidi Giacomo 
@date March 2015

*/
#include "CheckWaveEndedAction.h"

#include "Logic/GameControlMessages.h"


namespace AI {

	CCheckWaveEndedAction::CCheckWaveEndedAction(int waveId):IActionNode("CCheckWaveEndedAction"),_waveId(waveId),_waveEnded(false){}

	bool CCheckWaveEndedAction::onCheck(unsigned int msecs){
		return _waveEnded;
	};

	bool CCheckWaveEndedAction::acceptN(const std::shared_ptr<NMessage> &message)
	{
		return message->type.compare("EndWave")==0;
	};

	void CCheckWaveEndedAction::processN(const std::shared_ptr<NMessage> &message){
		if(message->type.compare("EndWave")==0)
		{
			auto m = std::static_pointer_cast<EndWave>(message);
			std::cout<<"recibe EndWave id"<<m->waveId<<" actionwaveid="<<_waveId<<std::endl;
			if(m->waveId == _waveId)
				_waveEnded = true;
		}

	};
}