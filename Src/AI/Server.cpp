/**
@file Server.cpp

Servidor de IA.

@author Gonzalo Flórez
@date Diciembre, 2010
*/
#include "Server.h"

#include <assert.h>

#include "StrategyController.h"
#include "EntityManager.h"
#include "ResourceServer.h"
#include "WaveManager.h"
#include "GameManager.h"
#include "WaypointsPathLearning.h"

#define WAYPOINT_TOLLERANCE 2.0f
namespace AI {
	/////////////////////////////////////////
	/**
	Instancia única de la clase.
	*/
	CServer* CServer::_instance = 0;
	/////////////////////////////////////////
	/**
	Constructor de la clase.
	*/
	CServer::CServer(void)
	{
		assert(!_instance && "Segunda inicialización de AI::CServer no permitida!");
		_f = new CAStarFunctionsGaleon();
		_aStar = new micropather::MicroPather(_f);
		_strategyController = new CStrategyController();
		_entityManager = new CEntityManager();
		_waveManager = new CWaveManager();
		_gameManager = new CGameManager();
		_waypointsLearningInstance = new CWaypointsPathLearning();

	}
	/////////////////////////////////////////
	/**
	Destructor de la clase.
	*/
	CServer::~CServer(void)
	{
		assert(_instance);
		delete _aStar;
		delete _f;
		delete _strategyController;
		delete _entityManager;
		delete _waveManager;
		delete _gameManager;
		delete _waypointsLearningInstance;
	}
	/////////////////////////////////////////
	/**
	Inicializa el servidor de IA. Esta operación sólo es necesario realizarla
	una vez durante la inicialización de la aplicación. 

	@return Devuelve false si no se ha podido inicializar.
	*/
	bool CServer::Init() 
	{
		_instance = new CServer();
		return true;
	}
	/////////////////////////////////////////
	/**
	Libera el servidor de IA. Debe llamarse al finalizar la aplicación.
	*/
	void CServer::Release()
	{
		ResourceServer::Release();

		if (_instance)
			delete _instance;
		_instance = 0;
	}
	/////////////////////////////////////////
	/**
	Añade un nuevo nodo al grafo de navegación.
	*/
	void CServer::addWaypoint(Vector3 waypoint)
	{
		_wpg.addWaypoint(waypoint);
	}
	/////////////////////////////////////////
	/**
	Recalcula el grafo de navegación a partir de los nodos que han sido
	añadidos con addGraphNode.
	*/
	void CServer::computeNavigationGraph() 
	{
		_wpg.computeNavigationGraph();
	}

	void CServer::removeAllWaypoints()
	{
		_wpg.removeAllWaypoints();
	}
	/////////////////////////////////////////
	/**
	Calcula una ruta usando A*.
	*/
	std::vector<Vector3> *CServer::getAStarRoute(Vector3 from, Vector3 to)
	{
		// Dadas dos posiciones devuelve una ruta para llegar de una a otra.
		// En primer lugar utilizamos los métodos del grafo de waypoints para obtener los nodos
		// más cercanos al origen y al destino (_wpg.getClosestWaypoint). 
		// A continuación usamos A* para calcular la ruta (_aStar->Solve).
		// Por último tenemos que devolver la ruta en un vector de posiciones. Para 
		// ello tendremos que convertir los ids de los nodos en sus posiciones (_wpg.getNode) 
		// y añadir las posiciones de origen y destino.
		int idFrom = _wpg.getClosestWaypoint(from);
		int idTo = _wpg.getClosestWaypoint(to);
		std::vector<void*>* path = nullptr;
		bool deletePath = false;
		//fix bug "Error calculate A*" when node idFrom is the same of idTo 
		if(idFrom == idTo)
		{
//			std::cout<<"fix id from = idTo"<<std::endl;
			path = new std::vector<void*>();
			path->push_back((void*)idFrom);
			deletePath = true;
		}else
		{
			float cost = 0.0f;
			//check if we have already computed the path
			path = _waypointsLearningInstance->getPath(idFrom,idTo);
			if(path==nullptr)
			{
				//if the path it's not already computed
				path = new std::vector<void*>();

				int solved = _aStar->Solve((void*) idFrom, (void*) idTo, path, &cost);
				//if there isn't a solution return null
				if (solved == micropather::MicroPather::NO_SOLUTION || path->size() == 0) {
					delete path;
					return NULL;
				}else//else store this solution 
					_waypointsLearningInstance->addPath(idFrom,idTo,path);
			}

		}

		std::vector<Vector3>* out = new std::vector<Vector3>();
		// Añadimos el punto inicial si no coincide con el primer nodo
		if (!from.positionEquals(_wpg.getWaypoint((int) path->at(0)), WAYPOINT_TOLLERANCE))
			out->push_back(from);

		for (std::vector<void*>::iterator it = path->begin(); it != path->end(); it++)
		{
			out->push_back(_wpg.getWaypoint((int) (*it)));
		}

		// Añadimos el punto final si no coincide con el último nodo
		if (!to.positionEquals(_wpg.getWaypoint((int) path->at(path->size() - 1)), WAYPOINT_TOLLERANCE))
			out->push_back(to);
		//only if the path contains one waypoint that are the same from source and destination
		if(deletePath)
			delete path;//we have to free a memory in order to not create memory leak
		return out;
	}
	/////////////////////////////////////////
	/**
	Dado un ángulo en radianes lo lleva al intervalo [-PI, PI]
	*/
	double CServer::correctAngle(double angle)
	{
		while (angle > Math::PI)
			angle -= 2 * Math::PI;
		while (angle < -Math::PI)
			angle += 2 * Math::PI;
		return angle;
	}

	void CServer::tick(unsigned int msecs){

		_strategyController->tick(msecs);

		_entityManager->tick(msecs);

		_waveManager->tick(msecs);

		_gameManager->tick(msecs);

	}

	void CServer::resetManagers()
	{
		removeAllWaypoints();
		_strategyController->reset();
		_entityManager->reset();
		_waveManager->reset();
		_gameManager->reset();
		ResourceServer::getInstance()->reset();

		_waypointsLearningInstance->reset();

	}
} // namespace AI