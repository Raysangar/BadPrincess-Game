/**
@file CameraMenu.h

Contiene la declaración del componente que controla la cámara gráfica
de una escena.

@see Logic::CCameraMenu
@see Logic::IComponent

@author David Llansó
@date Septiembre, 2010
*/
#ifndef __Logic_CameraMenu_H
#define __Logic_CameraMenu_H

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
	class CCameraMenu : public IComponent
	{
		DEC_FACTORY(CCameraMenu);
	public:
		/**
		This constructor initialize the component with its component name 
		and the rest attributes with their default value.
		*/
		CCameraMenu(std::string componentName) : IComponent(componentName) {}
		
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
		Método llamado en cada frame que actualiza el estado del componente.
		
		Se encarga de mover la cámara.

		@param msecs Milisegundos transcurridos desde el último tick.
		*/
		virtual void tick(unsigned int msecs);

		Vector3 getDestinationMenu() const { return _destinationMenu; }
		Vector3 getDestinationCastle() const { return _destinationCastle; }
		Vector3 getDestinationOptions() const { return _destinationOptions; }
		Vector3 getDestinationCredits() const { return _destinationCredits; }
		
		void goToMenu();
		void goToOptions();
		void goToCastle();
		void goToCredits();
		void goToHelp();
	protected:


		/**
		Cámara gráfica.
		*/
		Graphics::CCamera *_graphicsCameraMenu;

		/**
		Posición que se usa como objetivo
		*/
		Vector3 _targetCredits;
		Vector3 _targetMenu;
		Vector3 _targetOptions;
		Vector3 _targetCastle;
		Vector3 _targetHelp;
		Vector3 _target;

		/**
		Dirección de movimiento de la cámara
		**/
		Vector3 _movementDirection;
		Vector3 _newTargetDirection;


		/**
		Flag para indicar si la cámara se está moviendo automáticamente hasta una posición
		*/
		bool _isMoving;

		/**
		Vector 3 que indica el destino del mensaje CameraMenuMoveTo
		*/
		Vector3 _destinationCredits;
		Vector3 _destinationMenu; 
		Vector3 _destinationOptions; 
		Vector3 _destinationCastle;
		Vector3 _destinationHelp;
		Vector3 _destination;

	}; // class CCameraMenu

	REG_FACTORY(CCameraMenu);

} // namespace Logic

#endif // __Logic_CameraMenu_H
