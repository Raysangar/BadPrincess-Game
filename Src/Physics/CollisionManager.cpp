/**
@file CollisionManager.cpp

Contiene la implementaci�n del gestor de colisiones.

@see Physics::CCollisionManager

@author Antonio S�nchez Ruiz-Granados
@date Noviembre, 2012
*/

#include "CollisionManager.h"
#include "Logic/Entity/Components/PhysicController.h"
#include "Logic/Entity/Components/PhysicEntity.h"
#include "Conversions.h"

#include <PxRigidActor.h>
#include <PxShape.h> 

using namespace Physics;
using namespace Logic;
using namespace physx;

//--------------------------------------------------

CCollisionManager::CCollisionManager()
{
}

//--------------------------------------------------

CCollisionManager::~CCollisionManager()
{

}

//--------------------------------------------------

void CCollisionManager::onConstraintBreak(PxConstraintInfo *constraints, PxU32 count)
{
	// Por ahora ignoramos estos mensajes	
}

//--------------------------------------------------

void CCollisionManager::onWake(PxActor **actors, PxU32 count)
{
	// Por ahora ignoramos estos mensajes	
}

//--------------------------------------------------

void CCollisionManager::onSleep(PxActor **actors, PxU32 count)
{
	// Por ahora ignoramos estos mensajes	
}

//--------------------------------------------------

void CCollisionManager::onContact(const PxContactPairHeader &pairHeader, const PxContactPair *pairs, PxU32 nbPairs) 
{
	//std::cout << "oncontact" << std::endl;
}

//--------------------------------------------------

void CCollisionManager::onTrigger(PxTriggerPair *pairs, PxU32 count)
{
	// Recorrer el array de colisiones
	for (unsigned int i=0; i<count; i++) {
		
		// Ignoramos los pares en los que alguna de las shapes (del trigger o de la otra entidad) ha sido borrada
		if (pairs[i].flags & (PxTriggerPairFlag::eDELETED_SHAPE_TRIGGER | PxTriggerPairFlag::eDELETED_SHAPE_OTHER))
			continue;

		// Comprobamos si estamos saliendo o entrando en el trigger
		bool enter = pairs[i].status & PxPairFlag::eNOTIFY_TOUCH_FOUND;
		bool exit = pairs[i].status & PxPairFlag::eNOTIFY_TOUCH_LOST;

		// S�lo tenemos en cuenta los eventos de entrada y salida del trigger. En PhysX 2.8 y anteriores
		// tambi�n se notificada si el objeto permanec�a dentro del trigger. La documentaci�n no deja muy
		// claro si este comportamiento se ha eliminado de manera definitiva.
		if (!enter && !exit)
			continue;

		// Obtener el componente l�gico asociado al trigger f�sico
		IPhysics *triggerComponent = (IPhysics *) pairs[i].triggerShape->getActor()->userData;
		assert(triggerComponent);

		// Obtener el componente l�gico asociado a la otra entidad f�sica
		IPhysics *otherComponent = (IPhysics *) pairs[i].otherShape->getActor()->userData;
		assert(otherComponent);

		// Notificar a ambos componentes la colisi�n
		triggerComponent->onTrigger(otherComponent, enter);
		otherComponent->onTrigger(triggerComponent, enter);
	}	
}

//--------------------------------------------------

void CCollisionManager::onShapeHit(const PxControllerShapeHit &hit)
{
	// Obtener el componente l�gico asociado al controller
	CPhysicController *component = (CPhysicController *) hit.controller->getUserData();
	assert(component);

	// Notificar la colisi�n
	component->onShapeHit(hit);
}

//--------------------------------------------------

void CCollisionManager::onControllerHit(const PxControllersHit &hit) 
{
	// Obtener el componente l�gico asociado al controller
	CPhysicController *component = (CPhysicController *) hit.controller->getUserData();
	assert(component);
	

	// Notificar la colisi�n
	component->onControllerHit(hit);
}

//--------------------------------------------------

void CCollisionManager::onObstacleHit(const PxControllerObstacleHit &hit)
{
	// Por ahora ignoramos estos mensajes	
}

//--------------------------------------------------

bool CCollisionManager::filter(const PxController& a, const PxController& b)
{
	return true;
}

//--------------------------------------------------

physx::PxFilterFlags CCollisionManager::filterShader(
        PxFilterObjectAttributes attributes0, PxFilterData filterData0,
        PxFilterObjectAttributes attributes1, PxFilterData filterData1,
        PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
{
		
	//Si en ambos actores se han indicado que pueden chocar 
	if( (filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1) ) {
		//std::cout << "Entra: "<<std::endl;//
		//Si es un trigger
		if(PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1))
		{
			pairFlags = PxPairFlag::eTRIGGER_DEFAULT; //= eNOTIFY_TOUCH_FOUND | eNOTIFY_TOUCH_LOST 
			return PxFilterFlag::eDEFAULT;
		}

		//Activamos el flag de notify found O notifyLost
		pairFlags =  PxPairFlag::eNOTIFY_TOUCH_FOUND | PxPairFlag::eNOTIFY_TOUCH_LOST;

		// Si ademas el objeto NO es un trigger, queremos resolver la colision y obtener los
		// puntos de contacto
		if( !PxFilterObjectIsTrigger(attributes0) && !PxFilterObjectIsTrigger(attributes1) ) {
			pairFlags = PxPairFlag::eCONTACT_DEFAULT ;
		}
	}
	else
	{
		//Si no pueden chocar, entonces ignoro la colision
		return PxFilterFlag::eKILL;
	}

    return PxFilterFlag::eDEFAULT;
}


//------------------------------------------------------------


