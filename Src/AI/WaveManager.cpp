/**
@file WaveManager.cpp

Clase para mantener grafos de puntos de ruta (waypoints).

@see WaveManager

@author Guidi Giacomo 
@date March 2015

*/
#include "WaveManager.h"
#include "Wave.h"

#include "Logic\Entity\Entity.h"
#include "Logic\GameControlMessages.h"
#include "Logic/AudioMessages.h"
#include "BaseSubsystems\ScriptManager.h"

#include "GUI/Server.h"
#include "GUI/StatisticsController.h"


namespace AI {

	/**
	Constructor
	*/
	CWaveManager::CWaveManager(void):_gameTime(0)
	{
		_waveList = new std::list<CWave*>();
		_currentWaveList = new std::list<CWave*>();
		_messageList.clear();
	}
	/////////////////////////////////////////
	/**
	Destructor
	*/
	CWaveManager::~CWaveManager(void)
	{
		if(_waveList)
		{
			this->clearList(_waveList);
			delete _waveList;
		}
		if(_currentWaveList)
		{
			this->clearList(_currentWaveList);
			delete _currentWaveList;
		}
	}
	bool CWaveManager::acceptN(const std::shared_ptr<NMessage> &message)
	{
		//Accept here all message that could interest to wave manager		
		return _messageList.find(message->type)!=_messageList.end() || message->type.compare("StartWave")==0;
	} // accept
	//---------------------------------------------------------

	void CWaveManager::processN(const std::shared_ptr<NMessage> &message)
	{
		if(message->type.compare("StartWave")==0)
		{
			std::shared_ptr<StartWave> m = std::static_pointer_cast<StartWave>(message);
			//find if the list contains a wave with this id and start it (could be more then one)
			std::list<CWave*>::iterator cit=_waveList->begin();
			while(cit!=_waveList->end())
			{
				if(m->id==(*cit)->getId())
				{
					startWave(*cit);
					cit = _waveList->erase(cit);
					continue;
				}
				++cit;
			}
			
		}
		std::list<CWave*>::iterator it=_waveList->begin();
		while(it!=_waveList->end())
		{
			if((*it)->getEventType().compare("Message")==0 &&(*it)->getEventValue().compare(message->type)==0)
			{
				startWave(*it);
				it = _waveList->erase(it);
				continue;
			}
			++it;
		}
	} // process

	void CWaveManager::tick(unsigned int msecs)
	{
		_gameTime+=msecs;
		std::list<CWave*>::iterator cit=_waveList->begin();
		while(cit!=_waveList->end())
		{
			//if there is a Time event 
			if((*cit)->getEventType().compare("Time")==0)
			{
				//check the time value
				unsigned long t=strtoul((*cit)->getEventValue().c_str(),NULL,10);
				//if the current game time is grether than the time value
				if(_gameTime>=t)
				{
					//put the wave into currentWave to start it
					startWave(*cit);
					//and delete it from remaning wave list
					cit = _waveList->erase(cit);
					continue;
				}
			}
			cit++;
		}


		std::list<CWave*>::iterator it=_currentWaveList->begin();
		while(it!=_currentWaveList->end())
		{
			//execute the current wave
			(*it)->tick(msecs);
			if((*it)->isEnd())
			{
				//if it is finished delete it
				delete *it;
				it = _currentWaveList->erase(it);
				continue;
			}
			++it;
		}

	}
	
	void CWaveManager::addWave(CWave *wave){
		//put the message type into set list to faster check into AcceptN
		if(wave->getEventType().compare("Message")==0)
			_messageList.insert(wave->getEventValue());
		_waveList->push_back(wave);
	}
	void CWaveManager::loadWaveLevel(std::string file)
	{
		ScriptManager::CScriptManager *sm = ScriptManager::CScriptManager::GetPtrSingleton();
		sm->loadScript(file.c_str());
		sm->executeProcedure("loadWave");
	}
	CWave* CWaveManager::getWaveFromId(int id){
		CWave * res=nullptr;
		for(CWave * wave: *_waveList)
		{
			if(wave->getId()==id)
			{
				res=wave;
				break;
			}
		}
		return res;
	}
	void CWaveManager::reset()
	{
		this->clearList(_waveList);
		this->clearList(_currentWaveList);
		_gameTime=0;
	}

	void CWaveManager::clearList(std::list<CWave*> *list)
	{
		std::list<CWave*>::const_iterator it=list->begin();
		for(;it!=list->end();++it)
			delete *it;
		list->clear();
	}

	void CWaveManager::startWave(CWave* wave)
	{
		std::map<std::string,int> enemyNumber;
		std::deque<CWaveNode> nodeList = wave->getWaveNodeList();
		//get all wave node to sum all the entity created into the wave
		for(CWaveNode node: nodeList)
		{
			if(enemyNumber.find(node.getEnemyType())!=enemyNumber.end())
				enemyNumber[node.getEnemyType()]+=node.getEnemyNumber();
			else
				enemyNumber[node.getEnemyType()]=node.getEnemyNumber();			
		}
		//create a CEntityAction to check if all entity create into the wave are died
		for(auto e:enemyNumber)
		{
			//update the GUI
			GUI::CServer::getSingletonPtr()->getStatisticsController()->addEntityCounter(e.first,e.second);
		}
		_currentWaveList->push_back(wave);

	}


}