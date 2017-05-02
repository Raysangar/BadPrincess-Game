//---------------------------------------------------------------------------
// GaleonApplication.cpp
//---------------------------------------------------------------------------

/**
@file GaleonApplication.cpp

Contiene la implementación de la clase aplicacion para aplicaciones
basadas en Ogre. Esta clase maneja la ejecución de todo el juego.

@see Application::CGaleonApplication
@see Application::C3DApplication

@author David Llansó
@date Septiembre, 2010
*/
#include "GaleonApplication.h"

#include "LoadingGameState.h"
#include "ExitState.h"
#include "MenuState.h"
#include "GameState.h"
#include "PauseState.h"
#include "GameOverState.h"
#include "OptionsState.h"
#include "CreditsState.h"
#include "CampaignState.h"
#include "HelpState.h"

namespace Application {

	CGaleonApplication::CGaleonApplication() : C3DApplication()
	{

	} // CGaleonApplication

	//--------------------------------------------------------

	CGaleonApplication::~CGaleonApplication()
	{

	} // ~CGaleonApplication

	//--------------------------------------------------------

	bool CGaleonApplication::init() 
	{
		// Inicializamos la clase de la que heredamos.
		if (!C3DApplication::init())
			return false;
		// Creamos los estados. La aplicación se hace responsable de
		// destruirlos.
		if(!addState("menu", new CMenuState(this)))
			return false;

		if(!addState("exit", new CExitState(this)))
			return false;

		if(!addState("pause", new CPauseState(this)))
			return false;

		if(!addState("gameOver", new CGameOverState(this)))
			return false;

		if(!addState("loading", new CLoadingGameState(this)))
			return false;

		if(!addState("options", new COptionsState(this)))
			return false;

		if(!addState("credits", new CCreditsState(this)))
			return false;

		if(!addState("campaign", new CCampaignState(this)))
			return false;

		if(!addState("help", new CHelpState(this)))
			return false;

		if(!setState("loading"))
			return false;

		return true;

	} // init

	//--------------------------------------------------------

	void CGaleonApplication::release()
	{
		// Desactivamos y eliminamos todos los estados.
		releaseAllStates();

		C3DApplication::release();

	} // release

} // namespace Application
