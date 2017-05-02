/**

*/
#ifndef __GUI_GUIMessage_H
#define __GUI_GUIMessage_H

#include <string>
#include <list>

namespace CEGUI
{
	class Window;	
	class WindowManager;
}

// Declaración de la clase
namespace GUI
{
	/**
	
	*/
	class CGUIMessage
	{
	public:
		CGUIMessage(std::string type, std::string id, std::string text,float pos_x, float pos_y,std::string portraitName,int duration, std::string color = "FFD6EBFF");
		~CGUIMessage();
		void setVisible(bool option);
		std::string getId(){ return _id;}
		std::string getType(){ return _type;}	
		int getCurrentTime(){ return _currentTime;}
		void setCurrentTime(int time){ _currentTime = time;}
		int getDurationTime(){ return _durationTime;}
		void setPositionY(float pos_y);
		void setPosition(std::pair<float,float> pos){_position = pos;}
		std::pair<float,float> getPosition(){return _position;}
		
	protected:
	private:
		std::string _id;
		std::string _type;
		std::string _portraitName;
		CEGUI::Window* _text;
		CEGUI::Window* _image;
		int _durationTime;
		int _currentTime;
		std::pair<float,float> _position;
	};
}

#endif