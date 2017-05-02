/**
@file Camera.h

Contiene la declaración del componente que controla la cámara gráfica
de una escena.

@see Logic::CCamera
@see Logic::IComponent

@author David Llansó
@date Septiembre, 2010
*/
#ifndef __Logic_Camera_H
#define __Logic_Camera_H

#include "Logic/Entity/Component.h"

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Graphics 
{
	class CCamera;
	class CScene;
}

//declaración de la clase
namespace Logic 
{
/**
	Componente que se encarga de mover la cámara gráfica de una escena, para que 
	ésta se pueda reenderizar, siguiendo al jugador.
	<p>
	La cámara se situará a una distancia detrás del jugador y a una altura del 
	suelo y enfocará a un punto imaginario por delante del jugador que estará 
	también a cierta altura del suelo. Todas estas características son configurables 
	desde la declaración del mapa definiendo los nombres de atributo "distance",
	"height", "targetDistance" y "targetHeight".
	<p>
	El componente irá recalculando en cada ciclo las posiciones de la cámara y del
	punto de mira en función de la posición y orientación del jugador.
	
    @ingroup logicGroup

	@author David Llansó García
	@date Septiembre, 2010
*/
	class CCamera : public IComponent
	{
		DEC_FACTORY(CCamera);
	public:
		/**
		This constructor initialize the component with its component name 
		and the rest attributes with their default value.
		*/
		CCamera(std::string componentName) : IComponent(componentName), _graphicsCamera(0), _minHeight(60), _maxHeight(90),
			_targetDistanceZ(30), _target(Vector3(0,0,0)), _sensitivity(0), _movementDirection(Vector3(0,0,0)),_maxZoom(0.f)
			, _entityFollowed(nullptr),_cameraLimits(Vector3::ZERO) {}
		
		/**
		Inicialización del componente, utilizando la información extraída de
		la entidad leída del mapa (Maps::CEntity). Se accede a los atributos 
		necesarios como la cámara gráfica y se leen atributos del mapa.

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
		Método que activa el componente; invocado cuando se activa
		el mapa donde está la entidad a la que pertenece el componente.
		<p>
		Se coge el jugador del mapa, la entidad que se quiere "seguir".

		@return true si todo ha ido correctamente.
		*/
		virtual bool activate();
		
		/**
		Método que desactiva el componente; invocado cuando se
		desactiva el mapa donde está la entidad a la que pertenece el
		componente. Se invocará siempre, independientemente de si estamos
		activados o no.
		<p>
		Se pone el objetivo a seguir a NULL.
		*/
		virtual void deactivate();

		/**
		Metodo virtual que elige que mensajes son aceptados. Son válidos CONTROL.

		@param message Mensaje a chequear
		@return true si el mensaje es aceptado
		*/
		virtual bool acceptN(const std::shared_ptr<NMessage> &message);

		/**
		Método virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		virtual void processN(const std::shared_ptr<NMessage> &message);

		/**
		Método llamado en cada frame que actualiza el estado del componente.
		
		Se encarga de mover la cámara.

		@param msecs Milisegundos transcurridos desde el último tick.
		*/
		virtual void tick(unsigned int msecs);

		Vector3 getCameraTargetPosition();

		float getAspectRatio() const;

		float getFrustumHeight() const;

		float getFrustumWidth() const;

		/**
		Metodo encargado de bloquear el control de la camara para el jugador
		*/
		void setCameraBlock(bool cb);

		/**
		Metodo encargado de asignar una entidad a perseguir
		*/
		void setEntityFollowed(Logic::CEntity* ef);

		bool isEntityOnCamera(const Vector3 &entityPosition) const;


	protected:
		
		void adjustNewPositionToCameraLimits(Vector3& newPosition);

		/**
		Cámara gráfica.
		*/
		Graphics::CCamera *_graphicsCamera;

		/**
		Posición que se usa como objetivo
		*/
		Vector3 _target;

		/**
		Dirección de movimiento de la cámara
		**/
		Vector3 _movementDirection;

		/**
		Origen de vector de movimiento
		*/
		std::string _origin;

		/**
		Flag para indicar si la cámara se está moviendo automáticamente hasta una posición
		*/
		bool _isMoving;

		/**
		Vector 3 que indica el destino del mensaje CameraMoveTo
		*/
		Vector3 _destination;

		/**
		Flag para indicar si el jugador es capaz de controlar la camara
		*/
		bool _cameraBlocked;

		/**
		Distancia del objetivo del punto al que mira la cámara
		desde la posición Z de la cámara. Lo utilizamos para ajustar
		la inclinación de la cámara
		*/
		float _targetDistanceZ;

		/**
		Altura máxima de la cámara
		*/
		float _maxHeight;

		/**
		Altura mínima de la cámara
		*/
		float _minHeight;		

		/**
		Sensibilidad de la cámara
		*/
		float _sensitivity;
		/*
		Difference between _maxheight and _minHeight
		*/
		float _maxZoom;

		Vector3 _cameraLimits;

		/**
		Percentages of the map, used to limit the camera movement
		*/
		float _topLimit;
		float _bottomLimit;
		float _rightLimit;
		float _leftLimit;



		/**
		Entidad que la camara sigue
		*/
		Logic::CEntity* _entityFollowed;

		/**
		Flag si la camara ha llegado a la entidad
		*/
		bool _cameraOnUnit;

	}; // class CCamera

	REG_FACTORY(CCamera);

} // namespace Logic

#endif // __Logic_Camera_H
