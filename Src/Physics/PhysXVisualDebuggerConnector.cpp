/**
@file PhysXVisualDebuggerConnector.cpp

Contains the implementation of the PhysX Visual Debugger connector. 

@see Physics::CServer

@author Rayco Sánchez García
@date February, 2015
*/

#include "PhysXVisualDebuggerConnector.h"

#include <assert.h>

#include <pvd\PxVisualDebugger.h>
#include <extensions\PxVisualDebuggerExt.h>
#include <PxPhysicsAPI.h>
#include <extensions\PxExtensionsAPI.h> 

using namespace Physics;


physx::debugger::comm::PvdConnection* PhysXVisualDebuggerConnector::pvdConnection = nullptr;

//-------------------------------------------------------

const char* PhysXVisualDebuggerConnector::ip = "127.0.0.1";

//-------------------------------------------------------

const int PhysXVisualDebuggerConnector::port  = 5425;

//-------------------------------------------------------

const unsigned int PhysXVisualDebuggerConnector::timeout = 100;

//-------------------------------------------------------

void PhysXVisualDebuggerConnector::connectToPhysXVisualDebugger(physx::PxPhysics *physics)
{
	physx::debugger::comm::PvdConnectionManager *pvdConnectionManager = physics->getPvdConnectionManager();
	assert(pvdConnectionManager && "Error en PxPhysics::getPvdConnectionManager");	
	
	/**
	Object that stores flags about which information is
	going to be sent to PhysX Visual Debugger (Debug, profile, memory).
	*/
	static const physx::PxVisualDebuggerConnectionFlags connectionFlags = physx::PxVisualDebuggerExt::getAllConnectionFlags();

	pvdConnection = physx::PxVisualDebuggerExt::createConnection(pvdConnectionManager, ip, port, timeout, connectionFlags);
	physics->getVisualDebugger()->setVisualDebuggerFlag(physx::PxVisualDebuggerFlag::eTRANSMIT_SCENEQUERIES, true);
}

//-------------------------------------------------------

void PhysXVisualDebuggerConnector::releaseConnection()
{
	if (pvdConnection)
	{
		pvdConnection->release();
		pvdConnection = nullptr;
	}
}