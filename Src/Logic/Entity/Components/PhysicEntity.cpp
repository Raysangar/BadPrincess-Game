/**
@file PhysicEntity.h

Contiene la implementación del componente encargado de representar entidades físicas simples,
que son aquellas representadas mediante un único actor de PhysX. Este componente no sirve
para representar character controllers.

@see Logic::CPhysicEntity
@see Logic::IComponent
@see Logic::CPhysicController

@author Antonio Sánchez Ruiz-Granados
@date Noviembre, 2012
*/

#include "PhysicEntity.h"

#include "Logic/MovementMessages.h"
#include "Logic/PhysicMessages.h"
#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"
#include "Physics/Server.h"
#include "Physics/ActorController.h"
#include "Physics/SceneManager.h"
#include "Physics/ActorFactory.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <PxPhysicsAPI.h>

using namespace Logic;
using namespace Physics;
using namespace physx; 

IMP_FACTORY(CPhysicEntity);

//--------------------------------------------------------

CPhysicEntity::CPhysicEntity(std::string componentName) : IPhysics(componentName), _actor(nullptr), _movement(Vector3(0,0,0)) 
{
	actorFactory = CServer::getSingletonPtr()->getActorFactory();
	actorController = CServer::getSingletonPtr()->getActorController();
}

CPhysicEntity::~CPhysicEntity() 
{
	if (_actor) {
		actorFactory->destroyActor(_actor);
		_actor = NULL;
	}

	actorFactory = nullptr;
	actorController = nullptr;
} 

////---------------------------------------------------------

bool CPhysicEntity::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
{
	// Invocar al método de la clase padre
	if(!IComponent::spawn(entity,map,entityInfo))
		return false;

	// Crear el objeto físico asociado al componente
	_actor = createActor(entityInfo);

	PxRigidDynamic *dinActor = _actor->isRigidDynamic();
	if (dinActor)		
		actorController->setGlobalPose(dinActor, _entity->getTransform());

	return true;
} 

//---------------------------------------------------------

bool CPhysicEntity::activate()
{
	if (_actor)
		Physics::CServer::getSingletonPtr()->getSceneManager()->addToScene(_actor);
	return true;
}

//---------------------------------------------------------

void CPhysicEntity::deactivate()
{
	if (_actor)
		Physics::CServer::getSingletonPtr()->getSceneManager()->removeFromScene(_actor);
}

//---------------------------------------------------------

bool CPhysicEntity::acceptN(const std::shared_ptr<NMessage> &message)
{
	return (message->type.compare("KinematicMove") == 0) || (message->type == "TransformMessage")
		|| message->type == "ChangeCollisionGroup";
}

//---------------------------------------------------------

void CPhysicEntity::processN(const std::shared_ptr<NMessage> &message)
{
	if (message->type == "TransformMessage")
	{
		auto m = std::static_pointer_cast<TransformMessage>(message);
		PxRigidDynamic *dinActor = _actor->isRigidDynamic();
		if (!dinActor)
			return;
		actorController->setGlobalPose(dinActor, m->transform);
		
	}
	else if (message->type.compare("KinematicMove") == 0)
		_movement += std::static_pointer_cast<KinematicMove>(message)->movement;
	else
		Physics::CServer::getSingletonPtr()->setupFiltering(_actor, 
			Physics::CollisionGroup(std::static_pointer_cast<ChangeCollisionGroup>(message)->collisionGroup));
	
}

//---------------------------------------------------------

void CPhysicEntity::tick(unsigned int msecs) 
{
	// Invocar al método de la clase padre (IMPORTANTE)
	IComponent::tick(msecs);

	// Si es una entidad estática no hacemos nada
	PxRigidDynamic *dinActor = _actor->isRigidDynamic();
	if (!dinActor) 
		return;

	// Si el objeto físico es cinemático intentamos moverlo de acuerdo 
	// a los mensajes KINEMATIC_MOVE recibidos 
	if (actorController->isKinematic(dinActor)) {
		actorController->moveKinematicActor(dinActor, _movement, msecs);
		
		_movement = Vector3::ZERO;
	} 

	// Actualizar la posición y la orientación de la entidad lógica usando la 
	// información proporcionada por el motor de física	
	//_entity->setTransform(_server->getActorTransform(dinActor));
	// Para el nuevo sistema de mensajes
	_entity->setTransformN(actorController->getActorTransform(dinActor));

}

//---------------------------------------------------------

PxRigidActor* CPhysicEntity::createActor(const Map::CEntity *entityInfo)
{
	// Leer el tipo de entidad
	assert(entityInfo->hasAttribute("physic_entity"));
	const std::string physicEntity = entityInfo->getStringAttribute("physic_entity");
	assert((physicEntity == "rigid") || (physicEntity == "plane") || (physicEntity == "fromFile"));

	// Crear el tipo de entidad adecuada
	if (physicEntity == "plane") 
		return createPlane(entityInfo);
	
	if (physicEntity == "rigid") 
		return createRigid(entityInfo);
	
	if (physicEntity == "fromFile")
		return createFromFile(entityInfo);

	return NULL;
}

PxRigidStatic* CPhysicEntity::createPlane(const Map::CEntity *entityInfo)
{
	// La posición de la entidad es un punto del plano
	const Vector3 point = _entity->getPosition();
	
	// Leer el vector normal al plano
	assert(entityInfo->hasAttribute("physic_normal"));
	const Vector3 normal = entityInfo->getVector3Attribute("physic_normal");

	// Leer el grupo de colisión (por defecto grupo 0)
	int group = 0;
	if (entityInfo->hasAttribute("physic_group"))
		group = entityInfo->getIntAttribute("physic_group");
 
	// Crear el plano
	return actorFactory->createPlane(point, normal, group, this);
}

PxRigidActor* CPhysicEntity::createRigid(const Map::CEntity *entityInfo)
{
	// Leer la posición de la entidad
	const Vector3 position = _entity->getPosition();
	
	// Leer el tipo de entidad: estáticos, dinámico o cinemático
	assert(entityInfo->hasAttribute("physic_type"));
	const std::string physicType = entityInfo->getStringAttribute("physic_type");
	assert((physicType == "static") || (physicType == "dynamic") || (physicType == "kinematic"));

	// Leer la forma (shape)
	assert(entityInfo->hasAttribute("physic_shape"));
	const std::string physicShape = entityInfo->getStringAttribute("physic_shape");
	assert(physicShape == "box");

	// Leer si es un trigger (por defecto no)
	bool trigger = false;
	if (entityInfo->hasAttribute("physic_trigger"))
		trigger = entityInfo->getBoolAttribute("physic_trigger");

	// Leer el grupo de colisión (por defecto 0)
	int group = 0;
	if (entityInfo->hasAttribute("physic_group"))
		group = entityInfo->getIntAttribute("physic_group");


	if (physicType == "static") {
		if (physicShape == "box") {
			// Leer las dimensiones de la caja
			assert(entityInfo->hasAttribute("physic_dimensions"));
			const Vector3 physicDimensions = entityInfo->getVector3Attribute("physic_dimensions");
			
			// Crear una caja estática
			return actorFactory->createStaticBox(position, physicDimensions, trigger, group, this);
		}

	} else {
		// Leer la masa (por defecto 0)
		float mass = 0;
		if (entityInfo->hasAttribute("physic_mass"))
			mass = entityInfo->getFloatAttribute("physic_mass");
		
		// Leer si se trata de un actor cinemático
		bool kinematic = (physicType == "kinematic");

		if (physicShape == "box") {
			// Leer las dimensiones de la caja
			assert(entityInfo->hasAttribute("physic_dimensions"));
			const Vector3 physicDimensions = entityInfo->getVector3Attribute("physic_dimensions");
			
			// Crear una caja dinámica
			return actorFactory->createDynamicBox(position, physicDimensions, mass, kinematic, trigger, group, this);
		}
	}

	return NULL;
}

PxRigidActor* CPhysicEntity::createFromFile(const Map::CEntity *entityInfo)
{
	// Leer la ruta del fichero RepX
	assert(entityInfo->hasAttribute("physic_file"));
	const std::string file = entityInfo->getStringAttribute("physic_file");
	
	// Leer el grupo de colisión (por defecto 0)
	int group = 0;
	if (entityInfo->hasAttribute("physic_group"))
		group = entityInfo->getIntAttribute("physic_group");

	// Crear el actor a partir del fichero RepX
	return actorFactory->createFromFile(file, group, this);
}


void CPhysicEntity::onTrigger(IPhysics *otherComponent, bool enter)
{
	if (enter) {
		auto message = std::make_shared<OnTriggerEnter>();
		message->collisionEntity = otherComponent->getEntity();
		_entity->emitMessageN(message, this);
	} else {
		auto message = std::make_shared<OnTriggerExit>();
		message->collisionEntity = otherComponent->getEntity();
		_entity->emitMessageN(message, this);
	}
}

const Vector3 CPhysicEntity::getActorDimensions()
{
	physx::PxVec3 dimensions = _actor->getWorldBounds().getDimensions();
	return Vector3(dimensions.x, dimensions.y, dimensions.z);
}