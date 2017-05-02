/**
@file MinimapController.cpp

Contiene la implementación de la clase CMinimapController. Se encarga de
recibir eventos del teclado y del ratón y de interpretarlos para
mover al jugador.

@see GUI::CMinimapController

@author David Llansó
@date Agosto, 2010
*/

#include "MinimapController.h"

#include "Graphics\Server.h"
#include "Graphics\Scene.h"
#include "Graphics\Camera.h"

#include "Logic\Entity\Components\WorldComponent.h"
#include "Logic\Entity\Components\Camera.h"
#include "Logic\Entity\Components\AnimatedGraphics.h"

#include "Logic\CameraMessages.h"

#include "GUI/Server.h"
#include "GUI\PlayerController.h"

#include <iostream>
#include <cassert>

#include "InputManager.h"
#include "BaseSubsystems/Server.h"

#include "CEGUI\CEGUI.h"
#include "CEGUI\RendererModules\Ogre\Renderer.h"
#include "CEGUI\Vector.h"

#include "AI\Server.h"
#include "AI\EntityManager.h"

#include "Logic\Entity\Entity.h"
#include "Logic/Server.h"
#include "Logic/Maps/Map.h"

#include <OgreImage.h>
#include <OgreTexture.h>
#include <OgreTextureManager.h>
#include <OgreManualObject.h>
#include <OgreHardwarePixelBuffer.h>
#include <OgreSceneManager.h>

#include <list>

#define EVENT_MAX_TIME 4000
#define MINIMAP_CAM_HEIGHT 1000
#define PIXELS_IN_MINIMAP_FOR_UNITS 8
#define PIXELS_IN_MINIMAP_FOR_PRINCESS 10

namespace GUI {


	//--------------------------------------------------------
	
	CMinimapController::~CMinimapController()
	{
		deactivate();
	} // ~CMinimapController

	//--------------------------------------------------------

	void CMinimapController::awake(){

		//We set the first variables
		BaseSubsystems::CServer::getSingletonPtr()->getWindowExtents(_windowWidth,_windowHeight);
		
		_mRenderer = (CEGUI::OgreRenderer*)CEGUI::System::getSingletonPtr()->getRenderer();

		_worldComponent = static_cast<Logic::CWorldComponent*>(Logic::CServer::getSingletonPtr()->getMap()->getEntityByName("Ground")->getComponent("CWorldComponent"));
		_cameraComponent = static_cast<Logic::CCamera*>(Logic::CServer::getSingletonPtr()->getMap()->getEntityByName("Camera")->getComponent("CCamera"));

		_worldBounds = &(_worldComponent->getPhysicBounds());

		//Variables for the minimap camera
		Vector3 cameraPosition = Vector3(0,MINIMAP_CAM_HEIGHT, 0);
		Vector3 lookAt = Vector3(0,-1,-1);
 
		//create a texture for the mini-map to render on 
		Ogre::TexturePtr texture = Ogre::TextureManager::getSingleton().createManual("mmTex",
		   Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::TEX_TYPE_2D,
		   512, 512, 0, Ogre::PF_R8G8B8, Ogre::TU_RENDERTARGET );
 
		//Get the render buffer
		Ogre::RenderTexture* rt = texture->getBuffer()->getRenderTarget();
		
		//Get  the sceneManager
		Ogre::SceneManager* _sceneManager = Logic::CServer::getSingletonPtr()->getMap()->getScene()->getSceneMgr();

		//create a minimap camera, use the same settings as main camera
		Ogre::Camera* _mmCamera = _sceneManager->createCamera("minimapCamera");
 
		//AspectRatio(1) is for a square minimap
		_mmCamera->setAspectRatio(1);
		_mmCamera->setPosition(cameraPosition);
		_mmCamera->lookAt(lookAt);


		//Render the viewport of the camera into the render buffer
		Ogre::Viewport *vp = rt->addViewport(_mmCamera);

		vp->setClearEveryFrame(true);
		vp->setOverlaysEnabled(false);
		vp->setBackgroundColour(Ogre::ColourValue::Black);		
 
		//Create a CEGUI texture from mmTex
		CEGUI::Texture &cetex = _mRenderer->createTexture("mmTexure",texture);

		//Create a CEGUI image and add the cetex texture to it
		CEGUI::BasicImage* backgroundMinimap = (CEGUI::BasicImage*)( &CEGUI::ImageManager::getSingleton().create("BasicImage", "backgroundMinimap"));
		const CEGUI::Rectf rectmm(CEGUI::Vector2f(0.0f, 0.0f), cetex.getOriginalDataSize());
		backgroundMinimap->setTexture(&cetex);
		backgroundMinimap->setArea(rectmm);
		backgroundMinimap->setAutoScaled(CEGUI::ASM_Both);

		CEGUI::Window* backgroundMinimapContainer = CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChild("MinimapBackground");	
		backgroundMinimapContainer->setProperty("Image", "backgroundMinimap");

		_metersToPixels = _worldBounds->x > _worldBounds->z ? (float)_worldBounds->x : (float)_worldBounds->z;

		// Create the texture
		_minimapTexture = Ogre::TextureManager::getSingleton().createManual(
			"Minimap", // name
			Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			Ogre::TEX_TYPE_2D,      // type
			512, 512,         // width & height
			0,                // number of mipmaps
			Ogre::PF_A8R8G8B8,     // pixel format
			Ogre::TU_DYNAMIC_WRITE_ONLY_DISCARDABLE);      // usage; should be TU_DYNAMIC_WRITE_ONLY_DISCARDABLE for
							  // textures updated very often (e.g. each frame)

		_metersToPixels = _minimapTexture->getWidth() / _metersToPixels;

		CEGUI::Texture &minimapTex = _mRenderer->createTexture("minimaptex",_minimapTexture);
				
		const CEGUI::Rectf rect(CEGUI::Vector2f(0.0f, 0.0f), minimapTex.getOriginalDataSize());
		CEGUI::BasicImage* image = (CEGUI::BasicImage*)( &CEGUI::ImageManager::getSingleton().create("BasicImage", "RTTImage"));
		image->setTexture(&minimapTex);
		image->setArea(rect);
		image->setAutoScaled(CEGUI::ASM_Both);		
		
		_textureContainer = CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChild("MinimapTextureContainer");		

		//set image to be displayed in the satic image widget
		_textureContainer->setProperty("Image", "RTTImage");
		//_textureContainer->setProperty("AlwaysOnTop", "True");
		

		_minimapContainerWidth = CEGUI::CoordConverter::asAbsolute(_textureContainer->getWidth(),(float)_windowWidth);
		_minimapContainerHeight = CEGUI::CoordConverter::asAbsolute(_textureContainer->getHeight(),(float)_windowHeight);
		_pixelsToMetersX = _worldBounds->x / _minimapContainerWidth ;
		_pixelsToMetersY = _worldBounds->z / _minimapContainerHeight ;

		_textureContainer->subscribeEvent(CEGUI::PushButton::EventMouseButtonDown, CEGUI::SubscriberSlot(&CMinimapController::moveToClickPosition, this));  

		BaseSubsystems::CServer::getSingletonPtr()->getWindowExtents(_windowWidth,_windowHeight);

		_frustumTopLeftCorner = new Vector2();
		_frustumBottomRightCorner = new Vector2();
	}

	//--------------------------------------------------------

	void CMinimapController::activate()
	{		
		CInputManager::getSingletonPtr()->addKeyListener(this);
		CInputManager::getSingletonPtr()->addMouseListener(this);

		_minimapFrame = CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChild("MinimapFrame");
		_minimapWindow = CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChild("MinimapBackground");

		_minimapFrame->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::SubscriberSlot(&CMinimapController::handleMouseClick, this));
		_minimapFrame->subscribeEvent(CEGUI::Window::EventMouseDoubleClick, CEGUI::SubscriberSlot(&CMinimapController::handleMouseClick, this));	

		_minimapWindow->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::SubscriberSlot(&CMinimapController::handleMouseClick, this));
		_minimapWindow->subscribeEvent(CEGUI::Window::EventMouseDoubleClick, CEGUI::SubscriberSlot(&CMinimapController::handleMouseClick, this));	

	} // activate

	//--------------------------------------------------------

	void CMinimapController::deactivate()
	{
		CInputManager::getSingletonPtr()->removeKeyListener(this);
		CInputManager::getSingletonPtr()->removeMouseListener(this);
		
	} // deactivate

	void CMinimapController::reset()
	{
		CEGUI::ImageManager::getSingleton().destroy("RTTImage");
		CEGUI::ImageManager::getSingleton().destroy("backgroundMinimap");

		if(_mRenderer!=nullptr)
		{
			_mRenderer->destroyTexture("minimaptex");
			_mRenderer->destroyTexture("mmTexure");
		}
		
		if(!_minimapEvents.empty())
		{
			for(std::map<std::string,MinimapEvent*>::const_iterator it = _minimapEvents.begin();
				it != _minimapEvents.end(); ++it)
					delete it->second;
			_minimapEvents.clear();
		}
		if(_frustumTopLeftCorner != NULL)
			delete _frustumTopLeftCorner;

		if(_frustumBottomRightCorner!= NULL)
			delete _frustumBottomRightCorner;

	}
	//--------------------------------------------------------

	void CMinimapController::tick(unsigned int msecs) 
	{

		// Get the pixel buffer
		Ogre::HardwarePixelBufferSharedPtr pixelBuffer = _minimapTexture->getBuffer();
 
		// Lock the pixel buffer and get a pixel box
		pixelBuffer->lock(Ogre::HardwareBuffer::HBL_DISCARD); // for best performance use HBL_DISCARD!
		const Ogre::PixelBox& pixelBox = pixelBuffer->getCurrentLock();

		Ogre::uint32 *data = static_cast<Ogre::uint32*>(pixelBox.data);
		size_t height = pixelBox.getHeight();
		size_t width = pixelBox.getWidth();
		size_t pitch = pixelBox.rowPitch; // Skip between rows of image
		
		for(size_t y=0; y<height; ++y)
		{
			for(size_t x=0; x<width; ++x)
			{
				// 0xAARRGGBB -> fill the buffer with yellow pixels
				drawOnMinimap(data,x,y,pitch, (Ogre::uint32)0x00FFFFFF,1);				
			}
		}


		for(std::set<Logic::CEntity*>::const_iterator itChests = AI::CServer::getSingletonPtr()->getEntityManager()->getChestList()->begin();
			itChests != AI::CServer::getSingletonPtr()->getEntityManager()->getChestList()->end() ; ++itChests)
		{	
			Vector2 pos = Vector2::ZERO;
			tranformPositionToMinimap((*itChests)->getPosition().x,(*itChests)->getPosition().z,pos);		
			drawOnMinimap(data, (int)pos.x - PIXELS_IN_MINIMAP_FOR_UNITS/2, (int)pos.y - PIXELS_IN_MINIMAP_FOR_UNITS/2, pitch, (Ogre::uint32)0xFFFFFFFF,PIXELS_IN_MINIMAP_FOR_UNITS);		
		}

		for(std::set<Logic::CEntity*>::const_iterator itWell = AI::CServer::getSingletonPtr()->getEntityManager()->getWellList()->begin();
			itWell != AI::CServer::getSingletonPtr()->getEntityManager()->getWellList()->end() ; ++itWell)
		{	
			Vector2 pos = Vector2::ZERO;
			tranformPositionToMinimap((*itWell)->getPosition().x,(*itWell)->getPosition().z,pos);		
			drawOnMinimap(data, (int)pos.x - PIXELS_IN_MINIMAP_FOR_UNITS/2, (int)pos.y - PIXELS_IN_MINIMAP_FOR_UNITS/2, pitch, (Ogre::uint32)0xFFFFFFFF,PIXELS_IN_MINIMAP_FOR_UNITS);		
		}


		for(std::set<Logic::CEntity*>::const_iterator itAllies = AI::CServer::getSingletonPtr()->getEntityManager()->getAlliesList()->begin();
			itAllies != AI::CServer::getSingletonPtr()->getEntityManager()->getAlliesList()->end() ; ++itAllies)
		{	
			Vector2 pos = Vector2::ZERO;
			tranformPositionToMinimap((*itAllies)->getPosition().x,(*itAllies)->getPosition().z,pos);		
			drawOnMinimap(data, (int)pos.x - PIXELS_IN_MINIMAP_FOR_UNITS/2, (int)pos.y - PIXELS_IN_MINIMAP_FOR_UNITS/2, pitch, (Ogre::uint32)0xFFFFFFFF,PIXELS_IN_MINIMAP_FOR_UNITS);		
		}		

		
		for(std::set<Logic::CEntity*>::const_iterator itPrincess = AI::CServer::getSingletonPtr()->getEntityManager()->getPrincessesList()->begin();
			itPrincess != AI::CServer::getSingletonPtr()->getEntityManager()->getPrincessesList()->end() ; ++itPrincess)
		{	
			Vector2 pos = Vector2::ZERO;
			tranformPositionToMinimap((*itPrincess)->getPosition().x,(*itPrincess)->getPosition().z,pos);		
			drawOnMinimap(data,(int)pos.x - PIXELS_IN_MINIMAP_FOR_PRINCESS/2,(int)pos.y - PIXELS_IN_MINIMAP_FOR_PRINCESS/2, pitch, (Ogre::uint32)0xFFd6a2fd	,PIXELS_IN_MINIMAP_FOR_PRINCESS);	
		}	


		for(std::set<Logic::CEntity*>::const_iterator itEnemies = AI::CServer::getSingletonPtr()->getEntityManager()->getEnemiesList()->begin();
			itEnemies != AI::CServer::getSingletonPtr()->getEntityManager()->getEnemiesList()->end() ; ++itEnemies)
		{
			Logic::CAnimatedGraphics* animatedGraphicsComponent = static_cast<Logic::CAnimatedGraphics*>((*itEnemies)->getComponent("CAnimatedGraphics"));
			if(animatedGraphicsComponent->getGraphicsEntity()->getVisible())
			{
				Vector2 pos = Vector2::ZERO;
				tranformPositionToMinimap((*itEnemies)->getPosition().x,(*itEnemies)->getPosition().z,pos);		
				drawOnMinimap(data, (int)pos.x - PIXELS_IN_MINIMAP_FOR_UNITS/2, (int)pos.y - PIXELS_IN_MINIMAP_FOR_UNITS/2, pitch, (Ogre::uint32)0xFFFF0000,PIXELS_IN_MINIMAP_FOR_UNITS);	
			}
		}
		
		drawCameraFrustum(data,pitch);

		pixelBuffer->unlock(); 

		for(std::map<std::string, MinimapEvent*>::iterator it = _minimapEvents.begin(); it != _minimapEvents.end();)
		{
			it->second->acumTime += msecs;
			if(it->second->acumTime > EVENT_MAX_TIME)
			{	
				_textureContainer->removeChild(it->first);
				_minimapEventsImages.erase(_minimapEventsImages.find(it->first));
				delete (it->second);
				it = _minimapEvents.erase(it);				
			}
			else
				++it;
		}
		
	} // capture


	//--------------------------------------------------------

	bool CMinimapController::keyPressed(TKey key){

		return false;

	} // keyPressed

	//--------------------------------------------------------

	bool CMinimapController::keyReleased(TKey key){

		return false;

	} // keyReleased

	
	//--------------------------------------------------------
		
	bool CMinimapController::mousePressed(const CMouseState &mouseState,const bool alreadyManaged)
	{
		CEGUI::Vector2f mousePos = CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().getPosition();
		
		return _textureContainer->isHit(mousePos) || _minimapFrame->isHit(mousePos)|| _minimapFrame->isHit(mousePos);
	} // mousePressed

	//--------------------------------------------------------
	
	bool CMinimapController::mouseReleased(const CMouseState &mouseState,const bool alreadyManaged)
	{
		if(_clickAction)
		{
			_clickAction=false;
			return true;
		}else
			return false;
	} // mouseReleased

	//--------------------------------------------------------

	void CMinimapController::tranformPositionToMinimap(float coordX, float coordZ, Ogre::Vector2 &pos)
	{
		pos.x = (Ogre::Real)((int)(coordX * _metersToPixels) + (int)_minimapTexture->getWidth()/2);
		pos.y = (Ogre::Real)((int)(coordZ * _metersToPixels) + (int)_minimapTexture->getHeight()/2);
	}

	//--------------------------------------------------------

	void CMinimapController::tranformMinimapToPosition(float pixelX, float pixelY, Ogre::Vector3 &pos)
	{

		float x = (pixelX - _minimapContainerWidth/2) * _pixelsToMetersX;
		float z = (pixelY - _minimapContainerHeight/2) * _pixelsToMetersY;

		pos.x = x;
		pos.y = 0.0f;
		pos.z = z;
	}
	
	//--------------------------------------------------------

	void CMinimapController::tranformPositionToMinimapContainer(float coordX, float coordZ, Ogre::Vector2 &pos)
	{
		pos.x = _minimapContainerWidth/2 + coordX/_pixelsToMetersX;
		pos.y = _minimapContainerHeight/2 + coordZ/_pixelsToMetersY;
	}

	//--------------------------------------------------------

	void CMinimapController::drawOnMinimap(Ogre::uint32 *data, int posX, int posY, size_t pitch, Ogre::uint32 color, int size)
	{
		if(posX >= 0 && (posX + size) < (int)pitch && posY >= 0 && (posY + size) < (int)pitch)
		{
			for(int i = 0; i < size; ++i)		
				for(int j = 0; j < size; ++j)
					data[pitch * (posY + j) + posX + i] = color;			
		}
	}

	//-------------------------------------------------------

	void CMinimapController::drawCameraFrustum(Ogre::uint32 *data, size_t pitch)
	{
		_frustumHeight = _cameraComponent->getFrustumHeight();
		_frustumWidth = _frustumHeight * _cameraComponent->getAspectRatio();

		Vector2 pos = Vector2::ZERO;
		tranformPositionToMinimapContainer(_cameraComponent->getCameraTargetPosition().x - _frustumWidth/2,_cameraComponent->getCameraTargetPosition().z - _frustumHeight/2,(*_frustumTopLeftCorner));
		tranformPositionToMinimapContainer(_cameraComponent->getCameraTargetPosition().x + _frustumWidth/2,_cameraComponent->getCameraTargetPosition().z + _frustumHeight/2,(*_frustumBottomRightCorner));

		tranformPositionToMinimap(_cameraComponent->getCameraTargetPosition().x - _frustumWidth/2,_cameraComponent->getCameraTargetPosition().z - _frustumHeight/2,pos);		
		drawOnMinimap(data, (int)pos.x, (int)pos.y, pitch, (Ogre::uint32)0xFFFF0000,2);
		int maxX = (int)pos.x + (int)(_frustumWidth * _metersToPixels);
		int maxY = (int)pos.y + (int)(_frustumHeight * _metersToPixels);
		
		for(int i = (int)pos.x ; i <= maxX; ++i)
		{
			if(i == (int)pos.x + 2 || i == (int)pos.x + 1 || i == (int)pos.x || i == maxX || i == maxX - 1 || i == maxX - 2)
			{
				for(int j = (int)pos.y ; j <= maxY; ++j)
					drawOnMinimap(data,i,j,pitch, (Ogre::uint32)0xFFFFFFFF,1);
			}
			else
			{
				drawOnMinimap(data,i,(int)pos.y,pitch, (Ogre::uint32)0xFFFFFFFF,1);
				drawOnMinimap(data,i,(int)pos.y + 1,pitch, (Ogre::uint32)0xFFFFFFFF,1);
				drawOnMinimap(data,i,(int)pos.y + 2,pitch, (Ogre::uint32)0xFFFFFFFF,1);
				drawOnMinimap(data,i,maxY - 2,pitch, (Ogre::uint32)0xFFFFFFFF,1);
				drawOnMinimap(data,i,maxY - 1,pitch, (Ogre::uint32)0xFFFFFFFF,1);
				drawOnMinimap(data,i,maxY,pitch, (Ogre::uint32)0xFFFFFFFF,1);
			}
		}
	}

	//-------------------------------------------------------

	bool CMinimapController::moveToClickPosition(const CEGUI::EventArgs &e)
	{	
		if(!_clickAction)
		{
			CEGUI::Vector2f mousePos = CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().getPosition();
			
			float pixX = CEGUI::CoordConverter::screenToWindowX(*_textureContainer,mousePos.d_x);
			float pixY = CEGUI::CoordConverter::screenToWindowY(*_textureContainer,mousePos.d_y);
								
			Vector3 pos = Vector3::ZERO;	
			tranformMinimapToPosition(pixX,pixY,pos);

			auto m = std::make_shared<CameraPosition>();
			m->position = pos;		
			_cameraComponent->getEntity()->emitMessageN(m);
			_clickAction = true;
		}
		return true;
	}

	//--------------------------------------------------------
		
	bool CMinimapController::handleMouseClick(const CEGUI::EventArgs &e)
	{
		if(!_clickAction)
		{
			_clickAction=true;
		}
		return true;
	}

	//--------------------------------------------------------

	void CMinimapController::addMinimapEvent(std::string id, EventType type, Vector3 position)
	{
		if(_minimapEvents.find(id) == _minimapEvents.end())
		{
			Vector2 minimapPos = Vector2::ZERO;

			tranformPositionToMinimapContainer(position.x,position.z,minimapPos);

			if(minimapPos.x < _frustumTopLeftCorner->x || minimapPos.x > _frustumBottomRightCorner->x || 
				minimapPos.y < _frustumTopLeftCorner->y || minimapPos.y > _frustumBottomRightCorner->y)
			{

				std::map<std::string,MinimapEvent*>::iterator it = _minimapEvents.begin();
				bool positionAvailable = true;
				for(; it != _minimapEvents.end() && positionAvailable; ++it)
				{
					Vector2 v2(it->second->posX,it->second->posY);
					if(v2.distance(minimapPos) < 0.1f)
					{
						positionAvailable = false;
					}
				}
				if(positionAvailable)
				{
					MinimapEvent *minimapEvent = new MinimapEvent();

					minimapEvent->acumTime = 0;				
					minimapEvent->type = type;
					minimapEvent->posX = minimapPos.x;
					minimapEvent->posY = minimapPos.y;

					_minimapEvents[id] = minimapEvent;

					std::string minimapEventImage;

					switch (type)
					{
					case GUI::battle:
						minimapEventImage = "BadPrincessHud/MinimapBattleIcon";
						break;
					case GUI::princess:
						minimapEventImage = "BadPrincessHud/MinimapPrincessIcon";
						break;
					case GUI::chest:
						minimapEventImage = "BadPrincessHud/MinimapChestIcon";
						break;
					case GUI::trap:
						minimapEventImage = "BadPrincessHud/MinimapTrapIcon";
						break;
					}

					CEGUI::Window *staticImage = (CEGUI::Window*) CEGUI::WindowManager::getSingleton().createWindow("BadPrincess/Image",id);	

					staticImage->setPosition(CEGUI::UVector2( CEGUI::UDim(0.0f, minimapPos.x - 12.0f), CEGUI::UDim(0.0f,minimapPos.y - 12.0f)));
					staticImage->setSize(CEGUI::USize( CEGUI::UDim(0.0f, 25.0f ), CEGUI::UDim(0.0f, 25.0f)));
					staticImage->setProperty("Image",minimapEventImage);
					staticImage->setProperty("AlwaysOnTop","true");
					staticImage->subscribeEvent(CEGUI::PushButton::EventMouseButtonDown, CEGUI::SubscriberSlot(&CMinimapController::moveToClickPosition, this));  

					_textureContainer->addChild(staticImage);
					//CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->addChild(staticImage);
					_minimapEventsImages[id] = staticImage;
				}
			}
		}
	}

	//--------------------------------------------------------

} // namespace GUI
