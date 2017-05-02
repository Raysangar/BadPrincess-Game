#include "GUIMessage.h"
#include "CEGUI\CEGUI.h"
#include <iostream>
//#include <string>

#define TEXT_WIDTH 0.40f
#define TEXT_HEIGHT 0.50f
//#define IMAGE_ALIGN 0.05f
//#define PADDING 0.8f

namespace GUI 
{
	CGUIMessage::CGUIMessage(std::string type, std::string id, std::string text, float pos_x, float pos_y, std::string portraitName, int duration, std::string color): _type(type), _id(id),_portraitName(portraitName),_currentTime(0),_durationTime(duration)
	{
		// Se almacena la posición.

		_position = std::make_pair(pos_x,pos_y);

		// Se accede al gestor de ventanas de CEGUI.
		CEGUI::WindowManager& windowManager = CEGUI::WindowManager::getSingleton();

		// Se crea una ventana para texto estático.
		_text = (CEGUI::Window*) windowManager.createWindow("BadPrincess/StaticText");	

		_text->setProperty("Font", "Girls-14");


		// Se asigna el texto a la ventana.
		_text->setText(text);

		// Se toma la altura de la fuente.
		float fontHeight = _text->getFont()->getBaseline();//getFontHeight();

		//

		//fontHeight += fontHeight * PADDING;

		//

		float textSpacing = _text->getFont()->getLineSpacing();

		// Se toma lo que ocuparía (en px) el texto en una sola línea.
		float fontWidth = _text->getFont()->getTextAdvance(text) + (_text->getFont()->getTextAdvance(text) * 0.20f);


		// Se asigna el tamaño por defecto de la ventana.
		_text->setSize(CEGUI::USize( CEGUI::UDim(TEXT_WIDTH, 0.0f ), CEGUI::UDim(TEXT_HEIGHT, 0.0f)));

		// Se asigna la posición de la ventana.
		_text->setPosition(CEGUI::UVector2( CEGUI::UDim(pos_x, 0.0f ), CEGUI::UDim(pos_y, 0.0f)));

		// Se calcula el número de líneas que ocupará el texto. (Ancho total / Ancho de línea).
		float numLines = (float) std::ceil(fontWidth / _text->getPixelSize().d_width);

		// Se calcula la altura de la ventana ( Número de líneas * Altura fuente).
		float finalHeight = (float) (numLines * fontHeight);

		// Se calcula la altura porcentual de la ventana. (Regla de tres).
		float finalHeightP = ((finalHeight * TEXT_HEIGHT) / _text->getPixelSize().d_height) + ((numLines - 1) * 0.01f) + 0.06f;

		// Se garantiza una altura mínima de ventana de un 8%.
		if(finalHeightP < 0.08f)
			finalHeightP = 0.08f;

		//std::cout << "numLines_R: " << fontWidth / _text->getPixelSize().d_width << std::endl;
		//std::cout << text.substr(0,16) << "... - numLines: " << numLines << std::endl;

		// Si sólo existe una línea, se ajusta el ancho de la ventana al texto.
		if(numLines <= 1)
			_text->setSize( CEGUI::USize( CEGUI::UDim((fontWidth * TEXT_WIDTH / _text->getPixelSize().d_width)  + 0.05f, 0.0f ), CEGUI::UDim((finalHeightP), 0.0f ) ) );
		else
			_text->setSize( CEGUI::USize( CEGUI::UDim(TEXT_WIDTH, 0.0f ), CEGUI::UDim((finalHeightP), 0.0f )));
		
		if(type == "error" || type == "objective")
		{
			_text->setProperty("BackgroundEnabled","False");
			_text->setProperty("FrameEnabled","False");
			_text->setProperty("TextColours",color); // Ice Blue
		}
		

		// Se asigna la alineación del texto.
		_text->setProperty("HorzFormatting","WordWrapJustified");
		_text->setProperty("VertFormatting","VertCentred");

		// Se añade la ventana de texto estático a la ventana raíz.
		CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->addChild(_text);

		if(_portraitName != "")
		{
			_image = (CEGUI::Window*) windowManager.createWindow("BadPrincess/Image");	
			_image->setProperty("Image", portraitName);
			_image->setSize( CEGUI::USize( CEGUI::UDim( 0.0f, 165.0f ), CEGUI::UDim( 0.0f, 168.0f ) ) );
			_image->setPosition(CEGUI::UVector2( CEGUI::UDim(pos_x - 0.1f, 0.0f ), CEGUI::UDim(pos_y, 0.0f ) ));		
			CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->addChild(_image);
		}
	}

	CGUIMessage::~CGUIMessage()
	{	
		_text->destroy();
		if(_portraitName != "")
		 _image->destroy();
	}

	void CGUIMessage::setVisible(bool option)
	{
		_text->setVisible(option);
		if(_portraitName != "")
		 _image->setVisible(option);
	}

	void CGUIMessage::setPositionY(float pos_y)
	{
		_text->setPosition(CEGUI::UVector2( _text->getPosition().d_x, CEGUI::UDim(pos_y, 0.0f)));
	}
}