#ifndef _Logic_RouteCalculationMessages_H
#define _Logic_RouteCalculationMessages_H

#include "Logic\NMessage.h"

	DECLARE_MESSAGE_CLASS2P(AvatarRouteTo, bool, calculateNewRoute,Vector3,target)
	DECLARE_MESSAGE_CLASS1P(AvatarFinishingRoute, bool, finished)
	DECLARE_MESSAGE_CLASS0P(AvatarFinishedRoute)
	DECLARE_MESSAGE_CLASS0P(AvatarInterruptRoute)

#endif