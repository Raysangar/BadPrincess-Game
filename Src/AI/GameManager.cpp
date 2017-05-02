/**
@file GameManager.cpp

Clase que gestiona el guion del nivel y todos los eventos y las condiciones generales

@see GameManager

@author Guidi Giacomo 
@date March 2015

*/
#include "GameManager.h"
#include "ActionNode.h"

#include "AI/Server.h"

#include "Application/BaseApplication.h"
#include "Application/GameOverState.h"

#include "Logic\Entity\Entity.h"
#include "Logic/GameControlMessages.h"
#include "Logic/EnemyMessages.h"
#include "Logic/PrincessMessages.h"
#include "Logic/SmartObjectMessages.h"

#include "BaseSubsystems\ScriptManager.h"

#include "EntityManager.h"

#define MINDISTANCE 20
#define MAXDISTANCE 40

namespace AI {

	/**
	Constructor
	*/
	CGameManager::CGameManager(void):_sequentiallyActionIdx(0),_wavesCounter(0)
	{
		_actionList = new std::vector<IActionNode*>();
		_executionActionList = new std::vector<IActionNode*>();
		_tempExecutionActionList = new std::vector<IActionNode*>();
		_tempRemovingActionList = new std::vector<IActionNode*>();
		_asyncActionList = new std::vector<IActionNode*>();
		_princessKidnaped = false;
		_princess = nullptr;
		_knight = nullptr;
		//initialize functions to write lua table to file
		loadScript("media/lua/IOUtils.lua");
		//load the user profile
		loadScript("media/lua/userProfile.lua");
	}

	void CGameManager::onAwake()
	{
		_sequentiallyActionIdx = 0;
		if(_actionList->size()>0)
			this->executeAction(_actionList->at(_sequentiallyActionIdx));

		_wavesCounter=0;

	}
	/////////////////////////////////////////
	/**
	Destructor
	*/
	CGameManager::~CGameManager(void)
	{
		if(_executionActionList)
			delete _executionActionList;
		if(_tempExecutionActionList)
			delete _tempExecutionActionList;

		if(_tempRemovingActionList)
		{
			//it shouldn't be full because the action will remove at the end of the tick
			freeActionList(_tempRemovingActionList);
			delete _tempRemovingActionList;
		}
		if(_actionList)
		{
			freeActionList(_actionList);
			delete _actionList;
		}
		if(_asyncActionList)
		{
			freeActionList(_asyncActionList);
			delete _asyncActionList;
		}

		_princessKidnaped = false;
		_princess = nullptr;

	}
	bool CGameManager::acceptN(const std::shared_ptr<NMessage> &message)
	{
		bool accept= false;
		//check if the first sync operation accept a message
		//if(_actionList->size()>0)
		//	accept = accept || _actionList->front()->acceptN(message);
		//check if execution operation accept a message
		std::vector<IActionNode*>::iterator cit=_executionActionList->begin();
		for(;cit!=_executionActionList->end();++cit)
			accept = accept || (*cit)->acceptN(message);
		//Accept here all message that could interest to gamemanager	
		return accept || message->type.compare("EnableCreationEntity")==0|| message->type.compare("GameOver")==0;
	} // accept
	//---------------------------------------------------------

	void CGameManager::processN(const std::shared_ptr<NMessage> &message)
	{
		//if(_actionList->size()>0 && _actionList->front()->acceptN(message))
		//	_actionList->front()->processN(message);

		std::vector<IActionNode*>::iterator cit=_executionActionList->begin();
		for(;cit!=_executionActionList->end();++cit)		
		{
			if((*cit)->acceptN(message))
				(*cit)->processN(message);
		}

		if (message->type.compare("EnableCreationEntity") == 0)
		{
			std::shared_ptr<EnableCreationEntity> m = std::static_pointer_cast<EnableCreationEntity>(message);
			_entityCreationEnabled[m->entityType]=m->isEnabled;
		}else if (message->type.compare("GameOver") == 0)
		{
			std::shared_ptr<GameOver> m = std::static_pointer_cast<GameOver>(message);
			endGame(m->hasWin);
		}
	} // process

	void CGameManager::tick(unsigned int msecs)
	{
		//execute the action in execution list
		std::vector<IActionNode*>::iterator cit=_executionActionList->begin();
		while(cit!=_executionActionList->end())
		{
			(*cit)->tick(msecs);
			if((*cit)->getStatus()==IActionStatus::Ended)
			{
				cit = _executionActionList->erase(cit);
				continue;
			}
			cit++;
		}

		//remove action list that we have to remove		
		if(!_tempRemovingActionList->empty())
		{
			cit=_tempRemovingActionList->begin();
			for(;cit!=_tempRemovingActionList->end();++cit)
			{
				std::cout<<"remove action "<<(*cit)->getName()<<std::endl;
				std::vector<IActionNode*>::iterator itemToDelete=std::find(_executionActionList->begin(),_executionActionList->end(),*cit);
				if(itemToDelete!=_executionActionList->end())
					_executionActionList->erase(itemToDelete);
				delete *cit;
			}
			_tempRemovingActionList->clear();
		}

		//add the new action started
		if(!_tempExecutionActionList->empty())
		{
			cit=_tempExecutionActionList->begin();
			for(;cit!=_tempExecutionActionList->end();++cit)
				_executionActionList->push_back(*cit);
			_tempExecutionActionList->clear();
		}

		if(_actionList->size()>0 && _sequentiallyActionIdx<_actionList->size())
		{
			if(_actionList->at(_sequentiallyActionIdx)->getStatus()==IActionStatus::Ended)
			{
				++_sequentiallyActionIdx;
//				_actionList->pop_front();

				if(_sequentiallyActionIdx<_actionList->size())
					executeAction(_actionList->at(_sequentiallyActionIdx));
			}
		}


	}
	
	void CGameManager::addAction(IActionNode *action){
		_actionList->push_back(action);
	}
	void CGameManager::addAsyncAction(IActionNode *action){
		_asyncActionList->push_back(action);
		_executionActionList->push_back(action);
	}
	void CGameManager::executeAction(IActionNode *action){
		_tempExecutionActionList->push_back(action);
	}
	bool CGameManager::removeExecutingAction(IActionNode *action){
		std::vector<IActionNode*>::iterator itemToDelete=std::find(_executionActionList->begin(),_executionActionList->end(),action);
		if(itemToDelete!=_executionActionList->end())
		{
			_tempRemovingActionList->push_back(action);
			return true;
		}else
			return false;
	}
	void CGameManager::loadGameLevel(std::string file)
	{
		ScriptManager::CScriptManager *sm = ScriptManager::CScriptManager::GetPtrSingleton();
		sm->loadScript(file.c_str());
		sm->executeProcedure("loadGameAction");
	}
	void CGameManager::loadScript(std::string file)
	{
		ScriptManager::CScriptManager *sm = ScriptManager::CScriptManager::GetPtrSingleton();
		sm->loadScript(file.c_str());
	}

	bool CGameManager::isEntityCreationEnabled(std::string entityType)
	{
		if(_entityCreationEnabled.find(entityType) == _entityCreationEnabled.end())
			return false;
		else
			return _entityCreationEnabled[entityType];
	}
	void CGameManager::setEntityCreationEnabled(std::string entityType,bool isEnabled)
	{
		_entityCreationEnabled[entityType] = isEnabled;
	}

	void CGameManager::endGame(bool playerWon)
	{
		_lastGameProperty._hasWin = playerWon;
		_lastGameProperty._currentSurvivalPoint = _wavesCounter -1;

		Application::CGameOverState *gameOver=static_cast<Application::CGameOverState *>(Application::CBaseApplication::getSingletonPtr()->findState("gameOver"));
		Application::CBaseApplication::getSingletonPtr()->stackState("gameOver");
	}

	Vector3 CGameManager::getPositionFarToEntity(Logic::CEntity* target,Logic::CEntity* requester,float distance,int angleMax)
	{
		Vector3 dir = requester->getPosition()-target->getPosition();
		dir.normalise();
		float angle = (float)(rand()% angleMax) -(angleMax/2);
		Ogre::Degree d(angle);
		float cosAngle= cos(d.valueRadians());
		float sinAngle= sin(d.valueRadians());
		Vector3 res(dir.x*cosAngle+dir.z*sinAngle,0,-dir.x*sinAngle+dir.z*cosAngle);
		res.normalise();
		return target->getPosition()+ res * (distance);
	}

	void CGameManager::freeSmartPosition(Logic::CEntity* smartEntity,Vector3 smartPosition,Logic::CEntity* sender)
	{
		if(smartPosition!=Vector3::NEGATIVE_UNIT_Y && smartEntity != nullptr)
		{
			auto freeMessage = std::make_shared<SmartDataFreePosition>();
			freeMessage->entity=sender;
			freeMessage->position = smartPosition;
			smartEntity->emitMessageN(freeMessage);
		}
	}

	void CGameManager::freeActionList(std::vector<IActionNode*> *list)
	{
		std::vector<IActionNode*>::const_iterator cit=list->begin();
		for(;cit!=list->end();++cit)
			delete *cit;
		list->clear();
	}

	bool CGameManager::registerPrincessKidnapped (Logic::CEntity* princess, Logic::CEntity* knight)
	{
		if(!_princessKidnaped)
		{
			_princess = princess;
			_princessKidnaped = true;
			_knight = knight;
			return true;
		}
		else
			return false;
	}

	void CGameManager::unregisterPrincessKidnapped ()
	{
		_princessKidnaped = false;
		_princess = nullptr;
		_knight = nullptr;

		_firstQuad.clear();
		_secondQuad.clear();
		_thirdQuad.clear();
		_forthQuad.clear();
	}

	Logic::CEntity* CGameManager::getPrincess ()
	{
		return _princess;
	}

	Logic::CEntity* CGameManager::getKnight ()
	{
		return _knight;
	}

	Vector3 CGameManager::calculatePosition (Logic::CEntity* ent)
	{
		//Distancia a la que se pondra del caballero (radio)
		int distance = rand() % (MAXDISTANCE-MINDISTANCE) + MINDISTANCE;
		//Porcentaje para sacar una distancia en el eje x del caballero (el z sera 100-esta variable)
		int percentage = rand() % 85 + 5;
		float per = (float)percentage/100.0f;
		//Para decidir la posicion
		float x = (float)distance*per;
		float z = (float)distance*(1-per);
		int index = 0;
		Vector3 pos = Vector3(0.0f,0.0f,0.0f);
		int num[4] = {_firstQuad.size(), _secondQuad.size(), _thirdQuad.size(), _forthQuad.size()};
		
		for(int i = 1; i <= 3; i++)
		{
			if(num[i] < num[index])
				index = i;          
		}

		switch (index)
		{
		case 0:
			_firstQuad.push_back(ent);
			pos = Vector3(x, 0, z);
			break;
		case 1:
			_secondQuad.push_back(ent);
			pos = Vector3(-x, 0, -z);
			break;
		case 2:
			_thirdQuad.push_back(ent);
			pos = Vector3(-x, 0, z);
			break;
		case 3:
			_forthQuad.push_back(ent);
			pos = Vector3(x, 0, -z);
			break;
		default:
			break;
		}

		return pos;
	}

	bool CGameManager::protectKnight (Logic::CEntity* ent)
	{
		float distance = 100000000.0f;
		Logic::CEntity* knight = nullptr;
		int index = 0;
		bool ok = false;

		for (Logic::CEntity* knights : _firstQuad)
		{
			if(knights)
			{
				if(knights->getPosition().distance(ent->getPosition()) < distance)
				{
					distance = knights->getPosition().distance(ent->getPosition());
					knight = knights;
					index = 1;
				}
			}
			else
				_firstQuad.remove(knights);
		}
		for (Logic::CEntity* knights : _secondQuad)
		{
			if(knights)
			{
				if(knights->getPosition().distance(ent->getPosition()) < distance)
				{
					distance = knights->getPosition().distance(ent->getPosition());
					knight = knights;
					index = 2;
				}
			}
			else
				_secondQuad.remove(knights);
		}
		for (Logic::CEntity* knights : _thirdQuad)
		{
			if(knights)
			{
				if(knights->getPosition().distance(ent->getPosition()) < distance)
				{
					distance = knights->getPosition().distance(ent->getPosition());
					knight = knights;
					index = 3;
				}
			}
			else
				_thirdQuad.remove(knights);
		}
		for (Logic::CEntity* knights : _forthQuad)
		{
			if(knights)
			{
				if(knights->getPosition().distance(ent->getPosition()) < distance)
				{
					distance = knights->getPosition().distance(ent->getPosition());
					knight = knights;
					index = 4;
				}
			}
			else
				_forthQuad.remove(knights);
		}

		if(knight != nullptr)
		{
			auto m = std::make_shared<GoAttack>();
			m->ent = ent;
			knight->emitMessageN(m);

			switch(index)
			{
			case 1:
				_firstQuad.remove(knight);
				break;
			case 2:
				_secondQuad.remove(knight);
				break;
			case 3:
				_thirdQuad.remove(knight);
				break;
			case 4:
				_forthQuad.remove(knight);
				break;
			default:
				break;
			}

			ok = true;
		}

		return ok;
	}

	void CGameManager::removeKnight(Logic::CEntity* ent)
	{
		bool found = (std::find(_firstQuad.begin(), _firstQuad.end(), ent) != _firstQuad.end());
		if(found)
			_firstQuad.remove(ent);
		else
		{
			found = (std::find(_secondQuad.begin(), _secondQuad.end(), ent) != _secondQuad.end());
			if(found)
				_secondQuad.remove(ent);
			else
			{
				found = (std::find(_thirdQuad.begin(), _thirdQuad.end(), ent) != _thirdQuad.end());
				if(found)
					_thirdQuad.remove(ent);
				else
				{
					found = (std::find(_forthQuad.begin(), _forthQuad.end(), ent) != _forthQuad.end());
					if(found)
						_forthQuad.remove(ent);
				}
			}

		}

	}

	void CGameManager::reset()
	{
		if(_executionActionList)
			_executionActionList->clear();
		if(_tempExecutionActionList)
			_tempExecutionActionList->clear();

		if(_tempRemovingActionList)
			freeActionList(_tempRemovingActionList);
		if(_actionList)
			freeActionList(_actionList);
		if(_asyncActionList)
			freeActionList(_asyncActionList);

		_princessKidnaped = false;
		_princess = nullptr;
		_knight = nullptr;
		_firstQuad.clear();
		_secondQuad.clear();
		_thirdQuad.clear();
		_forthQuad.clear();

		_wavesCounter=0;

		//Semilla aqui para en cada diferente nivel o reseteo del nivel genere una nueva semilla
		//srand((unsigned int)time(0));
	}
	void CGameManager::addWavesCounter(int num)
	{
		_wavesCounter+=num;
		if(_wavesCounter<0)
			_wavesCounter=0;
	}
	
	void CGameManager::setupCurrentGame(std::string gameType)
	{
		_lastGameProperty._currentGameType = gameType;
		_lastGameProperty._currentSurvivalPoint = 0;
		_lastGameProperty._currentCampaignLevel = 0;
		_lastGameProperty._hasWin = false;
	}
}