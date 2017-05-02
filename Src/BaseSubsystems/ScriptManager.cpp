//---------------------------------------------------------------------------
// ScriptManager.cpp
//---------------------------------------------------------------------------

#include "ScriptManager.h"
#include "ScriptPublisher.h"

#include <cassert>

#include <iostream>
#include <sstream>

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
#include "luabind/class_info.hpp"		// Para RTTI desde LUA

namespace ScriptManager {

// Única instancia de la clase.
CScriptManager *CScriptManager::_instance = 0;


bool CScriptManager::Init() {

	assert(_instance == NULL);
	_instance = new CScriptManager();
	if (!_instance->open()) {
		// ¡¡Vaya!!
		Release();
		return false;
	}

	CScriptPublisher::PublishToLua();

	return true;

} // Init

//---------------------------------------------------------

void CScriptManager::Release() {

	assert(_instance);

	delete _instance;
	_instance = NULL;

} // Release

//---------------------------------------------------------

CScriptManager &CScriptManager::GetSingleton() {

	assert(_instance);

	return *_instance;

} // GetSingleton

//---------------------------------------------------------

CScriptManager *CScriptManager::GetPtrSingleton() {

	assert(_instance);

	return _instance;

} // GetPtrSingleton

//---------------------------------------------------------

bool CScriptManager::loadScript(const char *script) {

	assert(_lua);

	if (luaL_loadfile(_lua, script) != 0) {
		std::cout << "ScriptManager WARNING: error de sintaxis en el fichero " << script << std::endl;
		return false;
	}

	if (lua_pcall(_lua, 0, 0, 0) != 0) {
		std::cout << "Error en la ejecución de " << script << ": " << 
								lua_tostring(_lua, -1) << std::endl;
		lua_pop(_lua, 1);
		return false;
	}

	return true;

} // loadScript

//---------------------------------------------------------

bool CScriptManager::executeScript(const char *script) {

	assert(_lua);

	if (luaL_loadstring(_lua, script) != 0) {
		// Devuelve 0 si todo va bien, y 0 en otro caso
		// con un código de error, que ignoramos.
		std::cout << "Error de sintáxis del script inmediato " << script << std::endl;
		return false;
	}

	if (lua_pcall(_lua, 0, 0, 0) != 0) {
		std::cout << "Error en la ejecución de " << script << ": " << 
								lua_tostring(_lua, -1) << std::endl;
		lua_pop(_lua, 1);
		return false;
	}

	return true;

} // executeScript

//---------------------------------------------------------

int CScriptManager::getGlobal(const char *name, int defaultValue) {

	assert(_lua);

	luabind::object obj = luabind::globals(_lua)[name];

	if (!obj.is_valid() ||(luabind::type(obj) != LUA_TNUMBER))
		return defaultValue;

	return luabind::object_cast<int>(obj);

/*
#ifdef _DEBUG
	int topLua = lua_gettop(_lua);
#endif

	int result;

	lua_getglobal(_lua, name);

	if (!lua_isnumber(_lua, -1))
		result = defaultValue;
	else
		result = (int) lua_tonumber(_lua, -1);

	// Quitamos de la pila de Lua el elemento apilado por
	// lua_getglobal.
	// Si la variable global no existía, nos habrá
	// apilado nil, de modo que siempre hay algo :-)
	lua_pop(_lua, 1);

	// sanity-check: no dejamos nada en la cima de la pila...
	// (recuerda que assert sólo compila el interior en modo debug)
	assert(lua_gettop(_lua) == topLua);

	return result;
*/
} // getGlobal(int)

//---------------------------------------------------------

bool CScriptManager::getGlobal(const char *name, bool defaultValue) {

	assert(_lua);

	luabind::object obj = luabind::globals(_lua)[name];

	if (!obj.is_valid() ||(luabind::type(obj) != LUA_TBOOLEAN))
		return defaultValue;

	return luabind::object_cast<bool>(obj);

} // getGlobal(bool)

//---------------------------------------------------------

std::string CScriptManager::getGlobal(const char *name,
                                const char *defaultValue) {

	assert(_lua);

	luabind::object obj = luabind::globals(_lua)[name];

	if (!obj.is_valid() ||(luabind::type(obj) != LUA_TSTRING))
		return defaultValue;

	return luabind::object_cast<std::string>(obj);

} // getGlobal(char*)

//---------------------------------------------------------
bool CScriptManager::setGlobalField(const char *tableName,const char *field, std::string value)
{
	assert(_lua);

	luabind::object table = luabind::globals(_lua)[tableName];

	if (!table.is_valid() ||(luabind::type(table) != LUA_TTABLE))
		return false;

	table[field] = value;

	return true;
}

bool CScriptManager::setGlobalField(const char *tableName,const char *field, int value)
{
	assert(_lua);

	luabind::object table = luabind::globals(_lua)[tableName];

	if (!table.is_valid() ||(luabind::type(table) != LUA_TTABLE))
		return false;

	table[field] = value;

	return true;
}

//---------------------------------------------------------

int CScriptManager::getField(const char *table,
                             const char *field,
                             int defaultValue) {

	assert(_lua);

	luabind::object obj = luabind::globals(_lua)[table];

	if (!obj.is_valid() || (luabind::type(obj) != LUA_TTABLE))
		return defaultValue;

	luabind::object luafield = obj[field];

	if (!luafield.is_valid() || (luabind::type(luafield) != LUA_TNUMBER))
		return defaultValue;

	return luabind::object_cast<int>(luafield);
} // getField(int)


//---------------------------------------------------------

bool CScriptManager::getField(const char *table,
                             const char *field,
                             bool defaultValue) {

	assert(_lua);

	luabind::object obj = luabind::globals(_lua)[table];

	if (!obj.is_valid() || (luabind::type(obj) != LUA_TTABLE))
		return defaultValue;

	luabind::object luafield = obj[field];

	if (!luafield.is_valid() || (luabind::type(luafield) != LUA_TBOOLEAN))
		return defaultValue;

	return luabind::object_cast<bool>(luafield);

} // getField(bool)

//---------------------------------------------------------


std::string CScriptManager::getField(const char *table,
                               const char *field,
                               const char *defaultValue) {

	assert(_lua);

	luabind::object obj = luabind::globals(_lua)[table];

	if (!obj.is_valid() || (luabind::type(obj) != LUA_TTABLE))
		return defaultValue;

	luabind::object luafield = obj[field];

	if (!luafield.is_valid() || (luabind::type(luafield) != LUA_TSTRING))
		return defaultValue;

	return luabind::object_cast<std::string>(luafield);

} // getField(char*)

std::string CScriptManager::getField(const char *table, const char *arrayName, int idx, const char *fieldName,std::string defaultValue)
{
	assert(_lua);

	luabind::object obj = luabind::globals(_lua)[table];

	if (!obj.is_valid() || (luabind::type(obj) != LUA_TTABLE))
		return defaultValue;

	luabind::object luaArrayTable = obj[arrayName];

	if (!luaArrayTable.is_valid() || (luabind::type(luaArrayTable) != LUA_TTABLE))
		return defaultValue;

	luabind::object luaItem = luaArrayTable[idx];
	if (!luaItem.is_valid() || (luabind::type(luaItem) != LUA_TTABLE))
		return defaultValue;

	luabind::object luaField = luaItem[fieldName];

	if (!luaField.is_valid() || (luabind::type(luaField) != LUA_TSTRING))
		return defaultValue;

	return luabind::object_cast<std::string>(luaField);


}
float CScriptManager::getField(const char *table, const char *arrayName, int idx, const char *fieldName,float defaultValue)
{
	assert(_lua);

	luabind::object obj = luabind::globals(_lua)[table];

	if (!obj.is_valid() || (luabind::type(obj) != LUA_TTABLE))
		return defaultValue;

	luabind::object luaArrayTable = obj[arrayName];

	if (!luaArrayTable.is_valid() || (luabind::type(luaArrayTable) != LUA_TTABLE))
		return defaultValue;

	luabind::object luaItem = luaArrayTable[idx];
	if (!luaItem.is_valid() || (luabind::type(luaItem) != LUA_TTABLE))
		return defaultValue;

	luabind::object luaField = luaItem[fieldName];

	if (!luaField.is_valid() || (luabind::type(luaField) != LUA_TNUMBER))
		return defaultValue;

	return luabind::object_cast<float>(luaField);


}

int CScriptManager::getTableSize(const char *table, const char *subTable)
{
	assert(_lua);

	luabind::object obj = luabind::globals(_lua)[table];

	if (!obj.is_valid() || (luabind::type(obj) != LUA_TTABLE))
		return 0;

	luabind::object luafieldTable = obj[subTable];

	if (!luafieldTable.is_valid() || (luabind::type(luafieldTable) != LUA_TTABLE))
		return 0;

	int count=0;
	for(luabind::iterator i(luafieldTable), end; i != end; i++) {
		count++;
	}
	return count;
}
int CScriptManager::getTableSize(const char *table)
{
	assert(_lua);

	luabind::object obj = luabind::globals(_lua)[table];

	if (!obj.is_valid() || (luabind::type(obj) != LUA_TTABLE))
		return 0;

	int count=0;
	for(luabind::iterator i(obj), end; i != end; i++) {
		count++;
	}
	return count;
}

void  CScriptManager::getTableIndex(const char *table,std::vector<std::string> &result)
{
	assert(_lua);

	luabind::object obj = luabind::globals(_lua)[table];

	if (!obj.is_valid() || (luabind::type(obj) != LUA_TTABLE))
		return ;

	for(luabind::iterator i(obj), end; i != end; i++) {
		result.push_back(luabind::object_cast<std::string>(i.key()));
	}
}

//---------------------------------------------------------

bool CScriptManager::executeProcedure(const char *subroutineName) {

	assert(_lua);

	luabind::object obj = luabind::globals(_lua)[subroutineName];

	if(!obj.is_valid() || (luabind::type(obj)!= LUA_TFUNCTION))
	{
		std::cerr << "Error ejecutando el procedimiento " << subroutineName <<std::endl;
		return false;
	}

	obj();
	return true;

	/*

#ifdef _DEBUG
	int topLua = lua_gettop(_lua);
#endif

	// Lo primero es buscar la función (global) con ese
	// nombre.
	lua_getglobal(_lua, subroutineName);

	if (!lua_isfunction(_lua, -1)) {
		// ¡¡Vaya!!
		lua_pop(_lua, 1);
		return false;
	}

	// Llamamos a la función, sin argumentos y sin
	// resultado.
	if (lua_pcall(_lua, 0, 0, 0) != 0) {
		std::cout << "Error en la ejecución del procedimiento " << subroutineName << ": " << 
								lua_tostring(_lua, -1) << std::endl;
		lua_pop(_lua, 1);
		return false;
	}

	// sanity-check: no dejamos nada en la cima de la pila...
	// (recuerda que assert sólo compila el interior en modo debug)
	assert(lua_gettop(_lua) == topLua);

	return true;
	*/
} // executeProcedure

//---------------------------------------------------------

bool CScriptManager::executeProcedure(const char *subroutineName, int param1) {

	assert(_lua);

	try {
		luabind::globals(_lua)[subroutineName](param1);
	} catch (luabind::error &ex) {
		std::cerr << "Error ejecutando el procedimiento " << subroutineName <<std::endl;
		std::cerr << ex.what() << std::endl;
		return false;
	}

	return true;

} // executeProcedure(int)

//---------------------------------------------------------

bool CScriptManager::executeFunction(const char *subroutineName,
                                     int param1, int &result) {

	assert(_lua);

	try {
		luabind::object ret = luabind::globals(_lua)[subroutineName](param1);
		if (!ret.is_valid() || (luabind::type(ret) != LUA_TNUMBER))
			return false;
		result = luabind::object_cast<int>(ret);
		return true;
	} catch (luabind::error &ex) {
		std::cerr << "Error ejecutando el procedimiento " << subroutineName <<std::endl;
		std::cerr << ex.what() << std::endl;
		return false;
	}

	return true;

} // executeFunction

bool CScriptManager::executeFunction(const char *subroutineName, int param1, std::string param2[],int arrayLength,int param3, int &result)
{
	assert(_lua);

	try {
	     luabind::object arr = luabind::newtable(_lua); 

		  for (int i = 0; i < arrayLength; ++i) 
			  arr[i + 1] = param2[i]; 

		  luabind::object ret = luabind::globals(_lua)[subroutineName](param1,arr,param3);
		if (!ret.is_valid() || (luabind::type(ret) != LUA_TNUMBER))
			return false;
		result = luabind::object_cast<int>(ret);
		return true;
	} catch (luabind::error &ex) {
		std::cerr << "Error ejecutando el procedimiento " << subroutineName <<std::endl;
		std::cerr << ex.what() << std::endl;
		return false;
	}

	return true;
}
//---------------------------------------------------------

bool CScriptManager::executeFunction(const char *subroutineName,
                                     const char *param1, int &result) {

	assert(_lua);

#ifdef _DEBUG
	int topLua = lua_gettop(_lua);
#endif

	// Lo primero es buscar la función (global) con ese
	// nombre.
	lua_getglobal(_lua, subroutineName);

	if (!lua_isfunction(_lua, -1)) {
		// ¡¡Vaya!!
		lua_pop(_lua, 1);
		return false;
	}

	// Apilamos parámetro
	lua_pushstring(_lua, param1);

	// Llamamos a la función, sin argumentos y sin
	// resultado.
	if (lua_pcall(_lua, 1, 1, 0) != 0) {
		std::cout << "Error en la ejecución del procedimiento " << subroutineName << ": " << 
								lua_tostring(_lua, -1) << std::endl;
		lua_pop(_lua, 1);
		return false;
	}

	// Y miramos valor devuelto
	if (!lua_isnumber(_lua, -1)) {
		// ¡¡Vaya!!
		lua_pop(_lua, 1);
		return false;
	}
	result = (int) lua_tonumber(_lua, -1);

	lua_pop(_lua, 1);

	// sanity-check: no dejamos nada en la cima de la pila...
	// (recuerda que assert sólo compila el interior en modo debug)
	assert(lua_gettop(_lua) == topLua);

	return true;

} // executeFunction

//---------------------------------------------------------

void CScriptManager::registerFunction(lua_CFunction f, const char *luaName) {
	assert(_lua);

	lua_register(_lua, luaName, f);
} // registerFunction

//---------------------------------------------------------
//                   Métodos protegidos
//---------------------------------------------------------

CScriptManager::CScriptManager() : _lua(NULL) {

} // Constructor

//---------------------------------------------------------

CScriptManager::~CScriptManager() {

	close();

} // Destructor

//---------------------------------------------------------

bool CScriptManager::open() {

	_lua = lua_open();

	if (!_lua)
		return false;

	// Abrimos la librería base para hacer disponible
	// print() en Lua.
	luaopen_base(_lua);
	luaopen_math(_lua);

	// Y luabind
	luabind::open(_lua);
	luabind::bind_class_info(_lua);	// RTTI de luabind

	// Registramos la función de error
	luabind::set_pcall_callback(CScriptManager::luaError);


	return true;

} // open

//---------------------------------------------------------

void CScriptManager::close() {
	lua_close(_lua);
	_lua = nullptr;
} // close

int CScriptManager::luaError(lua_State *L) {
		// Error en LUA
	std::stringstream msg;

	// Cambiamos el mensaje, para meter la información
	// de la pila de LUA.
	msg << "Error en script LUA: " << lua_tostring(L, -1) << std::endl;
	msg << "Pila de LUA: " << std::endl;
	int level = 0;
	lua_Debug d;

	while (lua_getstack(L, level, &d) == 1) {
		lua_getinfo(L, "Sln", &d);

		// Nivel dentro de la pila
		msg << level << ":: ";

		// ¿Es una llamada de C?
		if (strcmp(d.what, "C") == 0) {
			msg << "Native";
		} else {
			msg << d.short_src << ":" << d.currentline;
			//msg << std::endl << d.source << std::endl;
		}
		// Si tiene un nombre, lo ponemos
		if (d.name != 0)
			msg << " (" << d.namewhat << " " << d.name << ")";
		msg << std::endl;
		level++;
	}

	// Sacamos la cadena de error vieja
	lua_pop(L, 1);
	// Y metemos la nueva mejorada con la información de la pila, etc.
	lua_pushstring(L, msg.str().c_str());

	return 1;
}

} // namespace ScriptManager
