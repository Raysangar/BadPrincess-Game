#include "RouteTo.h"

#include "Logic/MovementMessages.h"
#include "Logic/RouteCalculationMessages.h"

#include "Map/MapEntity.h"
#include "Logic/Entity/Entity.h"

#include "AI/Server.h"
#include "AI/Movement.h"

#include "Physics\PhysicPerceptionManager.h"

////DEBUG to draw line and point
//#include "Graphics/DrawManager.h"
//#include <OgreSceneNode.h>
//#include <OgreSceneManager.h>
//#include "Logic/Maps/Map.h"
//#include "Graphics/Scene.h"
//#include "Graphics/MeshEntity.h"
//#include "Graphics/Server.h"

namespace Logic 
{
	//this vector contains all the physic groups of the static entities that we want to avoid with the string algorithm	
	const Physics::CollisionGroup PHYSIC_ENTITY_FILTER []={Physics::CollisionGroup::eWorld};
	//---------------------------------------------------------

	IMP_FACTORY(CRouteTo);

	//---------------------------------------------------------
	/**
	Inicialización del componente, utilizando la información extraída de
	la entidad leída del mapa (Maps::CEntity). Toma del mapa el atributo
	speed que indica a la velocidad máxima a la que se moverá la entidad.

	@param entity Entidad a la que pertenece el componente.
	@param map Mapa Lógico en el que se registrará el objeto.
	@param entityInfo Información de construcción del objeto leído del
	fichero de disco.
	@return Cierto si la inicialización ha sido satisfactoria.
	*/
	bool CRouteTo::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		if ( entityInfo->hasAttribute( "physic_radius" ) )
            _radius = entityInfo->getFloatAttribute( "physic_radius" );

		return true;

	} // spawn

	//---------------------------------------------------------

	void CRouteTo::awake(){
		////DEBUG to draw line and point
  //      drawRaycast = _entity->getMap()->getScene()->getSceneMgr()->getRootSceneNode()->createChildSceneNode();
  //      drawLeftRaycast = _entity->getMap()->getScene()->getSceneMgr()->getRootSceneNode()->createChildSceneNode();
  //      drawRightRaycast = _entity->getMap()->getScene()->getSceneMgr()->getRootSceneNode()->createChildSceneNode();        		
		//pathRoute = _entity->getMap()->getScene()->getSceneMgr()->getRootSceneNode()->createChildSceneNode();        		
		_recalculateRoute=false;
		if(_currentRoute!=nullptr)
			delete _currentRoute;
		_currentRoute=nullptr;
		_arrived=true;
		_target=Vector3(0,0,0);
		_nextWaypoint=false;
		_currentNode=0; 
		_finishingRoute=false;
		_currentSteeringTarget=Vector3::ZERO;
	}

	//---------------------------------------------------------

	/**
	Método llamado en cada frame que actualiza el estado del componente.
	<p>
	Se encarga de mover la entidad en cada vuelta de ciclo cuando es necesario.

	@param msecs Milisegundos transcurridos desde el último tick.
	*/
	void CRouteTo::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);
		
		// En este método se gestiona el seguimiento de la ruta.
		// Realmente no se ocupa de mover la entidad, sino de controlar cuándo se ha 
		// llegado a un waypoint y notificar a SteeringMovement (que es quien mueve) 
		// del siguiente punto al que moverse y el tipo de movimiento a hacer.
		// El componente almacena su estado en las variables:
		// _target: destino final
		// _currentRoute: ruta actual (vector de posiciones)
		// _currentNode: posición de destino actual (índice del vector de posiciones)
		//
		// Lo primero que hay que hacer es recalcular la ruta si es necesario (usar la variable _recalculateRoute, que se establece en process)
		// Para recalcular la ruta usamos los métodos del servidor (getAStarRoute)
		// La posición de destino también se almacena en el método process en _target
		if (_recalculateRoute) {
			if (_currentRoute != NULL) delete _currentRoute;
			_currentRoute = AI::CServer::getSingletonPtr()->getAStarRoute(_entity->getPosition(), _target);
			_recalculateRoute = false;
			_finishingRoute = false;
			// Si no se puede calcular la ruta enviamos un mensaje de fallo
			if (_currentRoute == NULL) {
				_arrived = true;
				//TMessage message;
				//message._type = Message::FINISHED_ROUTE;
				//message._bool = false;
				//_entity->emitMessage(message, this);
				_finishingRoute = true;
				std::cout<<"error to calculate A*"<<std::endl;
			} else {
				//std::cout<<"calculate A*=";
				////DEBUG
				//pathlist.clear();
				//for(std::vector<Vector3>::const_iterator it=_currentRoute->begin();it!=_currentRoute->end();++it)
				//{
				//	pathlist.push_back(*it);
				//	std::cout<<" "<<*it;
				//}
				//_entity->getMap()->getScene()->getDrawManager()->drawPointList(pathlist,pathRoute,'b',"pathRouteA_"+std::to_string(_entity->getEntityID()),true);
				//std::cout<<std::endl;
				_currentNode = 0;
				_arrived = false;
				// Si hay ruta hacemos que vaya al primer punto
				sendMoveMessage(_currentRoute->at(0), AI::IMovement::MOVEMENT_KINEMATIC_SEEK);
			}
		}
		// A continuación tenemos que comprobar si hemos llegado ya al último punto de la ruta (variable _arrived).
		// Si no, comprobamos si hay que pasar al siguiente punto de la ruta.
		// Sabemos que tenemos que cambiar de punto al procesar un mensaje FINISHED_MOVE (variable _nextWaypoint).
		// Si hay que cambiar de nodo incrementamos el contador (_currentNode) y comprobamos si ya hemos terminado
		// la ruta. Distinguimos 2 (o 3) casos:
		//		· El punto al que hemos llegado era el último ==> 
		//			actualizamos _arrive 
		//			enviamos un mensaje para parar al componente de movimiento
		//			enviamos un mensaje para indicar que hemos terminado de seguir la ruta (FINISHED_ROUTE)
		//		· El punto al que hemos llegado era el penúltimo ==> (esto es opcional y puramente estético)
		//			enviamos un mensaje al componente de movimiento para hacer un movimiento de tipo ARRIVE al último punto
		//		· En otro caso ==> 
		//			enviamos un mensaje al componente de movimiento para hacer un movimiento de tipo SEEK al último punto
		if(!_arrived)
			stringAlgorithm();
		if (_nextWaypoint && !_arrived) {
			_nextWaypoint = false;
			// Pasamos al siguiente nodo
			_currentNode++;
			if (_currentNode >= _currentRoute->size()) {
				// Era el último nodo ==> parar
				_arrived = true;
				if (_currentRoute != NULL) 
				{
					delete _currentRoute;
					_currentRoute=NULL;
				}
//				sendMoveMessage(_target, AI::IMovement::MOVEMENT_NONE);
				// Enviar un mensaje para notificar que hemos llegado la destino
				//TMessage message;
				//message._type = Message::FINISHED_ROUTE;
				//message._bool = true;
				//_entity->emitMessage(message, this);

				auto m2 = std::make_shared<AvatarFinishedRoute>();
				_entity->emitMessageN(m2, this);
//				std::cout<<_entity->getName()<<"finished route sended target="<<_target<<std::endl;

			} else if (_currentNode == _currentRoute->size() - 1) {
				// Es el penúltimo nodo. Nos acercamos con Arrive
				sendMoveMessage(_currentRoute->at(_currentNode), AI::IMovement::MOVEMENT_KINEMATIC_ARRIVE);
			} else {
				// Nos movemos al siguiente
				sendMoveMessage(_currentRoute->at(_currentNode), AI::IMovement::MOVEMENT_KINEMATIC_ARRIVE);
			}
		}
	} // tick

	void CRouteTo::stringAlgorithm()
	{
		if(_currentRoute!=nullptr && _currentRoute->size()>0)
		{
			//loop through waypoints list from destination point to my current destination 
			std::vector<Vector3>::reverse_iterator rit = _currentRoute->rbegin();
			
			for (unsigned int index=_currentRoute->size()-1; index>_currentNode; ++rit,--index)
			{
				//launch a raycast to see if there is a direct free path from entity position to the waypoint 
				if(checkPathFreeToTarget(*rit))
				{
					_currentNode=index;
					////DEBUG
					//Graphics::CMeshEntity *e=new Graphics::CMeshEntity(std::to_string(rand()%1000)+"_wp_stringAlg","esfera.mesh");
					//Graphics::CServer::getSingletonPtr()->getScene("map.txt")->addEntity(e);
					//e->setPosition(*rit);
					//e->setScale(20);
					//std::cout<<"stringAlgorithm free to target into for="<<*rit<<" index="<<index<<std::endl;

					//if i see the final destination move the entity with arrive movement
					if(_currentNode==_currentRoute->size()-1)
						sendMoveMessage(*rit,AI::IMovement::MOVEMENT_KINEMATIC_ARRIVE);
					else
						sendMoveMessage(*rit,AI::IMovement::MOVEMENT_KINEMATIC_SEEK);
					break;
				}
			}
		}
	}

	bool CRouteTo::checkPathFreeToTarget(Vector3 target)
	{
		bool leftCollision=false;
		bool rightCollision=false;
		bool centerCollision=false;
		target.y=2;
		Vector3 position=_entity->getPosition();
		position.y=2;

		Vector3 dir = target - position;
		if(dir==Vector3::ZERO)
			return true;

//		dir.y = target.y;

		float dist=dir.length();
		dir.normalise();

		Ogre::Quaternion q;		
		q.FromAngleAxis(Ogre::Degree(90),Vector3::UNIT_Y);
		Vector3 normal = q * dir;			
		////DEBUG
		//_entity->getMap()->getScene()->getDrawManager()->drawLine(position,target,drawRaycast,'b',"raycastROUTETO_"+std::to_string(_entity->getEntityID()),true);
		//_entity->getMap()->getScene()->getDrawManager()->drawLine(position + (normal * _radius),target + (normal * _radius),drawLeftRaycast,'b',"raycastROUTETO_Left_"+std::to_string(_entity->getEntityID()),true);
		//_entity->getMap()->getScene()->getDrawManager()->drawLine(position - (normal * _radius),target - (normal * _radius),drawRightRaycast,'b',"raycastROUTETO_Right_"+std::to_string(_entity->getEntityID()),true);

		Logic::CEntity* entCol = Physics::CServer::getSingletonPtr()->getPhysicPerceptionManager()->raycastAdvanced(Ray(position, dir), dist,Vector3(), &std::vector<Physics::CollisionGroup> (PHYSIC_ENTITY_FILTER, PHYSIC_ENTITY_FILTER + sizeof(PHYSIC_ENTITY_FILTER) / sizeof(PHYSIC_ENTITY_FILTER[0])),_entity->getEntityID());		
		if(entCol != nullptr)
			return false;
		
		//entCol = Physics::CServer::getSingletonPtr()->getPhysicPerceptionManager()->raycastAdvanced(Ray(position + (normal * _radius), dir), dist,Vector3(), &std::vector<Physics::CollisionGroup> (PHYSIC_ENTITY_FILTER, PHYSIC_ENTITY_FILTER + sizeof(PHYSIC_ENTITY_FILTER) / sizeof(PHYSIC_ENTITY_FILTER[0])),_entity->getEntityID());
		//if(entCol!=nullptr)
		//	return false;

		//entCol = Physics::CServer::getSingletonPtr()->getPhysicPerceptionManager()->raycastAdvanced(Ray(position - (normal * _radius), dir), dist,Vector3(), &std::vector<Physics::CollisionGroup> (PHYSIC_ENTITY_FILTER, PHYSIC_ENTITY_FILTER + sizeof(PHYSIC_ENTITY_FILTER) / sizeof(PHYSIC_ENTITY_FILTER[0])),_entity->getEntityID());
		//if(entCol!=nullptr)
		//	return false;



		return true;
	}

	//---------------------------------------------------------
	/** 
	Envía un mensaje MOVE_TO.

	@param target Destino.
	@movementType Tipo de movimiento.
	*/
	void CRouteTo::sendMoveMessage(Vector3 target, int movementType)
	{

		_currentSteeringTarget = target;
		//JLC: nuevo sistema de mensajes
		auto m = std::make_shared<AvatarMoveToPosition>();
		m->target = target;
//		m->target.y = _entity->getPosition().y;
		m->moveType = movementType;
		_entity->emitMessageN(m, this);
	}

	//---------------------------------------------------------

	bool CRouteTo::acceptN(const std::shared_ptr<NMessage> &message)
	{
		return (message->type.compare("AvatarRouteTo") == 0) || (message->type.compare("AvatarFinishedMove") == 0)|| (message->type.compare("AvatarInterruptRoute") == 0);
	} // acceptN

	//---------------------------------------------------------

	void CRouteTo::processN(const std::shared_ptr<NMessage> &message)
	{

		if (message->type.compare("AvatarRouteTo")==0)
		{
			std::shared_ptr<AvatarRouteTo> m = std::static_pointer_cast<AvatarRouteTo>(message);
//			std::cout<<_entity->getName()<<" route to recived target "<<m->target;
			
			//if i see the target not compute the A* and move directly to that point
			if(checkPathFreeToTarget(m->target))
			{
				_recalculateRoute = false;
				_arrived = false;
				// Eliminamos la ruta
				if (_currentRoute != NULL) {
					_currentRoute->clear();
				}else
					_currentRoute=new std::vector<Vector3>();
				_currentRoute->push_back(m->target);
				_nextWaypoint=false;
				_currentNode=0;
				_target = m->target;
//				std::cout<<"process route to direct path"<<std::endl;

				sendMoveMessage(m->target,AI::IMovement::MOVEMENT_KINEMATIC_ARRIVE);

			}else if (m->calculateNewRoute) 				//PROCESO PARA ROUTE TO // Comprobamos el bool para saber si tenemos que // calcular una nueva ruta o detenernos
			{
					// Anotamos el vector de desplazamiento para usarlo posteriormente en 
					// el método tick. De esa forma, si recibimos varios mensajes ROUTE_TO
					// en el mismo ciclo sólo tendremos en cuenta el último.
					_target = m->target;
					_recalculateRoute = true;
//					std::cout<<"process route recalculate path"<<std::endl;

			} else {

				_recalculateRoute = false;
				_target = m->target;
				// if we don't want to recalculate a route and have already a computed route				
				if (_currentRoute != NULL) {
					// add the new final point to the route
					_currentRoute->push_back(_target);
//				std::cout<<"process route add point to path"<<std::endl;

				}else
				{
					//if we are here we haven't a new computed route but we don't see directly the target
					//and we have to recompute the A* we trust on steering movement to avoid entities
					_arrived = false;
					_currentRoute=new std::vector<Vector3>();
					_currentRoute->push_back(m->target);
					_nextWaypoint=false;
					_currentNode=0;
//					std::cout<<"process route exception"<<std::endl;

				}
				// Y enviamos un mensaje para parar
				
			}
				
		}else if (message->type.compare("AvatarFinishedMove")==0)
		{
			std::shared_ptr<AvatarRouteTo> m = std::static_pointer_cast<AvatarRouteTo>(message);
			// Hemos terminado el movimiento actual así que tenemos que pasar al
			// siguiente punto de la ruta (si existe)
//			std::cout<<_entity->getName()<<" avatarfinishedMove cst="<<_currentSteeringTarget<<"t="<<m->target<<std::endl;
			if(m->target.positionEquals(_currentSteeringTarget))
				_nextWaypoint = true;
		}else if (message->type.compare("AvatarInterruptRoute")==0)
		{
//			std::cout<<_entity->getName()<<" AvatarInterruptRoute"<<std::endl;
			_arrived = true;
			if (_currentRoute != NULL) 
			{
				delete _currentRoute;
				_currentRoute=NULL;
			}
			auto m = std::make_shared<AvatarInterruptMovement>();
			_entity->emitMessageN(m, this);
			auto m2 = std::make_shared<AvatarFinishedRoute>();			
			_entity->emitMessageN(m2, this);
		}
	} // processN

} //namespace Logic 

