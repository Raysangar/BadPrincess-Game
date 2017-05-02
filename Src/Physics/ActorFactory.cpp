/**
@file ActorFactory.cpp

Contains the implementation of the ActorController. 

@author Rayco S�nchez Garc�a
@date February, 2015
*/

#include "ActorFactory.h"
#include "SceneManager.h"
#include "PhysXServer.h"
#include "Conversions.h"
#include "Physics\Server.h"

#include <assert.h>

#include <PxPhysicsAPI.h>
#include <extensions\PxExtensionsAPI.h> 

using namespace Physics;
using namespace physx;


ActorFactory::ActorFactory(PhysXServer* physXServer) : physXServer(physXServer) {
	_collisionManager = physXServer->_collisionManager;
}

//--------------------------------------------------------

ActorFactory::~ActorFactory() {}

//--------------------------------------------------------

PxRigidStatic* ActorFactory::createPlane(const Vector3 &point, const Vector3 &normal, int group, 

	                                const Logic::IPhysics *component)
{

	// Crear un plano est�tico
	PxPlane plane(Vector3ToPxVec3(point), Vector3ToPxVec3(normal));
	PxRigidStatic *actor = PxCreatePlane(*physXServer->_physics, plane, *physXServer->_defaultMaterial);
	
	// Anotar el componente l�gico asociado a la entidad f�sica
	actor->userData = (void *) component;

	// Establecer el grupo de colisi�n
	//PxSetGroup(*actor, group);
	CServer::getSingletonPtr()->setupFiltering(actor,CollisionGroup(group));

	return actor;
}

//--------------------------------------------------------

PxRigidStatic* ActorFactory::createStaticBox(const Vector3 &position, const Vector3 &dimensions, bool trigger, 

	                                    int group, const Logic::IPhysics *component)
{

	// Nota: PhysX coloca el sistema de coordenadas local en el centro de la caja, mientras
	// que la l�gica asume que el origen del sistema de coordenadas est� en el centro de la 
	// cara inferior. Para unificar necesitamos realizar una traslaci�n en el eje Y.
	// Afortunadamente, el descriptor que se usa para crear el actor permite definir esta 
	// transformaci�n local, por lo que la conversi�n entre sistemas de coordenadas es transparente. 
	
	// Crear un cubo est�tico
	PxTransform pose(Vector3ToPxVec3(position));
	PxBoxGeometry geom(Vector3ToPxVec3(dimensions));
	PxTransform localPose(PxVec3(0, dimensions.y, 0)); // Transformaci�n de coordenadas l�gicas a coodenadas de PhysX
	PxRigidStatic *actor = PxCreateStatic(*physXServer->_physics, pose, geom, *physXServer->_defaultMaterial, localPose);
	
	// Transformarlo en trigger si es necesario
	if (trigger) {
		PxShape *shape;
		actor->getShapes(&shape, 1, 0);
		shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
		shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
	}

	// Anotar el componente l�gico asociado a la entidad f�sica
	actor->userData = (void *) component;

	// Establecer el grupo de colisi�n
	//PxSetGroup(*actor, group);
	CServer::getSingletonPtr()->setupFiltering(actor,CollisionGroup(group));


	return actor;
}

//--------------------------------------------------------

PxRigidDynamic* ActorFactory::createDynamicBox(const Vector3 &position, const Vector3 &dimensions, 
	                                      float mass, bool kinematic, bool trigger, int group, 

										  const Logic::IPhysics *component)
{

	// Nota: PhysX coloca el sistema de coordenadas local en el centro de la caja, mientras
	// que la l�gica asume que el origen del sistema de coordenadas est� en el centro de la 
	// cara inferior. Para unificar necesitamos realizar una traslaci�n en el eje Y.
	// Afortunadamente, el descriptor que se usa para crear el actor permite definir esta 
	// transformaci�n local, por lo que la conversi�n entre sistemas de coordenadas es transparente. 

	// Crear un cubo din�mico
	PxTransform pose(Vector3ToPxVec3(position));
	PxBoxGeometry geom(Vector3ToPxVec3(dimensions));

	float density = mass / (dimensions.x * dimensions.y * dimensions.z);
	PxTransform localPose(PxVec3(0, dimensions.y, 0)); // Transformaci�n de coordenadas l�gicas a coodenadas de PhysX

	// Crear cubo din�mico o cinem�tico
	PxRigidDynamic *actor;
	if (kinematic)
		actor = PxCreateKinematic(*physXServer->_physics, pose, geom, *physXServer->_defaultMaterial, density, localPose);
	else
		actor = PxCreateDynamic(*physXServer->_physics, pose, geom, *physXServer->_defaultMaterial, density, localPose);
	
	// Transformarlo en trigger si es necesario
	if (trigger) {
		PxShape *shape;
		actor->getShapes(&shape, 1, 0);
		shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
		shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
	}

	// Anotar el componente l�gico asociado a la entidad f�sica
	actor->userData = (void *) component;

	// Establecer el grupo de colisi�n
	//PxSetGroup(*actor, group);
	CServer::getSingletonPtr()->setupFiltering(actor,CollisionGroup(group));

	return actor;
}

//--------------------------------------------------------

PxRigidDynamic* ActorFactory::createDynamicSphere(const Vector3 &position, float radius, 
	                                      float mass, bool kinematic, bool trigger, int group, 

										  const Logic::IPhysics *component)
{

	// Nota: PhysX coloca el sistema de coordenadas local en el centro de la caja, mientras
	// que la l�gica asume que el origen del sistema de coordenadas est� en el centro de la 
	// cara inferior. Para unificar necesitamos realizar una traslaci�n en el eje Y.
	// Afortunadamente, el descriptor que se usa para crear el actor permite definir esta 
	// transformaci�n local, por lo que la conversi�n entre sistemas de coordenadas es transparente. 

	// Crear un cubo din�mico
	PxTransform pose(Vector3ToPxVec3(position));
	PxReal r = radius;
	PxSphereGeometry geom(r);

	float density = mass / ((4 * Math::PI * pow(radius,3)/3));
	PxTransform localPose(PxVec3(0, radius, 0)); // Transformaci�n de coordenadas l�gicas a coodenadas de PhysX

	// Crear cubo din�mico o cinem�tico
	PxRigidDynamic *actor;
	if (kinematic)
		actor = PxCreateKinematic(*physXServer->_physics, pose, geom, *physXServer->_defaultMaterial, density, localPose);
	else
		actor = PxCreateDynamic(*physXServer->_physics, pose, geom, *physXServer->_defaultMaterial, density, localPose);
	
	// Transformarlo en trigger si es necesario
	if (trigger) {
		PxShape *shape;
		actor->getShapes(&shape, 1, 0);
		shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
		shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
	}

	// Anotar el componente l�gico asociado a la entidad f�sica
	actor->userData = (void *) component;

	// Establecer el grupo de colisi�n
	//PxSetGroup(*actor, group);
	CServer::getSingletonPtr()->setupFiltering(actor,CollisionGroup(group));


	return actor;
}


PxRigidActor* ActorFactory::createFromFile(const std::string &file, int group, const Logic::IPhysics *component)
{

	// Preparar par�metros para deserializar
	PxSerializationRegistry* registry = PxSerialization::createSerializationRegistry(*physXServer->_physics); 
	PxDefaultFileInputData data(file.c_str());
	PxCollection* collection;

	// Deserializar a partir del fichero RepX
	collection = PxSerialization::createCollectionFromXml(data, *physXServer->_cooking, *registry);
	
	// A�adir entidades f�sicas a la escena
	//sceneManager->scene->addCollection(*collection); 
	
	// Buscar una entidad de tipo PxRigidActor. Asumimos que hay exactamente 1 en el fichero.
	PxRigidActor *actor = NULL;
	for (unsigned int i=0; (i<collection->getNbObjects()) && !actor; i++) {
		actor = collection->getObject(i).is<PxRigidActor>();		
	}
	assert(actor);
	
	// Anotar el componente l�gico asociado a la entidad f�sica
	actor->userData = (void *) component;

	// Establecer el grupo de colisi�n
	//PxSetGroup(*actor, group);
	CServer::getSingletonPtr()->setupFiltering(actor,CollisionGroup(group));
	// Liberar recursos
	collection->release();
	registry->release();

	return actor;
}

//--------------------------------------------------------

void ActorFactory::destroyActor(physx::PxActor *actor)
{
	actor->release();
}
