#pragma once
#ifndef __AI_LAGoTo_H
#define __AI_LAGoTo_H

#include "LatentAction.h"


namespace AI 
{
	class CLAGoTo : public CLatentAction
	{
	private:
		Vector3 _target;
		Logic::CEntity *currentSmartEntity;
	public:
		CLAGoTo(Logic::CEntity* entity, Vector3 &target) : _target(target), currentSmartEntity(nullptr), CLatentAction() {this->setEntity(entity);};
		~CLAGoTo() {};

		virtual bool acceptN(const std::shared_ptr<NMessage> &message);
		virtual void processN(const std::shared_ptr<NMessage> &message);

	protected:
		virtual LAStatus OnStart();
		virtual LAStatus OnRun() ;
	};
} // namespace AI

#endif // __AI_LAGoTo_H