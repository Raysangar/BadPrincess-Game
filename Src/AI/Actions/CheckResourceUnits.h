#pragma once

#ifndef __AI_CheckResourceUnits_H
#define __AI_CheckResourceUnits_H

#include "AI/ActionNode.h"

namespace AI {

	class CCheckResourceUnits:public IActionNode
	{
	public:
		CCheckResourceUnits(int resourceType, int quantity):IActionNode("CCheckResourceUnits"), 
			_resourceType(resourceType), _quantity(quantity){};

		virtual ~CCheckResourceUnits(){};

		virtual bool onCheck(unsigned int msecs);
	private:

		int _resourceType;

		int _quantity;

	};
}

#endif __AI_CheckResourceUnits_H
