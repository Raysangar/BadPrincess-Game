/**
@file ActionNode.cpp

Clase istancia de una oleada.

@see ActionNode

@author Guidi Giacomo 
@date March 2015

*/
#include "EntityAction.h"
#include "Logic\Entity\Entity.h"
#include "Logic\NMessage.h"
#include "Logic/EntityMessages.h"


namespace AI {

	CEntityAction::CEntityAction(std::string entityType,int qta):IActionNode("CEntityAction"),_entityType(entityType),_unitQuantity(qta),_currentUnitQuantity(0){}

	bool CEntityAction::onCheck(unsigned int msecs){
		//if require to create entity
		if(_unitQuantity>=0)
			return  _currentUnitQuantity>=_unitQuantity;
		else //require to destroy entity
			return  _currentUnitQuantity<=_unitQuantity;
	};

	bool CEntityAction::acceptN(const std::shared_ptr<NMessage> &message)
	{
		return (_unitQuantity<0 && message->type.compare("EntityDestroying")==0) || (_unitQuantity>=0 && message->type.compare("EntityInstantiated")==0);
	};

	void CEntityAction::processN(const std::shared_ptr<NMessage> &message){
		if(message->type.compare("EntityDestroying")==0)
		{
			auto m = std::static_pointer_cast<EntityDestroying>(message);
			if(m->entity->getType().compare(_entityType)==0)
			{
					--_currentUnitQuantity;
//					std::cout<<"Entity action "<<_entityType<<" destroyng "<<m->entity->getName()<<" cu="<<_currentUnitQuantity<<"/"<<_unitQuantity<<std::endl;
			}
		}else if(message->type.compare("EntityInstantiated")==0)
		{
			auto m = std::static_pointer_cast<EntityInstantiated>(message);
			if(m->entity->getType().compare(_entityType)==0)
			{
//				std::cout<<"Entity action "<<_entityType<<" instantiate "<<m->entity->getName()<<std::endl;
					++_currentUnitQuantity;
			}
		}


	};
}