#ifndef _Logic_AnimationMessages_H
#define _Logic_AnimationMessages_H

#include "Logic\NMessage.h"

	DECLARE_MESSAGE_CLASS3P(SetAnimation, std::string, name, bool, cycle,bool, reset)
	DECLARE_MESSAGE_CLASS2P(StopAnimation, std::string, animationName,bool, reset)
	DECLARE_MESSAGE_CLASS1P(AnimationFinished, std::string, animationName)

#endif