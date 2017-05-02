#ifndef _Logic_CarrierMessages_H
#define _Logic_CarrierMessages_H

#include "Logic\NMessage.h"

	DECLARE_MESSAGE_CLASS0P(TransportEntity);
	DECLARE_MESSAGE_CLASS1P(TransportEntityStart,CEntityPtr, entity);
	DECLARE_MESSAGE_CLASS0P(TransportEntityEnd);
	DECLARE_MESSAGE_CLASS1P(TransportEntityAbility,bool,active);
	DECLARE_MESSAGE_CLASS0P(InterruptTransport);
	DECLARE_MESSAGE_CLASS0P(ChaseCarriedEntity);
	DECLARE_MESSAGE_CLASS1P(CarriedEntityReached,CEntityPtr,entity);

#endif