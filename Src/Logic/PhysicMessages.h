#ifndef _Logic_PhysicMessages_H
#define _Logic_PhysicMessages_H

#include "Logic\NMessage.h"

	DECLARE_MESSAGE_CLASS0P(ToggleDrawPhysicDebug)
	DECLARE_MESSAGE_CLASS1P(OnTriggerEnter, CEntityPtr, collisionEntity)
	DECLARE_MESSAGE_CLASS1P(OnTriggerExit, CEntityPtr, collisionEntity)
	DECLARE_MESSAGE_CLASS1P(ChangeCollisionGroup, int, collisionGroup)
	DECLARE_MESSAGE_CLASS1P(SetPhysicControllerPosition, Vector3, position)

#endif