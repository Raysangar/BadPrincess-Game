#pragma once
#ifndef __AI_LAThiefDecision_H
#define __AI_LAThiefDecision_H

#include "AI\LatentAction.h"

namespace Logic
{
	class CInteractSOState;
}

namespace AI 
{
	class CLAThiefDecision : public CLatentAction
	{
	private:
		/**
		Componente que almacena el estado de la unidad.
		*/
		Logic::CInteractSOState* interactComponent;
		/**
		Entidad que la unidad está utilizando para decidir.
		*/
		Logic::CEntity* decisionEntity;
	public:
		CLAThiefDecision(Logic::CEntity* entity) : interactComponent(nullptr), decisionEntity(nullptr), CLatentAction() {this->setEntity(entity);}
		~CLAThiefDecision() {};

		virtual bool acceptN(const std::shared_ptr<NMessage> &message);
		virtual void processN(const std::shared_ptr<NMessage> &message);

	protected:
		virtual LAStatus OnStart();
	private:
		void goNearestResource(float distance);
	};
} // namespace AI

#endif // __AI_LARequestSP_H