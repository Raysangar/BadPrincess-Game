#ifndef _Logic_SmartObjectMessages_H
#define _Logic_SmartObjectMessages_H

#include "Logic\NMessage.h"

	DECLARE_MESSAGE_CLASS1P(SmartDataPositionRequest, CEntityPtr, entity);
	DECLARE_MESSAGE_CLASS2P(SmartDataAvatarReady, CEntityPtr, entity,Vector3,position);
	DECLARE_MESSAGE_CLASS2P(SmartDataPositionReceiver, CEntityPtr, smartObject,Vector3,position);
	DECLARE_MESSAGE_CLASS2P(SmartDataAction, std::string, animation,int,actionType);
	DECLARE_MESSAGE_CLASS2P(SmartDataFreePosition, CEntityPtr, entity,Vector3,position);
	DECLARE_MESSAGE_CLASS0P(SmartObjectFull);
	DECLARE_MESSAGE_CLASS0P(InteractWithSmartObject);
	DECLARE_MESSAGE_CLASS1P(ChangeSmartObjectState,bool, isEnabled);
	DECLARE_MESSAGE_CLASS1P(SmartObjectDisabled,CEntityPtr, entity);
	DECLARE_MESSAGE_CLASS1P(AttemptSmartObjectLimitReached,CEntityPtr, entity);
#endif