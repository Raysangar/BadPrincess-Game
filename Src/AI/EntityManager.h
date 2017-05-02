/**
@file EntityManager.h

manger that manage all type of entity list (chest,resource,enemy,..)
For now only Chest and Chest Trap

@author Guidi Giacomo
@date March, 2015
*/
#pragma once

#ifndef __AI_EntityManager_H
#define __AI_EntityManger_H

#include <list>
#include <memory>
#include "BaseSubsystems/Math.h"
#include "Logic/NMessage.h"

namespace Logic{
	class CEntity;
	class IComponent;
}


namespace AI {


	class CEntityManager
	{
		public:

			enum EntityType
			{
				Ally,
				Enemy,
				Chest,
				TrapChest,
				Resource,
				Other,
				Princess,
				SpawnPoint,
				Well
			};

			/**
			Constructor de la clase.
			*/
			CEntityManager();

			/**
			Destructor de la clase.
			*/
			~CEntityManager();
			/**
			Función invocada en cada vuelta para actualización.

			@param secs Segundos desde que se renderizó el último frame.
			@return Valor booleano indicando si todo fue bien.
			*/
			void tick(unsigned int secs);

			bool registerEntity(Logic::CEntity * e);

			bool unregisterEntity(Logic::CEntity * e);

			bool registerEntity(EntityType type,Logic::CEntity*e);

			bool unregisterEntity(EntityType type, Logic::CEntity*);

			Logic::CEntity* getClosestEntity(EntityType type,Vector3 position,Logic::CEntity*entityToAvoid=nullptr);

			Logic::CEntity* getClosestEntity(std::set<Logic::CEntity*> * list,Vector3 position,Logic::CEntity*entityToAvoid=nullptr);

			int getEntitiesNumber(EntityType listType,std::string entityType);

			int getEntitiesNumber(std::set<Logic::CEntity*> * list,std::string entityType);

			std::set<Logic::CEntity*>* getChestList(){ return &_chestList; }

			std::set<Logic::CEntity*>* getAlliesList() { return &_allyUnitList; }

			std::set<Logic::CEntity*>* getEnemiesList() { return &_enemyUnitList; }

			std::set<Logic::CEntity*>* getPrincessesList() { return &_princessList; }

			std::set<Logic::CEntity*>* getSpawnPointList() { return &_spawnPointList; }

			std::set<Logic::CEntity*>* getWellList() { return &_wellList; }

			std::set<Logic::CEntity*>* getTrapChestList() { return &_trapChestList; }

			/*
			Function that launch a physics overlaps near a position with a fixed radius and
			find the entity closest to a target position that it's of a specific type
			@param overlapsPosition center of physics overlaps
			@param entityType type of entity that we find
			@param overlapsRadius dimension of overlaps
			@param targetPosition position from that we want to get the closest entity
			@param idAvoid identifier of entity that we would descard
			@return pointer of closest entity or nullptr if there isn't any entity closest

			*/
			Logic::CEntity* getClosestEntityTypeWithOverlaps(Vector3 overlapsPosition,std::string entityType,float overlapRadius,Vector3 targetPosition,int idAvoid=-1);
			/*
			Function that launch a physics overlaps near a position with a fixed radius and
			find the entity closest to a target position that it's of a specific type
			@param overlapsPosition center of physics overlaps
			@param entityOverlaps entity where we want to find the next one
			@param overlapsRadius dimension of overlaps
			@param targetPosition position from that we want to get the closest entity
			@return pointer of closest entity or nullptr if there isn't any entity closest
			*/
			Logic::CEntity* getClosestEntityTypeWithOverlaps(Logic::CEntity*entityOverlaps,float overlapRadius,Vector3 targetPosition);

			/*
			Reset all variable referred to a single instance of game
			*/
			void reset();
			/*
			function that check if the entity is inside the world visible area
			@param e entity to check the position
			@return true if the entity is inside the world visible area or false otherwise
			*/
			bool checkEntityIsInsideMap(Logic::CEntity*e);
			/*
			function basically called from Camera entity that setting the world visible limits 
			*/
			void setupWorldLimits(float left,float top,float right,float bottom);

			void sendMessageToList(EntityType entityListType,const std::shared_ptr<NMessage> &message, Logic::IComponent* emitter = 0);

		private:
			std::set<Logic::CEntity*> _chestList;

			std::set<Logic::CEntity*> _trapChestList;

			std::set<Logic::CEntity*> _allyUnitList;

			std::set<Logic::CEntity*> _enemyUnitList;

			std::set<Logic::CEntity*> _resourceList;

			std::set<Logic::CEntity*> _otherList;

			std::set<Logic::CEntity*> _wellList;

			std::set<Logic::CEntity*> _princessList;

			std::set<Logic::CEntity*> _spawnPointList;

			EntityType getEntityType(std::string entity, Logic::CEntity *e);

			bool registerEntity(std::set<Logic::CEntity*> * activeList, Logic::CEntity*e);

			bool unregisterEntity(std::set<Logic::CEntity*> * activeList, Logic::CEntity*e);

			int _unitsCount;

			Vector2 _worldLimitsTL;
			Vector2 _worldLimitsBR;

			void sendMessageToList(std::set<Logic::CEntity*> * activeList,const std::shared_ptr<NMessage> &message, Logic::IComponent* emitter = 0);

			/*
			function that check if the all the entities of a list are inside the world visible area
			@param list entities list to check the position
			@return true if the entity is inside the world visible area or false otherwise
			*/
			void verifyEntityListIsInsideMap(std::set<Logic::CEntity*> * list);
	}; // class CEntityManager

} // namespace AI
#endif