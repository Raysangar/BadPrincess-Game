/**
@file PhysicPerceptionManager.h

Contains the declaration of the Physic Perception Manager. 

@author Rayco Sánchez García
@date February, 2015
*/

#ifndef __Physics_PhysicPerceptionManager_H
#define __Physics_PhysicPerceptionManager_H

#include "BaseSubsystems/Math.h"
#include "Physics\Server.h"

namespace Physics
{
	class SceneManager;
	class PhysXServer;
	class CCollisionManager;
};

namespace Logic {
	class CEntity;
};

namespace Physics
{
	
	class PhysicPerceptionManager
	{
	public:
		PhysicPerceptionManager(SceneManager* sceneManager, PhysXServer* physXServer);

		~PhysicPerceptionManager();

		/**
		 Lanza un rayo y devuelve la primera entidad lógica contra la que interseca que pertenezca 
		 al grupo de colisión indicado. Si el rayo no choca contra ninguna entidad de ese grupo
		 devuelve NULL.
		 
		 @param ray Rayo lanzado.
		 @param maxDist distancia máxima de la primera intersección.
		 @param group Grupo de colisión de la entidad buscada.
		 @param id Id de una entidad especifica que queriamos que no se colisiona (normalmente se mismo).
		 @return Primera entidad lógica alcanzada de ese grupo o NULL.
		 */
		Logic::CEntity* raycastAdvanced (const Ray& ray,float maxDist,Vector3 &point,std::vector<CollisionGroup> *groups,int id=-1) const;

		Logic::CEntity* raycastAdvanced (const Ray& ray,float maxDist,Vector3 &point,Vector3 &normal,std::vector<CollisionGroup> *groups,int id=-1) const;
		/**
		 Launch a truncate pyramid builds with the 4 rays, and return a pointer list of
		 all the static and dynamic physic entities enclosed in that geometrical figure  
		 
		 @param topLeft Ray of the topLeft edge.
		 @param topRight Ray of the topLeft edge.
		 @param bottomLeft Ray of the topLeft edge.
		 @param bottomRight Ray of the topLeft edge.
		 @param maxDist lenght of the edges.
		 @return pointer list of all the static and dynamic physic entities.
		 */
		std::list<Logic::CEntity*>* overlapQueries(const Vector3& first,const Vector3& second,const Vector3& third,const Vector3& fourth,float height,std::vector<CollisionGroup> *groups) const;

		std::list<Logic::CEntity*>* overlapQueries(const Vector3& position,float xLenght,float zLenght,float height,std::vector<CollisionGroup> *groups) const;
		
		std::list<Logic::CEntity*>* overlapQueries(const Vector3 position, float radius, std::vector<CollisionGroup> *groups) const;
	private:

		// PhysX Scene Manager
		SceneManager* sceneManager;

		//PhysX SDK server
		PhysXServer* physXServer;

		//Collision Manager
		CCollisionManager *_collisionManager;
	};
} //namespace Physics

#endif // __Physics_SceneManager_H