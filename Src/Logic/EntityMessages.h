#ifndef _Logic_EntityMessages_H
#define _Logic_EntityMessages_H

#include "Logic\NMessage.h"

	DECLARE_MESSAGE_CLASS1P(EntityDying,CEntityPtr,entity)
	DECLARE_MESSAGE_CLASS1P(EntityDestroying,CEntityPtr,entity)
	DECLARE_MESSAGE_CLASS1P(EntityInstantiated,CEntityPtr,entity)
	

#endif