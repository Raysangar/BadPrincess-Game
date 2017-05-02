/**
@file WaveManager.h

This class manage all enemy waves
@see WaveManager

@author Guidi Giacomo
@date March 2015

*/

#pragma once

#ifndef __AI_WaveManager_H
#define __AI_WaveManager_H

#include "BaseSubsystems/Math.h"
#include <memory>
#include "Logic\NMessage.h"

namespace AI {



	class CWave;

	/**
	This class manage all enemy wave,
	Firstly load the wave from Lua, and store the wave object into list.
	Then check when the condition any wave is accomplished and execute the wave.
	*/
	class CWaveManager
	{
	public:
		/**
		Constructor
		*/
		CWaveManager(void);
		/**
		Destructor
		*/
		~CWaveManager(void);

		void tick(unsigned int msecs);
		/*
		this function store a wave into wavelist

		@param wave The referene to wave
		*/
		void addWave(CWave *wave);
		/*
		this function load a wave from lua file

		@param file Lua file name
		*/
		void loadWaveLevel(std::string file);
		/**
		Método virtual que elige que mensajes son aceptados.

		@param message Mensaje a chequear.
		@return true si el mensaje es aceptado.
		*/
		bool acceptN(const std::shared_ptr<NMessage> &message);
		/**
		Método virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		void processN(const std::shared_ptr<NMessage> &message);

		CWave* getWaveFromId(int id);

		/*
		Reset all variable referred to a single instance of game
		*/
		void reset();
	private:
		/* list of waves loaded but not executed*/
		std::list<CWave*> *_waveList;
		/*list of started but not finished waves */
		std::list<CWave*> *_currentWaveList;
		/*list of all message type into loaded waves that we have to accept*/
		std::set<std::string> _messageList;


		unsigned int _gameTime;

		void clearList(std::list<CWave*> *list);

		void startWave(CWave*);

	}; // class CWaveManager

} // namespace AI

#endif __AI_WaveManager_H
