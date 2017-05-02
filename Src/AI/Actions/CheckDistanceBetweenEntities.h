#pragma once

#ifndef __AI_CheckDistanceBetweenEntities_H
#define __AI_CheckDistanceBetweenEntities_H

#include "AI/ActionNode.h"

namespace AI {

	class CCheckDistanceBetweenEntities:public IActionNode
	{
	public:
		CCheckDistanceBetweenEntities(std::string firstEntityName, std::string secondEntityName, float distance):IActionNode("CCheckDistanceBetweenEntities"), 
			firstEntityName(firstEntityName), secondEntityName(secondEntityName), _distance(distance) {};

		virtual ~CCheckDistanceBetweenEntities(){};

		virtual bool onCheck(unsigned int msecs);
	private:

		std::string firstEntityName;

		std::string secondEntityName;

		float _distance;
	};
}

#endif __AI_CheckDistanceBetweenEntities_H
