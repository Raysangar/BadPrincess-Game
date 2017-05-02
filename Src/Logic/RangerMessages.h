#ifndef _Logic_RangerMessages_H
#define _Logic_RangerMessages_H

#include "Logic\NMessage.h"

	DECLARE_MESSAGE_CLASS1P(TrapDestroyed, std::string, trapType)
	DECLARE_MESSAGE_CLASS1P(SetOwner, CEntityPtr, owner)
	DECLARE_MESSAGE_CLASS1P(SetTrap, Vector3, position)
	DECLARE_MESSAGE_CLASS0P(Muddy)
#endif