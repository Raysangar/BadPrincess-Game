#ifndef _Logic_EnemyMessages_H
#define _Logic_EnemyMessages_H

#include "Logic\NMessage.h"

	DECLARE_MESSAGE_CLASS0P(ThiefAttack);
	DECLARE_MESSAGE_CLASS1P(ThiefEscape,CEntityPtr,escapeEntity);
	DECLARE_MESSAGE_CLASS0P(KnightActivate)
	DECLARE_MESSAGE_CLASS0P(PrincessKidnapped)
	DECLARE_MESSAGE_CLASS0P(NewTarget)
	DECLARE_MESSAGE_CLASS0P(MissionComplete)
	DECLARE_MESSAGE_CLASS0P(ActivateAssassin)
	DECLARE_MESSAGE_CLASS0P(AskPosition);
	DECLARE_MESSAGE_CLASS2P(SendPosition, Vector3, pos, int, index)
	DECLARE_MESSAGE_CLASS0P(ProtectKnight)
	DECLARE_MESSAGE_CLASS1P(GoAttack, CEntityPtr, ent)
	DECLARE_MESSAGE_CLASS1P(PlayerSeen, CEntityPtr, ent)

#endif