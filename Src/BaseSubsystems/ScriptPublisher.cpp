//---------------------------------------------------------------------------
// ScriptPublisher.cpp
//---------------------------------------------------------------------------
#include "ScriptPublisher.h"
#include "ScriptManager.h"

#include "Map\MapParser.h"
#include "Map\MapEntity.h"

#include "AI\WaveManager.h"
#include "AI\Wave.h"
#include "AI\Server.h"
#include "AI\GameManager.h"
#include "AI\ActionNode.h"
#include "AI\Actions\BlockCameraAction.h"
#include "AI\Actions\MoveCameraAction.h"
#include "AI\Actions\ShowMessageAction.h"
#include "AI\Actions\RemoveMessageAction.h"
#include "AI\Actions\WaitTimeAction.h"
#include "AI\Actions\GatherResourceAction.h"
#include "AI\Actions\EntityAction.h"
#include "AI\Actions\DefendToWaveAction.h"
#include "AI\Actions\SendMessageAction.h"
#include "AI\Actions\FollowEntityAction.h"
#include "AI\Actions\LockMouseAction.h"
#include "AI\Actions\SetClickIsEnabledAction.h"
#include "AI\Actions\ImmortalUnitAction.h"
#include "AI\Actions\CheckSelectedUnits.h"
#include "AI\Actions\SetAbilityIsEnabledAction.h"
#include "AI\Actions\UnselectEntitiesAction.h"
#include "AI\Actions\TimerAction.h"
#include "AI\Actions\ChangeAttackAction.h"
#include "AI\Actions\CheckDistanceBetweenEntities.h"
#include "AI\Actions\CreateEntityAction.h"
#include "AI\Actions\CheckAbilityUsed.h"
#include "AI\Actions\CheckResourceUnits.h"
#include "AI\Actions\CheckSelectedTypes.h"
#include "AI\Actions\SurvivorAction.h"
#include "AI\Actions\IncrementsWavesCounter.h"
#include "AI\Actions\DeleteEntityAction.h"
#include "AI\Actions\TimedSurvivorAction.h"
#include "AI\Actions\KillAllEnemiesAction.h"
#include "AI\Actions\CheckWaveEndedAction.h"

#include "Logic\GameControlMessages.h"
#include "Logic\AllyUnitMessages.h"

#include <cassert>

#include <iostream>
#include <sstream>
#include <time.h>

// Incluímos las cabedceras de Lua.
// Como es código C (no C++), hay que indicarselo al
// compilador para que asuma el convenio de nombres
// de C en el código objeto.
extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>  // Para inicializar la librería base de Lua
}

// Y luabind
#include <luabind/luabind.hpp>
#include <luabind/adopt_policy.hpp>
#include "luabind/class_info.hpp"		// Para RTTI desde LUA
#include <boost/shared_ptr.hpp>
#include <luabind\shared_ptr_converter.hpp>

namespace luabind
{
    template <typename T>
    T* get_pointer(std::shared_ptr<T> const& p)
    {
        return p.get();
    }
}
namespace {

	void AddMapEntity(const Map::CEntity &entity) 
	{		
		Map::CMapParser::getSingletonPtr()->addEntity(entity);
	}

	void AddWave(AI::CWave *wave) 
	{		
		AI::CServer::getSingletonPtr()->getWaveManager()->addWave(wave);
	}

	void AddActionNode(AI::IActionNode *action) 
	{		
		AI::CServer::getSingletonPtr()->getGameManager()->addAction(action);
	}
	void AddAsyncActionNode(AI::IActionNode *action) 
	{		
		AI::CServer::getSingletonPtr()->getGameManager()->addAsyncAction(action);
	}

	unsigned int GetCurrentTimestamp()
	{
		return static_cast<unsigned int>(time(NULL));
	}

	void SaveStringToFile(std::string filename,std::string txt)
	{
		std::ofstream out(filename);
		out << txt;
		out.close();
	}
	std::string StringDuplicate(const char* str, int num) 
	{

		std::string res=""; 
		for(int i=0;i<num;++i)
			res+=str;
		return res;
	}
}
namespace ScriptManager
{

	void CScriptPublisher::PublishToLua()
	{
		ScriptManager::CScriptManager *sm = ScriptManager::CScriptManager::GetPtrSingleton();

		/*---------------------------------- LOAD STD CLASSES------------------------------*/

		luabind::module(sm->getNativeInterpreter())
			[
				luabind::class_<std::list<std::string>>("StringList")
				.def(luabind::constructor<>())
				.def("push_back",(void (std::list<std::string>::*)(const std::string&))&std::list<std::string>::push_back)
			];

		/*---------------------------------- LOAD MAP FROM LUA------------------------------*/
		luabind::module(sm->getNativeInterpreter())
			[
				luabind::def("AddEntity", AddMapEntity)
			];

		luabind::module(sm->getNativeInterpreter())
			[
				luabind::class_<Map::CEntity>("MapEntity")
					.def(luabind::constructor<std::string>())
					.def("SetType", &Map::CEntity::setType)
					.def("SetAttrib", &Map::CEntity::setAttribute)
			];
		/*---------------------------------- LOAD WAVE FROM LUA------------------------------*/
		luabind::module(sm->getNativeInterpreter())
			[
				luabind::class_<AI::CWaveNode>("WaveNode")
				.def(luabind::constructor<std::string,unsigned int,std::string>())
			];
		luabind::module(sm->getNativeInterpreter())
			[
				luabind::class_<AI::CWave>("Wave")
				.def(luabind::constructor<int,std::string,std::string,unsigned int>())
				.def(luabind::constructor<int,unsigned int>())
				.def("AddWaveNode", &AI::CWave::addWaveNode)
			];
		luabind::module(sm->getNativeInterpreter())
			[
				luabind::def("AddWave", AddWave,luabind::adopt(_1)+luabind::adopt(_2))
			];
		/*---------------------------------- LOAD GAME ACTION FROM LUA------------------------------*/
		luabind::module(sm->getNativeInterpreter())
			[
				luabind::class_<AI::IActionNode>("ActionNode")
				.def(luabind::constructor<std::string>())
				.def("AddStartActionNode", &AI::IActionNode::addStartActionNode,luabind::adopt(_2))
				.def("AddEndActionNode", &AI::IActionNode::addEndActionNode,luabind::adopt(_2))
				.def("AddStartAsyncActionNode", &AI::IActionNode::addStartAsyncActionNode,luabind::adopt(_2))
				.def("AddEndAsyncActionNode", &AI::IActionNode::addEndAsyncActionNode,luabind::adopt(_2))
			];
		luabind::module(sm->getNativeInterpreter())
			[
				luabind::def("AddActionNode", AddActionNode,luabind::adopt(_1)+luabind::adopt(_2)),
				luabind::def("AddAsyncActionNode", AddAsyncActionNode,luabind::adopt(_1)+luabind::adopt(_2))
			];
		luabind::module(sm->getNativeInterpreter())
			[
				luabind::class_< AI::CMoveCameraAction,AI::IActionNode>("MoveCameraAction")
				.def(luabind::constructor<std::string>())
			];
		luabind::module(sm->getNativeInterpreter())
			[
				luabind::class_< AI::CShowMessageAction,AI::IActionNode>("ShowMessageAction")
				.def(luabind::constructor<std::string,std::string,float,float,unsigned int,std::string,std::string>())
			];
		luabind::module(sm->getNativeInterpreter())
			[
				luabind::class_< AI::CRemoveMessageAction,AI::IActionNode>("RemoveMessageAction")
				.def(luabind::constructor<std::string>())
			];
		luabind::module(sm->getNativeInterpreter())
			[
				luabind::class_< AI::CWaitTimeAction,AI::IActionNode>("WaitTimeAction")
				.def(luabind::constructor<unsigned int>())
			];
		luabind::module(sm->getNativeInterpreter())
			[
				luabind::class_< AI::CGatherResourceAction,AI::IActionNode>("GatherResourceAction")
				.def(luabind::constructor<std::string,int>())
			];
		luabind::module(sm->getNativeInterpreter())
			[
				luabind::class_< AI::CEntityAction,AI::IActionNode>("EntityAction")
				.def(luabind::constructor<std::string,int>())
			];
		luabind::module(sm->getNativeInterpreter())
			[
				luabind::class_< AI::CSendMessageAction,AI::IActionNode>("SendMessageAction")
				.def(luabind::constructor<std::string,std::shared_ptr<NMessage>>())
			];
		luabind::module(sm->getNativeInterpreter())
			[
				luabind::class_< AI::CDefendToWaveAction,AI::IActionNode>("DefendToWaveAction")
				.def(luabind::constructor<int,long>())
			];
		luabind::module(sm->getNativeInterpreter())
			[
				luabind::class_< AI::CBlockCameraAction,AI::IActionNode>("BlockCameraAction")
				.def(luabind::constructor<bool>())
			];
		luabind::module(sm->getNativeInterpreter())
			[
				luabind::class_< AI::CFollowEntityAction,AI::IActionNode>("FollowEntityAction")
				.def(luabind::constructor<std::string>())
			];
		luabind::module(sm->getNativeInterpreter())
			[
				luabind::class_< AI::CLockMouseAction,AI::IActionNode>("SetMouseIsLockedAction")
				.def(luabind::constructor<bool>())
			];
		luabind::module(sm->getNativeInterpreter())
			[
				luabind::class_< AI::CSetClickIsEnabledAction,AI::IActionNode>("SetClickIsEnabledAction")
				.def(luabind::constructor<int,std::string,bool>())
			];
		luabind::module(sm->getNativeInterpreter())
			[
				luabind::class_< AI::CImmortalUnitAction,AI::IActionNode>("ImmortalUnitAction")
				.def(luabind::constructor<std::string,bool>())
			];
		luabind::module(sm->getNativeInterpreter())
			[
				luabind::class_< AI::CCheckSelectedUnits,AI::IActionNode>("CheckSelectedUnitsAction")
				.def(luabind::constructor<std::list<std::string>>())
			];
		luabind::module(sm->getNativeInterpreter())
			[
				luabind::class_< AI::CCheckSelectedTypes,AI::IActionNode>("CheckSelectedTypesAction")
				.def(luabind::constructor<std::list<std::string>>())
			];
		luabind::module(sm->getNativeInterpreter())
			[
				luabind::class_< AI::CSetAbilityIsEnabledAction,AI::IActionNode>("SetAbilityIsEnabledAction")
				.def(luabind::constructor<std::string,std::string,bool>())
			];
		luabind::module(sm->getNativeInterpreter())
			[
				luabind::class_< AI::CUnselectEntitiesAction,AI::IActionNode>("UnselectEntitiesAction")
				.def(luabind::constructor<>())
			];
		luabind::module(sm->getNativeInterpreter())
			[
				luabind::class_< AI::CTimerAction,AI::IActionNode>("TimerAction")
				.def(luabind::constructor<std::string,int>())
			];
		luabind::module(sm->getNativeInterpreter())
			[
				luabind::class_< AI::CChangeAttackAction,AI::IActionNode>("ChangeAttackAction")
				.def(luabind::constructor<std::string,float>())
			];
		luabind::module(sm->getNativeInterpreter())
			[
				luabind::class_< AI::CCheckDistanceBetweenEntities,AI::IActionNode>("CheckDistanceBetweenEntitiesAction")
				.def(luabind::constructor<std::string, std::string, float>())
			];
		luabind::module(sm->getNativeInterpreter())
			[
				luabind::class_< AI::CCreateEntityAction,AI::IActionNode>("CreateEntityAction")
				.def(luabind::constructor<std::string, std::string>())
			];
		luabind::module(sm->getNativeInterpreter())
			[
				luabind::class_< AI::CCheckAbilityUsed,AI::IActionNode>("CheckAbilityUsedAction")
				.def(luabind::constructor<std::string, std::string>())
			];
		luabind::module(sm->getNativeInterpreter())
			[
				luabind::class_< AI::CCheckResourceUnits,AI::IActionNode>("CheckResourceUnitsAction")
				.def(luabind::constructor<int, int>())
			];
		luabind::module(sm->getNativeInterpreter())
			[
				luabind::class_< AI::CSurvivorAction,AI::IActionNode>("SurvivorAction")
				.def(luabind::constructor<int,int,int, long>())
			];
		luabind::module(sm->getNativeInterpreter())
			[
				luabind::class_< AI::CIncrementsWavesCounter,AI::IActionNode>("IncrementsWavesCounter")
				.def(luabind::constructor<int,int>())
			];
		luabind::module(sm->getNativeInterpreter())
			[
				luabind::class_< AI::CDeleteEntityAction,AI::IActionNode>("DeleteEntityAction")
				.def(luabind::constructor<std::string>())
			];
		luabind::module(sm->getNativeInterpreter())
			[
				luabind::class_< AI::CTimedSurvivorAction,AI::IActionNode>("TimedSurvivorAction")
				.def(luabind::constructor<int,int,int,int,int>())
			];
		luabind::module(sm->getNativeInterpreter())
			[
				luabind::class_< AI::CKillAllEnemiesAction,AI::IActionNode>("KillAllEnemiesAction")
				.def(luabind::constructor<>())
			];
		luabind::module(sm->getNativeInterpreter())
			[
				luabind::class_< AI::CCheckWaveEndedAction,AI::IActionNode>("CheckWaveEndedAction")
				.def(luabind::constructor<int>())
			];
		
		/*---------------------------------- MESSAGES TO LUA------------------------------*/
		luabind::module(sm->getNativeInterpreter())
			[
				luabind::class_<NMessage,std::shared_ptr<NMessage>>("NMessage")
				.def(luabind::constructor<>())
			];
		luabind::module(sm->getNativeInterpreter())
			[
				luabind::class_< EnableCreationEntity,NMessage,std::shared_ptr<NMessage>>("EnableCreationEntity")
				.def(luabind::constructor<const std::string&,const bool&>())
			];
		luabind::module(sm->getNativeInterpreter())
			[
				luabind::class_< GameOver,NMessage,std::shared_ptr<NMessage>>("GameOver")
				.def(luabind::constructor<const bool&>())
			];
		luabind::module(sm->getNativeInterpreter())
			[
				luabind::class_< ActivateCamp,NMessage,std::shared_ptr<NMessage>>("ActivateCamp")
				.def(luabind::constructor<const int&,const int&>())
			];
		/*---------------------------------- UTILITY TO LUA------------------------------*/
		luabind::module(sm->getNativeInterpreter())
			[
				luabind::def("GetCurrentTimestamp", GetCurrentTimestamp)
			];
		luabind::module(sm->getNativeInterpreter())
			[
				luabind::def("SaveStringToFile", SaveStringToFile)
			];
		luabind::module(sm->getNativeInterpreter())
			[
				luabind::def("StringDuplicate", StringDuplicate)
			];
		
		//luabind::module(sm->getNativeInterpreter())
		//	[
		//		luabind::def("AddMessage", AddMessage)
		//	];
	}
} // namespace ScriptManager
