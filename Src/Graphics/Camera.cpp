//---------------------------------------------------------------------------
// Camera.cpp
//---------------------------------------------------------------------------

/**
@file Camera.cpp

Contiene la implementación de la clase que maneja la cámara.

@see Graphics::CCamera

@author David Llansó
@date Julio, 2010
*/

#include "Camera.h"
#include "Scene.h"

#include "BaseSubsystems/Server.h"
#include "BaseSubsystems/Math.h"

#include <assert.h>

#include <OgreCamera.h>
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreCompositorManager.h>

namespace Graphics 
{
	CCamera::CCamera(const std::string &name, CScene *scene)
		: _scene(0)
	{
		_name = name;
		_scene = scene;

		// Creamos la estructura de nodos de la cámara. Los nodos cuelgan
		// de la raiz, son globales.
		_cameraNode = _scene->getSceneMgr()->getRootSceneNode()->
					createChildSceneNode(name + "_camera_node");
		_targetNode = scene->getSceneMgr()->getRootSceneNode()->
					createChildSceneNode(name + "_target_node");

		// Hacemos que el nodo de la cámara siempre esté mirando al nodo
		// objetivo.
		_cameraNode->setAutoTracking(true, _targetNode);
		// Fijamos el viraje de la cámara para se mantenga paralelo al
		// suelo.
		_cameraNode->setFixedYawAxis(true);

		_camera = scene->getSceneMgr()->createCamera(name + "_camera");
		//HACK: Valores cableados de las distancias para reenderizar. 
		// Deberían poder configurarse.
		_camera->setNearClipDistance(5);
		_camera->setFarClipDistance(500);
		// Ajuste de campo de vision de la camara segun la resolucion
		// Parametros para el fustrum (rectangulo que se ve por la camara)
		//Ogre::Real outleft;
		//Ogre::Real outright;
		//Ogre::Real outtop;
		//Ogre::Real outbottom ;
		//_camera->getFrustumExtents(outleft, outright, outtop, outbottom);
		//// Obtencion de la resolucion de la pantalla
		Ogre::RenderWindow *v = BaseSubsystems::CServer::getSingletonPtr()->getRenderWindow();
		int width = v->getWidth();
		int height = v->getHeight();
		_aspectRatio = (float)width/(float)height;
		//
		//float originalAspectRatio = (float)4/(float)3;
		//float diff = _aspectRatio-originalAspectRatio;
		//// Si al diferencia es mayor que 0 se aumenta la vision por los lados
		//if(diff > 0)
		//{
		//	outleft = outleft + (outleft*diff);
		//	outright = outright + (outright*diff);
		//}
		//// Si es menor se disminuye
		//else if (diff < 0)
		//{
		//	diff = -diff;
		//	outleft = outleft - (outleft*diff);
		//	outright = outright - (outright*diff);
		//}
		//// Ajuste de los valores del fustrum para que escale el rango de vision con los valores base
		////PAY ATTENTION this distort the aspect ratio and when in Logic::CCamera of camera entity
		////we compute the world limits, the camera shows more x space then what it really have to show 
		////but it need because the view could be streched
		//_camera->setFrustumExtents(outleft, outright, outtop, outbottom);

		_camera->setAspectRatio(_aspectRatio);
		// Finalmente adjuntamos la cámara a su nodo.
		_cameraNode->attachObject (_camera);


	} // CCamera

	//--------------------------------------------------------

	CCamera::~CCamera() 
	{
		// desacoplamos la cámara de su nodo
		_cameraNode->detachAllObjects();
		_scene->getSceneMgr()->destroyCamera(_camera);
		_scene->getSceneMgr()->destroySceneNode(_cameraNode);
		_scene->getSceneMgr()->destroySceneNode(_targetNode);

	} // ~CCamera

	//--------------------------------------------------------
	
	const Vector3 &CCamera::getCameraPosition() 
	{
		return _cameraNode->getPosition();
	}

	//--------------------------------------------------------

	const Vector3 &CCamera::getTargetCameraPosition() 
	{
		return _targetNode->getPosition();
	}

	//--------------------------------------------------------

	const Quaternion &CCamera::getCameraOrientation() 
	{
		return _camera->getOrientation();
	}

	//--------------------------------------------------------

	void CCamera::setCameraPosition(const Vector3 &newPosition)
	{
		_cameraNode->setPosition(newPosition);
	}

	//--------------------------------------------------------

	void CCamera::setTargetCameraPosition(const Vector3 &newPosition)
	{
		_targetNode->setPosition(newPosition);
	}

	Ray CCamera::getCameraToViewportRay(float screenx, float screeny){
		return _camera->getCameraToViewportRay(Ogre::Real(screenx),Ogre::Real(screeny));
	}

	void CCamera::setCompositor(std::string compositorName)
	{
		Ogre::CompositorManager::getSingleton().addCompositor(_camera->getViewport(), compositorName);
	}

	void CCamera::enableCompositor(std::string compositorName)
	{
		Ogre::CompositorManager::getSingleton().setCompositorEnabled(_camera->getViewport(), compositorName, true);
	}

	void CCamera::disableCompositor(std::string compositorName)
	{
		Ogre::CompositorManager::getSingleton().setCompositorEnabled(_camera->getViewport(), compositorName, false);
	}
} // namespace Graphics
