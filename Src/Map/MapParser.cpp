/**
@file MapParser.cpp

Contiene la implementación de la clase que encapsula el parseo de mapas.

@see Map::CMapParser

@author David Llansó García
@date Agosto, 2010
*/

#include <fstream>
#include <sstream>
#include <cassert>

#include "MapEntity.h"
#include "MapParser.h"
#include "scanner.h"

#include "BaseSubsystems/ScriptManager.h"


namespace Map {

	CMapParser* CMapParser::_instance = 0;

	//--------------------------------------------------------

	CMapParser::CMapParser(): _traceScanning(false),
							  _traceParsing(false),
							  _entityInProgress(0)
	{
		_instance = this;
		// Configuramos aquí la parte de LUA. No es el sitio
		// más adecuado, en realidad...
		ScriptManager::CScriptManager::GetPtrSingleton()->loadScript("media/lua/MapParser.lua");

		
	} // CMapParser

	//--------------------------------------------------------

	CMapParser::~CMapParser()
	{
		releaseEntityList();
		_instance = 0;

	} // ~CMapParser
	
	//--------------------------------------------------------

	bool CMapParser::Init()
	{
		assert(!_instance && "Segunda inicialización de Map::CMapParser no permitida!");

		new CMapParser();
		return true;

	} // Init

	//--------------------------------------------------------

	void CMapParser::Release()
	{
		assert(_instance && "Map::CMapParser no está inicializado!");
		if(_instance)
		{
			delete _instance;
		}

	} // Release

	//--------------------------------------------------------

	bool CMapParser::parseStream(std::istream& in, const std::string& name)
	{
		_streamname = name;

		CScanner scanner(&in);
		scanner.set_debug(_traceScanning);
		_lexer = &scanner;

		releaseEntityList();
		CParser parser(*this);
		parser.set_debug_level(_traceParsing);
		return (parser.parse() == 0);
	} // parseStream

	//--------------------------------------------------------

	bool CMapParser::parseFile(const std::string &filename)
	{

		// Borramos la lista de entidades anterior
		releaseEntityList();

		int ok;
		ScriptManager::CScriptManager::GetPtrSingleton()->executeFunction("loadMap", filename.c_str(), ok);

		// Comprobamos que la carga fue correcta
		if (!ok) {
			error("Error durante la carga del mapa.");
			return false;
		} else if (_entityInProgress != nullptr) {
			error("Carga finalizada prematuramente (con una entidad a medio construir)");
			return false;
		}

		return true;

		//std::ifstream in(filename.c_str());
		//if (!in.good()) return false;
		//return parseStream(in, filename);

	} // parseFile

	//--------------------------------------------------------

	bool CMapParser::parseString(const std::string &input, const std::string& name)
	{
		std::istringstream iss(input);
		return parseStream(iss, name);
	} // parseString

	//--------------------------------------------------------

	void CMapParser::error(const class location& l,
			   const std::string& m)
	{
		std::cerr << l << ": " << m << std::endl;
	} // error

	//--------------------------------------------------------

	void CMapParser::error(const std::string& m)
	{
		std::cerr << m << std::endl;
	} // error

	//--------------------------------------------------------
		
	void CMapParser::releaseEntityList()
	{
		while(!_entityList.empty())
		{
			_entityInProgress = _entityList.back();
			_entityList.pop_back();
			delete _entityInProgress;
			_entityInProgress=nullptr;
		}
	}
	void CMapParser::addEntity(const Map::CEntity &entity)
	{
		#ifdef _DEBUG
		assert(entity.getType().compare("") && "No se ha establecido tipo a la entidad");
		Map::CMapParser::TEntityList::const_iterator it, end;
		if(_entityList.size()>0)
		{
			it = _entityList.begin();
			end = _entityList.end();
			for(; it != end; it++)
				assert((*it)->getName().compare(entity.getName()) && "Ya existe una entidad con este nombre.");
		}
		#endif // _DEBUG

		// La que recibimos ES DE LUA
		Map::CEntity *newEntity = new Map::CEntity(entity.getName());
		*newEntity = entity;
		_entityList.push_back(newEntity);
	}
} // namespace Map
