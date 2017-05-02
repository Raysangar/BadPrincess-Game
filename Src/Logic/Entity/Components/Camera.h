/**
@file Camera.h

Contiene la declaraci�n del componente que controla la c�mara gr�fica
de una escena.

@see Logic::CCamera
@see Logic::IComponent

@author David Llans�
@date Septiembre, 2010
*/
#ifndef __Logic_Camera_H
#define __Logic_Camera_H

#include "Logic/Entity/Component.h"

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
namespace Graphics 
{
	class CCamera;
	class CScene;
}

//declaraci�n de la clase
namespace Logic 
{
/**
	Componente que se encarga de mover la c�mara gr�fica de una escena, para que 
	�sta se pueda reenderizar, siguiendo al jugador.
	<p>
	La c�mara se situar� a una distancia detr�s del jugador y a una altura del 
	suelo y enfocar� a un punto imaginario por delante del jugador que estar� 
	tambi�n a cierta altura del suelo. Todas estas caracter�sticas son configurables 
	desde la declaraci�n del mapa definiendo los nombres de atributo "distance",
	"height", "targetDistance" y "targetHeight".
	<p>
	El componente ir� recalculando en cada ciclo las posiciones de la c�mara y del
	punto de mira en funci�n de la posici�n y orientaci�n del jugador.
	
    @ingroup logicGroup

	@author David Llans� Garc�a
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
		Inicializaci�n del componente, utilizando la informaci�n extra�da de
		la entidad le�da del mapa (Maps::CEntity). Se accede a los atributos 
		necesarios como la c�mara gr�fica y se leen atributos del mapa.

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa L�gico en el que se registrar� el objeto.
		@param entityInfo Informaci�n de construcci�n del objeto le�do del
			fichero de disco.
		@return Cierto si la inicializaci�n ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		@author Rayco S�nchez Garc�a
		@date February, 2015

		Method called after all components are created and added
		to the entity.
		*/
		virtual void awake();

		/**
		M�todo que activa el componente; invocado cuando se activa
		el mapa donde est� la entidad a la que pertenece el componente.
		<p>
		Se coge el jugador del mapa, la entidad que se quiere "seguir".

		@return true si todo ha ido correctamente.
		*/
		virtual bool activate();
		
		/**
		M�todo que desactiva el componente; invocado cuando se
		desactiva el mapa donde est� la entidad a la que pertenece el
		componente. Se invocar� siempre, independientemente de si estamos
		activados o no.
		<p>
		Se pone el objetivo a seguir a NULL.
		*/
		virtual void deactivate();

		/**
		Metodo virtual que elige que mensajes son aceptados. Son v�lidos CONTROL.

		@param message Mensaje a chequear
		@return true si el mensaje es aceptado
		*/
		virtual bool acceptN(const std::shared_ptr<NMessage> &message);

		/**
		M�todo virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		virtual void processN(const std::shared_ptr<NMessage> &message);

		/**
		M�todo llamado en cada frame que actualiza el estado del componente.
		
		Se encarga de mover la c�mara.

		@param msecs Milisegundos transcurridos desde el �ltimo tick.
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
		C�mara gr�fica.
		*/
		Graphics::CCamera *_graphicsCamera;

		/**
		Posici�n que se usa como objetivo
		*/
		Vector3 _target;

		/**
		Direcci�n de movimiento de la c�mara
		**/
		Vector3 _movementDirection;

		/**
		Origen de vector de movimiento
		*/
		std::string _origin;

		/**
		Flag para indicar si la c�mara se est� moviendo autom�ticamente hasta una posici�n
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
		Distancia del objetivo del punto al que mira la c�mara
		desde la posici�n Z de la c�mara. Lo utilizamos para ajustar
		la inclinaci�n de la c�mara
		*/
		float _targetDistanceZ;

		/**
		Altura m�xima de la c�mara
		*/
		float _maxHeight;

		/**
		Altura m�nima de la c�mara
		*/
		float _minHeight;		

		/**
		Sensibilidad de la c�mara
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
