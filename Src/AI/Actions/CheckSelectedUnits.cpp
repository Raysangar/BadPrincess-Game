#include "CheckSelectedUnits.h"

#include "Logic\Entity\Entity.h"

#include "GUI\Server.h"
#include "GUI\PlayerController.h"

namespace AI {

	IActionStatus CCheckSelectedUnits::onStart()
	{
		selectedUnits = GUI::CServer::getSingletonPtr()->getPlayerController()->getSelectedUnits();
		return IActionStatus::OnStart;
	}

	bool CCheckSelectedUnits::onCheck(unsigned int msecs){
		for (std::string unitName : units)
			if (!checkUnitIsSelected(unitName))
				return false;
		return true;
	};

	bool CCheckSelectedUnits::checkUnitIsSelected(const std::string &unitName) const
	{
		std::function<bool(const CEntityPtr unit)> unitNameComparator = 
			[&unitName] (const CEntityPtr unit ) { return unit->getName() == unitName; };
		return find_if(selectedUnits->begin(), selectedUnits->end(), unitNameComparator) != selectedUnits->end();
	}

}