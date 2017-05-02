#ifndef _Logic_AudioMessages_H
#define _Logic_AudioMessages_H

#include "Logic\NMessage.h"

	DECLARE_MESSAGE_CLASS1P(PlayAudio, std::string, eventName)
	DECLARE_MESSAGE_CLASS1P(StopAudio, std::string, eventName)
	DECLARE_MESSAGE_CLASS0P(StopAll)
	DECLARE_MESSAGE_CLASS3P(SetParameterValue, std::string, parameterName, float, value, std::string, eventName)

#endif