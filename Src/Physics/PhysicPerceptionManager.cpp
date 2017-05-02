/**
@file PhysicPerceptionManager.cpp

Contains the implementation of the Physic Perception Manager. 

@author Rayco Sánchez García
@date February, 2015
*/

#include "PhysicPerceptionManager.h"

#include "SceneManager.h"
#include "PhysXServer.h"
#include "Conversions.h"
#include "Logic/Entity/Components/Physics.h"
#include "Logic/Entity/Entity.h"

#include <assert.h>

#include <PxPhysicsAPI.h>
#include <extensions\PxExtensionsAPI.h> 

using namespace Physics;
using namespace physx;


PhysicPerceptionManager::PhysicPerceptionManager(SceneManager* sceneManager, PhysXServer* physXServer) : 
	sceneManager(sceneManager), physXServer(physXServer) 
{
	_collisionManager = physXServer->_collisionManager;
}

//--------------------------------------------------------

PhysicPerceptionManager::~PhysicPerceptionManager () 
{
	sceneManager = nullptr;
}

//--------------------------------------------------------

Logic::CEntity* PhysicPerceptionManager::raycastAdvanced (const Ray& ray, float maxDist,Vector3 &point,Vector3 &normal,std::vector<CollisionGroup> *groups,int id) const
{
	PxVec3 origin = Vector3ToPxVec3(ray.getOrigin());      // origen     
	PxVec3 unitDir = Vector3ToPxVec3(ray.getDirection());  // dirección normalizada   
	PxReal maxDistance = maxDist;                          // distancia máxima
	PxRaycastBufferN<64> buf;	
	PxHitFlags flag = PxHitFlag::eDISTANCE | PxHitFlag::eNORMAL| PxHitFlag::ePOSITION;

	// [in] Raycast against GROUP2 and GROUP3
	PxQueryFilterData filterData = PxQueryFilterData();
	filterData.data.word0 = 0;
	
	if(groups != NULL)
	{
		for(unsigned int i=0; i<groups->size(); ++i)
		{
			filterData.data.word0 |= (1 << groups->at(i));
		}
	}

	bool intersect= sceneManager->getScene()->raycast(origin,unitDir,maxDistance,buf,flag,filterData);
	//Si ha intersecado con algun objecto físico
	if(intersect)
	{

		if(groups!= nullptr)
		{
		// Buscar un actot que pertenezca al grupo de colisión indicado
			for (unsigned int i=0; i<buf.getNbAnyHits(); i++) {
				PxRigidActor *actor = buf.getAnyHit(i).shape->getActor();				
		
				Logic::IPhysics *component = (Logic::IPhysics *) actor->userData;

				if (component && component->getEntity()->getEntityID()!=id) {
					point=PxVec3ToVector3(buf.getAnyHit(i).position);
					normal=PxVec3ToVector3(buf.getAnyHit(i).normal);
					return component->getEntity();
				}
			}
		}else
		{
			PxRigidActor *actor = buf.getAnyHit(0).shape->getActor();

			Logic::IPhysics *component = (Logic::IPhysics *) actor->userData;
			if (component && component->getEntity()->getEntityID()!=id) {
				point=PxVec3ToVector3(buf.getAnyHit(0).position);
				normal=PxVec3ToVector3(buf.getAnyHit(0).normal);
				return component->getEntity();
			}
		}
	}
	return NULL;
}

Logic::CEntity* PhysicPerceptionManager::raycastAdvanced (const Ray& ray, float maxDist,Vector3 &point,std::vector<CollisionGroup> *groups,int id) const
{
	return this->raycastAdvanced(ray,maxDist,point,Vector3(),groups,id);
}

std::list<Logic::CEntity*>* PhysicPerceptionManager::overlapQueries(const Vector3& position,float xLenght,float zLenght,float height,std::vector<CollisionGroup> *groups) const
{
	Vector3 first(-xLenght/2,0,-zLenght/2);
	Vector3 second(+xLenght/2,0,-zLenght/2);
	Vector3 third(+xLenght/2,0,+zLenght/2);
	Vector3 fourth(-xLenght/2,0,+zLenght/2);
	return this->overlapQueries(position+first,position+second,position+third,position+fourth,height,groups);
}

std::list<Logic::CEntity*>* PhysicPerceptionManager::overlapQueries(const Vector3& first,const Vector3& second,const Vector3& third,const Vector3& fourth,float height,std::vector<CollisionGroup> *groups) const
{
	std::list<Logic::CEntity*>* result=new std::list<Logic::CEntity*>();
	PxVec3 convexVerts[] = {Vector3ToPxVec3(first - Vector3(0.0,-height,0.0)),Vector3ToPxVec3(second - Vector3(0.0,-height,0.0)),Vector3ToPxVec3(third - Vector3(0.0,-height,0.0)),
		Vector3ToPxVec3(fourth - Vector3(0.0,-height,0.0)),Vector3ToPxVec3(first),Vector3ToPxVec3(second),
		Vector3ToPxVec3(third),Vector3ToPxVec3(fourth)};

	PxConvexMeshDesc convexDesc;
	convexDesc.points.count     = 8;
	convexDesc.points.stride    = sizeof(PxVec3);
	convexDesc.points.data      = convexVerts;
	convexDesc.flags            = PxConvexFlag::eCOMPUTE_CONVEX;
	convexDesc.vertexLimit      = 256;

	PxDefaultMemoryOutputStream buf;
	if(!physXServer->_cooking->cookConvexMesh(convexDesc, buf))
	{
		delete result;
		return NULL;
	}
	PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
	PxConvexMesh* convexMesh = physXServer->_physics->createConvexMesh(input);


	PxTransform pose(PxVec3(0,0,0));
	PxMaterial *material = physXServer->_defaultMaterial;
	PxOverlapBufferN<64> hit;// [out] Sweep results
	
	PxQueryFilterData filterData(PxQueryFlag::eDYNAMIC | PxQueryFlag::eSTATIC);
	filterData.data.word0 = 0;
	
	if(groups != NULL)
	{
		for(unsigned int i=0; i<groups->size(); ++i)
		{
			filterData.data.word0 |= (1 << groups->at(i));
		}
	
	}

//	fd.flags |= PxQueryFlag::eANY_HIT;
	bool status = sceneManager->getScene()->overlap(PxConvexMeshGeometry(convexMesh), pose,hit,filterData);
	if((hit.hasAnyHits()))
	{
		for (unsigned int i=0; i<hit.getNbAnyHits(); i++) {
			PxRigidActor *actor = hit.getAnyHit(i).shape->getActor();			
			Logic::IPhysics *component = (Logic::IPhysics *) actor->userData;
			if (component) {
				result->push_back(component->getEntity());
			}			
		}
		//		// Buscar un actot que pertenezca al grupo de colisión indicado
		//for (unsigned int i=0; i<hit.getNbAnyHits(); i++) {
		//	PxRigidActor *actor = hit.getAnyHit(i).shape->getActor();
		//	if (groups!=nullptr && std::find(groups->begin(), groups->end(), PxGetGroup(*actor)) != groups->end()) {
		
		//		Logic::IPhysics *component = (Logic::IPhysics *) actor->userData;
		//		if (component) {
		//			result->push_back(component->getEntity());
		//		}
		//	}
		//}
	}

	return result;
}


std::list<Logic::CEntity*>* PhysicPerceptionManager::overlapQueries(const Vector3 position, float radius, std::vector<CollisionGroup> *groups) const
{
	std::list<Logic::CEntity*>* result=new std::list<Logic::CEntity*>();
	
	PxSphereGeometry sphere(radius);

	PxTransform spherePosition(PxVec3(position.x, position.y, position.z));
	PxMaterial *material = physXServer->_defaultMaterial;
	PxOverlapBufferN<64> hit;// [out] Sweep results
	
	PxQueryFilterData filterData(PxQueryFlag::eDYNAMIC | PxQueryFlag::eSTATIC);
	filterData.data.word0 = 0;
	
	if(groups != NULL)
		for(unsigned int i=0; i<groups->size(); ++i)
			filterData.data.word0 |= (1 << groups->at(i));

	bool status = sceneManager->getScene()->overlap(sphere, spherePosition, hit,filterData);
	if((hit.hasAnyHits()))
	{
		for (unsigned int i=0; i<hit.getNbAnyHits(); i++) {
			PxRigidActor *actor = hit.getAnyHit(i).shape->getActor();			
			Logic::IPhysics *component = (Logic::IPhysics *) actor->userData;
			if (component) {
				result->push_back(component->getEntity());
			}			
		}
	}

	return result;
}