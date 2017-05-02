#pragma once
#ifndef __AI_LARequestSP_H
#define __AI_LARequestSP_H

#include "LatentAction.h"

namespace Logic
{
	class CWorkmanState;
}


namespace AI 
{
	class CLARequestSP : public CLatentAction
	{
	private:
		/**
		Componente que almacena el estado de la unidad.
		*/
		Logic::CWorkmanState* stateComponent;
		/**
		smartEntity que la unidad está utilizando.
		*/
		Logic::CEntity* smartEntity;
	public:
		CLARequestSP(Logic::CEntity* entity) : stateComponent(nullptr), smartEntity(nullptr), CLatentAction() {this->setEntity(entity);};
		~CLARequestSP() {};

		virtual bool acceptN(const std::shared_ptr<NMessage> &message);
		virtual void processN(const std::shared_ptr<NMessage> &message);

	protected:
		virtual LAStatus OnStart();
		virtual LAStatus OnRun(unsigned int msecs);
		virtual LAStatus OnAbort();
	};
} // namespace AI

#endif // __AI_LARequestSP_H