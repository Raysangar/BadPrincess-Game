#include "OptionsState.h"
#include "LoadingGameState.h"

#include "GUI/Server.h"
#include <CEGUI/CEGUI.h>

#include "BaseSubsystems\Server.h"
#include "BaseSubsystems\Options.h"

#include "Logic/Server.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"
#include "Logic/Entity/Entity.h"
#include "Logic\Entity\Components\CameraMenu.h"

#include "Graphics\Camera.h"
#include "Graphics/Scene.h"
#include "Graphics\FogOfWar.h"

#include "Physics/SceneManager.h"
#include "Physics/Server.h"

#define TIME_FOR_SHOWING 750

namespace Application {

	COptionsState::~COptionsState() {}

	//--------------------------------------------------------

	bool COptionsState::init() 
	{
		CApplicationState::init();

		baseSubsystems = BaseSubsystems::CServer::getSingletonPtr();

		gameOptions = baseSubsystems->getGameOptions();

		// Cargamos la ventana que muestra el menú
		_optionsWindow = CEGUI::WindowManager::getSingletonPtr()->loadLayoutFromFile("Options.layout");
		
		// Asociamos los botones del menú con las funciones que se deben ejecutar.
		_optionsWindow->getChildElement("Back")->
			subscribeEvent(CEGUI::PushButton::EventClicked, 
				CEGUI::SubscriberSlot(&COptionsState::backReleased, this));

		_optionsWindow->getChildElement("Back")->
			subscribeEvent(CEGUI::PushButton::EventMouseEntersSurface, 
			CEGUI::SubscriberSlot(&COptionsState::buttonHover, this));

		initShadowsCombobox();

		initResolutionsCombobox();

		initAntialiasingCombobox();

		//initFullScreenOption();

		initVSyncOption();

		initAudioSettings();

		

		return true;

	}

	//--------------------------------------------------------

	void COptionsState::release() 
	{
		SFXFeedback->unload();
		delete SFXFeedback;

		CApplicationState::release();
	}

	//--------------------------------------------------------

	void COptionsState::activate() 
	{
		CApplicationState::activate();

		// Activamos la ventana que nos muestra el menú y activamos el ratón.
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().setRootWindow(_optionsWindow);
		_optionsWindow->setVisible(true);
		_optionsWindow->activate();
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().show();
		_optionsShowing = true;
	}

	//--------------------------------------------------------

	void COptionsState::deactivate() 
	{		
		// Desactivamos la ventana GUI con el menú y el ratón.
		CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().hide();
		_optionsWindow->deactivate();
		_optionsWindow->setVisible(false);

		CApplicationState::deactivate();

	}

	//--------------------------------------------------------

	void COptionsState::tick(unsigned int msecs) 
	{
		CApplicationState::tick(msecs);

		
		if(_optionsShowing)
		{
			_optionsHiding = false;
			float posX = _optionsWindow->getPosition().d_x.d_scale;
			_optionsWindow->setPosition(CEGUI::UVector2( CEGUI::UDim(posX + (1.0f/TIME_FOR_SHOWING) * msecs, 0.0f ), CEGUI::UDim(0.0f, 0.0f ) ));

			if(_optionsWindow->getPosition().d_x.d_scale > 0.0f)
			{
				_optionsWindow->setPosition(CEGUI::UVector2( CEGUI::UDim(0.0f, 0.0f ), CEGUI::UDim(0.0f, 0.0f ) ));
				_optionsShowing = false;
			}
		}

		if(_optionsHiding)
		{
			_optionsShowing = false;
			float posX = _optionsWindow->getPosition().d_x.d_scale;
			_optionsWindow->setPosition(CEGUI::UVector2( CEGUI::UDim(posX - (1.0f/TIME_FOR_SHOWING) * msecs, 0.0f ), CEGUI::UDim(0.0f, 0.0f ) ));

			if(_optionsWindow->getPosition().d_x.d_scale < -1.0f)
			{
				_optionsWindow->setPosition(CEGUI::UVector2( CEGUI::UDim(-1.0f, 0.0f ), CEGUI::UDim(0.0f, 0.0f ) ));
				_optionsHiding = false;
				_app->unstackState();
			}
		}

		// Simulación física
		Physics::CServer::getSingletonPtr()->tick(msecs);

		// Actualizamos la lógica de juego.
		Logic::CServer::getSingletonPtr()->tick(msecs);

		_time += msecs;

	}

	//--------------------------------------------------------

	bool COptionsState::keyPressed(GUI::TKey key)
	{
		return false;
	}

	//--------------------------------------------------------

	bool COptionsState::keyReleased(GUI::TKey key)
	{
		switch(key.keyId)
		{
		case GUI::Key::ESCAPE:
			CApplicationState::buttonClickedSound->play();
			((Logic::CCameraMenu*)Logic::CServer::getSingletonPtr()->getMap()->getEntityByName("CameraMenu")->getComponent("CCameraMenu"))->goToMenu();
			_optionsHiding = true;
			break;
		#ifdef _DEBUG
		case GUI::Key::F5:
			this->reloadGUI();
			break;
		#endif
		default:
			return false;
		}
		return true;

	}

	//--------------------------------------------------------
	
	bool COptionsState::mouseMoved(const GUI::CMouseState &mouseState)
	{
		return false;
	}

	//--------------------------------------------------------
		
	bool COptionsState::mousePressed(const GUI::CMouseState &mouseState)
	{
		return false;
	}

	//--------------------------------------------------------


	bool COptionsState::mouseReleased(const GUI::CMouseState &mouseState)
	{
		return false;
	}
			
	//--------------------------------------------------------

	bool COptionsState::backReleased(const CEGUI::EventArgs& e)
	{
		CApplicationState::buttonClickedSound->play();
		((Logic::CCameraMenu*)Logic::CServer::getSingletonPtr()->getMap()->getEntityByName("CameraMenu")->getComponent("CCameraMenu"))->goToMenu();
		_optionsHiding = true;
		
		return true;

	}

	//--------------------------------------------------------

	bool COptionsState::fullScreenCheckBoxClicked(const CEGUI::EventArgs& e)
	{
		CApplicationState::buttonClickedSound->play();
		CEGUI::ToggleButton* checkbox = static_cast<CEGUI::ToggleButton*>(_optionsWindow->getChild("Full Screen"));
		CEGUI::Combobox* resolutionsCombobox = static_cast<CEGUI::Combobox*>(_optionsWindow->getChildElement("Resolutions"));
		BaseSubsystems::Options::Resolution selectedResolution = 
			gameOptions->getAvailableResolutions()->at(resolutionsCombobox->getSelectedItem()->getID());
		baseSubsystems->setFullScreen(checkbox->isSelected(), selectedResolution);
		gameOptions->setAttribute("fullScreen", std::to_string(checkbox->isSelected() ? 1 : 0));
		return true;
	}

	//--------------------------------------------------------

	bool COptionsState::vSyncCheckBoxClicked(const CEGUI::EventArgs& e)
	{
		CApplicationState::buttonClickedSound->play();
		CEGUI::ToggleButton* checkbox = static_cast<CEGUI::ToggleButton*>(_optionsWindow->getChild("VSync"));
		baseSubsystems->setVSync(checkbox->isSelected());
		gameOptions->setAttribute("vsync", std::to_string(checkbox->isSelected() ? 1 : 0));
		return true;
	}

	//--------------------------------------------------------

	bool COptionsState::volumeChanged(const CEGUI::EventArgs& e)
	{
		CEGUI::Slider* volumeSlider = static_cast<CEGUI::Slider*>(_optionsWindow->getChild("Volume"));
		Audio::CServer::getInstance()->setGeneralVolume(volumeSlider->getCurrentValue());
		gameOptions->setAttribute("volume", std::to_string(volumeSlider->getCurrentValue()));
		return true;
	}

	//--------------------------------------------------------

	bool COptionsState::SFXChanged(const CEGUI::EventArgs& e)
	{
		CEGUI::Slider* volumeSlider = static_cast<CEGUI::Slider*>(_optionsWindow->getChild("SFX"));
		Audio::CServer::getInstance()->setSFXVolume(volumeSlider->getCurrentValue());
		gameOptions->setAttribute("sfx", std::to_string(volumeSlider->getCurrentValue()));
		return true;
	}

	//--------------------------------------------------------

	bool COptionsState::SFXSliderReleased(const CEGUI::EventArgs& e)
	{
		SFXFeedback->play();
		return true;
	}

	//--------------------------------------------------------

	bool COptionsState::resolutionChanged(const CEGUI::EventArgs& e)
	{
		CEGUI::Combobox* resolutionsCombobox = static_cast<CEGUI::Combobox*>(_optionsWindow->getChildElement("Resolutions"));
		BaseSubsystems::Options::Resolution selectedResolution = 
			gameOptions->getAvailableResolutions()->at(resolutionsCombobox->getSelectedItem()->getID());
		//CEGUI::ToggleButton* checkbox = static_cast<CEGUI::ToggleButton*>(_optionsWindow->getChild("Full Screen"));
		baseSubsystems->changeResolution(selectedResolution, false);
		gameOptions->setAttribute("actualResolutionId", std::to_string(resolutionsCombobox->getSelectedItem()->getID()));
		return true;
	}

	//--------------------------------------------------------

	bool COptionsState::antialiasingChanged(const CEGUI::EventArgs& e)
	{
		CApplicationState::buttonClickedSound->play();
		CEGUI::Combobox* antialiasingCombobox = static_cast<CEGUI::Combobox*>(_optionsWindow->getChildElement("Antialiasing"));
		baseSubsystems->changeAntialiasing(antialiasingCombobox->getSelectedItem()->getID()*2);
		gameOptions->setAttribute("currentAntialiasingId", std::to_string(antialiasingCombobox->getSelectedItem()->getID()));
		return true;
	}

	//--------------------------------------------------------

	bool COptionsState::shadowsChanged(const CEGUI::EventArgs& e)
	{
		CApplicationState::buttonClickedSound->play();
		CEGUI::Combobox* antialiasingCombobox = static_cast<CEGUI::Combobox*>(_optionsWindow->getChildElement("Shadows"));
		gameOptions->setAttribute("currentShadowsId", std::to_string(antialiasingCombobox->getSelectedItem()->getID()));
		return true;
	}

	//--------------------------------------------------------

	void COptionsState::reloadGUI()
	{
		this->deactivate();
		this->release();
		this->init();
		this->activate();
	}

	//--------------------------------------------------------

	void COptionsState::initResolutionsCombobox()
	{
		std::vector<BaseSubsystems::Options::Resolution>* availableResolutions = gameOptions->getAvailableResolutions();
		CEGUI::Combobox* resolutionsCombobox = static_cast<CEGUI::Combobox*>(_optionsWindow->getChildElement("Resolutions"));
		resolutionsCombobox->setReadOnly(true);

		for (unsigned int i = 0; i < availableResolutions->size(); ++i)
		{
			std::string resolutionString = std::string(std::to_string(availableResolutions->at(i).first) + " x " + std::to_string(availableResolutions->at(i).second));
			CEGUI::ListboxTextItem* resolutionItem = new CEGUI::ListboxTextItem(resolutionString, i);
			resolutionItem->setSelectionBrushImage("TaharezLook/MultiListSelectionBrush");
			resolutionsCombobox->addItem(resolutionItem);

			if (std::atoi(gameOptions->getAttribute("actualResolutionId").c_str()) == i)
			{
				resolutionItem->setSelected(true);
				resolutionsCombobox->setText(resolutionString);
			}
		}

		_optionsWindow->getChildElement("Resolutions")->subscribeEvent(CEGUI::Combobox::EventListSelectionAccepted,
			CEGUI::SubscriberSlot(&COptionsState::resolutionChanged, this));

		_optionsWindow->getChildElement("Resolutions")->subscribeEvent(CEGUI::Combobox::EventDropListDisplayed,
			CEGUI::SubscriberSlot(&COptionsState::buttonClicked, this));
		_optionsWindow->getChildElement("Resolutions")->subscribeEvent(CEGUI::Combobox::EventDropListRemoved,
			CEGUI::SubscriberSlot(&COptionsState::buttonClicked, this));
		_optionsWindow->getChildElement("Resolutions")->subscribeEvent(CEGUI::Combobox::EventMouseEntersSurface,
			CEGUI::SubscriberSlot(&COptionsState::buttonHover, this));
	}

	//--------------------------------------------------------

	void COptionsState::initAntialiasingCombobox()
	{
		CEGUI::Combobox* initialiasingCombobox = static_cast<CEGUI::Combobox*>(_optionsWindow->getChildElement("Antialiasing"));
		initialiasingCombobox->setReadOnly(true);
		std::string antialiasingOptionsText[4] = {"Disabled", "Low", "Medium", "High"};
		if (gameOptions->getAttribute(std::string("currentAntialiasingId")) == "")
		{
			gameOptions->setAttribute(std::string("currentAntialiasingId"), std::to_string(0));
			baseSubsystems->changeAntialiasing(0);
		}
		for (unsigned int i = 0; i < 4; ++i)
		{
			CEGUI::ListboxTextItem* antialiasingItem = new CEGUI::ListboxTextItem(antialiasingOptionsText[i], i);
			antialiasingItem->setSelectionBrushImage("TaharezLook/MultiListSelectionBrush");
			initialiasingCombobox->addItem(antialiasingItem);

			if (std::atoi(gameOptions->getAttribute("currentAntialiasingId").c_str()) == i)
			{
				antialiasingItem->setSelected(true);
				initialiasingCombobox->setText(antialiasingOptionsText[i]);
			}
		}

		_optionsWindow->getChildElement("Antialiasing")->subscribeEvent(CEGUI::Combobox::EventListSelectionAccepted,
			CEGUI::SubscriberSlot(&COptionsState::antialiasingChanged, this));

		_optionsWindow->getChildElement("Antialiasing")->subscribeEvent(CEGUI::Combobox::EventDropListDisplayed,
			CEGUI::SubscriberSlot(&COptionsState::buttonClicked, this));
		_optionsWindow->getChildElement("Antialiasing")->subscribeEvent(CEGUI::Combobox::EventDropListRemoved,
			CEGUI::SubscriberSlot(&COptionsState::buttonClicked, this));
		_optionsWindow->getChildElement("Antialiasing")->subscribeEvent(CEGUI::Combobox::EventMouseEntersSurface,
			CEGUI::SubscriberSlot(&COptionsState::buttonHover, this));
	}

	//--------------------------------------------------------

	void COptionsState::initShadowsCombobox()
	{
		CEGUI::Combobox* initialiasingCombobox = static_cast<CEGUI::Combobox*>(_optionsWindow->getChildElement("Shadows"));
		initialiasingCombobox->setReadOnly(true);
		std::string antialiasingOptionsText[4] = {"Disabled", "Low", "Medium", "High"};
		if (gameOptions->getAttribute(std::string("currentShadowsId")) == "")
			gameOptions->setAttribute(std::string("currentShadowsId"), std::to_string(0));
		for (unsigned int i = 0; i < 4; ++i)
		{
			CEGUI::ListboxTextItem* antialiasingItem = new CEGUI::ListboxTextItem(antialiasingOptionsText[i], i);
			antialiasingItem->setSelectionBrushImage("TaharezLook/MultiListSelectionBrush");
			initialiasingCombobox->addItem(antialiasingItem);

			if (std::atoi(gameOptions->getAttribute("currentShadowsId").c_str()) == i)
			{
				antialiasingItem->setSelected(true);
				initialiasingCombobox->setText(antialiasingOptionsText[i]);
			}
		}

		_optionsWindow->getChildElement("Shadows")->subscribeEvent(CEGUI::Combobox::EventListSelectionAccepted,
			CEGUI::SubscriberSlot(&COptionsState::shadowsChanged, this));

		_optionsWindow->getChildElement("Shadows")->subscribeEvent(CEGUI::Combobox::EventDropListDisplayed,
			CEGUI::SubscriberSlot(&COptionsState::buttonClicked, this));
		_optionsWindow->getChildElement("Shadows")->subscribeEvent(CEGUI::Combobox::EventDropListRemoved,
			CEGUI::SubscriberSlot(&COptionsState::buttonClicked, this));
		_optionsWindow->getChildElement("Shadows")->subscribeEvent(CEGUI::Combobox::EventMouseEntersSurface,
			CEGUI::SubscriberSlot(&COptionsState::buttonHover, this));
	}

	//--------------------------------------------------------

	void COptionsState::initFullScreenOption()
	{
		_optionsWindow->getChildElement("Full Screen")->subscribeEvent(CEGUI::ToggleButton::EventSelectStateChanged,
			CEGUI::SubscriberSlot(&COptionsState::fullScreenCheckBoxClicked, this));
		
		CEGUI::ToggleButton* checkbox = static_cast<CEGUI::ToggleButton*>(_optionsWindow->getChild("Full Screen"));
		checkbox->setSelected((std::atoi(gameOptions->getAttribute("fullScreen").c_str()) == 1));
		_optionsWindow->getChildElement("Full Screen")->subscribeEvent(CEGUI::ToggleButton::EventMouseEntersSurface,
			CEGUI::SubscriberSlot(&COptionsState::buttonHover, this));
	}

	//--------------------------------------------------------

	void COptionsState::initVSyncOption()
	{
		_optionsWindow->getChildElement("VSync")->subscribeEvent(CEGUI::ToggleButton::EventSelectStateChanged,
			CEGUI::SubscriberSlot(&COptionsState::vSyncCheckBoxClicked, this));
		
		CEGUI::ToggleButton* checkbox = static_cast<CEGUI::ToggleButton*>(_optionsWindow->getChild("VSync"));
		if (gameOptions->getAttribute(std::string("vsync")) == "")
		{
			gameOptions->setAttribute(std::string("vsync"), std::to_string(0));
			baseSubsystems->changeAntialiasing(0);
		}
		checkbox->setSelected(gameOptions->getAttribute("vsync") == "1");
		_optionsWindow->getChildElement("VSync")->subscribeEvent(CEGUI::ToggleButton::EventMouseEntersSurface,
			CEGUI::SubscriberSlot(&COptionsState::buttonHover, this));
	}

	//--------------------------------------------------------

	void COptionsState::initAudioSettings()
	{
		_optionsWindow->getChildElement("Volume")->subscribeEvent(CEGUI::Slider::EventValueChanged,
			CEGUI::SubscriberSlot(&COptionsState::volumeChanged, this));
		
		float volume = (float) std::atof(baseSubsystems->getGameOptions()->getAttribute("volume").c_str());

		static_cast<CEGUI::Slider*>(_optionsWindow->getChildElement("Volume"))->setCurrentValue(volume);
		
		_optionsWindow->getChildElement("SFX")->subscribeEvent(CEGUI::Slider::EventValueChanged,
			CEGUI::SubscriberSlot(&COptionsState::SFXChanged, this));

		float sfxVolume = (float)std::atof(baseSubsystems->getGameOptions()->getAttribute("sfx").c_str());

		static_cast<CEGUI::Slider*>(_optionsWindow->getChildElement("SFX"))->setCurrentValue(sfxVolume);

		_optionsWindow->getChildElement("SFX")->subscribeEvent(CEGUI::Slider::EventThumbTrackEnded,
			CEGUI::SubscriberSlot(&COptionsState::SFXSliderReleased, this));
	
		SFXFeedback = new Audio::C3DEvent("event:/Battle/Sword2", Vector3::ZERO);
		SFXFeedback->load();
	}

	//--------------------------------------------------------
		
	bool COptionsState::buttonHover(const CEGUI::EventArgs& e)
	{
		buttonHoveredSound->play();
		return true;
	}

	//--------------------------------------------------------
		
	bool COptionsState::buttonClicked(const CEGUI::EventArgs& e)
	{
		buttonClickedSound->play();
		return true;
	}
}
