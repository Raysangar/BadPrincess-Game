#ifndef _Logic_WorkmanMessages_H
#define _Logic_WorkmanMessages_H

#include "Logic\NMessage.h"

	DECLARE_MESSAGE_CLASS1P(GatherMessage,int, quantity)
	DECLARE_MESSAGE_CLASS1P(ResourceAutoDestroy,CEntityPtr, nextItem)
	DECLARE_MESSAGE_CLASS1P(BuildWell,CEntityPtr, entity);
	DECLARE_MESSAGE_CLASS1P(BuildKitchen,CEntityPtr, entity);
	DECLARE_MESSAGE_CLASS0P(DisableRepairAbility);
	DECLARE_MESSAGE_CLASS0P(BuildingEnded);

#endif