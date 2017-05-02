/**
@file PhysicController.cpp

Contiene la implementaci�n del componente que se utiliza para representar jugadores y enemigos en
el mundo f�sico usando character controllers.

@see Logic::CPhysicController
@see Logic::CPhysicEntity
@see Logic::IPhysics

@author Antonio S�nchez Ruiz-Granados
@date Noviembre, 2012
*/

#include "PhysicController.h"

#include "Logic/MovementMessages.h"
#include "Logic/PhysicMessages.h"
#include "Logic/Entity/Entity.h"
#include "SteeringMovement.h"
#include "UnitSelected.h"
#include "Map/MapEntity.h"
#include "Physics/Server.h"
#include "Physics/PhysicControllerManager.h"
#include "Physics/ActorController.h"
#include "Physics/Conversions.h"
#include "Physics/SceneManager.h"
#include <PxPhysicsAPI.h>

using namespace Logic;
using namespace Physics;
using namespace physx; 

#define PUSH_FORCE 4
IMP_FACTORY(CPhysicController);

//---------------------------------------------------------

CPhysicController::CPhysicController(std::string componentName) : IPhysics(componentName), _controller(NULL), _movement(0,0,0), _falling(false)
{
	physicControllerManager = CServer::getSingletonPtr()->getPhysicControllerManager();
	actorController = CServer::getSingletonPtr()->getActorController();
}

CPhysicController::~CPhysicController() 
{
	if (_controller) {
		_controller->release();
		_controller = NULL;
	}

	physicControllerManager = NULL;
} 

//---------------------------------------------------------

bool CPhysicController::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo)
{
	// Invocar al m�todo de la clase padre
	if(!IComponent::spawn(entity,map,entityInfo))
		return false;

	// Crear el character controller asociado al componente
	_controller = createController(entityInfo);
		
	_unitSelectedComponent = static_cast<Logic::CUnitSelected*>(_entity->getComponent("CUnitSelected"));
	return true;
}

//---------------------------------------------------------

void CPhysicController::awake(){
	_movement=Vector3::ZERO;
	_falling=false;
}

bool CPhysicController::activate()
{
	physicControllerManager->setControllerPosition(_controller,_entity->getPosition());
	return true;
}

//---------------------------------------------------------

void CPhysicController::deactivate()
{
	//this is for when the entity go to delete list, it mustn't affect the scene put it far far away 
	physicControllerManager->setControllerPosition(_controller,Vector3(1000,-1000,1000));
}
//---------------------------------------------------------

bool CPhysicController::acceptN(const std::shared_ptr<NMessage> &message)
{
	return (message->type.compare("AvatarMove") == 0) || message->type == "ChangeCollisionGroup" || message->type == "SetPhysicControllerPosition";
}

//---------------------------------------------------------

void CPhysicController::processN(const std::shared_ptr<NMessage> &message)
{
	if (message->type == "ChangeCollisionGroup")
		Physics::CServer::getSingletonPtr()->setupFiltering(_controller->getActor(), 
			Physics::CollisionGroup(std::static_pointer_cast<ChangeCollisionGroup>(message)->collisionGroup));
	else if (message->type == "AvatarMove")
		_movement = std::static_pointer_cast<AvatarMove>(message)->movement;
	else if (message->type == "SetPhysicControllerPosition")
	{
		std::shared_ptr<SetPhysicControllerPosition> m = std::static_pointer_cast<SetPhysicControllerPosition>(message);
		physicControllerManager->setControllerPosition(_controller,m->position);
	}

}

//---------------------------------------------------------

void CPhysicController::tick(unsigned int msecs) 
{
	// Llamar al m�todo de la clase padre (IMPORTANTE).
	IComponent::tick(msecs);

	// Actualizar la posici�n y orientaci�n de la entidad l�gica usando la 
	// informaci�n proporcionada por el motor de f�sica	
	//_entity->setPosition(_server->getControllerPosition(_controller), this);
	// Para el nuevo sistema de mensajes
	_entity->setPositionN(physicControllerManager->getControllerPosition(_controller), this);

	// Si estamos cayendo modificar el vector de desplazamiento para simular el 
	// efecto de la gravedad. Lo hacemos de manera sencilla y pero poco realista.
	if (_falling) {
		_movement += Vector3(0,-1,0);
	}

	// Intentamos mover el controller a la posici�n recibida en el �ltimo mensaje 
	// de tipo AVATAR_WALK. 
	unsigned flags = physicControllerManager->moveController(_controller, _movement, msecs);

	// Actualizamos el flag que indica si estamos cayendo
	_falling =  !(flags & PxControllerFlag::eCOLLISION_DOWN);

	// Ponemos el movimiento a cero
	_movement = Vector3::ZERO;
}

//---------------------------------------------------------

PxCapsuleController* CPhysicController::createController(const Map::CEntity *entityInfo)
{
	// Obtenemos la posici�n de la entidad. Inicialmente colocaremos el controller
	// un poco por encima del suelo, porque si lo ponemos justo en el suelo a veces
	// lo atraviesa en el primer ciclo de simulaci�n.
	Vector3 position = _entity->getPosition() + Vector3(0, 0.5f, 0);
	
	// Leer el volumen de colisi�n del controller. Por ahora s�lo admitimos c�psulas.
	std::string shape = "capsule";
	if (entityInfo->hasAttribute("physic_shape")) {
		shape = entityInfo->getStringAttribute("physic_shape");
		assert(shape == "capsule");
	}

	// Leer el radio de la c�psula
	assert(entityInfo->hasAttribute("physic_radius"));
	float radius = entityInfo->getFloatAttribute("physic_radius");

	// Leer la altura de la c�psula
	assert(entityInfo->hasAttribute("physic_height"));
	float height = entityInfo->getFloatAttribute("physic_height");

	// Leer el grupo de fisica
	assert(entityInfo->hasAttribute("physic_group"));
	int group = entityInfo->getIntAttribute("physic_group");
	// Crear el controller de tipo c�psula
	return physicControllerManager->createCapsuleController(position, radius, height,group,this);
} 

//---------------------------------------------------------

void CPhysicController::onTrigger(IPhysics *otherComponent, bool enter)
{

}

//---------------------------------------------------------

void CPhysicController::onShapeHit (const PxControllerShapeHit &hit)
{
	// Si chocamos contra una entidad est�tica no hacemos nada
	PxRigidDynamic* actor = hit.shape->getActor()->isRigidDynamic();
	if(!actor)
		return;

	// Si chocamos contra una entidad cinem�tica no hacemos nada
	if (actorController->isKinematic(actor))
		return;
	
	// Aplicar una fuerza a la entidad en la direcci�n del movimiento
	actor->addForce(hit.dir * hit.length * 1000.0f);
}

//---------------------------------------------------------

void CPhysicController::onControllerHit (const PxControllersHit &hit)
{
	//// Obtener el componente l�gico asociado a la otra entidad f�sica
	//IPhysics *otherComponent = (IPhysics *) hit.other->getUserData();

	//if(_entity->isPlayer() && _unitSelectedComponent->isSelected() && otherComponent->getEntity()->isPlayer())
	//{
	//	Logic::CSteeringMovement *_steeringComponent = static_cast<Logic::CSteeringMovement*>(otherComponent->getEntity()->getComponent("CSteeringMovement"));
	//	if(_steeringComponent->getCurrentSpeed() == Vector3::ZERO){
	//		Vector3 dir = _entity->getPosition() - otherComponent->getEntity()->getPosition();
	//		auto m = std::make_shared<AvatarMove>();
	//		m->movement = dir.normalisedCopy() * PUSH_FORCE;		
	//		otherComponent->getEntity()->emitMessageN(m);
	//	}
	//}
}


//---------------------------------------------------------



