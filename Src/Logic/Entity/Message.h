#ifndef __Logic_Message_H
#define __Logic_Message_H

#include "BaseSubsystems/Math.h"

namespace Logic{
	class CEntity;
}


class NMessage
{
public:
	NMessage(): type(""), sender(nullptr){};
	NMessage(std::string messageType, Logic::CEntity* sender): type(messageType), sender(sender){};
	~NMessage(){};
	std::string type;
	Logic::CEntity * sender;
};
typedef Logic::CEntity* CEntityPtr;

#define DECLARE_MESSAGE_CLASS0P(ClassName) \
class ClassName : public NMessage\
{ \
public: \
	ClassName() : NMessage(#ClassName, nullptr) {}; \
	ClassName(Logic::CEntity* sender) : NMessage(#ClassName, sender){};\
	~ClassName(){};\
	ClassName(const ClassName& msg):NMessage(msg) {};\
	ClassName& operator=(const ClassName& msg)\
	{\
		return *this;\
	}\
};

#define DECLARE_MESSAGE_CLASS1P(ClassName,F1type,F1name) \
class ClassName : public NMessage\
{ \
public: \
	ClassName() : NMessage(#ClassName, nullptr) {}; \
	ClassName(Logic::CEntity* sender) : NMessage(#ClassName, sender){};\
	ClassName(const F1type &F1name) : F1name(F1name) {};\
	~ClassName(){};\
	ClassName(const ClassName& msg) :NMessage(msg), F1name(msg.F1name) {};\
	ClassName& operator=(const ClassName& msg)\
	{\
		F1name = msg.F1name;\
		return *this;\
	}\
	F1type F1name; \
};
#define DECLARE_MESSAGE_CLASS2P(ClassName,F1type,F1name,F2type,F2name) \
class ClassName : public NMessage\
{ \
public: \
	ClassName() : NMessage(#ClassName, nullptr) {}; \
	ClassName(Logic::CEntity* sender) : NMessage(#ClassName, sender){};\
	ClassName(const F1type &F1name,const F2type &F2name) : F1name(F1name),F2name(F2name) {};\
	~ClassName(){};\
	ClassName(const ClassName& msg) : NMessage(msg), F1name(msg.F1name),F2name(msg.F2name) {};\
	ClassName& operator=(const ClassName& msg)\
	{\
		F1name = msg.F1name;\
		F2name = msg.F2name;\
		return *this;\
	}\
	F1type F1name; \
	F2type F2name; \
};
#define DECLARE_MESSAGE_CLASS3P(ClassName,F1type,F1name,F2type,F2name,F3type,F3name) \
class ClassName : public NMessage\
{ \
public: \
	ClassName() : NMessage(#ClassName, nullptr) {}; \
	ClassName(Logic::CEntity* sender) : NMessage(#ClassName, sender){};\
	ClassName(const F1type &F1name,const F2type &F2name,const F3type &F3name) : F1name(F1name),F2name(F2name),F3name(F3name) {};\
	~ClassName(){};\
	ClassName(const ClassName& msg) :NMessage(msg), F1name(msg.F1name),F2name(msg.F2name),F3name(msg.F3name) {};\
	ClassName& operator=(const ClassName& msg)\
	{\
		F1name = msg.F1name;\
		F2name = msg.F2name;\
		F3name = msg.F3name;\
		return *this;\
	}\
	F1type F1name; \
	F2type F2name; \
	F3type F3name; \
};
DECLARE_MESSAGE_CLASS1P(TMVida,int,vida)
DECLARE_MESSAGE_CLASS2P(TMMove,Vector3,position,int,direction)
DECLARE_MESSAGE_CLASS3P(TMTest,Vector3,position,int,direction,std::string,name)
DECLARE_MESSAGE_CLASS2P(AvatarControl,std::string,controlType,float,attribute)
DECLARE_MESSAGE_CLASS1P(TransformMessage,Matrix4,transform)
DECLARE_MESSAGE_CLASS1P(CameraControl, Vector3, direction)
DECLARE_MESSAGE_CLASS1P(KinematicMove, Vector3, movement)
DECLARE_MESSAGE_CLASS2P(SetAnimation, std::string, name, bool, cycle)
DECLARE_MESSAGE_CLASS1P(StopAnimation, std::string, animationName)
DECLARE_MESSAGE_CLASS1P(AvatarMove, Vector3, movement)
DECLARE_MESSAGE_CLASS1P(AvatarSelection, bool, selected)
DECLARE_MESSAGE_CLASS2P(AvatarMoveToPosition, Vector3, target, int, moveType)
DECLARE_MESSAGE_CLASS1P(AvatarMoveToEntity, CEntityPtr, target)
DECLARE_MESSAGE_CLASS1P(AvatarFinishedMove, bool, finished)
DECLARE_MESSAGE_CLASS2P(AvatarRouteTo, bool, calculateNewRoute,Vector3,target)
DECLARE_MESSAGE_CLASS1P(AvatarFinishedRoute, bool, finished)
DECLARE_MESSAGE_CLASS1P(SmartDataPositionRequest, CEntityPtr, entity);
DECLARE_MESSAGE_CLASS1P(SmartDataAvatarReady, CEntityPtr, entity);
DECLARE_MESSAGE_CLASS2P(SmartDataPositionReceiver, CEntityPtr, smartObject,Vector3,position)
DECLARE_MESSAGE_CLASS2P(SmartDataAction, std::string, animation,int,actionType)
DECLARE_MESSAGE_CLASS1P(GatherMessage,int, quantity)
DECLARE_MESSAGE_CLASS2P(StoreResource,int,resourceType,int, quantity)
DECLARE_MESSAGE_CLASS2P(SmartDataFreePosition, CEntityPtr, entity,Vector3,position)
DECLARE_MESSAGE_CLASS1P(ResourceAutoDestroy,CEntityPtr, nextItem)
DECLARE_MESSAGE_CLASS0P(ToggleDrawPhysicDebug)
DECLARE_MESSAGE_CLASS1P(OnTriggerEnter, CEntityPtr, collisionEntity);
DECLARE_MESSAGE_CLASS1P(OnTriggerExit, CEntityPtr, collisionEntity);
DECLARE_MESSAGE_CLASS1P(DamageMessage, int, damage);
DECLARE_MESSAGE_CLASS2P(SetTrap, Vector3, position, std::string, trapName);
DECLARE_MESSAGE_CLASS1P(CastAbility, std::string, ability);
DECLARE_MESSAGE_CLASS1P(SetOwner, CEntityPtr, owner);
DECLARE_MESSAGE_CLASS1P(TrapDestroyed, std::string, trapType);

#endif