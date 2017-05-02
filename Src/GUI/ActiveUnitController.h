/**
@file ActiveUnitController.h

Contiene la declaraci�n de la clase CActiveUnitController. Se encarga de gestionar los
eventos en la parte de la interfaz dedicada a la unidad activa entre las seleccionadas.

@author Ivan Leon
@date Mayo, 2015
*/
#ifndef __GUI_ActiveUnitController_H
#define __GUI_ActiveUnitController_H



#include "InputManager.h"


namespace CEGUI
{	
	class Window;
	class EventArgs;
	class ProgressBar;
	class PushButton;
}

namespace Logic
{
	class CEntity;
	class CEnemyDetectionManager;
	class CGUIManager;
	class CLife;
}


// Declaraci�n de la clase
namespace GUI
{
	class CMouseState;
	/**
	
	*/
	class CActiveUnitController : public CKeyboardListener, public CMouseListener
	{
	public:

		/**
		Constructor.
		*/
		CActiveUnitController();
		/**
		Destructor.
		*/
		~CActiveUnitController();

		void awake();

		/*
		Resetea todas las informaciones relativa a una sigola partida (GameState)
		*/
		void reset();
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
		void tick();

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

		/***************************************************************
		M�todos de CKeyboardListener
		***************************************************************/
		
		/**
		M�todo que ser� invocado siempre que se mueva el rat�n.

		@param mouseState Estado del rat�n cuando se lanza el evento.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamar� a otros listeners.
		*/
		bool mouseMoved(const CMouseState &mouseState,const bool alreadyManaged);
		
		/**
		M�todo que ser� invocado siempre que se pulse un bot�n.

		@param mouseState Estado del rat�n cuando se lanza el evento.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamar� a otros listeners.
		*/
		bool mousePressed(const CMouseState &mouseState,const bool alreadyManaged);

		/**
		//M�todo que ser� invocado siempre que se termine la pulsaci�n
		//de un bot�n.

		//@param mouseState Estado del rat�n cuando se lanza el evento.
		//@return true si el evento ha sido procesado. En este caso 
		//el gestor no llamar� a otros listeners.
		*/
		bool mouseReleased(const CMouseState &mouseState,const bool alreadyManaged);

		/**
		M�todo para establecer una referencia a la unidad activa entre las unidades
		seleccionadas

		@param activeUnit La unidad activa
		*/
		Logic::CEntity* getActiveUnit() { return _activeUnit; }

		/**
		M�todo para establecer una referencia a la unidad activa entre las unidades
		seleccionadas

		@param activeUnit La unidad activa
		*/
		void setActiveUnit(Logic::CEntity* activeUnit) { _activeUnit = activeUnit; }

		/**
		M�todo para activar todos los elementos de la interfaz que cambien con la unidad
		*/
		void activateElements();		

		/**
		M�todo para desactivar todos los elementos de la interfaz que cambien con la unidad
		*/
		void deactivateElements();

		/**
		M�todo que establece si una habilidad de una clase aliada se puede usar o no

		@param type Nombre de la clase
		@param abiltiy Tecla de la habilidad
		@usable bool Establece si la habilidad se puede usar o no.
		*/
		void setAbilityUsability(std::string type, std::string ability, bool usable);
		/**
		M�todo que gestion la interfaz (activacion/desactivacion,icono boton con hover y press) de l'abilidad Q
		En caso que la entidad no tenga esa abilidad desactiva el interfaz
		*/	
		void setNameAbilityQ(std::string name);

		void setTooltipAbilityQ(std::string name, std::string luaField);

		/**
		M�todo que gestion la interfaz (activacion/desactivacion,icono boton con hover y press) de l'abilidad W
		En caso que la entidad no tenga esa abilidad desactiva el interfaz
		*/
		void setNameAbilityW(std::string name);

		void setTooltipAbilityW(std::string name, std::string luaField);

		/**
		M�todo que gestion la interfaz (activacion/desactivacion,icono boton con hover y press) de l'abilidad E
		En caso que la entidad no tenga esa abilidad desactiva el interfaz
		*/
		void setNameAbilityE(std::string name);

		void setTooltipAbilityE(std::string name, std::string luaField);

		/**
		M�todo que gestion la interfaz (activacion/desactivacion,icono boton con hover y press) de l'abilidad R
		En caso que la entidad no tenga esa abilidad desactiva el interfaz
		*/
		void setNameAbilityR(std::string name);

		void setTooltipAbilityR(std::string name, std::string luaField);

		void setNameAbility(CEGUI::PushButton *button, std::string name);

		void setMouseCursor(std::string cursor);

		void restoreMouseCursor();

		bool checkAbilityUsability(std::string name);

	protected:

		/**
		Diccionario que por cada tipo de unidad que gaste recursos, le asigna aquellas habilidades que cuestan recursos
		*/
		std::map<std::string,std::map<std::string,char*>> _spendingAbilitiesPerType;

		//Variables referentes a la unidad
		Logic::CEntity* _activeUnit;
		Logic::CEnemyDetectionManager *_enemyDetectionComponent;
		Logic::CGUIManager *_GUIManagerComponent;
		Logic::CLife *_lifeComponent;

		//Variable para gestionar el glow
		Logic::CEntity* _lastEntityUnderMouse;

		//Elementos de la interfaz
		CEGUI::Window* _unitsBox;
		CEGUI::Window* _activeUnitFrame;
		CEGUI::Window* _activeUnitPortrait;
		CEGUI::Window* _activeUnitHealth;
		CEGUI::ProgressBar* _activeUnitHealthBar;
		CEGUI::Window* _activeUnitType;
		
		CEGUI::Window* _abilityQlabel;
		CEGUI::Window* _abilityQtooltip;
		CEGUI::PushButton *_abilityQ;
		

		CEGUI::Window* _abilityWlabel;
		CEGUI::Window* _abilityWtooltip;
		CEGUI::PushButton *_abilityW;
		

		CEGUI::Window* _abilityElabel;
		CEGUI::Window* _abilityEtooltip;
		CEGUI::PushButton *_abilityE;
		

		CEGUI::Window* _abilityRlabel;
		CEGUI::Window* _abilityRtooltip;
		CEGUI::PushButton *_abilityR;
		

		CEGUI::Window *_aggressiveModeLabel;
		CEGUI::Window *_currentModeLabel;
		CEGUI::Window* _aggressiveModeTooltip;
		CEGUI::Window* _passiveModeTooltip;
		CEGUI::PushButton *_aggressiveMode;
		CEGUI::PushButton *_passiveMode;

		CEGUI::Window *_costLabel;
		
		std::list<CEGUI::Window*> uninteractiveElements;
		std::list<CEGUI::Window*> interactiveElements;

		/*Variable que indica que hemos gestionado un evento de CEGUI 
			se usa porque gestionamos el evento CEGUI::PushButton::EventMouseButtonDown asi que podemos decir a los otro componente que escuchan el mouseRelese de CEGUI que hemos
			hecho algo en la interfaz
		*/
		bool clickAction;

		/**
		Diccionario que por cada tipo de unidad aliada, se le asigna un diccionario de habilidades, en el que por cada habilidad se establece si se puede usar o no
		*/
		std::map<std::string,std::map<std::string,bool>> _abilitiesUsability;

		bool _abilityUsabilityChanged;
		
		/**
		Marca si el cursor se ha cambiado para que, en caso necesario, vuelva al cursor original
		*/
		bool _changeCursor;

		/**
		Guarda el nombre del cursor actual
		*/
		std::string _currentCursor;
				
		/**
		Diccionario donde por cada tipo de entidad que necesite cambiar el cursor, se le asigna el nombre del cursor al cual debe de cambiar
		*/
		std::map<std::string,std::string> _cursorsPerType;

		/**
		Guarda el estado del rat�n
		*/
		CMouseState _mouseState;


	private :	

		void setMovingMouseCursor(const CMouseState &mouseState);

		bool existsInAbilitiesUsability(std::string type, std::string ability);

		void initAbilitiesUsability();
		/*Function that check if the Q ability of the _activeUnit is enabled
			It take into account the entity type abilityUsability and the single unit state
		*/
		void checkAbilityQEnabled();
		/*Function that check if the W ability of the _activeUnit is enabled
			It take into account the entity type abilityUsability and the single unit state
		*/
		void checkAbilityWEnabled();
		/*Function that check if the E ability of the _activeUnit is enabled
			It take into account the entity type abilityUsability and the single unit state
		*/
		void checkAbilityEEnabled();
		/*Function that check if the R ability of the _activeUnit is enabled
			It take into account the entity type abilityUsability and the single unit state
		*/
		void checkAbilityREnabled();

		/*
		Metodo que se invoca para usar la habilidad Q

		@param e Es el evento de CEGUI a que somos registrado
		*/
		bool useAbilityQ(const CEGUI::EventArgs &e);

		/*
		Metodo que se invoca para mostrar el tooltip de la habilidad Q

		@param e Es el evento de CEGUI a que somos registrado
		*/
		bool showTooltipQ(const CEGUI::EventArgs &e);

		/*
		Metodo que se invoca para esconder el tooltip de la habilidad Q

		@param e Es el evento de CEGUI a que somos registrado
		*/
		bool hideTooltipQ(const CEGUI::EventArgs &e);

		/*
		Metodo que se invoca para usar la habilidad W

		@param e Es el evento de CEGUI a que somos registrado
		*/
		bool useAbilityW(const CEGUI::EventArgs &e);

		/*
		Metodo que se invoca para esconder el tooltip de la habilidad W

		@param e Es el evento de CEGUI a que somos registrado
		*/
		bool hideTooltipW(const CEGUI::EventArgs &e);

		/*
		Metodo que se invoca para mostrar el tooltip de la habilidad W

		@param e Es el evento de CEGUI a que somos registrado
		*/
		bool showTooltipW(const CEGUI::EventArgs &e);

		/*
		Metodo que se invoca para usar la habilidad E

		@param e Es el evento de CEGUI a que somos registrado
		*/
		bool useAbilityE(const CEGUI::EventArgs &e);

		/*
		Metodo que se invoca para mostrar el tooltip de la habilidad E

		@param e Es el evento de CEGUI a que somos registrado
		*/
		bool showTooltipE(const CEGUI::EventArgs &e);
		
		/*
		Metodo que se invoca para esconder el tooltip de la habilidad E

		@param e Es el evento de CEGUI a que somos registrado
		*/
		bool hideTooltipE(const CEGUI::EventArgs &e);


		/*
		Metodo que se invoca para usar la habilidad R

		@param e Es el evento de CEGUI a que somos registrado
		*/
		bool useAbilityR(const CEGUI::EventArgs &e);

		/*
		Metodo que se invoca para mostrar el tooltip de la habilidad R

		@param e Es el evento de CEGUI a que somos registrado
		*/
		bool showTooltipR(const CEGUI::EventArgs &e);

		/*
		Metodo que se invoca para esconder el tooltip de la habilidad R

		@param e Es el evento de CEGUI a que somos registrado
		*/
		bool hideTooltipR(const CEGUI::EventArgs &e);

		/*
		Metodo que se invoca para usar cualquier habilidad

		@param ability Tecla correspondiente a la habilidad a utilizar
		*/
		void sendUseAbilityMessage(std::string ability);

		/*
		Metodo que se invoca para cambiar el modo de guardia de la unidad a agresivo

		@param e Es el evento de CEGUI a que somos registrado
		*/
		bool setAggressiveMode(const CEGUI::EventArgs &e);

		/*
		Metodo que se invoca para cambiar el modo de guardia de la unidad a pasivo

		@param e Es el evento de CEGUI a que somos registrado
		*/
		bool setPassiveMode(const CEGUI::EventArgs &e);

		/*
		Metodo que se invoca para mostrar el tooltip del cambio de Stand Mode

		@param e Es el evento de CEGUI a que somos registrado
		*/
		bool showAgressiveModeTooltip(const CEGUI::EventArgs &e);

		/*
		Metodo que se invoca para esconder el tooltip del cambio de Stand Mode

		@param e Es el evento de CEGUI a que somos registrado
		*/
		bool hideAgressiveModeTooltip(const CEGUI::EventArgs &e);

		/*
		Metodo que se invoca para mostrar el tooltip del cambio de Stand Mode

		@param e Es el evento de CEGUI a que somos registrado
		*/
		bool showPassiveModeTooltip(const CEGUI::EventArgs &e);

		/*
		Metodo que se invoca para esconder el tooltip del cambio de Stand Mode

		@param e Es el evento de CEGUI a que somos registrado
		*/
		bool hidePassiveModeTooltip(const CEGUI::EventArgs &e);

		/*
		M�todo para activar un elemento concreto

		@param element Elemento a activar
		*/
		void activateElement(CEGUI::Window* element);

		/*
		M�todo para desactivar un elemento concreto

		@param element Elemento a desactivar
		*/
		void deactivateElement(CEGUI::Window* element);


		/*
		Metodo que recoge los clicks sin funcionalidad en esta parte de la interfaz

		@param e Es el evento de CEGUI a que somos registrado
		*/
		bool handleMouseClick(const CEGUI::EventArgs &e);

		bool checkResourceAviability(std::string abilityName);

		
	}; // class CActiveUnitController

} // namespace GUI

#endif // __GUI_ActiveUnitController_H
