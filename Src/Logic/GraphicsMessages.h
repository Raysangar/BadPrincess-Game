#ifndef _Logic_GraphicsMessages_H
#define _Logic_GraphicsMessages_H

#include "Logic\NMessage.h"
namespace Graphics
{
	class CFrustumEntity;
}

typedef  Graphics::CFrustumEntity* CFrustumEntityPtr;

	DECLARE_MESSAGE_CLASS3P(AreaOfEffect,std::string, entityName, std::string, materialToProject,CFrustumEntityPtr, frustum)
	DECLARE_MESSAGE_CLASS1P(PointDestination, Vector3, destination)	
	DECLARE_MESSAGE_CLASS1P(ToggleGlow, bool, activate)	
	DECLARE_MESSAGE_CLASS0P(SwitchFOW)
	DECLARE_MESSAGE_CLASS1P(PlayParticle, std::string, particleName)
	DECLARE_MESSAGE_CLASS2P(ScaleParticle, std::string, particleName,float, scale)
	DECLARE_MESSAGE_CLASS1P(StopParticle, std::string, particleName)
	DECLARE_MESSAGE_CLASS1P(PauseParticle, std::string, particleName)	
	DECLARE_MESSAGE_CLASS0P(Cover)
	DECLARE_MESSAGE_CLASS0P(Uncover)

#endif
