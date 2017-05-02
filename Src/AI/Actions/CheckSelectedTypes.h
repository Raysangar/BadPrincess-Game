/**
@file CheckSelectedTypes.h

Action that checks if the units passed on contructor have been selected by the player.

@see CheckSelectedTypes

@author Rayco Sánchez García
@date May 2015

*/

#pragma once

#ifndef __AI_CheckSelectedTypes_H
#define __AI_CheckSelectedTypes_H

#include "AI/ActionNode.h"

namespace Logic
{
	class CEntity;
}

namespace AI {

	class CCheckSelectedTypes:public IActionNode
	{
	public:
		CCheckSelectedTypes(std::list<std::string> types):IActionNode("CCheckUnitsSelected"), 
			_types(types), selectedUnits(nullptr){};

		virtual ~CCheckSelectedTypes(){};

		virtual IActionStatus onStart();

		virtual bool onCheck(unsigned int msecs);
	private:
		bool checkUnitIsSelected(const std::string &unitName) const;

		std::list<std::string> _types;

		std::list<Logic::CEntity*>* selectedUnits;
	};
}

#endif __AI_CheckSelectedTypes_H
