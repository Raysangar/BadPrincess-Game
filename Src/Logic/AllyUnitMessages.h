#ifndef _Logic_AllyUnitMessages_H
#define _Logic_AllyUnitMessages_H

#include "Logic\NMessage.h"

	DECLARE_MESSAGE_CLASS1P(CastAbility, std::string, ability)
	DECLARE_MESSAGE_CLASS1P(InstantiateAbilityObject, Vector3, position)
	DECLARE_MESSAGE_CLASS1P(AvatarSelection, bool, selected)
	DECLARE_MESSAGE_CLASS1P(AvatarActivation, bool, actived)
	DECLARE_MESSAGE_CLASS1P(ShootArrow, CEntityPtr, target)
	DECLARE_MESSAGE_CLASS0P(PrincessHasFallen)
	DECLARE_MESSAGE_CLASS0P(ChangeEntityTypeIntoWell)
	DECLARE_MESSAGE_CLASS0P(EnterIntoExchangeTypeArea)
	DECLARE_MESSAGE_CLASS0P(KitchenRecoveryStart)
	DECLARE_MESSAGE_CLASS0P(KitchenRecoveryEnd)
	DECLARE_MESSAGE_CLASS1P(ChestTrapCreated, CEntityPtr, trap)
	DECLARE_MESSAGE_CLASS2P(ActivateCamp,int,waveId,int ,maxWaveTime)

#endif