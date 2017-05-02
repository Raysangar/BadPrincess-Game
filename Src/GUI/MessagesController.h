/**

*/
#ifndef __GUI_MessagesController_H
#define __GUI_MessagesController_H

#include <map>

#define CMessagesMap std::map<std::string, CGUIMessage*> 


namespace CEGUI
{
	/*
	class Listbox;
	class Window;
	class EventArgs;
	*/	
}


// Declaraci�n de la clase
namespace GUI
{
	class CGUIMessage;
	/**
	
	*/
	class CMessagesController
	{
	public:

		/**
		Constructor.
		*/
		CMessagesController();

		/**
		Destructor.
		*/
		~CMessagesController();

		/**
		
		*/
		void activate();

		/**
		
		*/
		void deactivate();

		/**
		
		*/
		void tick(unsigned int msecs);

		/**
		M�todo que crea un mensaje en pantalla (GUI).
		*/
		void showMessage(std::string id, std::string text, float pos_x, float pos_y, std::string portraitName);

		void showTemporalMessage(std::string id, std::string text, float pos_x, float pos_y, std::string portraitName,int duration);


		/**
		M�todo que crea un mensaje de error en pantalla (GUI).
		*/
		void showObjectiveMessage(std::string id, std::string text, float pos_x, float pos_y, std::string portraitName, std::string color = "FFFAFA00");

		/**
		M�todo que crea un mensaje de error en pantalla (GUI).
		*/
		void showErrorMessage(std::string id, std::string text, float pos_x, float pos_y, std::string portraitName);

		void showTemporalErrorMessage(std::string id, std::string text, float pos_x, float pos_y, std::string portraitName, int duration);

		/**
		M�todo que limpia la pantalla, eliminando todos los mensajes actuales (GUI).
		*/
		void clearScreen();

		/**
		M�todo que limpia la pantalla, eliminando todos los mensajes de tipo ordinario (GUI).
		*/
		void clearRegularMessages();

		/**
		M�todo que limpia la pantalla, eliminando todos los mensajes de tipo error (GUI).
		*/
		void clearErrorMessages();

		/**
		M�todo que elimina el mensaje especificado (GUI).
		*/
		void removeMessage(std::string id);

		/**
		Resetea todas las informaciones relativa a una sigola partida (GameState)
		*/
		void reset();

		/**
		Devuelve el n�mero de mensajes que pertenecen a eventos.
		*/
		int countEventMessages(){ return _eventMessagesCount; };

		void recalculatePositions();

	protected:
		
		
	private :
		CMessagesMap _messagesMap;
		unsigned int _eventMessagesCount;	
	}; // class CMessagesController

} // namespace GUI

#endif // __GUI_MapController_H
