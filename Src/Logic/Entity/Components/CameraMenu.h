/**
@file CameraMenu.h

Contiene la declaraci�n del componente que controla la c�mara gr�fica
de una escena.

@see Logic::CCameraMenu
@see Logic::IComponent

@author David Llans�
@date Septiembre, 2010
*/
#ifndef __Logic_CameraMenu_H
#define __Logic_CameraMenu_H

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
		M�todo llamado en cada frame que actualiza el estado del componente.
		
		Se encarga de mover la c�mara.

		@param msecs Milisegundos transcurridos desde el �ltimo tick.
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
		C�mara gr�fica.
		*/
		Graphics::CCamera *_graphicsCameraMenu;

		/**
		Posici�n que se usa como objetivo
		*/
		Vector3 _targetCredits;
		Vector3 _targetMenu;
		Vector3 _targetOptions;
		Vector3 _targetCastle;
		Vector3 _targetHelp;
		Vector3 _target;

		/**
		Direcci�n de movimiento de la c�mara
		**/
		Vector3 _movementDirection;
		Vector3 _newTargetDirection;


		/**
		Flag para indicar si la c�mara se est� moviendo autom�ticamente hasta una posici�n
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
