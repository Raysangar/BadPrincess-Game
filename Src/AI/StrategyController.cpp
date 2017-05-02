/**
@file StrategyController.cpp

Servidor de IA.

@author Gonzalo Flórez
@date Diciembre, 2010
*/
#include "StrategyController.h"
#include "Logic\Entity\Entity.h"
#include "Logic\Entity\Components\GroupBehaviour.h"

#define GROUP_RADIUS 14.0f
namespace AI {
	
	/**
	Constructor de la clase.
	*/
	CStrategyController::CStrategyController()
	{

	}
	/////////////////////////////////////////
	/**
	Destructor de la clase.
	*/
	CStrategyController::~CStrategyController(void)
	{

	}

	void CStrategyController::tick(unsigned int msecs){
 	}

	void CStrategyController::moveGroup(std::list<Logic::CEntity*>* units, Vector3 destination)
	{
		Vector3 centerPosition = centroid(units);

		Vector3 direction = destination - centerPosition;
		direction.y = destination.y;		
		direction.normalise();

		Ogre::Quaternion q;		
		q.FromAngleAxis(Ogre::Degree(90),Vector3::UNIT_Y);
		Vector3 normal = q * direction;
		

		std::list<Logic::CEntity*>::iterator it = units->begin();

		Logic::CGroupBehaviour *component;

		Vector3 backwardDirection = direction.normalisedCopy() * -1;
		int unitCount = 1;
		int rows = 0;
		
		std::list<Logic::CEntity*> unitsToAsign; 
		std::vector<Ogre::Vector3> positionsToAsign;
		
		for(it = units->begin();it != units->end(); it++)
		{
			Vector3 distributionCenter = destination + backwardDirection * GROUP_RADIUS * (float)rows;					
			Vector3 unitDestination;
			
			float lateralDistance = GROUP_RADIUS;
			if(units->size() % 3 == 2 && (units->size() - unitCount) <= 1 )
			{
				lateralDistance /= 2;
				if(unitCount % 3 == 1)
				{
					unitDestination = distributionCenter - normal * lateralDistance;				
				}
				else if (unitCount % 3 == 2)
				{				
					unitDestination = distributionCenter + normal * lateralDistance;					
				}
			}
			else{
				if(unitCount % 3 == 1)
				{
					unitDestination = distributionCenter;				
				}
				else if (unitCount % 3 == 0)
				{				
					unitDestination = distributionCenter + normal * lateralDistance;
					++rows;
				}
				else if (unitCount % 3 == 2)
				{				
					unitDestination = distributionCenter - normal * lateralDistance;				
				}
			}
			
			unitDestination.y = destination.y;

			positionsToAsign.push_back(unitDestination);
			unitsToAsign.push_back((*it));

			++unitCount;
		}	

		for(unsigned int i = 0; i < positionsToAsign.size(); ++i)
		{
			std::list<Logic::CEntity*>::iterator it = unitsToAsign.begin();
			Logic::CEntity* closerUnit = (*it);
			float minDistance = closerUnit->getPosition().distance(positionsToAsign.at(i));

			for(;it != unitsToAsign.end(); it++)
			{
				if((*it)->getType() == "Princess" && positionsToAsign.at(i).positionCloses(destination))
				{
					closerUnit = (*it);
					break;
				}
				else if((*it)->getPosition().distance(positionsToAsign.at(i)) < minDistance)
				{
					closerUnit = (*it);
					minDistance = closerUnit->getPosition().distance(positionsToAsign.at(i));
				}
			}
			component = static_cast<Logic::CGroupBehaviour*>(closerUnit->getComponent("CGroupBehaviour"));
			component->groupMoveToPosition(positionsToAsign.at(i));			
			unitsToAsign.remove(closerUnit);
		}
		
	}

	Vector3 CStrategyController::centroid(std::list<Logic::CEntity*>* units)
	{
		Vector3 centroid = Vector3(0.0f,0.0f,0.0f);

		//Checking if a polygon exisits between units's positions
		bool isPolygon = false;

		std::list<Logic::CEntity*>::iterator itcheck = units->begin();
		
		float firstX = (*itcheck)->getPosition().x;
		float firstZ = (*itcheck)->getPosition().z;

		++itcheck;

		for(;itcheck != units->end(); itcheck++)		
		{
			if((*itcheck)->getPosition().x != firstX &&
				(*itcheck)->getPosition().z != firstZ)
			{
				isPolygon = true;
				break;
			}
				
		}

		//If there is a polygon and the number of units is bigger than 2
		if(units->size() > 2 && isPolygon)
		{		
			float signedArea = 0.0;
			float x0 = 0.0; // Current pos X
			float z0 = 0.0; // Current pos Z
			float x1 = 0.0; // Next pos X
			float z1 = 0.0; // Next pos Z
			float a = 0.0;  // Partial signed area

			std::list<Logic::CEntity*>::iterator it ;
			// For all vertices except last
			for(it = units->begin();it != units->end(); it++)		
			{
				std::list<Logic::CEntity*>::iterator it2 = it;

				//Next entity
				it2++;

				if(it2 == units->end())
					it2 = units->begin();				
			
				x0 = (*it)->getPosition().x;
				z0 = (*it)->getPosition().z;
				x1 = (*it2)->getPosition().x;
				z1 = (*it2)->getPosition().z;

				a = x0*z1 - x1*z0;

				signedArea += a;

				centroid.x += (x0 + x1)*a;
				centroid.z += (z0 + z1)*a;
			}

			signedArea *= 0.5f;
			centroid.x /= (6.0f*signedArea);
			centroid.z /= (6.0f*signedArea);
			centroid.y = 6.5f;
		}
		//Otherwise, we just calculate the middle position between the first and the last unit
		else
		{
			std::list<Logic::CEntity*>::iterator it = units->begin();
			std::list<Logic::CEntity*>::reverse_iterator it2 = units->rbegin();

			centroid = ((*it)->getPosition() + (*it2)->getPosition())/2.0f;
		}

		return centroid;

	}

} // namespace AI
