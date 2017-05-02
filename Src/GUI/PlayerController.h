/**
@file CPlayerController.h

Contiene la declaración de la clase CPlayerController. Se encarga de
recibir eventos del teclado y del ratón y de interpretarlos para
mover al jugador.

@see GUI::CPlayerController

@author David Llansó
@date Agosto, 2010
*/
#ifndef __GUI_PlayerController_H
#define __GUI_PlayerController_H

#define MIN_SCROLL_SELECTION 0.01

#include "InputManager.h"

#include <OgreVector3.h>

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Graphics 
{
	class CEntity;
	class CScene;
 }

namespace Logic 
{
	class CEntity;
}

namespace Ogre
{
	class Vector3;
	class SceneNode;
}


// Declaración de la clase
namespace GUI
{
	class CInputManager;

	/**
	Esta clase sirve para gestionar el teclado y el ratón, y mover un avatar 
	acorde con las pulsaciones y el movimiento. En el constructor se registra en 
	el gestor de teclado y en el de ratón (InputManager) para ser avisado ante 
	los eventos. No obstante, puede activarse y desactivarse a voluntad con 
	activate() y deactivate(). El componente CAvatarController del jugador
	es responsable de ponerse como foco a traves de GUI::CServer y así recibir 
	las instrucciones dadas por esta clase.

	@ingroup GUIGroup

	@author David Llansó
	@date Agosto, 2010
	*/
	class CPlayerController : public CKeyboardListener, public CMouseListener
	{
	public:

		/**
		Constructor.
		*/
		CPlayerController ();

		/**
		Destructor.
		*/
		~CPlayerController();

		/**
		Establece el componente de la cámara para moverla con libertad

		@param camera Componente al que enviaremos acciones demovimiento del 
		ratón.
		*/
		void setCamera(Logic::CEntity *camera) 
										{_camera = camera;} 

		/**
		@author Rayco Sánchez García
		@date February, 2015

		Method called once all entities are loaded
		*/
		void awake();

		/**
		Activa la la clase, se registra en el CInputManager y así empieza a 
		escuchar eventos.
		*/
		void activate();

		/**
		Desctiva la la clase, se deregistra en el CInputManager y deja de 
		escuchar eventos.
		*/
		void deactivate();

		/*
		Resetea todas las informaciones relativa a una sigola partida (GameState)
		*/
		void reset();

		/*
		Tick
		*/
		void tick(unsigned int msecs);

		/***************************************************************
		Métodos de CKeyboardListener
		***************************************************************/
		
		/**
		Método que será invocado siempre que se pulse una tecla.

		@param key Código de la tecla pulsada.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamará a otros listeners.
		*/
		bool keyPressed(TKey key);
		
		/**
		Método que será invocado siempre que se termine la pulsación
		de una tecla.

		@param key Código de la tecla pulsada.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamará a otros listeners.
		*/
		bool keyReleased(TKey key);

		/***************************************************************
		Métodos de CKeyboardListener
		***************************************************************/
		
		/**
		Método que será invocado siempre que se mueva el ratón.

		@param mouseState Estado del ratón cuando se lanza el evento.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamará a otros listeners.
		*/
		bool mouseMoved(const CMouseState &mouseState,const bool alreadyManaged);
		
		/**
		Método que será invocado siempre que se pulse un botón.

		@param mouseState Estado del ratón cuando se lanza el evento.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamará a otros listeners.
		*/
		bool mousePressed(const CMouseState &mouseState,const bool alreadyManaged);

		/**
		Método que será invocado siempre que se termine la pulsación
		de un botón.

		@param mouseState Estado del ratón cuando se lanza el evento.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamará a otros listeners.
		*/
		bool mouseReleased(const CMouseState &mouseState,const bool alreadyManaged);

		/**
		@author Rayco Sánchez García
		@date February, 2015

		Method that returns the point where is pointing the mouse
		*/
		Ogre::Vector3 getActualMouseWorldPosition();

		/**
		@author Rayco Sánchez García
		@date February, 2015

		Method for attaching an entity to mouse movement

		@param entity Entity which is wanted to attach.
		*/
		void attachEntityToMouse(Logic::CEntity* entity);

		/**
		@author Rayco Sánchez García
		@date February, 2015

		Method for dettaching an entity from mouse movement
		*/
		void dettachEntityFromMouse();

		/**
		@author Rayco Sánchez García
		@date February, 2015

		Removes the entity from the _selectedUnits list

		@param entity The entity that is wanted to remove
		*/
		void removeEntityFromSelectedUnits(Logic::CEntity* entity);

		/**
		Método que devuelve la lista de unidades seleccionadas

		@return _selectedUnits Unidades seleccionadas.
		*/
		std::list<Logic::CEntity*>* getSelectedUnits() { return _selectedUnits; }


		/**
		Método para cambiar la unidad activa por otra nueva

		@param newActiveUnit Nueva unidad que pasará a ser la unidad activa
		*/
		void changeActiveUnit(Logic::CEntity* newActiveUnit);

		bool hasAttachedEntity() const { return mouseAttachedEntity ? true : false; }

		/**
		Método para modificar uno de los diccionarios por tipos de click.

		@param button entero 1: botón izquierdo, 2: botón derecho.
		@param type String con el tipo de entidad a bloquear/desbloquear
		@param clickable Bool que bloquea/desbloquea el click a un tipo de entidad
		*/
		void setTypeIsClickable(int button, std::string type, bool clickable);

		bool getMouseIsLocked() { return _mouseLocked; }

		void setMouseIsLocked (bool locked) { _mouseLocked = locked; }

		void unselectUnits();

		Logic::CEntity* getEntityUnderMouse(const CMouseState &mouseState, bool checkRemovableWorld = false);

		void selectUnitsOfType(std::string type);

		/**
		Método que selecciona una entidad especifica

		@param entity etidad que tiene que ser seleccionada.
		*/
		void selectEntity(Logic::CEntity* entity);
	protected:

		void checkRemovableWorld(const CMouseState &mouseState);

		/**
		Método que lanza un Raycast. Si el Raycast colisiona
		contra una entidad de tipo jugador, esa entidad pasa
		estar seleccionada

		@param mouseState Para comprobar la posición del ratón en 
		la pantalla en el momento del Raycast.
		*/
		void selectEntity(const CMouseState & mouseState);

		/**
		Método que crea una pirámide desde la cámara hasta 
		los cuatro vértices del rectángulo de selección para hacer
		selección de unidades múltiple.

		@param endMousePosX Current mouse position over the X axis
		@param endMousePosY Current mouse position over the Y axis
		*/
		void performSelectionBox(float endMousePosX,float endMousePosY);

		/**
		Draws a rectangle on field to select units.

		@param endMousePosX Current mouse position over the X axis
		@param endMousePosY Current mouse position over the Y axis
		*/
		void drawSelectionBox(float endMousePosX,float endMousePosY);

		/**
		Sends a message to selected units indicating whether they 
		are selected or unselected.

		@param isSelected Indicates the selection's condition
		*/
		void sendSelectMessage(bool isSelect);

		/**
		Indicates the destination entity

		@param mouseState Position of the mouse
		*/
		Logic::CEntity* selectDestinationEntity(const CMouseState & mouseState);	

		/**
		Indicates the position where the selected units will try to move

		@param mouseState Position of the mouse
		*/
		Ogre::Vector3 selectDestinationPosition(const CMouseState & mouseState);

		/**
		Function that checks if there is an entity attached to the mouse
		and if this entity is on a correct physic position for activating it.

		@return True if conditions for activating the entity are met.
		*/
		bool checkMouseAttachedEntityIsAbleForActivate();

		/**
		Function that initializes both dictionaries of clicks
		*/
		void initClickDictionaries();

		/**
		List of types that are able to be clicked with the mouse right button
		*/
		std::map <std::string, bool> _rightClickTypes;

		/**
		List of types that are able to be clicked with the mouse left button
		*/
		std::map <std::string, bool> _leftClickTypes;

		/**
		Flag para establecer si el ratón está bloqueado (no puede hacer clicks) o no
		*/
		bool _mouseLocked;
		
		/**
		Will keep the position of the first click when selecting units.
		*/
		Ogre::Vector3* _firstVertexSelectionBox;	

		/**
		Entidad cámara a la que enviaremos acciones de movimiento 
		del ratón cuando esté situado en alguno de los bordes.
		*/
		Logic::CEntity *_camera;
		
		/**
		Entero para guardar el ancho de la ventana
		*/
		unsigned int _windowWidth;
		
		/**
		Entero para guardar el alto de la ventana
		*/
		unsigned int _windowHeight;

		/**
		Nodo de escena ogre para gestionar el rectángulo que indica la selección 
		sobre el mapa.
		*/
		Ogre::SceneNode* _selectionRectangleNode;

		/**
		Lista de unidades seleccionadas
		*/
		std::list<Logic::CEntity*>* _selectedUnits;

		std::map<int,std::list<Logic::CEntity*>*> _selectedUnitsGroups;
		bool _lCtrlPushed;
		bool _rCtrlPushed;

		// Active unit from selectedUnits;
		std::list<Logic::CEntity*>::iterator activeUnit;

		/**
		Posición X del ratón donde se empezará a generar el rectángulo de selección múltiple
		*/
		float startMousePosX;

		/**
		Posición Y del ratón donde se empezará a generar el rectángulo de selección múltiple
		*/
		float startMousePosY;

		/**
		Flag para indicar si se está haciendo una selección múltiple.
		*/
		bool _leftButtonDrag;

		/**
		Escena gráfica donde se encontrarán las representaciones gráficas de
		las entidades. La guardamos para la destrucción de la entidad gráfica.
		*/
		Graphics::CScene* _scene;

		// Entity attached to mouse for positioning buildings and traps
		Logic::CEntity* mouseAttachedEntity;

		/**
		Numero de teclas pulsadas
		*/
		unsigned int _numKeysPressed;

		/**
		Raton moviendo la camara
		*/
		bool _mouseCamera;

		/**
		Teclas moviendo la camara
		*/
		bool _keysCamera;

		/**
		Vector director de la camara
		*/
		Ogre::Vector3 _cameraVector;

		/**
		Flag para activar o desactivar la trasparencia de mallas bajo el ratón
		*/		
		bool _activeTransparency;
		Logic::CEntity* _transparentEntity;

		/**
		Isntancia del InputManager
		*/
		CInputManager *_inputManager;


		/**
		Tiempo de reloj en el que se hizo click
		*/
		int _doubleClickMsecs;

		/**
		Flag que activado indica al playercontroller que tiene que empezar a contar tiempo para comprobar si se hace doble click
		*/
		bool _checkDoubleClick;

	}; // class CPlayerController

} // namespace GUI

#endif // __GUI_PlayerController_H
