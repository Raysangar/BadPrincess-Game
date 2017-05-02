//---------------------------------------------------------------------------
// ScriptManager.h
//---------------------------------------------------------------------------


#ifndef __ScriptManager_ScriptManager_H
#define __ScriptManager_ScriptManager_H

#include <string>
#include <vector>

// Predeclaración de clases para ahorrar tiempo de compilación
// al ahorrarnos la inclusión de ficheros .h en otros .h

// Estructura con el contexto (estado) del intérprete de Lua.
struct lua_State;

typedef int (*lua_CFunction) (lua_State *L);

namespace ScriptManager {

class CScriptManager {
public:

	static bool Init();

	static void Release();

	static CScriptManager &GetSingleton();

	static CScriptManager *GetPtrSingleton();

	bool loadScript(const char *script);

	bool executeScript(const char *script);

	int getGlobal(const char *name, int defaultValue);

	bool getGlobal(const char *name, bool defaultValue);

	std::string getGlobal(const char *name, const char *defaultValue);

	int getField(const char *table, const char *field, int defaultValue);

	bool getField(const char *table, const char *field, bool defaultValue);

	std::string getField(const char *table, const char *arrayName, int idx, const char *field,std::string defaultValue);

	float getField(const char *table, const char *arrayName, int idx, const char *field,float defaultValue);

	void  getTableIndex(const char *table,std::vector<std::string> &result);

	int getTableSize(const char *table);

	int getTableSize(const char *table, const char *subtable);

	std::string getField(const char *table, const char *field, const char *defaultValue);

	bool setGlobalField(const char *table,const char *field, std::string value);

	bool setGlobalField(const char *table,const char *field, int value);

	bool executeProcedure(const char *subroutineName);

	bool executeProcedure(const char *subroutineName, int param1);

	bool executeFunction(const char *subroutineName, int param1, int &result);

	bool executeFunction(const char *subroutineName, const char *param1, int &result);

	bool executeFunction(const char *subroutineName, int param1, std::string param2[],int arrayLength,int param3, int &result);

	void registerFunction(lua_CFunction f, const char *luaName);

	lua_State *getNativeInterpreter() { return _lua; }

protected:

	static int luaError(lua_State *s);

	CScriptManager();

	~CScriptManager();

	bool open();

	void close();

	static CScriptManager *_instance;

	lua_State *_lua;

};

}

#endif