/**
@file Wave.h

Clase que representa la oleada.

@see Wave

@author Guidi Giacomo
@date March 2015

*/

#pragma once

#ifndef __AI_Wave_H
#define __AI_Wave_H

#include "BaseSubsystems/Math.h"

namespace AI {
	/*
		This class contains the information what enemy,
		how many enemies and where the enemies have to appear
		in the current phase of wave 
	*/
	class CWaveNode
	{
	public:
		CWaveNode():_enemyType(""),_enemyNum(0),_spawnPoint(""){};
		CWaveNode(std::string eType,unsigned int eNum,std::string spPointName):_enemyType(eType),_enemyNum(eNum),_spawnPoint(spPointName){};

		const std::string getEnemyType() const {return _enemyType;};

		const std::string getSpawnPoint()const{return _spawnPoint;};

		const unsigned int getEnemyNumber() const{return _enemyNum;};

	private:
		std::string _enemyType;
		unsigned int _enemyNum;
		std::string _spawnPoint;
	};
	
	/*
		This class manage a single wave
	*/
	class CWave
	{
	public:
		CWave(int id,std::string eventType,std::string eventValue,unsigned int frequency);

		CWave(int id,unsigned int frequency);

		~CWave();

		void tick(unsigned int msecs);
		/*
		Method that add a phase at the wave

		@param w Info to the wave phase to add
		*/
		void addWaveNode(CWaveNode &w);
		/*
		Method that find a spawnPoint and send it a message to create a new enemy

		@param enemyType enemy that we have to create
		@param spawnPointEntity entity that manage where other entity appear
		*/
		void createEnemy(std::string enemyType,std::string spawnPointEntity);

		const std::string getEventType() const {return _eventType;};

		const std::string getEventValue()const{return _eventValue;};

		const unsigned int getEnemyFrequency() const{return _enemyFrequencyMsecs;};

		const bool isEnd() const{return _isEnd;};

		int getId(){return _id;};

		const std::deque<CWaveNode> &getWaveNodeList(){return *_enemyList;};

	private:
		/*
		Type of event that unleash the wave
		For this moment are accepted "Message" and "Time"
		*/
		std::string _eventType;
		/*
		generally contains the name of Message or the time that have to pass for unleash the wave
		*/
		std::string _eventValue;
		/*
		Waiting Time between two wave 
		*/
		unsigned int _enemyFrequencyMsecs;
		unsigned int _currentTime;
		/*
			Store how many entity are create into each wave phase
		*/
		unsigned int _currentEnemyLeave;
		/*
			List of wave phase that compound the entire wave
		*/
		std::deque<CWaveNode> *_enemyList;
		bool _isEnd;
		int _id;
	};
	
} // namespace AI

#endif __AI_WaveManager_H
