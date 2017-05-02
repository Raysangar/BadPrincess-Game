/**
@file ActionNode.cpp

Clase istancia de una oleada.

@see ActionNode

@author Guidi Giacomo 
@date March 2015

*/
#include "ActionNode.h"
#include "AI\Server.h"
#include "AI\GameManager.h"

namespace AI {
	IActionNode::IActionNode(std::string name): _name(name),_status(IActionStatus::Ready)
	{
		_startActionList= new std::list<IActionNode*>();
		_endActionList= new std::list<IActionNode*>();
		_startAsyncActionList= new std::list<IActionNode*>();
		_endAsyncActionList= new std::list<IActionNode*>();
	};

	IActionNode::~IActionNode()
	{
		freeList(_startActionList);
		freeList(_endActionList);
		freeList(_startAsyncActionList);
		freeList(_endAsyncActionList);
		freeRuntimeAction();
	}

	
	void IActionNode::tick(unsigned int msecs)
	{
		//if is the first execution call the init function onStart
		if(_status==IActionStatus::Ready)
		{
			_status = onStart();
			if(_status==IActionStatus::OnStart)
			{
				//At the first activation of Action launch the async operation
				startAction(_startAsyncActionList);
//				_startAsyncActionList->clear();
				startAction(_startActionList);

			}
		}

		if(_status==IActionStatus::OnStart)
		{	
			
			//check if all action that i have to do before start check operation are finished
			std::list<IActionNode*>::iterator cit=_startActionList->begin();
			bool ended=true;
			while(cit!=_startActionList->end() && ended)
			{
				if((*cit)->getStatus()!=IActionStatus::Ended)
					ended=false;
				++cit;
			}

			//when all the synchronous action in start state are finished go to check
			if(ended)
				_status = IActionStatus::OnCheck;
		}
		//check the condition
		if(_status==IActionStatus::OnCheck)
		{
			if(onCheck(msecs))
			{
				_status = IActionStatus::OnEnd;
			//the first time that the action go to end status launch the async operation
				startAction(_endAsyncActionList);
//				_endAsyncActionList->clear();

				startAction(_endActionList);

			}
		}
		//when the condition is cumplied
		if(_status==IActionStatus::OnEnd)
		{	
			//if i have to do some synchronous opereation before definetly end the action
			std::list<IActionNode*>::iterator cit=_endActionList->begin();
			bool ended=true;
			while(cit!=_endActionList->end() && ended)
			{
				if((*cit)->getStatus()!=IActionStatus::Ended)
					ended=false;
				++cit;
			}
			if(ended)
				_status = onEnd();//call the function to end the action, this have to return Ended status to end definetly the action
		}

	}

	IActionStatus IActionNode::onEnd()
	{
		//delete the action because this action don't be created into LUA	
		freeRuntimeAction();
		return IActionStatus::Ended;
	}


	void IActionNode::startAction(std::list<IActionNode*>* list)
	{
			if(!list->empty())
			{
				std::list<IActionNode*>::iterator cit=list->begin();
				for(;cit!=list->end();++cit)
					AI::CServer::getSingletonPtr()->getGameManager()->executeAction(*cit);
			}
	}
	
	void IActionNode::addStartActionNode(IActionNode *w)
	{
		_startActionList->push_back(w);
	}
	
	void IActionNode::addEndActionNode(IActionNode *w)
	{
		_endActionList->push_back(w);
	}
	void IActionNode::addStartAsyncActionNode(IActionNode *w)
	{
		_startAsyncActionList->push_back(w);
	}
	
	void IActionNode::addEndAsyncActionNode(IActionNode *w)
	{
		_endAsyncActionList->push_back(w);
	}

	void IActionNode::freeList(std::list<IActionNode*> *list)
	{
		if(list!=nullptr)
		{
			std::list<IActionNode*>::const_iterator cit=list->begin();
			for(;cit!=list->end();++cit)
				delete *cit;
			delete list;
			list=nullptr;
		}
	}

	void IActionNode::freeRuntimeAction()
	{
		if(_runtimeCreatedAction.size()>0)
		{
			for(IActionNode *action:_runtimeCreatedAction)
			{
				//if the action it isn't on execution (it's ended or ready we are the own of deletion of this action) 
				if(!AI::CServer::getSingletonPtr()->getGameManager()->removeExecutingAction(action))
					delete action;
			}
			_runtimeCreatedAction.clear();
		}
	}

}