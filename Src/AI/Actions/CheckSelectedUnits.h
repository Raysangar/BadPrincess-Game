/**
@file CheckSelectedUnits.h

Action that checks if the units passed on contructor have been selected by the player.

@see CheckSelectedUnits

@author Rayco Sánchez García
@date May 2015

*/

#pragma once

#ifndef __AI_CheckSelectedUnits_H
#define __AI_CheckSelectedUnits_H

#include "AI/ActionNode.h"

namespace Logic
{
	class CEntity;
}

namespace AI {

	class CCheckSelectedUnits:public IActionNode
	{
	public:
		CCheckSelectedUnits(std::list<std::string> units):IActionNode("CCheckUnitsSelected"), 
			units(units), selectedUnits(nullptr){};

		virtual ~CCheckSelectedUnits(){};

		virtual IActionStatus onStart();

		virtual bool onCheck(unsigned int msecs);
	private:
		bool checkUnitIsSelected(const std::string &unitName) const;

		std::list<std::string> units;

		std::list<Logic::CEntity*>* selectedUnits;
	};
}

#endif __AI_CheckSelectedUnits_H
