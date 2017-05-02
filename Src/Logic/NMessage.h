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
	ClassName(const F1type &F1name) : NMessage(#ClassName, sender),F1name(F1name) {};\
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
	ClassName(const F1type &F1name,const F2type &F2name) : NMessage(#ClassName, sender),F1name(F1name),F2name(F2name) {};\
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
	ClassName(const F1type &F1name,const F2type &F2name,const F3type &F3name) : NMessage(#ClassName, sender), F1name(F1name),F2name(F2name),F3name(F3name) {};\
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
#define DECLARE_MESSAGE_CLASS4P(ClassName,F1type,F1name,F2type,F2name,F3type,F3name,F4type,F4name) \
	class ClassName : public NMessage\
	{ \
	public: \
	ClassName() : NMessage(#ClassName, nullptr) {}; \
	ClassName(Logic::CEntity* sender) : NMessage(#ClassName, sender){};\
	ClassName(const F1type &F1name,const F2type &F2name,const F3type &F3name,const F4type &F4name) : NMessage(#ClassName, sender), F1name(F1name),F2name(F2name),F3name(F3name),F4name(F4name) {};\
	~ClassName(){};\
	ClassName(const ClassName& msg) :NMessage(msg), F1name(msg.F1name),F2name(msg.F2name),F3name(msg.F3name),F4name(msg.F4name) {};\
	ClassName& operator=(const ClassName& msg)\
	{\
	F1name = msg.F1name;\
	F2name = msg.F2name;\
	F3name = msg.F3name;\
	F4name = msg.F4name;\
	return *this;\
	}\
	F1type F1name; \
	F2type F2name; \
	F3type F3name; \
	F4type F4name; \
	};


#endif