/**
@file MinimapController.h

Contiene la declaraci�n de la clase CMapController. Se encarga de controlar 
el comportamiento del minimapa.

@see GUI::CMinimapController

@author Iv�n Le�n
@date Abril, 2015
*/
#ifndef __GUI_MinimapController_H
#define __GUI_MinimapController_H


#include <OgreTexture.h>
#include "InputManager.h"

namespace Logic{
	class CWorldComponent;
	class CCamera;
}

namespace CEGUI
{
	class Listbox;
	class Window;
	class EventArgs;
	class ProgressBar;
	class OgreRenderer;
}

namespace Ogre
{
	class Vector3;	
	class Vector2;
}


// Declaraci�n de la clase
namespace GUI
{
	enum EventType
	{
		battle = 0,
		princess = 1,
		chest = 2,
		trap = 3
	};

	struct MinimapEvent
	{
		EventType type;		
		float posX;
		float posY;
		int acumTime;
	};

	/**
	Esta clase sirve para gestionar el teclado y el rat�n, y mover un avatar 
	acorde con las pulsaciones y el movimiento. En el constructor se registra en 
	el gestor de teclado y en el de rat�n (InputManager) para ser avisado ante 
	los eventos. No obstante, puede activarse y desactivarse a voluntad con 
	activate() y deactivate(). El componente CAvatarController del jugador
	es responsable de ponerse como foco a traves de GUI::CServer y as� recibir 
	las instrucciones dadas por esta clase.

	@ingroup GUIGroup

	@author David Llans�
	@date Agosto, 2010
	*/
	class CMinimapController : public CKeyboardListener, public CMouseListener
	{
	public:

		/**
		Constructor.
		*/
		CMinimapController::CMinimapController():_minimapFrame(0),_minimapWindow(0),_textureContainer(0),_mRenderer(0),_worldComponent(0),_frustumWidth(0),_frustumHeight(0),
		_clickAction(false), _frustumTopLeftCorner(0), _frustumBottomRightCorner(0){}
		// CMinimapController

		/**
		Destructor.
		*/
		~CMinimapController();

		void awake();

		/**
		Activa la la clase, se registra en el CInputManager y as� empieza a 
		escuchar eventos.
		*/
		void activate();

		/**
		Desctiva la la clase, se deregistra en el CInputManager y deja de 
		escuchar eventos.
		*/
		void deactivate();

		/** 
		Comprueba si ha habido eventos en los dispositivos de entrada.
		Debe llamarse en cada vuelta de la aplicaci�n.
		*/
		void tick(unsigned int msecs);

		/*
		limpia todos lo que est� relacionado con la partida actual
		*/
		void reset();

		/***************************************************************
		M�todos de CKeyboardListener
		***************************************************************/
		
		/**
		M�todo que ser� invocado siempre que se pulse una tecla.

		@param key C�digo de la tecla pulsada.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamar� a otros listeners.
		*/
		bool keyPressed(TKey key);
		
		/**
		M�todo que ser� invocado siempre que se termine la pulsaci�n
		de una tecla.

		@param key C�digo de la tecla pulsada.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamar� a otros listeners.
		*/
		bool keyReleased(TKey key);

		/**
		M�todo que ser� invocado siempre que se pulse un bot�n.

		@param mouseState Estado del rat�n cuando se lanza el evento.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamar� a otros listeners.
		*/
		bool mousePressed(const CMouseState &mouseState,const bool alreadyManaged);

		/**
		M�todo que ser� invocado siempre que se termine la pulsaci�n
		de un bot�n.

		@param mouseState Estado del rat�n cuando se lanza el evento.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamar� a otros listeners.
		*/
		bool mouseReleased(const CMouseState &mouseState,const bool alreadyManaged);

		void addMinimapEvent(std::string id, EventType type, Ogre::Vector3 position);

		void removeMinimapEvent(std::string id);

	protected:

		/**
		M�todo que mueve la c�mara a la posici�n del mapa correpondiente a la posici�n del minimapa donde se ha hecho click.
		*/
		bool moveToClickPosition(const CEGUI::EventArgs &e);

		/**
		M�todo para transformar una coordenada 3d en una posici�n dentro del minimapa
		@param coordX Posici�n X a transformar
		@param coordY Posici�n Z a transformar
		@param Pos Vector a modificar
		*/
		void tranformPositionToMinimap(float coordX, float coordZ, Ogre::Vector2 &pos);

		
		/**
		M�todo para transformar una coordenada 3d en una posici�n de la ventana CEGUI que contiene al minimapa
		@param coordX Posici�n X a transformar
		@param coordY Posici�n Z a transformar
		@param Pos Vector a modificar
		*/
		void tranformPositionToMinimapContainer(float coordX, float coordZ, Ogre::Vector2 &pos);

		/**
		M�todo para transformar una coordenada 3d en una posici�n dentro del minimapa
		@param PixelX del minimapa a tranformar
		@param PixelY del minimapa a tranformar
		@param Pos Vector a modificar
		*/
		void tranformMinimapToPosition(float pixelX, float pixelY, Ogre::Vector3 &pos);

		/**
		M�todo para dibujar en una posici�n del mapa

		@param data Buffer de p�xeles del minimapa.
		@param posX Coordenada X del primer pixel a dibujar.
		@param posY Coordenada X del primer pixel a dibujar.
		@param pitch N�mero de columnas/filas de p�xeles del minimapa.
		@param color Color del p�xel.
		@param size Altura y anchura del cuadrado a pintar		
		
		*/
		void drawOnMinimap(Ogre::uint32 *data, int posX, int posY, size_t pitch, Ogre::uint32 color, int size);

		/**
		M�todo para dibujar el frustum de la c�mara en el minimapa

		@param data Buffer de p�xeles del minimapa.
		@param pitch N�mero de columnas/filas de p�xeles del minimapa.		
		*/
		void drawCameraFrustum(Ogre::uint32 *data, size_t pitch);
		
		/*
		Metodo que recoge los clicks sin funcionalidad en esta parte de la interfaz

		@param e Es el evento de CEGUI a que somos registrado
		*/
		bool handleMouseClick(const CEGUI::EventArgs &e);

		/*
		Metodo que recoge los clicks sobre los eventos del minimapa

		@param e Es el evento de CEGUI a que somos registrado
		*/
		bool handleClickOnEvent(const CEGUI::EventArgs &e);

		//Variables para elementos CEGUI
		CEGUI::Window* _minimapFrame;
		CEGUI::Window* _minimapWindow;
		CEGUI::Window* _textureContainer;
		CEGUI::OgreRenderer* _mRenderer;
		Ogre::TexturePtr _minimapTexture;

		/**
		Variables para la obtencion del tama�o real del mapa
		*/		
		Logic::CWorldComponent *_worldComponent;
		const Ogre::Vector3 *_worldBounds;

		/**
		Entero para guardar la variable de transformacion entre metros
		del mundo y pixeles del minimapa
		*/
		float _metersToPixels;
		float _pixelsToMetersX;
		float _pixelsToMetersY;

		/**
		Variables para dibujar el frustrum de la c�mara
		*/
		Logic::CCamera *_cameraComponent;
		Ogre::Vector2 *_frustumTopLeftCorner;
		Ogre::Vector2 *_frustumBottomRightCorner;

		bool _clickAction;

		float _frustumWidth;
		float _frustumHeight;

		unsigned int _windowWidth;
		unsigned int _windowHeight;
		float _minimapContainerHeight;
		float _minimapContainerWidth;

		/**
		Variables para los eventos en el minimapa
		*/
		std::string _battleIconName;
		std::string _princessIconName;
		std::string _chestIconName;
		std::string _trapIconName;

		std::map<std::string,MinimapEvent*> _minimapEvents;
		std::map<std::string,CEGUI::Window*> _minimapEventsImages;


	}; // class CMinimapController

} // namespace GUI

#endif // __GUI_MapController_H
