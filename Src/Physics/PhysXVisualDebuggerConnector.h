/**
@file PhysXVisualDebuggerConnector.h

Contains the declaration of the PhysX Visual Debugger connector. 

@author Rayco Sánchez García
@date February, 2015
*/

#ifndef __Physics_PhysXVisualDebuggerConnector_H
#define __Physics_PhysXVisualDebuggerConnector_H

namespace physx {
	class PxPhysics;

	namespace debugger {
		namespace comm {
			class PvdConnection;
		};
	};
};


namespace Physics
{
	class PhysXVisualDebuggerConnector
	{
	public:

		/**
		@author Rayco Sánchez García
		@date February, 2015

		Function which tries to connect to PhysX
		Visual Debugger if application is on Debug.

		@param physics Object that allows to access to PhysX SDK.
		*/
		static void connectToPhysXVisualDebugger(physx::PxPhysics *physics);

		/**
		@author Rayco Sánchez García
		@date February, 2015

		Release the connection with PhysX Visual Debuger.

		*/
		static void releaseConnection();

	private:

		/**
		Object that stores the connection with PhysX Visual Debugger.
		*/
		static physx::debugger::comm::PvdConnection *pvdConnection;

		static const char *ip;

		static const int port;

		static const unsigned int timeout;

	};
} //namespace Physics

#endif // __Physics_PhysXVisualDebuggerConnector_H