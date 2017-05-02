#ifndef _Logic_UnitMessages_H
#define _Logic_UnitMessages_H

#include "Logic\NMessage.h"

	DECLARE_MESSAGE_CLASS2P(AvatarControl,std::string,controlType,float,attribute)
	DECLARE_MESSAGE_CLASS1P(HealUnit, float, healing);
	DECLARE_MESSAGE_CLASS4P(ModifyStat,std::string,modID,int,modType,float,percentage,int,durationMsecs);
	DECLARE_MESSAGE_CLASS1P(RestoreStat,std::string,modID);
	DECLARE_MESSAGE_CLASS1P(Attacking, bool, attacking)
	DECLARE_MESSAGE_CLASS1P(SetBerserkerMode, bool, active)

#endif