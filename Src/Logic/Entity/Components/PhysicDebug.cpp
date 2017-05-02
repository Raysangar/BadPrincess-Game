/**
@file PhysicEntity.h

Contiene la implementación del componente encargado de representar entidades físicas simples,
que son aquellas representadas mediante un único actor de PhysX. Este componente no sirve
para representar character controllers.

@see Logic::CPhysicDebug
@see Logic::IComponent
@see Logic::CPhysicController

@author Antonio Sánchez Ruiz-Granados
@date Noviembre, 2012
*/

#include "PhysicDebug.h"

#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"
#include "Physics/Server.h"
#include "Logic/PhysicMessages.h"
#include "Logic/Maps/Map.h"
#include "PhysicController.h"
#include "PhysicEntity.h"
#include "Graphics/Scene.h"
#include "Graphics/ManualEntity.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <PxPhysicsAPI.h>

using namespace Logic;
using namespace Physics;
using namespace physx; 

IMP_FACTORY(CPhysicDebug);

//--------------------------------------------------------

CPhysicDebug::CPhysicDebug(std::string componentName) : IComponent(componentName), debugEntity(nullptr), isVisible(false),scene(nullptr) 
{
}

CPhysicDebug::~CPhysicDebug() 
{
	if(debugEntity)
	{
		deactivate();
		delete debugEntity;
		debugEntity = 0;
	}
	scene=nullptr;
} 

////---------------------------------------------------------

bool CPhysicDebug::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
{
	// Invocar al método de la clase padre
	if(!IComponent::spawn(entity,map,entityInfo))
		return false;
	scene = _entity->getMap()->getScene();
	debugEntity = new Graphics::CManualEntity("physicDebug"+_entity->getName());

	return true;
} 

//---------------------------------------------------------

void CPhysicDebug::awake()
{
	if(!debugEntity->isEmpty())
		return;
	//check before if it is a player to get the PxCapsuleController that contains the physic PxRigidActor
	IComponent *c= _entity->getComponent("CPhysicController");
	if(c!=nullptr)
	{
		CPhysicController *p=static_cast<CPhysicController*>(c);
		this->drawActor(p->getCapsuleController()->getActor());
		debugEntity->setVisible(isVisible);
		return;
	}
	
	//ohterwise check if it has a CPhysicEntity to get an PxRigidActor
	c= _entity->getComponent("CPhysicEntity");
	if(c!=nullptr)
	{
		CPhysicEntity *p=static_cast<CPhysicEntity*>(c);
		this->drawActor(p->getActor());
		debugEntity->setVisible(isVisible);
	}
}

//---------------------------------------------------------

bool CPhysicDebug::acceptN(const std::shared_ptr<NMessage> &message)
{
	return (message->type.compare("ToggleDrawPhysicDebug") == 0);
}

//---------------------------------------------------------

void CPhysicDebug::processN(const std::shared_ptr<NMessage> &message)
{
	std::shared_ptr<ToggleDrawPhysicDebug> m = std::static_pointer_cast<ToggleDrawPhysicDebug>(message);
	isVisible=!isVisible;
	debugEntity->setVisible(isVisible);
}

//---------------------------------------------------------

bool CPhysicDebug::activate() 
{ 
	if(!scene->addEntity(debugEntity))
		return false;
	return true;
}

//---------------------------------------------------------

void CPhysicDebug::deactivate()
{
	scene->removeEntity(debugEntity);
	
}

//---------------------------------------------------------

void CPhysicDebug::tick(unsigned int msecs) 
{
	// Invocar al método de la clase padre (IMPORTANTE)
	IComponent::tick(msecs);

	if(isVisible)
	{
		debugEntity->setTransform(_entity->getTransform());
//		debugEntity->setPosition(_entity->getPosition());
//		debugEntity->setOrientation(_entity->getTransform().extractQuaternion());
	}
	
}

void CPhysicDebug::drawActor(const PxRigidActor*actor)
{
	
	PxU32 nShapes = actor->getNbShapes(); 
    PxShape** shapes=new PxShape*[nShapes];
	//try to draw each shape that compound the actor geometry
	actor->getShapes(shapes, nShapes);    
    while (nShapes--) 
    { 
        this->drawShape(actor,shapes[nShapes]); 
    } 
	delete [] shapes;
}
void CPhysicDebug::drawShape(const PxRigidActor*actor,PxShape* shape)
{
	PxGeometryType::Enum type = shape->getGeometryType();
    switch(type) 
    {          
		case PxGeometryType::eBOX:
			drawBoxShape(actor,shape);
		break;
		case PxGeometryType::eCAPSULE:
			drawCapsuleShape(actor,shape);
		break;
    } 
}
void CPhysicDebug::drawBoxShape(const PxRigidActor*actor,PxShape* pShape)
{
	PxTransform pT = actor->getGlobalPose();
	
	PxBoxGeometry tg;
	pShape->getBoxGeometry(tg);
	
	//draw the cube with unitary dimension	
	debugEntity->drawUnitaryBoxShape();

	//scale the cube with shape dimension and set position and orientation
	debugEntity->setScale(Vector3(tg.halfExtents.x*2,tg.halfExtents.y*2,tg.halfExtents.z*2));
	debugEntity->setPosition(Vector3(pT.p.x,pT.p.y,pT.p.z));
	debugEntity->setOrientation(Quaternion(pT.q.w,pT.q.x,pT.q.y,-pT.q.z));

}
void CPhysicDebug::drawCapsuleShape(const PxRigidActor*actor,PxShape* pShape)
{
	PxTransform pT = actor->getGlobalPose();
	PxCapsuleGeometry tg;
	pShape->getCapsuleGeometry(tg);
	
	debugEntity->drawCapsuleShape(tg.halfHeight,tg.radius);
	//TODO insert drawManager drawcapsule
	debugEntity->setPosition(Vector3(pT.p.x,pT.p.y,pT.p.z));
	debugEntity->setOrientation(Quaternion(pT.q.w,pT.q.x,pT.q.y,pT.q.z));
}

