#ifndef _Logic_PrincessMessages_H
#define _Logic_PrincessMessages_H

#include "Logic\NMessage.h"

	DECLARE_MESSAGE_CLASS1P(ModifyWrath, int, reason)
	DECLARE_MESSAGE_CLASS2P(PrincessTaken, CEntityPtr, raptor, CEntityPtr, princess);
	DECLARE_MESSAGE_CLASS0P(PrincessToTakenState);
	DECLARE_MESSAGE_CLASS0P(PrincessIsFree);
	DECLARE_MESSAGE_CLASS0P(PrincessLeft);
	DECLARE_MESSAGE_CLASS3P(StunHability, Vector3, origin, float, distance, float, time);
#endif