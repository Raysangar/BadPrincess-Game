#ifndef _Logic_MovementMessages_H
#define _Logic_MovementMessages_H

#include "Logic\NMessage.h"
	DECLARE_MESSAGE_CLASS1P(TransformMessage,Matrix4,transform)
	DECLARE_MESSAGE_CLASS1P(KinematicMove, Vector3, movement)
	DECLARE_MESSAGE_CLASS1P(AvatarMove, Vector3, movement)
	DECLARE_MESSAGE_CLASS2P(AvatarMoveToPosition, Vector3, target, int, moveType)
	DECLARE_MESSAGE_CLASS1P(AvatarMoveToEntity, CEntityPtr, target)
	DECLARE_MESSAGE_CLASS2P(AvatarFinishedMove, bool, finished,Vector3,target)
	DECLARE_MESSAGE_CLASS0P(AvatarInterruptMovement)
	DECLARE_MESSAGE_CLASS1P(ModifyMaxLinearSpeed, float, speed)	
#endif

