/**
@file AvatarController.cpp

Contiene la implementaci�n del componente que controla el movimiento 
de la entidad.
 
@see Logic::CAvatarController
@see Logic::IComponent

@author David Llans�
@date Agosto, 2010
*/

#include "AvatarController.h"

#include "Logic/UnitMessages.h"
#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"

// Includes para pruebas de paso de mensajes a componentes de IA
#include "AI/Server.h"
#include "AI/Movement.h"


namespace Logic 
{
	IMP_FACTORY(CAvatarController);

	//--------------------------------------------------------

	CAvatarController::CAvatarController(std::string componentName) : IComponent(componentName), _walking(false), _walkingBack(false), 
			_strafingLeft(false), _strafingRight(false), _speed(0.05f) 
		{
			avatarActions = std::map<std::string, AvatarAction>();
			avatarActions["walk"] = &CAvatarController::walk;
			avatarActions["walkBack"] = &CAvatarController::walkBack;
			avatarActions["stopWalk"] = &CAvatarController::stopWalk;
			avatarActions["strafeLeft"] = &CAvatarController::strafeLeft;
			avatarActions["strafeRight"] = &CAvatarController::strafeRight;
			avatarActions["stopStrafe"] = &CAvatarController::stopStrafe;
			avatarActions["specialAction"] = &CAvatarController::specialAction;
		}
	
	//---------------------------------------------------------

	bool CAvatarController::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;
		
		if(entityInfo->hasAttribute("speed"))
			_speed = entityInfo->getFloatAttribute("speed");

		return true;

	} // spawn
	
	//---------------------------------------------------------

	void CAvatarController::awake(){
		 _walking=false;
		 _walkingBack=false; 
		_strafingLeft=false;
		_strafingRight=false;
	}

	//---------------------------------------------------------

	bool CAvatarController::activate()
	{			
		return true;
	} // activate
	
	//---------------------------------------------------------

	void CAvatarController::deactivate()
	{

	} // deactivate
	
	//---------------------------------------------------------

	bool CAvatarController::acceptN(const std::shared_ptr<NMessage> &message)
	{
		return (message->type.compare("AvatarControl") == 0);
	} // accept
	//---------------------------------------------------------

	void CAvatarController::processN(const std::shared_ptr<NMessage> &message)
	{
		std::shared_ptr<AvatarControl> m = std::static_pointer_cast<AvatarControl>(message);
		if(!m->controlType.compare("turn"))
			turn(m->attribute);
		else
			(*this.*avatarActions[m->controlType])();
			
	} // process

	//---------------------------------------------------------

	void CAvatarController::turn(float amount) 
	{
		_entity->yawN(amount);

	} // turn
	
	//---------------------------------------------------------

	void CAvatarController::walk() 
	{
		_walking = true;

		// Cambiamos la animaci�n
//		TMessage message;
		//message._type = Message::SET_ANIMATION;
		//message._string = "Walk";
		//message._bool = true;
		//_entity->emitMessage(message,this);

	} // walk
	
	//---------------------------------------------------------

	void CAvatarController::walkBack() 
	{
		_walkingBack = true;

		// Cambiamos la animaci�n
		//TMessage message;
		//message._type = Message::SET_ANIMATION;
		//message._string = "WalkBack";
		//message._bool = true;
		//_entity->emitMessage(message,this);

	} // walkBack
	
	//---------------------------------------------------------

	void CAvatarController::stopWalk() 
	{
		_walking = _walkingBack = false;

		// Cambiamos la animaci�n si no seguimos desplaz�ndonos
		// lateralmente
		if(!(_strafingLeft || _strafingRight))
		{
			//TMessage message;
			//message._type = Message::SET_ANIMATION;
			//message._string = "Idle";
			//message._bool = true;
			//_entity->emitMessage(message,this);
		}

	} // stopWalk
	
	//---------------------------------------------------------

	void CAvatarController::strafeLeft() 
	{
		_strafingLeft = true;

		// Cambiamos la animaci�n
		//TMessage message;
		//message._type = Message::SET_ANIMATION;
		//message._string = "StrafeLeft";
		//message._bool = true;
		//_entity->emitMessage(message,this);

	} // walk
	
	//---------------------------------------------------------

	void CAvatarController::strafeRight() 
	{
		_strafingRight = true;

		// Cambiamos la animaci�n
		//TMessage message;
		//message._type = Message::SET_ANIMATION;
		//message._string = "StrafeRight";
		//message._bool = true;
		//_entity->emitMessage(message,this);

	} // walkBack
	
	//---------------------------------------------------------
	void CAvatarController::specialAction() 
	{
		// TODO PR�CTICA IA
		// Este m�todo es llamado cuando se activa la acci�n especial del jugador (por defecto con la tecla E).
		// Aqu� se pueden enviar mensajes para testear el funcionamiento de otros componentes.

		// Descomentar el siguiente c�digo para probar solamente los steering behaviours
		//TMessage message2;
		//message2._type = Message::MOVE_TO;
		//message2._vector3 = _entity->getPosition() + _entity->getOrientation() * Vector3(0.0, 0.0, -100.0);
		//message2._int = AI::IMovement::MOVEMENT_DYNAMIC_ARRIVE;
		//_entity->emitMessage(message2, this);


		// Descomentar el siguiente c�digo para probar el seguimiento de rutas
		AI::CWaypointGraph *wg = AI::CServer::getSingletonPtr()->getNavigationGraph();
		int waypoints = wg->getWaypointCount();
		int targetId = rand() % waypoints;
		Vector3 target = wg->getWaypoint(31);
		//std::cout << "Navigating to: " << target << std::endl;
		//TMessage message2;
		//message2._type = Message::ROUTE_TO;
		//message2._bool = true;
		//message2._vector3 = target;
		//_entity->emitMessage(message2, this);


	} // specialAction
	
	//---------------------------------------------------------

	void CAvatarController::stopStrafe() 
	{
		_strafingLeft = _strafingRight = false;

		// Cambiamos la animaci�n si no seguimos andando
		if(!(_walking || _walkingBack))
		{
			//TMessage message;
			//message._type = Message::SET_ANIMATION;
			//message._string = "Idle";
			//message._bool = true;
			//_entity->emitMessage(message,this);
		}

	} // stopWalk
	
	//---------------------------------------------------------

	void CAvatarController::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);

		// Si nos estamos desplazando calculamos la pr�xima posici�n
		// Calculamos si hay vectores de direcci�n de avance y strafe,
		// hayamos la direcci�n de la suma y escalamos seg�n la
		// velocidad y el tiempo transcurrido.
		//std::cout << _entity->getPosition().x << " " << _entity->getPosition().y << " " << _entity->getPosition().z << std::endl;

		if(_walking || _walkingBack || _strafingLeft || _strafingRight)
		{
			Vector3 direction(Vector3::ZERO);
			Vector3 directionStrafe(Vector3::ZERO);

			if(_walking || _walkingBack)
			{
				direction = Math::getDirection(_entity->getYaw());
				if(_walkingBack)
					direction *= -1;
			}

			if(_strafingLeft || _strafingRight)
			{
				directionStrafe = 
						Math::getDirection(_entity->getYaw() + Math::PI/2);
				if(_strafingRight)
					directionStrafe *= -1;
			}

			direction += directionStrafe;
			direction.normalise();
			direction *= msecs * _speed;

			// Enviar un mensaje para que el componente f�sico mueva el personaje
			//TMessage message;
			//message._type = Message::AVATAR_WALK;
			//message._vector3 = direction;
			//_entity->emitMessage(message);

			//Vector3 newPosition = _entity->getPosition() + direction;
			//_entity->setPosition(newPosition);
		}

	} // tick

} // namespace Logic

