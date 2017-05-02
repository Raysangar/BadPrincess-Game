#pragma once
#ifndef __AI_LAWorkmanInteractSO_H
#define __AI_LAWorkmanInteractSO_H

#include "LatentAction.h"


namespace Logic
{
	class CWorkmanState;
	class CInteractSOState;
}


namespace AI 
{
	class CLAWorkmanInteractSO : public CLatentAction
	{
	private:

		Logic::CInteractSOState* interactComponent;
		/**
		Componente que almacena el estado de la unidad.
		*/
		Logic::CWorkmanState* stateComponent;
		/**
		smartEntity que la unidad está utilizando.
		*/
		Logic::CEntity* smartEntity;
		/**
		Tiempo que la unidad lleva recolectando.
		*/
		unsigned int currentTimeGather;
		/**
		Animación corresponiente a la acción que se realiza.
		*/
		std::string animation;

	public:
		CLAWorkmanInteractSO(Logic::CEntity* entity) :animation("Idle"), currentTimeGather(0), stateComponent(nullptr), 
			interactComponent(nullptr), smartEntity(nullptr), CLatentAction() {this->setEntity(entity);};

		~CLAWorkmanInteractSO(){};

		virtual bool acceptN(const std::shared_ptr<NMessage> &message);
		virtual void processN(const std::shared_ptr<NMessage> &message);

	protected:
		virtual LAStatus OnStart();
		virtual void OnStop();
		virtual LAStatus OnRun(unsigned int msecs);
		virtual LAStatus OnAbort();
		void lookAtSmartObject();
		void goToChest();
		bool checkGatherMaxReached();

	};
} // namespace AI

#endif // __AI_LAWorkmanInteractSO_H