//---------------------------------------------------------------------------
// DrawManager.cpp
//---------------------------------------------------------------------------

#include "DrawManager.h"
#include "Scene.h"

#include <string>
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
#include <OgreManualObject.h>
#include <BaseSubsystems\Math.h>

#define _USE_MATH_DEFINES
#include <math.h>

namespace Graphics 
{
	CDrawManager::CDrawManager(CScene *scene)
	{	
		_scene = scene;

	} // CCamera

	//--------------------------------------------------------

	CDrawManager::~CDrawManager() 
	{
		// desacoplamos la cámara de su nodo
		_scene = NULL;

	} // ~CCamera

	//--------------------------------------------------------
	
	void CDrawManager::compileColor(std::string name, float red, float green, float blue, float alpha)
	{
		if(Ogre::MaterialManager::getSingletonPtr()->resourceExists(name))
			return;
		Ogre::MaterialPtr myMaterial = Ogre::MaterialManager::getSingleton().create(name,"General"); 
		myMaterial->setReceiveShadows(false); 
		myMaterial->getTechnique(0)->setLightingEnabled(true); 
		myMaterial->getTechnique(0)->getPass(0)->setDiffuse(red,green,blue,alpha); 
		myMaterial->getTechnique(0)->getPass(0)->setAmbient(0,0,0); 
		myMaterial->getTechnique(0)->getPass(0)->setSelfIllumination(1,0,0); 
		myMaterial->getTechnique(0)->getPass(0)->setPointSize(5);
		myMaterial->compile();

	}

	//--------------------------------------------------------
	void CDrawManager::drawLine(const Ogre::Vector3& initPos, const Ogre::Vector3& endPos, Ogre::ManualObject* line, char color)
	{
		this->setColor(line,color,Ogre::RenderOperation::OT_LINE_STRIP);			

		// define start and end point
		line->position(initPos);
		line->position(endPos);
 
		// tell Ogre, your definition has finished
		line->end(); 
	}
	void CDrawManager::drawLine(const Ogre::Vector3& initPos, const Ogre::Vector3& endPos, Ogre::SceneNode* sceneNode, char color, std::string name, bool dynamic)
	{
		Ogre::ManualObject* line;		
		if(!_scene->getSceneMgr()->hasManualObject(name))
		{
			line = _scene->getSceneMgr()->createManualObject(name);

			if(dynamic)
				line->setDynamic(true);
			
			this->setColor(line,color,Ogre::RenderOperation::OT_LINE_STRIP);			

			sceneNode->attachObject(line);
		}
		else
		{
			line = _scene->getSceneMgr()->getManualObject(name);

			if(dynamic)
				line->beginUpdate(0);
			else{
				this->setColor(line,color,Ogre::RenderOperation::OT_LINE_STRIP);
			}
		}
		

		// define start and end point
		line->position(initPos);
		line->position(endPos);
 
		// tell Ogre, your definition has finished
		line->end(); 
	}
	void CDrawManager::drawLine(const std::list<Ogre::Vector3> &list, Ogre::SceneNode* sceneNode, char color, std::string name, bool dynamic)
	{
		Ogre::ManualObject* line;		
		if(!_scene->getSceneMgr()->hasManualObject(name))
		{
			line = _scene->getSceneMgr()->createManualObject(name);

			if(dynamic)
				line->setDynamic(true);
			
			this->setColor(line,color, Ogre::RenderOperation::OT_LINE_STRIP);

			sceneNode->attachObject(line);
		}
		else
		{
			line = _scene->getSceneMgr()->getManualObject(name);

			if(dynamic)
				line->beginUpdate(0);
			else{
				this->setColor(line,color, Ogre::RenderOperation::OT_LINE_STRIP);

			}
		}
		

		// define start and end point
		std::list<Vector3>::const_iterator it=list.begin();
		for(;it!=list.end();++it)
		{
			line->position(*it);
		}
 
		// tell Ogre, your definition has finished
		line->end(); 
	}
	void CDrawManager::drawSegmentList(const std::list<Ogre::Vector3> &list, Ogre::SceneNode* sceneNode, char color, std::string name, bool dynamic)
	{
		Ogre::ManualObject* line;		
		if(!_scene->getSceneMgr()->hasManualObject(name))
		{
			line = _scene->getSceneMgr()->createManualObject(name);

			if(dynamic)
				line->setDynamic(true);
				this->setColor(line,color, Ogre::RenderOperation::OT_LINE_STRIP);
			sceneNode->attachObject(line);
		}
		else
		{
			line = _scene->getSceneMgr()->getManualObject(name);

			if(dynamic)
				line->beginUpdate(0);
			else{
				this->setColor(line,color, Ogre::RenderOperation::OT_LINE_STRIP);

			}
		}
		

		// define start and end point
		std::list<Vector3>::const_iterator it=list.begin();
		for(;it!=list.end();++it)
		{
			line->position(*it);
		}
 
		// tell Ogre, your definition has finished
		line->end(); 
	}
	void CDrawManager::drawPointList(const std::list<Ogre::Vector3> &list, Ogre::SceneNode* sceneNode, char color, std::string name, bool dynamic)
	{
		Ogre::ManualObject* line;		
		if(!_scene->getSceneMgr()->hasManualObject(name))
		{
			line = _scene->getSceneMgr()->createManualObject(name);

			if(dynamic)
				line->setDynamic(true);
			
			this->setColor(line,color, Ogre::RenderOperation::OT_POINT_LIST);

			sceneNode->attachObject(line);
		}
		else
		{
			line = _scene->getSceneMgr()->getManualObject(name);

			if(dynamic)
				line->beginUpdate(0);
			else{
				this->setColor(line,color, Ogre::RenderOperation::OT_POINT_LIST);

			}
		}
		

		// define start and end point
		std::list<Vector3>::const_iterator it=list.begin();
		for(;it!=list.end();++it)
		{
			line->position(*it);
		}
 
		// tell Ogre, your definition has finished
		line->end(); 
	}

	//--------------------------------------------------------

	void CDrawManager::drawRectangle(const Ogre::Vector3& firstPos, const Ogre::Vector3& secondPos, const Ogre::Vector3& thirdPos, const Ogre::Vector3& fourthPos, 
		Ogre::SceneNode* sceneNode, char color, std::string name, bool dynamic)
	{
		Ogre::ManualObject* rectangle;		
		if(!_scene->getSceneMgr()->hasManualObject(name))
		{
			rectangle = _scene->getSceneMgr()->createManualObject(name);

			if(dynamic)
				rectangle->setDynamic(true);
			this->setColor(rectangle,color,Ogre::RenderOperation::OT_LINE_STRIP);

			sceneNode->attachObject(rectangle);
		}
		else
		{
			rectangle = _scene->getSceneMgr()->getManualObject(name);
			if(dynamic)
				rectangle->beginUpdate(0);
			else
			{
				this->setColor(rectangle,color,Ogre::RenderOperation::OT_LINE_STRIP);
	
			}
		}		

		rectangle->position(firstPos);  // start position
		rectangle->position(secondPos);  // draw first line

		rectangle->position(thirdPos);
		rectangle->position(fourthPos);
		rectangle->position(firstPos);  // draw fourth line

		rectangle->end();
	}

	//-------------------------------------------------------

	void CDrawManager::drawCilinderShapeXZ(Ogre::ManualObject * cilinder,const Ogre::Vector3 center,const float halfHeight,const float radius,const float accuracy)
{
	this->drawCircleShapeXZ(cilinder,Ogre::Vector3(center.x,center.y+halfHeight,center.z),radius,accuracy,0,2*Math::PI);
	this->drawCircleShapeXZ(cilinder,Ogre::Vector3(center.x,center.y-halfHeight,center.z),radius,accuracy,0,2*Math::PI);

	cilinder->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_LIST);
 
    unsigned point_index = 0;
	//divide the circle into (Math::PI / accuracy) part and draw a vertical segment
	for(float theta = 0; theta <= 2*Math::PI; theta += Math::PI / accuracy) {
		cilinder->position(center.x+(radius * cos(theta)), center.y+halfHeight,center.z+(radius * sin(theta)));
		cilinder->position(center.x+(radius * cos(theta)), center.y-halfHeight,center.z+(radius * sin(theta)));
		cilinder->index(point_index++);
		cilinder->index(point_index++);
    }
	cilinder->end();
}
void CDrawManager::drawCircleShapeXZ(Ogre::ManualObject * circle,const Ogre::Vector3 center,const float radius,const float accuracy,const float startAngle,const float endAngle)
{
    circle->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_STRIP);
 
    unsigned point_index = 0;
	//divide the circle into (Math::PI / accuracy) part and draw a segment in XZ axes with linked point
	for(float theta = startAngle; theta <= endAngle; theta += Math::PI / accuracy) {

		circle->position(center.x+(radius * cos(theta)), center.y,center.z+(radius * sin(theta)));
        circle->index(point_index++);
    }
    circle->index(0); // Rejoins the last point to the first.
    circle->end();
}
void CDrawManager::drawCircleShapeXY(Ogre::ManualObject * circle,const Ogre::Vector3 center,const float zRotation,const float radius,const float accuracy,const float startAngle,const float endAngle)
{
    circle->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_STRIP);
	
	Ogre::Vector3 p;
    unsigned point_index = 0;
	float r=radius;
	float dr=2*(radius/accuracy);
	//divide the circle into (Math::PI / accuracy) part and draw a segment in XY axes with zRotation in Z axe
	for(float theta = startAngle; theta <= endAngle; theta += Math::PI / accuracy) {
		p.x=center.x+(r*(cos(zRotation)));
		p.y=center.y+(radius * sin(theta));
		p.z=center.z+(r * sin(zRotation));
		r-=dr;
		circle->position(p);
        circle->index(point_index++);
    }
    circle->index(0); // Rejoins the last point to the first.
    circle->end();

}
void CDrawManager::drawUnitaryBoxShape(Ogre::ManualObject * cube)
{
	cube->begin("BaseWhiteNoLighting",Ogre::RenderOperation::OT_LINE_LIST);
   
   cube->position(-0.5,0.0,-0.5);
   cube->position(-0.5,1.0,-0.5);
   cube->position(0.5,1.0,-0.5);
   cube->position(0.5,0.0,-0.5);
   cube->position(-0.5,0.0,0.5);
   cube->position(-0.5,1.0,0.5);
   cube->position(0.5,1.0,0.5);
   cube->position(0.5,0.0,0.5);




   cube->index(0);
	cube->index(1);
   cube->index(1);
	cube->index(2);
	cube->index(2);
	cube->index(3);
	cube->index(3);
	cube->index(0);
	cube->index(0);
   cube->index(4);
   cube->index(4);
	cube->index(5);
	cube->index(5);
	cube->index(6);
	cube->index(6);
	cube->index(7);
	cube->index(7);
	cube->index(4);
	cube->index(1);
	cube->index(5);
	cube->index(2);
	cube->index(6);
	cube->index(3);
	cube->index(7);

   cube->end(); 
}

void CDrawManager::drawCapsuleShape(Ogre::ManualObject * capsule,const float halfHeight,const float radius)
{
	//accuracy must be power of 2
	float accuracy=8;	
	this->drawCilinderShapeXZ(capsule,Vector3(0,halfHeight+radius,0),halfHeight,radius,accuracy);
	//draw the half sphere on top and bottom of the cilinder
	 for(float theta = 0; theta <= Math::PI; theta += Math::PI / accuracy) {
		 this->drawCircleShapeXY(capsule,Vector3(0,2*halfHeight+radius,0),theta,radius,accuracy,0,Math::PI);
		 this->drawCircleShapeXY(capsule,Vector3(0,radius,0),theta,radius,accuracy,Math::PI,2*Math::PI);
	 }
};

void CDrawManager::setColor(Ogre::ManualObject * m,char color,Ogre::RenderOperation::OperationType op)
{
				switch (color)
				{
					case 'w':
						m->begin("BaseWhiteNoLighting", op);
						break;
					case 'r':
						compileColor("PureRed",100,0,0,0);
						m->begin("PureRed", op);
						break;
					case 'g':
						compileColor("PureGreen",0,100,0,0);
						m->begin("PureGreen", op);
						break;
					case 'b':
						compileColor("PureBlue",0,0,100,0);
						m->begin("PureBlue",op);
						break;
					default:				
						m->begin("BaseWhiteNoLighting", op);
						break;
				}			
}
} // namespace Graphics
