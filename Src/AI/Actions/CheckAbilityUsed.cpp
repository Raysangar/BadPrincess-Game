#include "CheckAbilityUsed.h"

#include "Logic\Entity\Entity.h"
#include "Logic/Maps/Map.h"
#include "Logic/Server.h"
#include "Logic/AllyUnitMessages.h"

#include "GUI\Server.h"
#include "GUI\ActiveUnitController.h"


namespace AI {

	bool CCheckAbilityUsed::acceptN(const std::shared_ptr<NMessage> &message)
	{
		return message->type == "CastAbility";
	}
		
	void CCheckAbilityUsed::processN(const std::shared_ptr<NMessage> &message)
	{
		abilityCasted = (std::static_pointer_cast<CastAbility>(message)->ability == ability);
	}

	IActionStatus CCheckAbilityUsed::onStart()
	{
		unit = Logic::CServer::getSingletonPtr()->getMap()->getEntityByName(unitName);
		return IActionStatus::OnStart;
	}

	bool CCheckAbilityUsed::onCheck(unsigned int msecs){
		return ((GUI::CServer::getSingletonPtr()->getActiveUnitController()->getActiveUnit() == unit) && abilityCasted);
	};

}