#ifndef _Logic_GameControlMessages_H
#define _Logic_GameControlMessages_H

#include "Logic\NMessage.h"

	DECLARE_MESSAGE_CLASS2P(EnableCreationEntity,std::string,entityType,bool,isEnabled)
	DECLARE_MESSAGE_CLASS1P(StartWave,int,id)
	DECLARE_MESSAGE_CLASS1P(EndWave,int,waveId)
	DECLARE_MESSAGE_CLASS1P(GameOver,bool,hasWin)
	DECLARE_MESSAGE_CLASS0P(SkipText)

#endif