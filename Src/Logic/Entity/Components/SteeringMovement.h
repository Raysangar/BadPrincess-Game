/**
@file SteeringMovement.h

Contiene la declaración de la clase CSteringMovement. Se encarga de
mover un avatar utilizando movimientos cinéticos o dinámicos.

@author Gonzalo Flórez
@date Diciembre, 2010
*/
#pragma once

#ifndef __Logic_SteeringMovement_H
#define __Logic_SteeringMovement_H

#include "Logic/Entity/Component.h"
#include "AI/Movement.h"
#include "AI/Server.h"
#include "AI/KinematicMovement.h"
#include "AI/DynamicMovement.h"

//DEBUG
//namespace Ogre{
//	class SceneNode;
//}

namespace Logic 
{

	/**
	Componente que se encarga de trasladar una entidad de un punto a otro
	usando SteeringMovements. 
	Acepta mensajes del tipo MOVE_TO. Los parámetros relevantes del mensaje
	son el _vector, que indica el destino del movimiento, y el _int que indica 
	el tipo de movimiento (ver los tipos en IMovement).
	*/
	class CSteeringMovement : public IComponent
	{
		DEC_FACTORY(CSteeringMovement);

	public:
		/**
		This constructor initialize the component with its component name 
		and the rest attributes with their default value.
		*/
		CSteeringMovement(std::string componentName) : IComponent(componentName), _arrived(true), 
			_maxLinearSpeed(0.05f), _maxAngularSpeed(0.01f),
			_maxLinearAccel(0.001f), _maxAngularAccel(0.001f), _currentMovement(0), 
			_currentMovementType(AI::IMovement::MOVEMENT_NONE), _yaw(NULL), _evade(NULL),
			_tolerance(0), _currentTarget(Vector3(0,0,0)), _target(Vector3(0,0,0)),_stoppedPos(Vector3::ZERO),_stoppedPosTime(0),_oldCollisionDirection(Vector3::ZERO),
			_movType(AI::IMovement::MOVEMENT_NONE),_currentMaxLinearSpeed(0.05f) {}

		/**
		Destructor
		*/
		~CSteeringMovement(void);
		/**
		Inicialización del componente, utilizando la información extraída de
		la entidad leída del mapa (Maps::CEntity). Toma del mapa los atributos
		speed, angularSpeed, accel y angularAccel, que indican los valores máximos 
		de velocidad y aceleración lineales y angulares.

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa Lógico en el que se registrará el objeto.
		@param entityInfo Información de construcción del objeto leído del
			fichero de disco.
		@return Cierto si la inicialización ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		@author Rayco Sánchez García
		@date February, 2015

		Method called after all components are created and added
		to the entity.
		*/
		virtual void awake();

		/**
		Método llamado en cada frame que actualiza el estado del componente.
		<p>
		Se encarga de mover la entidad en cada vuelta de ciclo cuando es necesario.

		@param msecs Milisegundos transcurridos desde el último tick.
		*/
		virtual void tick(unsigned int msecs);
		/**
		Este componente sólo acepta mensajes de tipo MOVE_TO.
		*/
		virtual bool acceptN(const std::shared_ptr<NMessage> &message);
		/**
		Si se recibe un mensaje de tipo MOVE_TO, almacena el vector como 
		destino del movimiento y el tipo de movimiento.
		*/
		virtual void processN(const std::shared_ptr<NMessage> &message);
		/**
			Return the current speed vector
		*/
		Vector3 getCurrentSpeed(){return _currentProperties.linearSpeed;};
		
		void setMaxLinearSpeed(float newMaxLeinearSpeed);

		float getMaxLinearSpeed() const { return _currentMaxLinearSpeed; } 

		/*
			function that stop the movement
			@param msecs millisecond passed from last tick
		*/
		void sendStopMessage(bool destinationReached = true);

	private:
		/** 
		Valores máximos de velocidades y aceleraciones.
		*/
		float _maxLinearSpeed, _maxAngularSpeed, _maxLinearAccel, _maxAngularAccel, _tolerance, _currentMaxLinearSpeed;
		/**
		Este campo indica si se ha alcanzado el objetivo
		*/
		bool _arrived;
		/**
		Destino del movimiento.
		*/
		Vector3 _currentTarget, _target;
		/**
		Tipo de movimiento.
		*/
		int _currentMovementType, _movType;
		/**
		Instancia del movimiento actual.
		*/
		AI::IMovement* _currentMovement;
		/**
		Instancia de un movimiento de giro que se utilizará para mantener la dirección.
		*/
		AI::IMovement* _yaw;
		/**
		Instancia de un movimiento de esquivación que se utilizará como ejemplo de la combinación de movimientos
		*/
		AI::IMovement* _evade;

		AI::IMovement::DynamicMovement _evadeProperties;

		std::list<Vector3> *_collisionPointList;
		/**
		Propiedades actuales del movimiento. En esta variable se almacenan las velocidades actuales, que 
		serán consultadas por los movimientos dinámicos */
		AI::IMovement::DynamicMovement _currentProperties;

		/*
			function that check if the entity will collide to some other entities if it continue to move in current direction
			@param collisionPoint out value that contains where the raycast collided
			@param collisionNormal out value that contains the normal value from the point where the raycast collided
			@return the pointer to the entity which it will collide or nullptr
		*/
		Logic::CEntity* checkCollisionAvoidance(Vector3 &collisionPoint,Vector3 &collisionNormal);
		/*
			function that check if a position is inside a convex polygon
			@param nverts number of polygon vertices
			@param vert aray of polygon vertices
			@param p position of the target to check
		*/
		bool checkPointInsidePolygon2d(int nvert, Vector2 *vert, Vector2 p);
		/*
			function that check the speed of entity asking directly the linear speed to the steering component
			@param e the entity which i want to know the speed
			@return the liner speed of entity or Vector3::ZERO if the entity haven't the steeringMovement component
		*/
		Vector3 getEntityMovement(Logic::CEntity*e);
		/*
			function that launch a raycast from target position torward Vector3::UNIT_Y direction
			and check if it collide with the entity

			@param e the entity which i test the collision
			@return true if the ray collide with entity
		*/
		bool isTargetOccupedFromEntity(Logic::CEntity*e);
		/*
			function that launch a raycast torward the orientation vector from the center of entity
			and check if it collide with some entity.
			Also check if the target it's not reachable 

			@param orientation orientation of the entity
			@param position position of the entity
			@param lengthRaycast length of raycast
			@param collisionPoint out value that contains where the raycast collided
			@param collisionNormal out value that contains the normal value from the point where the raycast collided
			@param collisionEntity out value that contains the pointer to the entity which the raycast collided
			@return true if the ray collide with an entity
		*/
		bool launchRaycastCollision(const Vector3 &orientation,const Vector3 &position,float lenghtRaycast,Vector3 &collisionPoint,Vector3 &collisionNormal,Logic::CEntity **collisionEntity);
		/*
			function that launch a raycast parallel to the orientation vector
			and check if it collide with some entity.
			

			@param orientation orientation of the entity
			@param position position of the entity
			@param distance distance from orientation vector
			@param lengthRaycast length of raycast
			@param collisionPoint out value that contains where the raycast collided
			@param collisionNormal out value that contains the normal value from the point where the raycast collided
			@param collisionEntity out value that contains the pointer to the entity which the raycast collided
			@param direction -1 or 1 to define the left or right side
			@param pStart out value of raycast starting point 
			@return true if the ray collide with an entity
		*/

		bool launchParallelRaycastCollision(const Vector3 &orientation,const Vector3 &position,int direction,float distance,float lenghtRaycast,Vector3 &collisionPoint,Vector3 &collisionNormal,Logic::CEntity **collisionEntity,Vector3 &pStartRay);
		/*
			function that check if the target point is inside a circle area composed from my position and a collision point

			@param orientation orientation of the entity
			@param position position of the entity
			@param collisionPoint out value that contains where the raycast collided
			@param collisionNormal out value that contains the normal value from the point where the raycast collided
			@return true the target is inside the area
		*/
		bool checkTargetInsideCollisionArea(const Vector3 &orientation,const Vector3 &position,const Vector3 &collisionPoint,const Vector3 &collisionNormal);


		/*
			function that compute the movement to avoid the collision

			@param collisionPoint the point where the raycast collided
			@param collisionNormal the normal value from the point where the raycast collided
			@param msecs millisecond passed from last tick
		*/
		void calculateAvoidance(const Vector3 &collisionPoint,const Vector3 &collisionNormal,unsigned int msecs);
		/*
			function that calculate the steering movement to avoid collision
			@param msecs millisecond passed from last tick
		*/
		void steeringAvoid(unsigned int msecs);
		/*
			function that change the target and intitialize the new movement
			@param msecs millisecond passed from last tick
		*/
		void changeMovement(unsigned int msecs);
		/*
			function that send the movement message to the phisics
			@param msecs millisecond passed from last tick
		*/
		void computeMovement(unsigned int msecs);

		bool getLineIntersection(float p0_x, float p0_y, float p1_x, float p1_y,float p2_x, float p2_y, float p3_x, float p3_y, float *i_x, float *i_y);

		void computeParallelMovement(Vector3 &eVel);

		void computePerpendicularMovement(const Vector3 &collisionPoint,const Vector3 &collisionNormal,unsigned int msecs);

		/*
			function that check if there is a free path to the target point and in case compute the avoid movement
			@param msecs millisecond passed from last tick
		*/
		void calculateLateralAvoidance(unsigned int msecs);
		/*
			function that rotate the entity orientation to prefixed angle to escape from closing cone
		*/
		void tryToEscapeFromCloseCone();

		void resetAvoidCollisionVariable(unsigned int msecs);

		////DEBUG to draw line and point 
		//Ogre::SceneNode* drawRaycast;
		//Ogre::SceneNode* drawLeftRaycast;
		//Ogre::SceneNode* drawRightRaycast;
		//Ogre::SceneNode* drawTargetBox;
		//Ogre::SceneNode* drawNormal;
		
		float _radius;
		float _angle;

		int _stoppedPosTime;
		Vector3 _stoppedPos;
		Vector3 _oldCollisionDirection;

	};

	REG_FACTORY(CSteeringMovement);

} //namespace Logic

#endif //__Logic_SteeringMovement_H
