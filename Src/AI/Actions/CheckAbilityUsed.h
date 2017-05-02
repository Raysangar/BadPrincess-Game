#pragma once

#ifndef __AI_CheckAbilityUsed_H
#define __AI_CheckAbilityUsed_H

#include "AI/ActionNode.h"

namespace Logic
{
	class CEntity;
}

namespace AI {

	class CCheckAbilityUsed:public IActionNode
	{
	public:
		CCheckAbilityUsed(std::string unitName, std::string ability):IActionNode("CCheckAbilityUsed"), 
			unitName(unitName), ability(ability), unit(nullptr), abilityCasted(false) {};

		virtual ~CCheckAbilityUsed(){};

		virtual bool acceptN(const std::shared_ptr<NMessage> &message);
		
		virtual void processN(const std::shared_ptr<NMessage> &message);

		virtual IActionStatus onStart();

		virtual bool onCheck(unsigned int msecs);
	private:

		std::string unitName;

		Logic::CEntity* unit;

		std::string ability;

		bool abilityCasted;

	};
}

#endif __AI_CheckAbilityUsed_H
