/**
@file Wave.cpp

Clase istancia de una oleada.

@see Wave

@author Guidi Giacomo 
@date March 2015

*/
#include "Wave.h"

#include "Logic\Maps\Map.h"
#include "Logic\Server.h"
#include "Logic\Entity\Entity.h"
#include "Logic\Entity\Components\SpawnPointComponent.h"

namespace AI {
	CWave::CWave(int id,std::string eventType,std::string eventValue,unsigned int frequency):_id(id),_eventType(eventType),_eventValue(eventValue),_enemyFrequencyMsecs(frequency),_currentTime(0),_currentEnemyLeave(0),_isEnd(false)
	{
		_enemyList= new std::deque<CWaveNode>();
	};
	CWave::CWave(int id,unsigned int frequency):_id(id),_eventType(""),_eventValue(""),_enemyFrequencyMsecs(frequency),_currentTime(0),_currentEnemyLeave(0),_isEnd(false){
		_enemyList= new std::deque<CWaveNode>();
	};
	CWave::~CWave()
	{
		delete _enemyList;
	}

	void CWave::addWaveNode(CWaveNode &w)
	{
		_enemyList->push_back(w);
	}

	void CWave::tick(unsigned int msecs)
	{

		//if(!_isEnd)
		//{
			_currentTime+=msecs;
			//if it's pass the waiting time between two create enemy
			while (_currentTime>=_enemyFrequencyMsecs &&  !_isEnd)
			{				
				_currentTime = _currentTime -_enemyFrequencyMsecs;
				//check if the wave it's not wrong and have some units to create
				if((*_enemyList).front().getEnemyNumber()>0)
				{
					//create an enemy
					createEnemy((*_enemyList).front().getEnemyType(),(*_enemyList).front().getSpawnPoint());
					_currentEnemyLeave++;
				}
				//if i have create all the entity for this phase
				if(_currentEnemyLeave>=(*_enemyList).front().getEnemyNumber())
				{
					//delete this phase and pass to the next in FIFO order
					_currentEnemyLeave=0;
					_enemyList->pop_front();

					//if there isn't another wave phase
					if(_enemyList->size()<=0)
						//the wave is ended
						_isEnd=true;
				}
			}
		//}

	}
	
	void CWave::createEnemy(std::string enemyName,std::string spawnPointEntity)
	{
		Logic::CEntity *e = Logic::CServer::getSingletonPtr()->getMap()->getEntityByName(enemyName);
		Logic::CEntity *sp = Logic::CServer::getSingletonPtr()->getMap()->getEntityByName(spawnPointEntity);
		if(sp == nullptr || e==nullptr)
		{
			std::cerr<<"Error during Enemy creation, "<<spawnPointEntity<<" or "<<enemyName<<" not exists in maps"<<std::endl;
			return ;
		}

		Logic::CSpawnPointComponent *c=static_cast<Logic::CSpawnPointComponent*>( sp->getComponent("CSpawnPointComponent"));
		if(c!=nullptr)
			c->createEntity(e);
	}


}