/**
@file OscilationManager.cpp

Componente para crear una animación de oscilación en un objeto

@see Logic::IComponent

@author Iván León
@date June, 2015
*/

#include "OscilationManager.h"

#include "Logic/Server.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"


#include "Graphics/Scene.h"
#include "Graphics/Server.h"
#include "Graphics/MeshEntity.h"
#include "Graphics/FrustumEntity.h"

#include <OgreSceneNode.h>
#include <OgreSceneManager.h>

#define TIME_FOR_MOVEMENT 1000

namespace Logic 
{
	IMP_FACTORY(COscilationManager);
	
	//---------------------------------------------------------

	COscilationManager::~COscilationManager() 
	{
	//	_circleNode->detachAllObjects();
	//	_scene->getSceneMgr()->destroySceneNode(_circleNode);

	} // ~CGraphics
	 
	//---------------------------------------------------------

	void COscilationManager::awake()
	{
		//Ogre::Real duration = 1.0f;
		//unsigned int steps = 4;
		//Ogre::Real stepLength = duration / Ogre::Real(steps);		
		//Ogre::Real displacementSize = 1.0f;

		//Ogre::Vector3 keyFramePos = _entity->getPosition();
		//Ogre::SceneNode *node = Logic::CServer::getSingletonPtr()->getMap()->getScene()->getSceneMgr()->getSceneNode(_entity->getName() + "_node");			
		//
		//Ogre::Animation *animation = Logic::CServer::getSingletonPtr()->getMap()->getScene()->getSceneMgr()->createAnimation("anim",duration);		
		//Ogre::NodeAnimationTrack* track = animation->createNodeTrack(0,node);		
		//Ogre::TransformKeyFrame *kf;

		////Keyframe 0: centro
		//kf = track->createNodeKeyFrame(stepLength * 0);
		//kf->setTranslate(keyFramePos);
		//

		////Keyframe 1: arriba
		//kf = track->createNodeKeyFrame(stepLength * 1);
		//keyFramePos += Ogre::Vector3::UNIT_Y * displacementSize;		
		//kf->setTranslate(keyFramePos);
		//

		////Keyframe 3: abajo
		//kf = track->createNodeKeyFrame(stepLength * 3);
		//keyFramePos += Ogre::Vector3::UNIT_Y * displacementSize * -2;		
		//kf->setTranslate(keyFramePos);
		//

		////Keyframe 4: centro
		//kf = track->createNodeKeyFrame(stepLength * 4);
		//keyFramePos += Ogre::Vector3::UNIT_Y * displacementSize;		
		//kf->setTranslate(keyFramePos);
		//		
		//
		//animation->setInterpolationMode(Ogre::Animation::IM_SPLINE);
		//animationTranslation = Logic::CServer::getSingletonPtr()->getMap()->getScene()->getSceneMgr()->createAnimationState("anim");
		//animationTranslation->setEnabled(true);
		//animationTranslation->setLoop(true);
		maxYPos = _entity->getPosition().y + 1.0f;
		minYPos = _entity->getPosition().y - 1.0f;
		_posYincrement = 2.0f/TIME_FOR_MOVEMENT;
	}

	//---------------------------------------------------------

	void COscilationManager::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);

		if(_entity->getPosition().y + _posYincrement * msecs >= maxYPos) 
		{
			_entity->setPositionN(Vector3(_entity->getPosition().x,maxYPos,_entity->getPosition().z));
			_posYincrement *= -1;
		}

		else if(_entity->getPosition().y + _posYincrement * msecs <= minYPos) 
		{
			_entity->setPositionN(Vector3(_entity->getPosition().x,minYPos,_entity->getPosition().z));
			_posYincrement *= -1;
		}
		else
			_entity->setPositionN(_entity->getPosition() + Vector3(0,_posYincrement*msecs,0));
	}

	//---------------------------------------------------------

	bool COscilationManager::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		return true;

	} // spawn	

	//---------------------------------------------------------

	bool COscilationManager::acceptN(const std::shared_ptr<NMessage> &message)
	{
		return false;
	} // acceptN

	//---------------------------------------------------------

	void COscilationManager::processN(const std::shared_ptr<NMessage> &message)
	{

	} // processN

} // namespace Logic

