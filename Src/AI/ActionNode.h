/**
@file ActionNode.h

Clase que representa la oleada.

@see ActionNode

@author Guidi Giacomo
@date March 2015

*/

#pragma once

#ifndef __AI_ActionNode_H
#define __AI_ActionNode_H

#include "BaseSubsystems/Math.h"
#include <memory>

class NMessage;

namespace AI {
	enum IActionStatus{
		Ready,
		OnStart,
		OnCheck,
		OnEnd,
		Ended,
	};

	/*
		This class manage a single ActionNode
	*/
	class IActionNode
	{
	public:
		IActionNode(std::string name);

		virtual ~IActionNode();
		/**
		Método virtual que elige que mensajes son aceptados.

		@param message Mensaje a chequear.
		@return true si el mensaje es aceptado.
		*/
		virtual bool acceptN(const std::shared_ptr<NMessage> &message){return false;};
		/**
		Método virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		virtual void processN(const std::shared_ptr<NMessage> &message){};

		void tick(unsigned int msecs);
		/*
		Function used to to some operation when the action started
		@return generally have to return IActionStatus::OnStart for normally execution of the action,
				could be return another state to manage exception
		*/
		virtual IActionStatus onStart(){return IActionStatus::OnStart;};
		/*
		Function used tocheck if the action is ended
		@return true if action is ended, false otherwise
		*/
		virtual bool onCheck(unsigned int msecs){return true;};

		/*
		Function used to to some operation when the action started
		@return generally have to return IActionStatus::OnStart for normally execution of the action,
				could be return another state to manage exception
		*/
		virtual IActionStatus onEnd();

		IActionStatus getStatus(){return _status;};

		/*
		Method that add an action on start

		@param w the action to add
		*/
		void addStartActionNode(IActionNode *w);
		/*
		Method that add an asyncronous action on start of this action

		@param w the action to add
		*/
		void addStartAsyncActionNode(IActionNode *w);
		/*
		Method that add an action on end

		@param w the action to add
		*/
		void addEndActionNode(IActionNode *w);
		/*
		Method that add an asyncronous action on the end of this action

		@param w the action to add
		*/
		void addEndAsyncActionNode(IActionNode *w);
		
		std::string getName(){return _name;};
	protected:
		/*
			List of ActionNode phase that compound the entire ActionNode
		*/
		std::list<IActionNode*> *_startActionList;
		/*
			List of ActionNode phase that compound the entire ActionNode
		*/
		std::list<IActionNode*> *_startAsyncActionList;
		/*
			List of ActionNode phase that compound the entire ActionNode
		*/
		std::list<IActionNode*> *_endActionList;
		/*
			List of ActionNode phase that compound the entire ActionNode
		*/
		std::list<IActionNode*> *_endAsyncActionList;

		IActionStatus _status;

		void startAction(std::list<IActionNode*>* list);

		std::string _name;

		void freeList(std::list<IActionNode*> *list);
		/*
			list of action that could be created and lauched at runtime into an action
		*/
		std::list<IActionNode*> _runtimeCreatedAction;
		/*
			function that remove the action created at tuntime
		*/
		void freeRuntimeAction();



	};
	
} // namespace AI

#endif __AI_ActionNodeManager_H
