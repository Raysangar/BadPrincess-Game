/**
@file WaypointsPathLearning.cpp

@see WaypointsPathLearning

@author Guidi Giacomo
@date Junio 2015

*/
#include "WaypointsPathLearning.h"


namespace AI {
	
	/**
	Constructor
	*/
	CWaypointsPathLearning::CWaypointsPathLearning(void)
	{
		_pathSaved.clear();
	}
	/////////////////////////////////////////
	/**
	Destructor
	*/
	CWaypointsPathLearning::~CWaypointsPathLearning(void)
	{
		clearPathList();
	}

	void CWaypointsPathLearning::reset()
	{
		clearPathList();
	}

	void CWaypointsPathLearning::addPath(int idFrom,int idTo,std::vector<void*>* pathList)
	{
		PathNode node(idFrom,idTo);
		std::map<PathNode,std::vector<void*>*>::const_iterator it = _pathSaved.find(node);
		if(it==_pathSaved.end())
			_pathSaved[node]=pathList;
		else
			std::cout<<"Error to add path: already exists this path"<<std::endl;
	}

	std::vector<void*>* const CWaypointsPathLearning::getPath(int idFrom,int idTo)
	{
		PathNode node(idFrom,idTo);
		std::map<PathNode,std::vector<void*>*>::const_iterator it = _pathSaved.find(node);
		if(it!=_pathSaved.end())
			return it->second;
		else
			return nullptr;
		
	}

	void CWaypointsPathLearning::clearPathList()
	{
		std::map<PathNode,std::vector<void*>*>::const_iterator it = _pathSaved.begin();
		for(;it!=_pathSaved.end();++it)
			delete it->second;
		_pathSaved.clear();
	}

}