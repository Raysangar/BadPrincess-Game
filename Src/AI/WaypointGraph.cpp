/**
@file WaypointGraph.cpp

Clase para mantener grafos de puntos de ruta (waypoints).

@see WaypointGraph

@author Gonzalo Flórez
@date Diciembre 2010

*/
#include "WaypointGraph.h"
#include "DisjointSets.h"
#include "Physics\Server.h"
#include "Physics\PhysicPerceptionManager.h"

#include "Logic\Entity\Entity.h"

////DEBUG
/*
#include "Graphics\MeshEntity.h"
#include "Graphics\Scene.h"
#include "Graphics\Server.h"
#include "Graphics\ManualEntity.h"
#include "Graphics\DrawManager.h"
*/


namespace AI {
	
	//physic group of elements that the entity have to avoid
	const Physics::CollisionGroup PHYSIC_ENTITY_FILTER []={Physics::CollisionGroup::eResource,Physics::CollisionGroup::eWorld};

	/**
	Constructor
	*/
	CWaypointGraph::CWaypointGraph(void) : _generated(false) , _connected(false), _clean(false)
	{
	}
	/////////////////////////////////////////
	/**
	Destructor
	*/
	CWaypointGraph::~CWaypointGraph(void)
	{
	}
	/////////////////////////////////////////
	/**
	Añade un nuevo waypoint al grafo.
	*/
	void CWaypointGraph::addWaypoint(Vector3 waypoint) 
	{
		
		////DEBUG
		/*
		std::string name("wpg_"+std::to_string(_g.getNodeCount()));
		Graphics::CMeshEntity *e=new Graphics::CMeshEntity(name,"esfera.mesh");
		//_wpgGraphicsList.push_back(e);
		Graphics::CServer::getSingletonPtr()->getScene("survivorMap.txt")->addEntity(e);
		e->setPosition(waypoint);
		e->setScale(10);
		//std::cout<<"load wp="<<name<<" at "<<waypoint<<std::endl;
		*/
		
		_g.addNode(new CNode(waypoint));
		_clean = false;
	}

	void CWaypointGraph::removeAllWaypoints()
	{
		_g.resetGraph();
		_generated = false;
		_connected = false;
		_clean = false;
	}

	/////////////////////////////////////////
	/**
	Calcula las aristas entre los waypoints. Para ello, entre cada par 
	de waypoints hace una comprobación de visibilidad. Si un waypoint es
	visible desde otro, se añade una arista entre ellos.
	*/
	void CWaypointGraph::computeNavigationGraph()
	{
		// Generar las aristas para el grafo de navegación
		// Para cada par de nodos del grafo tenemos que comprobar si se puede llegar en línea recta
		// El grafo subyacente en el que se basa esta clase se encuentra en la variable _g, y es un 
		// grafo no dirigido implementado en la clase CUndirectedGraph. En el fichero de cabecera de 
		// esta clase se pueden encontrar los métodos para acceder a los nodos, añadir aristas, etc.
		// Los métodos que vais a necesitar serán getNode(i) para obtener las posiciones de cada nodo
		// y addEdge(i, j, dist) para añadir las aristas.
		// Como el grafo es no dirigido, sólo necesitamos hacer comparaciones en 1 sentido (si B es 
		// visible desde A sabemos que A es visible desde B). Al añadir la arista de A a B se añade 
		// automáticamente la arista de B a A.
		// Para comprobar la visibilidad usamos el método raycastClosest que nos proporciona el 
		// servidor de física. Este método recibe un rayo, la distancia máxima y el grupo de colisión 
		// y devuelve la entidad con la que ha colisionado o NULL. Sólo queremos añadir aristas si no 
		// hay colisión con el mundo (WORLD_COLISION_ENTITY: grupo 4)

		int numNodes = _g.getNodeCount();
		for (int i = 0; i < numNodes - 1; i++) {
			for (int j = i + 1; j < numNodes; j++) {
				const CNode* node1 = _g.getNode(i);
				Vector3 pos1 = node1->position;
				const CNode* node2 = _g.getNode(j);
				Vector3 pos2 = node2->position;
				pos1.y=1;
				pos2.y=1;
				Vector3 dir=pos2-pos1;
				dir.normalise();
				// Sacamos la dirección (en pos2) y la distancia
//				pos2 -= pos1;
				float dist = (pos2-pos1).length();
				// El rayo necesita la dirección normalizada
//				pos2 /= dist;
				// Comprobamos si el rayo choca contra el "mundo" (entidad de colisión 4)
				// Si el rayo no choca ==> devuelve NULL ==> Se puede andar entre los waypoints
				Logic::CEntity* entCol = 
					Physics::CServer::getSingletonPtr()->getPhysicPerceptionManager()->raycastAdvanced(Ray(pos1, dir), dist,Vector3(), &std::vector<Physics::CollisionGroup> (PHYSIC_ENTITY_FILTER, PHYSIC_ENTITY_FILTER + sizeof(PHYSIC_ENTITY_FILTER) / sizeof(PHYSIC_ENTITY_FILTER[0])));
				if (entCol == NULL) {
					_g.addEdge(i, j, dist);
					
					////DEBUG
					//Graphics::CManualEntity *e=new Graphics::CManualEntity("edge_"+std::to_string(i)+"_"+std::to_string(j));
					//Graphics::CServer::getSingletonPtr()->getScene("tutorialMap.txt")->addEntity(e);
					//_wpgGraphicsEdgeList.push_back(e);
					//e->drawSegment(pos1,pos2,'b');
					
				}
				//else
				//	std::cout<<"edge "<<i<<"_"<<j<<" collision with "<<entCol->getName()<<std::endl;
			}
		}
		_clean = true;
		_generated = true;
	}
	/////////////////////////////////////////
	/**
	Obtiene el waypoint más cercano a una posición dada.
	<p>
	Hace una búsqueda lineal en la lista de nodos y devuelve el más cercano.
	*/
	int CWaypointGraph::getClosestWaypoint(Vector3 position)
	{
		// Recorremos la lista de waypoints y devolvemos el más cercano a la posición dada.
		// En lugar de calcular la distancia podemos calcular la distancia al cuadrado 
		// (squaredDistance). De esta manera nos ahorramos una raíz cuadrada.
		// En algunos casos, el resultado de este método no es válido para calcular una ruta, pues
		// no existe un camino en línea recta desde la posición de entrada hasta el waypoint.
		// Para evitar este problema, podemos comprobar que el nodo es visible desde la posición 
		// indicada. Esto puede aumentar el coste de esta operación.
		int numNodes = _g.getNodeCount();
		if (numNodes == 0) return -1;

		int closestNode = 0;
		Vector3 node = _g.getNode(0)->position;
		double minDist = node.squaredDistance(position);
		for (int i = 1; i < numNodes; i++) {
			Vector3 node2 = _g.getNode(i)->position;
			double dist = node2.squaredDistance(position);
			if (dist <= minDist) {
				minDist = dist;
				closestNode = i;
			}
		}
		return closestNode;
	}

	/** 
 	Cambia la altura de todos los waypoints del grafo

	@param height Nueva altura
	*/	
	void CWaypointGraph::setWaypointHeight(float height) 
	{
		for (int i = 0; i < _g.getNodeCount(); i++) {
			_g.getNode(i)->position.y = height;
		}
	}

}