/**
@file Server.h

Servidor de IA.

@author Gonzalo Flórez
@date Diciembre, 2010
*/
#pragma once

#ifndef __AI_Server_H
#define __AI_Server_H

#include "WaypointGraph.h"
#include "AStarFunctionsGaleon.h"
#include "micropather.h"

namespace AI {
	class CStrategyController;
	class CEntityManager;
	class CWaveManager;
	class CGameManager;
	class CWaypointsPathLearning;
/**
Servidor de IA. De momento sólo se encarga de mantener el
grafo con los waypoints y de centralizar las llamadas a A*.

La idea es que en un futuro también se ocupe de gestionar la 
percepción.
*/
class CServer
{
	public:

		/**
		Devuelve la única instancia de la clase.

		@return Puntero al servidor de IA.
		*/
		static CServer *getSingletonPtr() { return _instance; }
		
		/**
		Inicializa el servidor de IA. Esta operación sólo es necesario realizarla
		una vez durante la inicialización de la aplicación. 

		@return Devuelve false si no se ha podido inicializar.
		*/
		static bool Init();

		/**
		Libera el servidor de IA. Debe llamarse al finalizar la aplicación.
		*/
		static void Release();

		/**
		Función invocada en cada vuelta para actualización.

		@param secs Segundos desde que se renderizó el último frame.
		@return Valor booleano indicando si todo fue bien.
		*/
		void tick(unsigned int secs);

		/**
		Añade un nuevo nodo al grafo de navegación.
		*/
		void addWaypoint(Vector3 waypoint);
		/**
		Recalcula el grafo de navegación a partir de los nodos que han sido
		añadidos con addGraphNode.
		*/
		void computeNavigationGraph();
		/**
		Quita todo los nodos al grafo de navegación.
		*/
		void removeAllWaypoints();

		/**
		Devuelve el grafo de navegación.
		*/
		CWaypointGraph* getNavigationGraph() {return &_wpg; };
		/**
		Calcula una ruta usando A*.
		*/
		std::vector<Vector3> *getAStarRoute(Vector3 from, Vector3 to);
		/**
		Dado un ángulo en radianes lo lleva al intervalo [-PI, PI]
		*/
		static double correctAngle(double angle);

		CStrategyController * getStrategyController(){return _strategyController;}

		CEntityManager * getEntityManager(){return _entityManager;}

		CWaveManager * getWaveManager(){return _waveManager;}

		CGameManager * getGameManager(){return _gameManager;}
	
		void resetManagers();

	private:
		/**
		Constructor de la clase.
		*/
		CServer();

		/**
		Destructor de la clase.
		*/
		virtual ~CServer();
		/**
		Instancia única de la clase.
		*/
		static CServer *_instance;

		CWaypointGraph _wpg;
		/**
		Clase que se encarga de calcular la mejor ruta con A*
		*/
		micropather::MicroPather* _aStar;
		/** 
		Funciones de distancia para calcular A*
		*/
		CAStarFunctionsGaleon* _f;

		//Strategy Controller instance
		CStrategyController * _strategyController;
		//Entity Manager instance
		CEntityManager * _entityManager;
		//Wave Manager instance	
		CWaveManager * _waveManager;
		//Game Manager instance	
		CGameManager * _gameManager;
		//Waypoints learning instance
		CWaypointsPathLearning * _waypointsLearningInstance;

}; // class CServer

} // namespace AI
#endif