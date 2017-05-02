#include "CheckDistanceBetweenEntities.h"

#include "Logic\Entity\Entity.h"
#include "Logic/Maps/Map.h"
#include "Logic/Server.h"


#include "GUI\Server.h"
#include "GUI\PlayerController.h"

namespace AI {

	bool CCheckDistanceBetweenEntities::onCheck(unsigned int msecs){
		Logic::CEntity* firstEntity = Logic::CServer::getSingletonPtr()->getMap()->getEntityByName(firstEntityName);
		Logic::CEntity* secondEntity = Logic::CServer::getSingletonPtr()->getMap()->getEntityByName(secondEntityName);

		return (firstEntity!=NULL && secondEntity!=NULL && firstEntity->getPosition().distance(secondEntity->getPosition()) <= _distance);
	};

}