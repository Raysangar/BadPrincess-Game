/**
@file SteeringMovement.cpp

Contiene la declaración de la clase CSteringMovement. Se encarga de
mover un avatar utilizando movimientos cinéticos o dinámicos.

@author Gonzalo Flórez
@date Diciembre, 2010
*/
#include "SteeringMovement.h"

#include "Map/MapEntity.h"

#include "AI/Movement.h"

#include "Logic/Server.h"

#include "Physics\PhysicPerceptionManager.h"

#include "Logic/MovementMessages.h"
#include "Logic/AnimationMessages.h"
#include "Logic/RouteCalculationMessages.h"
#include "Logic/UnitMessages.h"
#include "Logic/Entity/Entity.h"



//DEBUG to draw line and point
//#include "Graphics/DrawManager.h"
//#include <OgreSceneNode.h>
//#include <OgreSceneManager.h>
//#include "Logic/Maps/Map.h"
//#include "Graphics/Scene.h"
//#include "Graphics/Server.h"


//Lenght of the central raycast used to avoid collision
#define RAYCAST_LENGTH 8
//Lenght of force used to bounce off the collision item
#define COLLISION_AVOID_FORCE 2
//length used to positioning the collision point far away from the entity
//to mantain the magnitude of speed(useful in case we use arrive movement) 
#define COLLISION_AVOID_DISTANCE 8
//lateral offset used to check collision
#define RADIUS_OFFSET 0.7f
//rotation to apply when it try to escape from closed cone angle
#define AVOID_CLOSED_CONE_ROTATION Math::PI/2
//angle factor between two entity's directions that represent the lowest value
//to determine if the two entities are going to the same direction
#define COLLISION_DIRECTION_PARALLEL_FACTOR 0.7
//speed attenuation in case of two entities are going to the same direction
#define COLLISION_PARALLEL_ATTENUATION 5
//time used to determine if an entity are in a close area
#define STOPPED_POS_TIME_TOLLERANCE 700
//distance tollerance used to determine if an entity are in a close area
#define STOPPED_POS_TOLLERANCE 1
//raycast height used to collide with all entities
#define ENTITY_HEIGHT 5
//degree angle to determine if the collision normal is next to be inverse to entity orientation  
#define NORMAL_INVERSE_ANGLE -50

namespace Logic
{

    //physic group of elements that the entity have to avoid
	const Physics::CollisionGroup PHYSIC_COLLISION_ENTITY_FILTER []={Physics::CollisionGroup::eResource, Physics::CollisionGroup::eChest, Physics::CollisionGroup::eWorld, 
		Physics::CollisionGroup::ePlayer, Physics::CollisionGroup::eEnemy};

    IMP_FACTORY( CSteeringMovement );

    //---------------------------------------------------------
    /**
    Destructor
    */
    CSteeringMovement::~CSteeringMovement()
    {
        if ( _currentMovement != 0 )
            delete _currentMovement;
        if ( _yaw != 0 )
            delete _yaw;
        if ( _evade != NULL )
            delete _evade;
        ////DEBUG to draw line and point
        //_collisionPointList->clear();
        //delete _collisionPointList;
    }

    //---------------------------------------------------------
    /**
    Inicialización del componente, utilizando la información extraída de
    la entidad leída del mapa (Maps::CEntity). Toma del mapa los atributos
    speed, angularSpeed, accel y angularAccel, que indican los valores máximos
    de velocidad y aceleración lineales y angulares. El atributo tolerance indica
    la distancia a la que se considera que se ha alcanzado una posición.

    @param entity Entidad a la que pertenece el componente.
    @param map Mapa Lógico en el que se registrará el objeto.
    @param entityInfo Información de construcción del objeto leído del
    fichero de disco.
    @return Cierto si la inicialización ha sido satisfactoria.
    */
    bool CSteeringMovement::spawn( CEntity *entity, CMap *map, const Map::CEntity *entityInfo )
    {
        if ( !IComponent::spawn( entity, map, entityInfo ) )
            return false;

        if ( entityInfo->hasAttribute( "speed" ) )
            _maxLinearSpeed = entityInfo->getFloatAttribute( "speed" );
        if ( entityInfo->hasAttribute( "angularSpeed" ) )
            _maxAngularSpeed = entityInfo->getFloatAttribute( "angularSpeed" );
        if ( entityInfo->hasAttribute( "accel" ) )
            _maxLinearAccel = entityInfo->getFloatAttribute( "accel" );
        if ( entityInfo->hasAttribute( "angularAccel" ) )
            _maxAngularAccel = entityInfo->getFloatAttribute( "angularAccel" );
        if ( entityInfo->hasAttribute( "movementTolerance" ) )
            _tolerance = entityInfo->getFloatAttribute( "movementTolerance" );
        if ( entityInfo->hasAttribute( "physic_radius" ) )
            _radius = entityInfo->getFloatAttribute( "physic_radius" );

        // En yaw guardamos el movimiento correspondiente a la rotación de la entidad.
        // En este caso suponemos que la entidad siempre se mueve hacia adelante,
        // así que hacemos que siempre esté alineado con su vector de velocidad.

		////DEBUG to draw line and point
  //      _collisionPointList = new std::list<Vector3>();
        return true;

    } // spawn

    //---------------------------------------------------------

    void CSteeringMovement::awake() {

        //DEBUG to draw line and point
        //drawRaycast = _entity->getMap()->getScene()->getSceneMgr()->getRootSceneNode()->createChildSceneNode();
        //drawLeftRaycast = _entity->getMap()->getScene()->getSceneMgr()->getRootSceneNode()->createChildSceneNode();
        //drawRightRaycast = _entity->getMap()->getScene()->getSceneMgr()->getRootSceneNode()->createChildSceneNode();
        //drawTargetBox = _entity->getMap()->getScene()->getSceneMgr()->getRootSceneNode()->createChildSceneNode();
        //drawNormal = _entity->getMap()->getScene()->getSceneMgr()->getRootSceneNode()->createChildSceneNode();
		 _arrived=true; 
		 _currentMaxLinearSpeed = _maxLinearSpeed;
		 if(_currentMovement!=nullptr)
			 delete _currentMovement;
		 _currentMovement=0; 
		 _currentMovementType=AI::IMovement::MOVEMENT_NONE;
		 if(_yaw != NULL)
			 delete _yaw;
	    _yaw = AI::IMovement::getMovement( AI::IMovement::MOVEMENT_KINEMATIC_ALIGN_TO_SPEED,
                                        _currentMaxLinearSpeed, _maxAngularSpeed, _maxLinearAccel, _maxAngularAccel );
	    _yaw->setEntity( _entity );

		 if ( _evade != NULL )
            delete _evade;
		 _evade =NULL;
		_currentTarget=Vector3(0,0,0);
		_target=Vector3(0,0,0);
		_stoppedPos=Vector3::ZERO;
		_stoppedPosTime=0;
		_oldCollisionDirection = Vector3::ZERO;
		_movType=AI::IMovement::MOVEMENT_NONE;
    }

    //---------------------------------------------------------

    /**
    Método llamado en cada frame que actualiza el estado del componente.
    <p>
    Se encarga de mover la entidad en cada vuelta de ciclo cuando es necesario.

    @param msecs Milisegundos transcurridos desde el último tick.
    */
    void CSteeringMovement::tick( unsigned int msecs )
    {
        IComponent::tick( msecs );
		
        // Comprobamos si hay que cambiar de movimiento o destino
        if ( _movType != _currentMovementType || _currentTarget != _target ) {
            this->changeMovement( msecs );
        }

		
		//if we are moving and we are closest to the target position
        bool justArrived = !_arrived && _entity->getPosition().positionEquals( _target, _tolerance );
        if ( justArrived ) {
			//stopping the movement
            sendStopMessage();

            //DEBUG to draw line and point
            //if ( _collisionPointList->size() > 1 )
            //{
            //    _entity->getMap()->getScene()->getDrawManager()->drawPointList( *_collisionPointList, drawTargetBox, 'r', "targetBox_" + _entity->getEntityID(), true );
            //}
            //_collisionPointList->clear();

        }

        // Si nos estamos desplazando calculamos la próxima posición
        if ( !_arrived && _currentMovement != NULL ) {
			//check if we are in the same position for a prefixed time due to impossible physic movement
            if(_stoppedPos.positionEquals(_entity->getPosition(),STOPPED_POS_TOLLERANCE))
            {
            	_stoppedPosTime+=msecs;
            	//std::cout<<_entity->getName()<<" is in the same position"<<_stoppedPosTime<<std::endl;
            	if(_stoppedPosTime>=STOPPED_POS_TIME_TOLLERANCE)
            	{
            		//std::cout<<_entity->getName()<<" blocked into a position"<<std::endl;
            		this->sendStopMessage();
            		return;
            	}
            }else
            {
            	_stoppedPos=_entity->getPosition();
            	_stoppedPosTime=0;
            }
            _currentMovement->move( msecs, _currentProperties );
			//check avoidance
            this->steeringAvoid( msecs );

            // Calculamos la rotación
            _yaw->move( msecs, _currentProperties );

            ////DEBUG to draw line and point
            //Vector3 n=_currentProperties.linearSpeed.normalisedCopy();
            //Vector3 p1End= _entity->getPosition() + ( n * 10 );
            //_entity->getMap()->getScene()->getDrawManager()->drawLine(_entity->getPosition(),p1End,drawLeftRaycast,'w',"raycastLeft_"+_entity->getEntityID(),true);

            this->computeMovement( msecs );
        }

    } // tick

    void CSteeringMovement::computeMovement( unsigned int msecs )
    {
		// Acelerar
        _currentProperties.linearSpeed += _currentProperties.linearAccel * ( Ogre::Real )msecs;
        // Clipping
        float speedValue = _currentProperties.linearSpeed.length();
        if ( speedValue > _currentMaxLinearSpeed ) {
            _currentProperties.linearSpeed *= ( _currentMaxLinearSpeed / speedValue );
        }

        _currentProperties.angularSpeed += _currentProperties.angularAccel * msecs;
        if ( _currentProperties.angularSpeed > _maxAngularSpeed )
            _currentProperties.angularSpeed = Ogre::Math::Sign( ( Ogre::Real )_currentProperties.angularSpeed ) * _maxAngularSpeed;

        // Enviar un mensaje para que el componente físico mueva el personaje
        //JLC: Nuevo sistema de mensajes
        auto m = std::make_shared<AvatarMove>();
        m->movement = _currentProperties.linearSpeed * ( Ogre::Real )msecs;
        _entity->emitMessageN( m, this );

        // Aplicar la rotación
        //JLC: nuevo sistema de mensajes
        auto m2 = std::make_shared<AvatarControl>();
        m2->controlType = "turn";
        m2->attribute = ( float ) _currentProperties.angularSpeed * ( Ogre::Real )msecs;
        _entity->emitMessageN( m2, this );



    }

    //---------------------------------------------------------

    bool CSteeringMovement::acceptN( const std::shared_ptr<NMessage> &message )
    {
        return ( message->type.compare( "AvatarMoveToPosition" ) == 0 )||( message->type.compare( "AvatarInterruptMovement" ) == 0);
    } // acceptN

    //---------------------------------------------------------

    void CSteeringMovement::processN( const std::shared_ptr<NMessage> &message )
    {
		if ( message->type.compare( "AvatarMoveToPosition" ) == 0 )
        {
            std::shared_ptr<AvatarMoveToPosition> m = std::static_pointer_cast<AvatarMoveToPosition>( message );
            
			//rotate the entity to the target point directly
            Vector3 t = m->target;
            t.y = _entity->getPosition().y;
            Vector3 direction = t - _entity->getPosition();
            Vector3 orientation = Math::getDirection( _entity->getOrientation() );
            Quaternion q = orientation.getRotationTo( direction );
            float actualYaw = _entity->getYaw();
            _entity->setYawN( actualYaw + q.getYaw().valueRadians() );

			// Anotamos el vector de desplazamiento para usarlo posteriormente en
            // el método tick. De esa forma, si recibimos varios mensajes AVATAR_MOVE
            // en el mismo ciclo sólo tendremos en cuenta el último.
            _target = m->target;
            _movType = m->moveType;
			_arrived = false;
			resetAvoidCollisionVariable(0);

        }else if( message->type.compare( "AvatarInterruptMovement" ) == 0 )
		{
			_arrived = true;
			_currentProperties.reset();
		}
    }
    void CSteeringMovement::changeMovement( unsigned int msecs )
    {
        if ( _currentMovement != NULL ) {
            delete _currentMovement;
        }
        if ( _evade != NULL ) {
			delete _evade;
        }

        //DEBUG to draw line and point
        //if ( _collisionPointList->size() > 1 )
        //{
        //    _entity->getMap()->getScene()->getDrawManager()->drawPointList( *_collisionPointList, drawTargetBox, 'r', "targetBox_" + _entity->getEntityID(), true );
        //}
        //_collisionPointList->clear();

        _evadeProperties.reset();
        _currentMovementType = _movType;
        _currentTarget = _target;
        _currentMovement = AI::IMovement::getMovement( _currentMovementType, _currentMaxLinearSpeed, _maxAngularSpeed, _maxLinearAccel, _maxAngularAccel );

		//the evade movement is the same of the current movement
		_evade = AI::IMovement::getMovement( _currentMovementType, _currentMaxLinearSpeed, _maxAngularSpeed, _maxLinearAccel, _maxAngularAccel );

        // Si hay un nuevo movimiento
        if ( _currentMovement != NULL ) {
            // Ajustamos el target
           
            _currentMovement->setEntity( _entity );
            _currentMovement->setTarget( _currentTarget );

			 _arrived = false;
			 resetAvoidCollisionVariable(msecs);

        }
    }

	void CSteeringMovement::resetAvoidCollisionVariable(unsigned int msecs)
	{
		  _stoppedPos = _entity->getPosition();
            _stoppedPosTime = 0 - msecs;
	}
    void CSteeringMovement::steeringAvoid( unsigned int msecs )
    {
        Vector3 collisionPoint;
        Vector3 collisionNormal;
//		check collision
        Logic::CEntity*e = this->checkCollisionAvoidance( collisionPoint, collisionNormal );
        if ( e != nullptr && _evade != NULL )
        {
            this->calculateAvoidance( collisionPoint, collisionNormal, msecs );
            Vector3 eVel = this->getEntityMovement( e );
            //if the entity is moving
            if ( eVel != Vector3::ZERO )
            {
				this->computeParallelMovement(eVel);
 
            } else //else i collide with stopped entity and check if the collision normal is next to be inverse to entity orientation  
            {
                Vector3 orientation = Math::getDirection( _entity->getOrientation() );
                orientation.normalise();
                Radian angle( orientation.dotProduct( collisionNormal ) );
                if ( angle.valueDegrees() < NORMAL_INVERSE_ANGLE )
                {
					this->computePerpendicularMovement(collisionPoint,collisionNormal,msecs);
                }
            }

            _currentProperties = _evadeProperties;
        } else
        {
            this->calculateLateralAvoidance(msecs);
        }
    }
	void CSteeringMovement::computeParallelMovement(Vector3 &eVel)
	{
		               //compare the directions
                Vector2 eVel2( eVel.x, eVel.z );
                eVel2.normalise();
                Vector2 mVel2( _currentProperties.linearSpeed.x, _currentProperties.linearSpeed.z );
                mVel2.normalise();
                Ogre::Real dot = mVel2.dotProduct( eVel2 );

                //if directions converge
                if ( dot > 0 )
                {
                    //if directions are parallel
                    if ( dot > COLLISION_DIRECTION_PARALLEL_FACTOR )
                    {
                        //go slowly
                        _evadeProperties.linearSpeed /= ( ( Ogre::Real )( ( rand() % COLLISION_PARALLEL_ATTENUATION ) + 1 ) );
                        //std::cout << "go slowly for parallel entity" << std::endl;
                    } else
                    {
                        //stop movementfor this tick
                        //std::cout << "wait for other cross entity" << std::endl;
                        _currentProperties.reset();
                        return;
                    }
                }
	}

	void CSteeringMovement::computePerpendicularMovement(const Vector3 &collisionPoint,const Vector3 &collisionNormal,unsigned int msecs)
	{
			//compute the point that i have to seek to avoid the obstacle
            Vector3 finalCollisionPoint = collisionPoint + ( collisionNormal * COLLISION_AVOID_FORCE );
			//check if this point is on the left or the right of the orientation vector
            float det = ( collisionPoint.x - _entity->getPosition().x ) * ( finalCollisionPoint.z - _entity->getPosition().z ) - ( collisionPoint.z - _entity->getPosition().z ) * ( finalCollisionPoint.x - _entity->getPosition().x );
			if(det!=0)
			{
				float sign = abs( det ) / det;
	//                    std::cout << "sign" << sign << std::endl;
				//get the perpendicular vector of my orientation
				Matrix4 m1 = _entity->getTransform();
				Math::yaw( Math::PI / 2, m1 );
				Vector3 orientationPerp = Math::getDirection( m1 );
				orientationPerp.normalise();
				//put the seek point far away on perpendicular direction
				float distance=(_target-_entity->getPosition()).length();
				finalCollisionPoint += ( -sign ) * orientationPerp * COLLISION_AVOID_DISTANCE;
				_evade->setEntity( _entity );
				_evade->setTarget( finalCollisionPoint );

				_evade->move( msecs, _evadeProperties );
			}
            ////DEBUG to draw line and point
            //_collisionPointList->push_back( finalCollisionPoint );
	}

    void CSteeringMovement::calculateLateralAvoidance( unsigned int msecs )
    {
		//if the entity speed to go to the target is ZERO 
		if(_currentProperties.linearSpeed==Vector3::ZERO)
		{
			 //go directly to target
            _evadeProperties.reset();
			return;
		}
        //check if the route toward target is free
        Vector3 targetDirection = _currentProperties.linearSpeed.normalisedCopy();
        Vector3 position = _entity->getPosition();
        //lift the y value to collide with the workman goblin capsule
        position.y = ENTITY_HEIGHT;
        ////DEBUG to draw line and point
        //Vector3 p0End =  _entity->getPosition() + ( targetDirection * ( RAYCAST_LENGTH ) );
        //_entity->getMap()->getScene()->getDrawManager()->drawLine(_entity->getPosition(),p0End,drawRaycast,'w',"raycast_"+_entity->getEntityID(),true);
          
        Vector3 tempCollisionPoint = Vector3::ZERO, tempCollisionNormal = Vector3::ZERO;
        Logic::CEntity *collisionEntity = nullptr;
        //launch a central ray straight on target orientation
		bool centerCollision=this->launchRaycastCollision(targetDirection,position,RAYCAST_LENGTH,tempCollisionPoint,tempCollisionNormal,&collisionEntity);

        //check if it's not free
        if ( collisionEntity )
        {
            //continue to avoid obstacle
            if ( _evadeProperties.linearSpeed == Vector3::ZERO )
            {
                this->calculateAvoidance( tempCollisionPoint, tempCollisionNormal, msecs );
            }
            _currentProperties = _evadeProperties;
//            std::cout << "path to target collision avoid=" << _currentProperties.linearSpeed << std::endl;

        }
        else {
            //go directly to target
            _evadeProperties.reset();
        }

  
    }

    Vector3 CSteeringMovement::getEntityMovement( Logic::CEntity*e ) {
        CSteeringMovement *component = static_cast<CSteeringMovement*>( e->getComponent( "CSteeringMovement" ) );
        if ( component == nullptr )
            return Vector3::ZERO;
        else
            return component->getCurrentSpeed();
    }

    void CSteeringMovement::calculateAvoidance( const Vector3 &collisionPoint, const Vector3 &collisionNormal, unsigned int msecs )
    {
		//DEBUG
        //Vector3 cEnd = collisionPoint + ( collisionNormal * 15 );
        //_entity->getMap()->getScene()->getDrawManager()->drawLine( collisionPoint, cEnd, drawNormal, 'b', "normal_" + std::to_string( _entity->getEntityID() ), true );

		//get the distance to manteind the magnitude
		float distance=(_target-_entity->getPosition()).length();
        //turn torward to a point that is at specific distance into the normal vector calculate from entity orientation direction and collision point
        Vector3 finalCollisionPoint = collisionPoint + ( collisionNormal * COLLISION_AVOID_FORCE );
		Vector3 dir=finalCollisionPoint-_entity->getPosition();
		dir.normalise();
		finalCollisionPoint+=dir*distance;

        //DEBUG to draw line and point
        //_collisionPointList->push_back(finalCollisionPoint);
        
		_evade->setEntity( _entity );

        _evade->setTarget( finalCollisionPoint );

        _evade->move( msecs, _evadeProperties );


		

    }

    Logic::CEntity * CSteeringMovement::checkCollisionAvoidance( Vector3 &collisionPoint, Vector3 &collisionNormal )
    {
        Logic::CEntity * leftCollision = nullptr;
        Logic::CEntity * rightCollision = nullptr;
        Logic::CEntity * centerCollision = nullptr;
        //calculate collision with central raycast
        //get actual _entity orientation
        Vector3 orientation = Math::getDirection( _entity->getOrientation() );
        orientation.normalise();
        Vector3 position = _entity->getPosition();
        //lift the y value to collide with the workman goblin capsule
        position.y = ENTITY_HEIGHT;


        Logic::CEntity* collisionEntity = nullptr;
        collisionPoint = Vector3::ZERO;
        collisionNormal = Vector3::ZERO;
        Vector3 tempCollisionPoint = Vector3::ZERO;
        Vector3 tempCollisionNormal = Vector3::ZERO;
        Logic::CEntity* tempCollisionEntity = nullptr;

        //launch a central ray straight on entity orientation from the center of entity

        if ( this->launchRaycastCollision( orientation, position, ( RAYCAST_LENGTH + _radius ), tempCollisionPoint, tempCollisionNormal, &tempCollisionEntity ) )
        {
            centerCollision = tempCollisionEntity;
            if ( this->getEntityMovement( tempCollisionEntity ) == Vector3::ZERO && this->isTargetOccupedFromEntity( tempCollisionEntity ) )
            {
//                std::cout << "target occuped from e=" << tempCollisionEntity->getName() << std::endl;
                sendStopMessage();
                return nullptr;
            }
            if ( this->checkTargetInsideCollisionArea( orientation, position, tempCollisionPoint, tempCollisionNormal ) )
            {
				//if the target is between me and collision enetity and we are closest stop movement
				if(position.distance(_target)<(_radius+_tolerance*2))	
	                sendStopMessage();
				//else don't avoid and go near the target
                return nullptr;
            } else
            {
                collisionPoint = tempCollisionPoint;
                collisionNormal = tempCollisionNormal;
                collisionEntity = tempCollisionEntity;
            }
        }
        Vector3 pStartRay;
        //launch a left ray straight on entity orientation from the left side of entity
        if ( this->launchParallelRaycastCollision( orientation, position, 1, _radius + RADIUS_OFFSET, ( ( RAYCAST_LENGTH / 2 ) + _radius ), tempCollisionPoint, tempCollisionNormal, &tempCollisionEntity, pStartRay ) )
        {
            leftCollision = tempCollisionEntity;
            //if the collision point is closest respect the previous one, prioritise this one
            if ( collisionPoint == Vector3::ZERO || ( collisionPoint != Vector3::ZERO && position.distance( collisionPoint ) > position.distance( tempCollisionPoint ) ) )
            {

                if ( this->getEntityMovement( tempCollisionEntity ) == Vector3::ZERO && this->isTargetOccupedFromEntity( tempCollisionEntity ) )
                {
  //                  std::cout << "target occuped from e=" << tempCollisionEntity->getName() << std::endl;
                    sendStopMessage();
                    return nullptr;
                }
                //put the collision point into the central raycast line with an angle of 90degree respect the left raycast and the real collision point
                //calculate the normal and the collision point on
                Vector3 v = position + orientation * pStartRay.distance( tempCollisionPoint );
                Vector3 vNormal = v - tempCollisionPoint;
                vNormal.normalise();
                tempCollisionPoint = v;
                tempCollisionNormal = vNormal;
                if ( this->checkTargetInsideCollisionArea( orientation, pStartRay, tempCollisionPoint, tempCollisionNormal ) )
                {
					//if the target is between me and collision enetity and we are closest stop movement
					if(position.distance(_target)<(_radius+_tolerance*2))	
						sendStopMessage();
                    return nullptr;
                } else
                {
                    collisionPoint = tempCollisionPoint;
                    collisionNormal = tempCollisionNormal;
                    collisionEntity = tempCollisionEntity;
                }
            }
        }
        //launch a right ray straight on entity orientation from the right side of entity
        if ( this->launchParallelRaycastCollision( orientation, position, -1, _radius + RADIUS_OFFSET, ( ( RAYCAST_LENGTH / 2 ) + _radius ), tempCollisionPoint, tempCollisionNormal, &tempCollisionEntity, pStartRay ) )
        {
            rightCollision = tempCollisionEntity;

            //if the collision point is closest respect the previous one, prioritise this one
            if ( collisionPoint == Vector3::ZERO || ( collisionPoint != Vector3::ZERO && position.distance( collisionPoint ) > position.distance( tempCollisionPoint ) ) )
            {

                if ( this->getEntityMovement( tempCollisionEntity ) == Vector3::ZERO && this->isTargetOccupedFromEntity( tempCollisionEntity ) )
                {
     //               std::cout << "target occuped from e=" << tempCollisionEntity->getName() << std::endl;
                    sendStopMessage();
                    return nullptr;
                }
                //put the collision point into the central raycast line with an angle of 90degree respect the left raycast and the real collision point
                //calculate the normal and the collision point on
                Vector3 v = position + orientation * pStartRay.distance( tempCollisionPoint );
                Vector3 vNormal = v - tempCollisionPoint;
                vNormal.normalise();
                tempCollisionPoint = v;
                tempCollisionNormal = vNormal;
                if ( this->checkTargetInsideCollisionArea( orientation, pStartRay, tempCollisionPoint, tempCollisionNormal ) )
                {
					//if the target is between me and collision enetity and we are closest stop movement
					if(position.distance(_target)<(_radius+_tolerance*2))	
						sendStopMessage();
                    return nullptr;
                } else
                {
                    collisionPoint = tempCollisionPoint;
                    collisionNormal = tempCollisionNormal;
                    collisionEntity = tempCollisionEntity;
                }
            }
        }

        //std::cout << _entity->getName() << " c=" << centerCollision << " l=" << leftCollision << " r=" << rightCollision << " " << std::endl;
		bool isRightEntityStopped=(rightCollision)? this->getEntityMovement(rightCollision)==Vector3::ZERO:false;
		bool isLeftEntityStopped=(leftCollision)?this->getEntityMovement(leftCollision)==Vector3::ZERO:false;
		bool isCenterEntityStopped=(centerCollision)?this->getEntityMovement(centerCollision)==Vector3::ZERO:false;
		//if i collide with left and right raycast with 2 static entity i'm in a closed cone
		if ( ( leftCollision && rightCollision && !centerCollision && isRightEntityStopped && isLeftEntityStopped) )
        {
            this->tryToEscapeFromCloseCone();
            return nullptr;
        }
        //restabelecer the real y value
        collisionPoint.y = _entity->getPosition().y;
        //used to avoid strange raycast result that was occurred sometimes
        if ( collisionPoint.distance( position ) > RAYCAST_LENGTH )
            collisionEntity = nullptr;
        return collisionEntity;

    }

    bool CSteeringMovement::launchRaycastCollision( const Vector3 &orientation, const Vector3 &position, float lenghtRaycast, Vector3 &collisionPoint, Vector3 &collisionNormal, Logic::CEntity **collisionEntity )
    {

        Ray r( position, orientation );
        //end position of central raycast
        Vector3 p0End = position + ( orientation * lenghtRaycast );
        //DEBUG to draw line and point
        //_entity->getMap()->getScene()->getDrawManager()->drawLine( position, p0End, drawRaycast, 'w', "raycast_" + _entity->getEntityID(), true );

        //check collision with central ray
        *collisionEntity = Physics::CServer::getSingletonPtr()->getPhysicPerceptionManager()->raycastAdvanced( r, RAYCAST_LENGTH, collisionPoint, collisionNormal, &std::vector<Physics::CollisionGroup> ( PHYSIC_COLLISION_ENTITY_FILTER, PHYSIC_COLLISION_ENTITY_FILTER + sizeof( PHYSIC_COLLISION_ENTITY_FILTER ) / sizeof( PHYSIC_COLLISION_ENTITY_FILTER[0] ) ), _entity->getEntityID() );
        if ( *collisionEntity )
        {
            return true;
        }
        return false;

    }

    bool CSteeringMovement::launchParallelRaycastCollision( const Vector3 &orientation, const Vector3 &position, int direction, float distance, float lenghtRaycast, Vector3 &collisionPoint, Vector3 &collisionNormal, Logic::CEntity **collisionEntity, Vector3 &pStartRay )
    {
        Ogre::Quaternion q;
        q.FromAngleAxis( Ogre::Degree( 90 ), Vector3::UNIT_Y );
        Vector3 orientationPerp = q * orientation;
		

        orientationPerp.normalise();
        //start and end position of the left (the direction value is -1 or right (the direction is 1) raycast
        pStartRay = position + orientationPerp * ( direction * distance );
        //the lateral raycast is shorter then central
        Vector3 p1End = pStartRay + ( orientation * lenghtRaycast );

        Ray r( pStartRay, orientation );


        //DEBUG to draw line and point
        //if ( direction == 1 )
        //    _entity->getMap()->getScene()->getDrawManager()->drawLine( pStartRay, p1End, drawLeftRaycast, 'w', "raycastLeft_" + _entity->getEntityID(), true );
        //else
        //    _entity->getMap()->getScene()->getDrawManager()->drawLine( pStartRay, p1End, drawRightRaycast, 'w', "raycastRight_" + _entity->getEntityID(), true );

        //check collision with the left raycast

        *collisionEntity = Physics::CServer::getSingletonPtr()->getPhysicPerceptionManager()->raycastAdvanced( r, lenghtRaycast, collisionPoint, collisionNormal, &std::vector<Physics::CollisionGroup> ( PHYSIC_COLLISION_ENTITY_FILTER, PHYSIC_COLLISION_ENTITY_FILTER + sizeof( PHYSIC_COLLISION_ENTITY_FILTER ) / sizeof( PHYSIC_COLLISION_ENTITY_FILTER[0] ) ), _entity->getEntityID() );
        //check collision with raycast
        if ( *collisionEntity )
        {
            return true;

        }
        return false;

    }

    bool CSteeringMovement::checkTargetInsideCollisionArea( const Vector3 &orientation, const Vector3 &position, const Vector3 &tempCollisionPoint, const Vector3 &tempCollisionNormal )
    {
        Vector3 p1 = position + orientation * _radius;
        Vector2 p12d( p1.x, p1.z );
        float dColl = p12d.distance( Vector2( tempCollisionPoint.x, tempCollisionPoint.z ) );
        float dTarg = p12d.distance( Vector2( _target.x, _target.z ) );
        if ( dTarg <= dColl )
            return true;
        return false;
    }

    void CSteeringMovement::tryToEscapeFromCloseCone()
    {
		float len =_currentProperties.linearSpeed.length();
        Ogre::Quaternion q;
		q.FromAngleAxis( Radian(AVOID_CLOSED_CONE_ROTATION), Vector3::UNIT_Y );
        Vector3 targetPerp = q * _currentProperties.linearSpeed;
        targetPerp.normalise();
		_currentProperties.linearSpeed=targetPerp*len;
        float actualYaw = _entity->getYaw();
        _entity->setYawN( actualYaw + AVOID_CLOSED_CONE_ROTATION );
    }

    bool CSteeringMovement::checkPointInsidePolygon2d( int nvert, Vector2 *vert, Vector2 p )
    {
        int i, j;
        bool c = false;
        for ( i = 0, j = nvert - 1; i < nvert; j = i++ ) {
            if ( ( ( vert[i].y > p.y ) != ( vert[j].y > p.y ) ) &&
                    ( p.x < ( vert[j].x - vert[i].x ) * ( p.y - vert[i].y ) / ( vert[j].y - vert[i].y ) + vert[i].x ) )
                c = !c;
        }
        return c;
    }
    bool CSteeringMovement::getLineIntersection( float p0_x, float p0_y, float p1_x, float p1_y, float p2_x, float p2_y, float p3_x, float p3_y, float *i_x, float *i_y )
    {
        float s02_x, s02_y, s10_x, s10_y, s32_x, s32_y, s_numer, t_numer, denom, t;
        s10_x = p1_x - p0_x;
        s10_y = p1_y - p0_y;
        s02_x = p0_x - p2_x;
        s02_y = p0_y - p2_y;

        s_numer = s10_x * s02_y - s10_y * s02_x;
        if ( s_numer < 0 )
            return false; // No collision

        s32_x = p3_x - p2_x;
        s32_y = p3_y - p2_y;
        t_numer = s32_x * s02_y - s32_y * s02_x;
        if ( t_numer < 0 )
            return false; // No collision

        denom = s10_x * s32_y - s32_x * s10_y;
        if ( s_numer > denom || t_numer > denom )
            return false; // No collision

        // Collision detected
        t = t_numer / denom;
        if ( i_x != NULL )
            *i_x = p0_x + ( t * s10_x );
        if ( i_y != NULL )
            *i_y = p0_y + ( t * s10_y );

        return true;
    }

    bool CSteeringMovement::isTargetOccupedFromEntity( Logic::CEntity*e )
    {
        Ray r( _target, Vector3::UNIT_Y );
        Logic::CEntity*collision = Physics::CServer::getSingletonPtr()->getPhysicPerceptionManager()->raycastAdvanced( r, RAYCAST_LENGTH, Vector3(), Vector3(), &std::vector<Physics::CollisionGroup> ( PHYSIC_COLLISION_ENTITY_FILTER, PHYSIC_COLLISION_ENTITY_FILTER + sizeof( PHYSIC_COLLISION_ENTITY_FILTER ) / sizeof( PHYSIC_COLLISION_ENTITY_FILTER[0] ) ) );

        if ( collision == e )
            return true;
        return false;

    }

	void CSteeringMovement::setMaxLinearSpeed (float newMaxLinearSpeed)
	{
		_currentMaxLinearSpeed = newMaxLinearSpeed;		
	}

    void CSteeringMovement::sendStopMessage(bool destinationReached)
    {
        //JLC: Nuevo sistema de mensajes
        // Enviar un mensaje para informar de que hemos llegado a destino

        _arrived = true;

		auto m = std::make_shared<AvatarFinishedMove>();
        m->finished = destinationReached;		
		m->target = _currentTarget;
        _entity->emitMessageN( m, this );        

//		std::cout<<_entity->getName()<<" avatarfinishedMove ct="<<_currentTarget<<"t="<<_target<<std::endl;
        _currentProperties.reset();

    }
}
