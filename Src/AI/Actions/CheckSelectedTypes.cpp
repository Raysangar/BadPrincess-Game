#include "CheckSelectedTypes.h"

#include "Logic\Entity\Entity.h"

#include "GUI\Server.h"
#include "GUI\PlayerController.h"

namespace AI {

	IActionStatus CCheckSelectedTypes::onStart()
	{
		selectedUnits = GUI::CServer::getSingletonPtr()->getPlayerController()->getSelectedUnits();
		return IActionStatus::OnStart;
	}

	bool CCheckSelectedTypes::onCheck(unsigned int msecs){
		for (std::string typeName : _types)
			if (!checkUnitIsSelected(typeName))
				return false;
		return true;
	};

	bool CCheckSelectedTypes::checkUnitIsSelected(const std::string &typeName) const
	{
		std::function<bool(const CEntityPtr unit)> unitNameComparator = 
			[&typeName] (const CEntityPtr unit ) { return unit->getType() == typeName; };
		return find_if(selectedUnits->begin(), selectedUnits->end(), unitNameComparator) != selectedUnits->end();
	}

}