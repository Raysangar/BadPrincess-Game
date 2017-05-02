#ifndef _Logic_CombatMessages_H
#define _Logic_CombatMessages_H

#include "Logic\NMessage.h"

DECLARE_MESSAGE_CLASS1P(EnemyAtSight,CEntityPtr,enemy)
DECLARE_MESSAGE_CLASS1P(EnemyLost,CEntityPtr,enemy)
DECLARE_MESSAGE_CLASS0P(AttackEnemy);
DECLARE_MESSAGE_CLASS1P(AsignEnemy,CEntityPtr,enemy)
DECLARE_MESSAGE_CLASS1P(DamageMessage, int, damage)
DECLARE_MESSAGE_CLASS1P(SetInvicible, bool, isInvicible)

#endif