/**
@file GameManager.h

This class manage all action that we have to do into the game
@see GameManager

@author Guidi Giacomo
@date March 2015

*/

#pragma once

#ifndef __AI_GameManager_H
#define __AI_GameManager_H

#include "BaseSubsystems/Math.h"
#include <memory>
#include "Logic\NMessage.h"

namespace AI {



	class IActionNode;

	struct GameProperties{

		GameProperties():_currentGameType(""),_currentSurvivalPoint(0),_hasWin(false),_currentCampaignLevel(0){}

		std::string _currentGameType;
		int _currentSurvivalPoint;
		int _currentCampaignLevel;
		bool _hasWin;
	};

	/**
	This class manage all enemy wave,
	Firstly load the wave from Lua, and store the wave object into list.
	Then check when the condition any wave is accomplished and execute the wave.
	*/
	class CGameManager
	{
	public:
		/**
		Constructor
		*/
		CGameManager(void);
		/**
		Destructor
		*/
		~CGameManager(void);

		void onAwake();

		/*
		Reset all variable referred to a single instance of game
		*/
		void reset();

		void tick(unsigned int msecs);

		/*
		this function store an synchronous action

		@param action The action's reference 
		*/
		void addAction(IActionNode *action);
		/*
		this function store an async action

		@param action The action's reference 
		*/
		void addAsyncAction(IActionNode *action);
		/*
		this function put the action in a list to be executed

		@param action The action's reference 
		*/
		void executeAction(IActionNode *action);
		/*
		this function find if the action is still executing and remove it at the next tick
		ATTENTION: it used only for action not created from lua but created at runtime into the eexecution of someother action

		@param action The action's reference 
		@param return true if the action still executing and it will be removed at the end of this tick		
		*/
		bool removeExecutingAction(IActionNode *action);
		/*
		this function load a game level from lua file

		@param file Lua file name
		*/
		void loadGameLevel(std::string file);
		/*
		this function load a script from lua file

		@param file Lua file name
		*/
		void loadScript(std::string file);
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

		bool isEntityCreationEnabled(std::string entityType);

		void setEntityCreationEnabled(std::string entityType,bool isEnabled);

		/**
		Metodo para cambiar el estado del juego

		@param playerWon variable para decir si el jugador ha ganado 
				cumpliendo todos los objectivos o ha perdido
		*/
		void endGame(bool playerWon);

		/**
		Para controlar que 2 caballeros secuestren a la misma princesa en el mismo tick

		@param princess Princesa que esta siendo secuestrada
		@param knight Caballero que la secuestra
		*/
		bool registerPrincessKidnapped (Logic::CEntity* princess, Logic::CEntity* knight);

		/**
		Para controlar que 2 caballeros secuestren a la misma princesa en el mismo tick
		*/
		void unregisterPrincessKidnapped ();

		/**
		Para controlar que 2 caballeros secuestren a la misma princesa en el mismo tick
		*/
		Logic::CEntity* getPrincess ();

		/**
		Para los caballeros al inicio de una oleada comprobar si existe un caballero al que proteger
		*/
		Logic::CEntity* getKnight ();

		/*
		this function get a position far from another entity with a random angle direction

		@param target The entity that we want to go far 
		@param requester The entity that we have to move
		@param distance How far we want to go
		@param angleMax max degree rotation respect the direction formed from target to requester entity (angle in degree notation) 
						range [-angleMax/2,angleMax/2]
		@return the position vector far from target entity calculated
		*/
		Vector3 getPositionFarToEntity(Logic::CEntity* target,Logic::CEntity* requester,float distance,int angleMax);

		void freeSmartPosition(Logic::CEntity* smartEntity,Vector3 position,Logic::CEntity* sender);

		/**
		Funcion para calcular al posicion del caballero que se une

		@param ent Entidad que pide la posicion
		*/
		Vector3 calculatePosition (Logic::CEntity* ent);

		/**
		Metodo para indicar al GameManager que hay que proteger al caballero que lleva a la princesa

		@param ent Goblin que ataca al caballero
		*/
		bool protectKnight (Logic::CEntity* ent);

		/**
		Metodo para indicar al GameManager que un caballero se sale de al formación

		@param ent Caballero que se sale de la formación
		*/
		void removeKnight (Logic::CEntity* ent);

		/*
		Getter and setter of the wave counter
		*/
		void addWavesCounter(int num);
		int getWavesCounter(){return _wavesCounter;};

		void setupCurrentGame(std::string gameType);

		void setCurrentCampaignLevel(int level){_lastGameProperty._currentCampaignLevel = level;};

		GameProperties const getLastGameProperties(){return _lastGameProperty;}

		bool isPrincessKidnapped(){return _princessKidnaped;}
	private:
		/* list of action that are executed sequentially during the game*/
		std::vector<IActionNode*> *_actionList;
		unsigned int _sequentiallyActionIdx;
		/* list of action to be executed asyncronous during the game*/
		std::vector<IActionNode*> *_executionActionList;
		/*list used to store the async action generated during the execution of another action*/
		std::vector<IActionNode*> *_tempExecutionActionList;
		/*list used to store the action that it will be remove*/
		std::vector<IActionNode*> *_tempRemovingActionList;
		/*list used to store the async action generated from lua
			used to free the memory
		*/
		std::vector<IActionNode*> *_asyncActionList;
		/*Map that store what type of entity we could create*/
		std::map<std::string,bool> _entityCreationEnabled;

		void freeActionList(std::vector<IActionNode*> *list);
		//Si la princesa esta secuestrada
		bool _princessKidnaped;

		//Princes en cuestion
		Logic::CEntity* _princess;

		//Caballero que tiene a la princesa
		Logic::CEntity* _knight;

		/**
		Unidades por cuadrante
		*/
		std::list<Logic::CEntity*> _firstQuad;
		std::list<Logic::CEntity*> _secondQuad;
		std::list<Logic::CEntity*> _thirdQuad;
		std::list<Logic::CEntity*> _forthQuad;

		int _wavesCounter;
		/*
			Data structure that store if in the last game we played to survival mode, tutorial or campaign mode
			and if we won and how many waves we push back
		*/
		GameProperties _lastGameProperty;
	}; // class CGameManager

} // namespace AI

#endif __AI_GameManager_H
