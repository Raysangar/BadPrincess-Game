/**
@file WaypointsPathLearning.h

Clase para guardar el calculo de una ruta de waypoints y reutilizar ese dato guardado para ahorrar
los siguente calculo .

@see WaypointsPathLearning

@author Guidi Giacomo
@date Junio 2015

*/

#pragma once

#ifndef __AI_WaypointsPathLearning_H
#define __AI_WaypointsPathLearning_H

#include "BaseSubsystems/Math.h"

namespace AI {

	struct PathNode{
		int _idFrom;
		int _idTo;

		PathNode(int idFrom,int idTo):_idFrom(idFrom),_idTo(idTo){}


		bool operator<(const PathNode &o)  const 
		{
			return _idFrom < o._idFrom || (_idFrom == o._idFrom && _idTo < o._idTo);
		}
	};

	class CWaypointsPathLearning
	{
	public:
		/**
		Constructor
		*/
		CWaypointsPathLearning(void);
		/**
		Destructor
		*/
		~CWaypointsPathLearning(void);

		void reset();

		/*
		Function that save a calculed route 		
		@param idFrom index of start waypoint graph node
		@param idTo index of destination waypoint graph node
		@param pathList vector of waypoint graph node that compose the path
		*/
		void addPath(int idFrom,int idTo,std::vector<void*>* pathList);
		/*
		Function that find if there is a path already computed and stored for the waypoints passed as parameter
		@param idFrom index of start waypoint graph node
		@param idTo index of destination waypoint graph node

		*/
		std::vector<void*>* const getPath(int idFrom,int idTo);
		/*
		Free the memory and delete the path computed
		*/
		void clearPathList();


	private:
		/*
		Containers of path already computed
		*/
		std::map<PathNode,std::vector<void*>*> _pathSaved;	


	}; // class CWaypointsPathLearning

} // namespace AI

#endif __AI_WaypointsPathLearning_H
