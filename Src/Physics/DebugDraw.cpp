#include "DebugDraw.h"
#include "BaseSubsystems/Server.h"
#include "Graphics/Server.h"
#include "Graphics/Scene.h"
#include "Physics/Server.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Server.h"
#include "Logic/Maps/Map.h"

#include <OgreSceneManager.h>
#include <OgreManualObject.h>

//int manualCount;
Ogre::ManualObject* manual;

void compileColors()
{
	Ogre::MaterialPtr myManualObjectMaterial1 = Ogre::MaterialManager::getSingleton().create("PureRed","General"); 
	myManualObjectMaterial1->setReceiveShadows(false); 
	myManualObjectMaterial1->getTechnique(0)->setLightingEnabled(true); 
	myManualObjectMaterial1->getTechnique(0)->getPass(0)->setDiffuse(100,0,0,0); 
	myManualObjectMaterial1->getTechnique(0)->getPass(0)->setAmbient(0,0,0); 
	myManualObjectMaterial1->getTechnique(0)->getPass(0)->setSelfIllumination(1,0,0); 
	myManualObjectMaterial1->compile();

	Ogre::MaterialPtr myManualObjectMaterial2 = Ogre::MaterialManager::getSingleton().create("PureGreen","General"); 
	myManualObjectMaterial2->setReceiveShadows(false); 
	myManualObjectMaterial2->getTechnique(0)->setLightingEnabled(true); 
	myManualObjectMaterial2->getTechnique(0)->getPass(0)->setDiffuse(0,100,0,0); 
	myManualObjectMaterial2->getTechnique(0)->getPass(0)->setAmbient(0,0,0); 
	myManualObjectMaterial2->getTechnique(0)->getPass(0)->setSelfIllumination(0,1,0); 
	myManualObjectMaterial2->compile();

	Ogre::MaterialPtr myManualObjectMaterial3 = Ogre::MaterialManager::getSingleton().create("PureBlue","General"); 
	myManualObjectMaterial3->setReceiveShadows(false); 
	myManualObjectMaterial3->getTechnique(0)->setLightingEnabled(true); 
	myManualObjectMaterial3->getTechnique(0)->getPass(0)->setDiffuse(0,0,100,0); 
	myManualObjectMaterial3->getTechnique(0)->getPass(0)->setAmbient(0,0,0); 
	myManualObjectMaterial3->getTechnique(0)->getPass(0)->setSelfIllumination(0,0,1); 
	myManualObjectMaterial3->compile();
		
	//manualCount = 0;
}

void drawLine(const Vector3& initPos, const Vector3& endPos, char color)
{
	// create ManualObject

	if(manual == NULL)
	{
		manual = Logic::CServer::getSingletonPtr()->getMap()->getScene()->getSceneMgr()->createManualObject("PhysXLines");
		manual->setRenderingDistance(0);
		manual->setDynamic(true);
		manual->setRenderingMinPixelSize(80);
		Logic::CServer::getSingletonPtr()->getMap()->getScene()->getSceneMgr()->getRootSceneNode()->attachObject(manual);
	}
	// specify the material (by name) and rendering type

	
	switch (color)
	{
		case 'w':
			manual->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_LIST);
			break;
		case 'r':
			manual->begin("PureRed", Ogre::RenderOperation::OT_LINE_LIST);
			break;
		case 'g':
			manual->begin("PureGreen", Ogre::RenderOperation::OT_LINE_LIST);
			break;
		case 'b':
			manual->begin("PureBlue", Ogre::RenderOperation::OT_LINE_LIST);
			break;
	}

	// define start and end point
	manual->position(initPos);
	manual->position(endPos);
 
	// tell Ogre, your definition has finished
	manual->end();
 
	// add ManualObject to the RootSceneNode (so it will be visible)
	
	//++manualCount;
}

void drawTriangle(Vector3 firstPos, Vector3 secondPos, Vector3 thirdPos)
{
	// create ManualObject
	Ogre::ManualObject* manual = Logic::CServer::getSingletonPtr()->getMap()->getScene()->getSceneMgr()->createManualObject();
	
	// specify the material (by name) and rendering type

	manual->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_STRIP);
	
	// define start and end point
	manual->position(firstPos);
	manual->position(secondPos);
	manual->position(thirdPos);
 
	// tell Ogre, your definition has finished
	manual->end();
 
	// add ManualObject to the RootSceneNode (so it will be visible)
	Logic::CServer::getSingletonPtr()->getMap()->getScene()->getSceneMgr()->getRootSceneNode()->attachObject(manual);
	
}

void removeLines()
{
	//for(int i = 0; i <= manualCount; ++i)
	//Logic::CServer::getSingletonPtr()->getMap()->getScene()->getSceneMgr()->destroyManualObject(std::to_string(manualCount));
	if(manual != NULL)
		manual->clear();
	
	//manualCount = 0;
}