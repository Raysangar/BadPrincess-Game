#include "CheckResourceUnits.h"

#include "Logic\Entity\Entity.h"
#include "Logic/Maps/Map.h"
#include "Logic/Server.h"

#include "AI\Server.h"
#include "AI\ResourceServer.h"

namespace AI {

	bool CCheckResourceUnits::onCheck(unsigned int msecs){
		return AI::ResourceServer::getInstance()->getResource((AI::ResourceType)_resourceType) >= _quantity;
	};

}