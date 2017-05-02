/**
@file PlayerController.cpp

Contiene la implementación de la clase CPlayerController. Se encarga de
recibir eventos del teclado y del ratón y de interpretarlos para
mover al jugador.

@see GUI::CPlayerController

@author David Llansó
@date Agosto, 2010
*/

#include "PlayerController.h"

#include <iostream>
#include <cassert>

#include "GUI\Server.h"
#include "GUI\ActiveUnitController.h"

#include "InputManager.h"
#include "SelectedUnitsController.h"
#include "Server.h"

#include "BaseSubsystems/Server.h"
#include "Graphics/Server.h"
#include "Graphics/Scene.h"
#include "Graphics/DrawManager.h"
#include "Physics/Server.h"
#include "Physics\PhysicPerceptionManager.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Server.h"
#include "Logic/Maps/Map.h"
#include "Logic\Maps\EntityFactory.h"
#include "Logic\CameraMessages.h"
#include "Logic\PhysicMessages.h"
#include "Logic\AllyUnitMessages.h"
#include "Logic\RangerMessages.h"
#include "Logic\MovementMessages.h"
#include "Logic\EnemyMessages.h"
#include "Logic\AimsMessages.h"
#include "Logic\EntityMessages.h"
#include "Logic/RouteCalculationMessages.h"
#include "Logic/CombatMessages.h"
#include "Logic/GraphicsMessages.h"
#include "Logic/GameControlMessages.h"

#include "Logic\Entity\Components\SetPhysicEntityHelperComponent.h"
#include "Logic\Entity\Components\Life.h"
#include "Logic\Entity\Components\Graphics.h"
#include "Logic\Entity\Components\RemovableWorldComponent.h"
#include "Logic\Entity\Components\Camera.h"

#include "AI/Server.h"
#include "AI/StrategyController.h"
#include "AI/ResourceServer.h"
#include "AI/EntityManager.h"

#include <OgreSceneNode.h>
#include <OgreSceneManager.h>

#define TURN_FACTOR 0.001f
#define SCROLL_FACTOR 0.001f
#define CLOSE_TO_EDGE 30
#define MIN_DELTA_TO_SELECTION_BOX 0.2
#define MAX_RAY_DISTANCE 1000
#define OVERLAP_QUERIES_HEIGHT 20
#define SELECTION_UNIT_OVERLAP_RADIUS 2.5f
#define SELECTION_MAX_UNITS 9
#define DOUBLE_CLICK_TIME 300

namespace GUI {
	/*
		These vector are used to apply the group filter into the physic raycast and physic overlaps
		They contain all the physic_group integer value of the entities that we are interested in 
	*/
	
	const Physics::CollisionGroup PHYSIC_REMOVABLE_WORLD_FILTER []={Physics::CollisionGroup::eRemovableWorld};
	//this vector contains the world physic group
	const Physics::CollisionGroup PHYSIC_WORLD_FILTER []={Physics::CollisionGroup::eWorldGround};
	//this vector contains all the physic groups of selectable entities (with left mouse button)
	const Physics::CollisionGroup PHYSIC_SELECTABLE_FILTER[]={Physics::CollisionGroup::ePlayer};
	const Physics::CollisionGroup PHYSIC_SELECTABLE_AND_WORLD_FILTER[]={Physics::CollisionGroup::ePlayer,Physics::CollisionGroup::eWorldGround};
	//this vector contains all the physic groups of the entities that we could select with right mouse button
	const Physics::CollisionGroup PHYSIC_DESTINATION_ENTITY_FILTER[]={Physics::CollisionGroup::eEnemy,Physics::CollisionGroup::eResource, Physics::CollisionGroup::eChest, Physics::CollisionGroup::ePlayer};
	const Physics::CollisionGroup PHYSIC_DESTINATION_ENTITY_AND_WORLD_FILTER[]={Physics::CollisionGroup::eEnemy,Physics::CollisionGroup::eResource, Physics::CollisionGroup::eChest, Physics::CollisionGroup::ePlayer,Physics::CollisionGroup::eWorldGround};
	
	CPlayerController::CPlayerController() : _camera(0), startMousePosX(-1.0f),startMousePosY(-1.0f), _selectionRectangleNode(0), 
		mouseAttachedEntity(nullptr), _numKeysPressed(0), _cameraVector(Vector3(0.0f,0.0f,0.0f)), _mouseCamera(false),_leftButtonDrag(false),
		_mouseLocked(false),_activeTransparency(false),_transparentEntity(0), _keysCamera(false),_doubleClickMsecs(0),_checkDoubleClick(false),
		_rCtrlPushed(false),_lCtrlPushed(false)
	{
		for(int i = 0; i < 10; ++i)
		{
			_selectedUnitsGroups[i] = new std::list<Logic::CEntity*>();
		}
		
		_selectedUnits = new std::list<Logic::CEntity*>();
		_firstVertexSelectionBox = new Vector3();
		_inputManager = CInputManager::getSingletonPtr();

	} 
	// CPlayerController

	//--------------------------------------------------------
	
	CPlayerController::~CPlayerController()
	{
		deactivate();
		

		for(int i = 0; i < 10; ++i)
		{
			if(_selectedUnitsGroups[i] != NULL)
			{
				delete _selectedUnitsGroups[i];
				_selectedUnitsGroups[i] = NULL;
			}
		}

		if(_selectedUnits != NULL)
		{
			delete _selectedUnits;
			_selectedUnits = NULL;
		}
		if( _firstVertexSelectionBox != NULL)
		{
			delete _firstVertexSelectionBox;
			_firstVertexSelectionBox=NULL;
		}

		_inputManager = nullptr;
		
	} // ~CPlayerController

	//--------------------------------------------------------

	void CPlayerController::awake()
	{
		initClickDictionaries();
		activeUnit = _selectedUnits->end();
		_selectionRectangleNode = Logic::CServer::getSingletonPtr()->getMap()->getScene()->getSceneMgr()->getRootSceneNode()->createChildSceneNode();	
	}

	//--------------------------------------------------------

	void CPlayerController::activate()
	{		
		CInputManager::getSingletonPtr()->addKeyListener(this);
		CInputManager::getSingletonPtr()->addMouseListener(this);

		BaseSubsystems::CServer::getSingletonPtr()->getWindowExtents(_windowWidth,_windowHeight);
		_leftButtonDrag = false;
	} // activate

	//--------------------------------------------------------

	void CPlayerController::deactivate()
	{
		CInputManager::getSingletonPtr()->removeKeyListener(this);
		CInputManager::getSingletonPtr()->removeMouseListener(this);				
		
	} // deactivate

	void CPlayerController::reset()
	{
		_camera=nullptr;
		startMousePosX= startMousePosY=-1.0f;
		_selectionRectangleNode=nullptr;
		mouseAttachedEntity=nullptr; 
		_numKeysPressed=0;
		_cameraVector=Vector3::ZERO;
		_mouseCamera=false;
		_leftButtonDrag=false;
		_selectedUnits->clear();
		activeUnit = _selectedUnits->end();
		*_firstVertexSelectionBox = Vector3::ZERO;
		_keysCamera = false;
		_mouseLocked = false;
		_doubleClickMsecs = 0;
		_checkDoubleClick = false;
		_activeTransparency = false;
		_transparentEntity = nullptr;
		initClickDictionaries();
	}
	//-------------------------------------------------------
	
	void CPlayerController::tick (unsigned int msecs)
	{
		if(_checkDoubleClick)
			_doubleClickMsecs += msecs;

		if(_doubleClickMsecs > DOUBLE_CLICK_TIME)
		{
			_checkDoubleClick = false;
			_doubleClickMsecs = 0;
		}

		if(!_mouseCamera)
		{
			TKey k(0 , GUI::Key::UPARROW);
			if(_cameraVector != Vector3::ZERO)
			{
				Vector3 camera = Vector3::ZERO;
				camera.y = _cameraVector.y;

				if(_inputManager->isKeyPressed(k))
					camera += Vector3(0, 0, -1);

				k.keyId = GUI::Key::DOWNARROW;

				if(_inputManager->isKeyPressed(k))
					camera += Vector3(0, 0, 1);

				k.keyId = GUI::Key::LEFTARROW;

				if(_inputManager->isKeyPressed(k))
					camera += Vector3(-1, 0, 0);

				k.keyId = GUI::Key::RIGHTARROW;

				if(_inputManager->isKeyPressed(k))
					camera += Vector3(1, 0, 0);

				if(camera.x == 0 && camera.z == 0)
					_keysCamera = false;

				if(camera != _cameraVector)
				{
					auto mCC = std::make_shared<CameraControl>();
					mCC->direction = camera;
					_camera->emitMessageN(mCC);
					_cameraVector = camera;

					if(_cameraVector.x == 0 && _cameraVector.z == 0)
						_keysCamera = false;
					else
						_keysCamera = true;


					//std::cout << "Reajustando camara: " << _cameraVector << std::endl;
				}
			}
			k.keyId = GUI::Key::ADD;

			if(_inputManager->isKeyPressed(k))
			{			
				auto message = std::make_shared<CameraScroll>();
				message->scroll = -12.0f;
				_camera->emitMessageN(message);
			}
			
			k.keyId = GUI::Key::SUBTRACT;

			if(_inputManager->isKeyPressed(k))
			{		
				auto message = std::make_shared<CameraScroll>();
				message->scroll = 12.0f;
				_camera->emitMessageN(message);
			}

			k.keyId = GUI::Key::RBRACKET;

			if(_inputManager->isKeyPressed(k))
			{			
				auto message = std::make_shared<CameraScroll>();
				message->scroll = -12.0f;
				_camera->emitMessageN(message);
			}
			
			k.keyId = GUI::Key::SLASH;

			if(_inputManager->isKeyPressed(k))
			{		
				auto message = std::make_shared<CameraScroll>();
				message->scroll = 12.0f;
				_camera->emitMessageN(message);
			}
		}
	}

	//-------------------------------------------------------

	bool CPlayerController::keyPressed(TKey key){

		Vector3 direction = _cameraVector;

		switch(key.keyId)
		{
			#ifdef _DEBUG
				case GUI::Key::P:
					{
						auto m = std::make_shared<ToggleDrawPhysicDebug>();
						Logic::CServer::getSingletonPtr()->getMap()->sendMessageToAll(m);
						break;
					}
			#endif
			case GUI::Key::LCONTROL:
				{
					_lCtrlPushed = true;
					break;
				}
			case GUI::Key::RCONTROL:
				{
					_lCtrlPushed = true;
					break;
				}
			case GUI::Key::NUMBER0:
				{
					if((_lCtrlPushed || _rCtrlPushed) && _selectedUnits->size() > 0)
					{
						if(_selectedUnitsGroups[0]->size() > 0)
						{
							_selectedUnitsGroups[0]->clear();								
						}
						_selectedUnitsGroups[0]->insert(_selectedUnitsGroups[0]->end(), _selectedUnits->begin(), _selectedUnits->end());							
					}					
					else
					{
						if(_selectedUnitsGroups[0]->size() > 0)
						{
							this->sendSelectMessage(false);
							_selectedUnits->clear();
							_selectedUnits->insert(_selectedUnits->end(),_selectedUnitsGroups[0]->begin(),_selectedUnitsGroups[0]->end());
							activeUnit = _selectedUnits->begin();
							this->sendSelectMessage(true);
						}
					}
					break;
				}
			case GUI::Key::NUMBER1:
				{
					if((_lCtrlPushed || _rCtrlPushed) && _selectedUnits->size() > 0)
					{
						if(_selectedUnitsGroups[1]->size() > 0)
						{
							_selectedUnitsGroups[1]->clear();								
						}
						_selectedUnitsGroups[1]->insert(_selectedUnitsGroups[1]->end(), _selectedUnits->begin(), _selectedUnits->end());							
					}					
					else
					{
						if(_selectedUnitsGroups[1]->size() > 0)
						{
							this->sendSelectMessage(false);
							_selectedUnits->clear();
							_selectedUnits->insert(_selectedUnits->end(),_selectedUnitsGroups[1]->begin(),_selectedUnitsGroups[1]->end());
							activeUnit = _selectedUnits->begin();
							this->sendSelectMessage(true);
						}
					}
					break;
				}

			case GUI::Key::NUMBER2:
				{
					if((_lCtrlPushed || _rCtrlPushed) && _selectedUnits->size() > 0)
					{
						if(_selectedUnitsGroups[2]->size() > 0)
						{
							_selectedUnitsGroups[2]->clear();								
						}
						_selectedUnitsGroups[2]->insert(_selectedUnitsGroups[2]->end(), _selectedUnits->begin(), _selectedUnits->end());							
					}					
					else
					{
						if(_selectedUnitsGroups[2]->size() > 0)
						{
							this->sendSelectMessage(false);
							_selectedUnits->clear();
							_selectedUnits->insert(_selectedUnits->end(),_selectedUnitsGroups[2]->begin(),_selectedUnitsGroups[2]->end());
							activeUnit = _selectedUnits->begin();
							this->sendSelectMessage(true);
						}
					}
					break;
				}

			case GUI::Key::NUMBER3:
				{
					if((_lCtrlPushed || _rCtrlPushed) && _selectedUnits->size() > 0)
					{
						if(_selectedUnitsGroups[3]->size() > 0)
						{
							_selectedUnitsGroups[3]->clear();								
						}
						_selectedUnitsGroups[3]->insert(_selectedUnitsGroups[3]->end(), _selectedUnits->begin(), _selectedUnits->end());							
					}					
					else
					{
						if(_selectedUnitsGroups[3]->size() > 0)
						{
							this->sendSelectMessage(false);
							_selectedUnits->clear();
							_selectedUnits->insert(_selectedUnits->end(),_selectedUnitsGroups[3]->begin(),_selectedUnitsGroups[3]->end());
							activeUnit = _selectedUnits->begin();
							this->sendSelectMessage(true);
						}
					}
					break;
				}

			case GUI::Key::NUMBER4:
				{
					if((_lCtrlPushed || _rCtrlPushed) && _selectedUnits->size() > 0)
					{
						if(_selectedUnitsGroups[4]->size() > 0)
						{
							_selectedUnitsGroups[4]->clear();								
						}
						_selectedUnitsGroups[4]->insert(_selectedUnitsGroups[4]->end(), _selectedUnits->begin(), _selectedUnits->end());							
					}					
					else
					{
						if(_selectedUnitsGroups[4]->size() > 0)
						{
							this->sendSelectMessage(false);
							_selectedUnits->clear();
							_selectedUnits->insert(_selectedUnits->end(),_selectedUnitsGroups[0]->begin(),_selectedUnitsGroups[4]->end());
							activeUnit = _selectedUnits->begin();
							this->sendSelectMessage(true);
						}
					}
					break;
				}

			case GUI::Key::NUMBER5:
				{
					if((_lCtrlPushed || _rCtrlPushed) && _selectedUnits->size() > 0)
					{
						if(_selectedUnitsGroups[5]->size() > 0)
						{
							_selectedUnitsGroups[5]->clear();								
						}
						_selectedUnitsGroups[5]->insert(_selectedUnitsGroups[5]->end(), _selectedUnits->begin(), _selectedUnits->end());							
					}					
					else
					{
						if(_selectedUnitsGroups[5]->size() > 0)
						{
							this->sendSelectMessage(false);
							_selectedUnits->clear();
							_selectedUnits->insert(_selectedUnits->end(),_selectedUnitsGroups[5]->begin(),_selectedUnitsGroups[5]->end());
							activeUnit = _selectedUnits->begin();
							this->sendSelectMessage(true);
						}
					}
					break;
				}
			case GUI::Key::NUMBER6:
				{
					if((_lCtrlPushed || _rCtrlPushed) && _selectedUnits->size() > 0)
					{
						if(_selectedUnitsGroups[6]->size() > 0)
						{
							_selectedUnitsGroups[6]->clear();								
						}
						_selectedUnitsGroups[6]->insert(_selectedUnitsGroups[6]->end(), _selectedUnits->begin(), _selectedUnits->end());							
					}					
					else
					{
						if(_selectedUnitsGroups[6]->size() > 0)
						{
							this->sendSelectMessage(false);
							_selectedUnits->clear();
							_selectedUnits->insert(_selectedUnits->end(),_selectedUnitsGroups[6]->begin(),_selectedUnitsGroups[6]->end());
							activeUnit = _selectedUnits->begin();
							this->sendSelectMessage(true);
						}
					}
					break;
				}
			case GUI::Key::NUMBER7:
				{
					if((_lCtrlPushed || _rCtrlPushed) && _selectedUnits->size() > 0)
					{
						if(_selectedUnitsGroups[7]->size() > 0)
						{
							_selectedUnitsGroups[7]->clear();								
						}
						_selectedUnitsGroups[7]->insert(_selectedUnitsGroups[7]->end(), _selectedUnits->begin(), _selectedUnits->end());							
					}					
					else
					{
						if(_selectedUnitsGroups[7]->size() > 0)
						{
							this->sendSelectMessage(false);
							_selectedUnits->clear();
							_selectedUnits->insert(_selectedUnits->end(),_selectedUnitsGroups[7]->begin(),_selectedUnitsGroups[7]->end());
							activeUnit = _selectedUnits->begin();
							this->sendSelectMessage(true);
						}
					}
					break;
				}
			case GUI::Key::NUMBER8:
				{
					if((_lCtrlPushed || _rCtrlPushed) && _selectedUnits->size() > 0)
					{
						if(_selectedUnitsGroups[8]->size() > 0)
						{
							_selectedUnitsGroups[8]->clear();								
						}
						_selectedUnitsGroups[8]->insert(_selectedUnitsGroups[8]->end(), _selectedUnits->begin(), _selectedUnits->end());							
					}					
					else
					{
						if(_selectedUnitsGroups[8]->size() > 0)
						{
							this->sendSelectMessage(false);
							_selectedUnits->clear();
							_selectedUnits->insert(_selectedUnits->end(),_selectedUnitsGroups[8]->begin(),_selectedUnitsGroups[8]->end());
							activeUnit = _selectedUnits->begin();
							this->sendSelectMessage(true);
						}
					}
					break;
				}
			case GUI::Key::NUMBER9:
			{
				if((_lCtrlPushed || _rCtrlPushed) && _selectedUnits->size() > 0)
				{
					if(_selectedUnitsGroups[9]->size() > 0)
					{
						_selectedUnitsGroups[9]->clear();								
					}
					_selectedUnitsGroups[9]->insert(_selectedUnitsGroups[9]->end(), _selectedUnits->begin(), _selectedUnits->end());							
				}					
				else
				{
					if(_selectedUnitsGroups[9]->size() > 0)
					{
						this->sendSelectMessage(false);
						_selectedUnits->clear();
						_selectedUnits->insert(_selectedUnits->end(),_selectedUnitsGroups[9]->begin(),_selectedUnitsGroups[9]->end());
						activeUnit = _selectedUnits->begin();
						this->sendSelectMessage(true);
					}
				}
				break;
			}

			case GUI::Key::SPACE:
				{
					if (activeUnit != _selectedUnits->end())
					{
						auto m = std::make_shared<CameraPosition>();
						m->position = (*activeUnit)->getPosition();					
						_camera->emitMessageN(m);
					}
					auto m2 = std::make_shared<SkipText>();
					Logic::CServer::getSingletonPtr()->getMap()->getEntityByName("GameManager")->emitMessageN(m2);
					break;
				}
			case GUI::Key::TAB:
				if (activeUnit != _selectedUnits->end())
				{
					auto m = std::make_shared<AvatarActivation>();
					m->actived = false;
					(*activeUnit)->emitMessageN(m);

					auto m2 = std::make_shared<AvatarSelection>();
					m2->selected = true;
					(*activeUnit)->emitMessageN(m2);

					++activeUnit;
					if (activeUnit == _selectedUnits->end())
						activeUnit = _selectedUnits->begin();

					auto m3 = std::make_shared<AvatarActivation>();
					m3->actived = true;
					(*activeUnit)->emitMessageN(m3);
				}
				if(mouseAttachedEntity!=nullptr)
					dettachEntityFromMouse();
				break;

			case GUI::Key::UPARROW:
				{
					direction += Vector3(0, 0, -1);
				}
				break;
			case GUI::Key::DOWNARROW:
				{
					direction += Vector3(0, 0, 1);
				}
				break;
			case GUI::Key::LEFTARROW:
				{
					direction += Vector3(-1, 0, 0);
				}
				break;
			case GUI::Key::RIGHTARROW:
				{
					direction += Vector3(1, 0, 0);
				}
				break;
			case GUI::Key::ADD:
				{					
					auto message = std::make_shared<CameraScroll>();
					message->scroll = -12.0f;
					_camera->emitMessageN(message);
				}
				break;
			case GUI::Key::SUBTRACT:
				{
					auto message = std::make_shared<CameraScroll>();
					message->scroll = 12.0f;
					_camera->emitMessageN(message);
				}
				break;
			case GUI::Key::RBRACKET:
				{
					auto message = std::make_shared<CameraScroll>();
					message->scroll = -12.0f;
					_camera->emitMessageN(message);
				}
				break;
			case GUI::Key::SLASH:
				{
					auto message = std::make_shared<CameraScroll>();
					message->scroll = 12.0f;
					_camera->emitMessageN(message);
				}
				break;				
			#ifdef _DEBUG
			case GUI::Key::D:				
				{			
					auto m = std::make_shared<DamageMessage>();
					m->damage=30;
					for(std::list<Logic::CEntity*>::iterator it = _selectedUnits->begin();it != _selectedUnits->end(); it++)
					{
						(*it)->emitMessageN(m);
					}
				}
				break;
			#endif
		}

		if(_cameraVector != direction && !_mouseCamera)
		{
			auto mCC = std::make_shared<CameraControl>();
			mCC->direction = direction;
			_camera->emitMessageN(mCC);
			_cameraVector = direction;
			
			if(_cameraVector.x == 0 && _cameraVector.z == 0)
				_keysCamera = false;
			else
				_keysCamera = true;
		}

		return true;
	} // keyPressed

	//--------------------------------------------------------

	bool CPlayerController::keyReleased(TKey key){

		Vector3 direction = _cameraVector;
		switch(key.keyId)
		{
			case GUI::Key::LCONTROL:
				{
					_lCtrlPushed = false;
				}
				break;
			case GUI::Key::RCONTROL:
				{
					_rCtrlPushed = false;
				}
				break;
			case GUI::Key::UPARROW:
				{
					direction += Vector3(0, 0, 1);
				}
				break;

			case GUI::Key::DOWNARROW:
				{
					direction += Vector3(0, 0, -1);
				}
				break;
			case GUI::Key::LEFTARROW:
				{
					direction += Vector3(1, 0, 0);
				}
				break;
			case GUI::Key::RIGHTARROW:
				{
					direction += Vector3(-1, 0, 0);
				}
				break;
		}

		if(_cameraVector != direction && !_mouseCamera)
		{
			auto mCC = std::make_shared<CameraControl>();
			mCC->direction = direction;
			_camera->emitMessageN(mCC);
			_cameraVector = direction;

			if(_cameraVector.x == 0 && _cameraVector.z == 0)
				_keysCamera = false;
			else
				_keysCamera = true;
		}

		return false;

	} // keyReleased

	//--------------------------------------------------------
	
	bool CPlayerController::mouseMoved(const CMouseState &mouseState,const bool alreadyManaged)
	{
		checkRemovableWorld(mouseState);

		if (mouseAttachedEntity)
			mouseAttachedEntity->setPositionN(selectDestinationPosition(mouseState));		

		if(!_mouseLocked && !alreadyManaged && (startMousePosX>=0 && startMousePosY>=0) && _leftClickTypes["Ground"])
		{		
			//dibuja rectangulo			
			
			drawSelectionBox(mouseState.posRelX,mouseState.posRelY);
			_leftButtonDrag=true;		
		}

		if(_camera)
		{
			if(mouseState.scrool != 0)
			{
				auto message = std::make_shared<CameraScroll>();
				message->scroll = mouseState.scrool * -0.1f;
				_camera->emitMessageN(message);
			}


			Vector3 direction = Vector3::ZERO;

			if(mouseState.posAbsX <= CLOSE_TO_EDGE)
				direction += Vector3(-1,0,0);
			else if (mouseState.posAbsX >= _windowWidth - CLOSE_TO_EDGE)
				direction += Vector3(1,0,0);

			if(mouseState.posAbsY <= CLOSE_TO_EDGE)
				direction += Vector3(0,0,-1);
			else if (mouseState.posAbsY >= _windowHeight - CLOSE_TO_EDGE)
				direction += Vector3(0,0,1);

			if(_cameraVector != direction && !_keysCamera)
			{
				auto message = std::make_shared<CameraControl>();
				_cameraVector = direction;
				direction.y = 0;
				message->direction = direction;
				_camera->emitMessageN(message);
			}

			if(direction != Vector3::ZERO)
				_mouseCamera = true;
			else
				_mouseCamera = false;

			//std::cout<< "_mouseCamera: " << _mouseCamera <<std::endl;

			return true;
		}

		return false;

	} // mouseMoved

	//--------------------------------------------------------
		
	bool CPlayerController::mousePressed(const CMouseState &mouseState,const bool alreadyManaged)
	{
		if(!alreadyManaged && !_mouseLocked)
		{
			if(mouseState.button == TButton::LEFT && !_leftButtonDrag && mouseAttachedEntity == nullptr)		
			{						
				startMousePosX = mouseState.posRelX;
				startMousePosY = mouseState.posRelY;
				Ogre::Ray firstSelectionBoxRay = Graphics::CServer::getSingletonPtr()->getCameraToViewportRay(mouseState.posRelX, mouseState.posRelY);
				//in that case we want the point that intersect the world plane, we pass as parameter the PHYSIC_WORLD_FILTER array as std::vector that contains the right physic_group
				Logic::CEntity* getEntity= Physics::CServer::getSingletonPtr()->getPhysicPerceptionManager()->raycastAdvanced(firstSelectionBoxRay,MAX_RAY_DISTANCE,*_firstVertexSelectionBox,&std::vector<Physics::CollisionGroup> (PHYSIC_WORLD_FILTER, PHYSIC_WORLD_FILTER + sizeof(PHYSIC_WORLD_FILTER) / sizeof(PHYSIC_WORLD_FILTER[0])));
				_firstVertexSelectionBox->y += 0.1f;
				return true;
			}

			if (!_selectedUnits->empty() && mouseState.button == TButton::RIGHT)
			{
				if(mouseAttachedEntity!=nullptr)
				{
					dettachEntityFromMouse();					
				}
				else
				{
					CEntityPtr e=selectDestinationEntity(mouseState);
					if(e && _rightClickTypes[e->getType()])
					{
							auto m2 = std::make_shared<AvatarMoveToEntity>();
							m2->target = e;				
							for(std::list<Logic::CEntity*>::iterator it = _selectedUnits->begin();
								it != _selectedUnits->end(); it++)
								(*it)->emitMessageN(m2);
					}else if(_rightClickTypes["Ground"])
					{
						Vector3 targetPos = selectDestinationPosition(mouseState);
						if(_selectedUnits->size() > 1)
						{
							AI::CServer::getSingletonPtr()->getStrategyController()->moveGroup(_selectedUnits,targetPos);					
						}
						else
						{	
							auto m2 = std::make_shared<AvatarRouteTo>();
							m2->target = targetPos;
							m2->calculateNewRoute = true;
				
							for(std::list<Logic::CEntity*>::iterator it = _selectedUnits->begin();
								it != _selectedUnits->end(); it++)
									(*it)->emitMessageN(m2);							
						}
						auto m3 = std::make_shared<PointDestination>();
							m3->destination = targetPos;
							(*activeUnit)->getMap()->sendMessageToAll(m3);
					}
				}
				return true;
			}
			if (mouseState.button == TButton::LEFT && checkMouseAttachedEntityIsAbleForActivate())
			{
				if(AI::ResourceServer::getInstance()->checkResourceAvailability(mouseAttachedEntity->getType().c_str()))
				{
					AI::ResourceServer::getInstance()->useResource(mouseAttachedEntity->getType().c_str());
					auto instantiateMessage = std::make_shared<InstantiateAbilityObject>();
					instantiateMessage->position = mouseAttachedEntity->getPosition();
					(*activeUnit)->emitMessageN(instantiateMessage);
				}
			}
		}
		return false;
	} // mousePressed

	//--------------------------------------------------------

	bool CPlayerController::mouseReleased(const CMouseState &mouseState,const bool alreadyManaged)
	{
		if(!alreadyManaged && !_mouseLocked)
		{
			bool ret=false;
		
			if(mouseState.button == TButton::LEFT && _leftButtonDrag && mouseAttachedEntity == nullptr)
			{
				Vector2 endMousePos(mouseState.posRelX,mouseState.posRelY);
				float dist=endMousePos.distance(Vector2(startMousePosX,startMousePosY));
				if(dist>MIN_SCROLL_SELECTION)
				{
					performSelectionBox(mouseState.posRelX,mouseState.posRelY);
				}	
				_selectionRectangleNode->setVisible(false);
				_leftButtonDrag = false;
				ret= true;
			}else if(mouseState.button == TButton::LEFT && !_leftButtonDrag && mouseAttachedEntity == nullptr)
			{
				selectEntity(mouseState);	
				ret= true;
			}
			else if (mouseAttachedEntity != nullptr)
			{
				dettachEntityFromMouse();
			}
			startMousePosX=-1.0f;
			startMousePosY=-1.0f;
			_leftButtonDrag=false;
		}
		return false;
		
	} // mouseReleased

	//--------------------------------------------------------

	void CPlayerController::selectEntity(const CMouseState & mouseState){				
		
		bool selectAll = false;

		if(!_checkDoubleClick)
			_checkDoubleClick = true;
		else
		{
			if(_doubleClickMsecs < DOUBLE_CLICK_TIME)
			{
				_doubleClickMsecs = 0;
				_checkDoubleClick = false;
				selectAll = true;
			}
		}

		Vector3 point;
		
		//deselect old units
		this->sendSelectMessage(false);
		_selectedUnits->clear();

		 //Obtener el rayo a lanzar
		 //A esta función se le pasa la posición relativa (entre 0 y 1) del ratón en pantalla (es fácil conseguirla)
		Ogre::Ray mouseRay = Graphics::CServer::getSingletonPtr()->getCameraToViewportRay(mouseState.posRelX, mouseState.posRelY);
		std::list<Logic::CEntity*>* selectedEntities;
		//in that case we want the selectable entity that intersect with the raycast, we pass as parameter the PHYSIC_SELECTABLE_FILTER array as std::vector that contains the right physic_group
		Logic::CEntity* getEntity= Physics::CServer::getSingletonPtr()->getPhysicPerceptionManager()->raycastAdvanced(mouseRay,MAX_RAY_DISTANCE,point,
			&std::vector<Physics::CollisionGroup> (PHYSIC_SELECTABLE_AND_WORLD_FILTER, PHYSIC_SELECTABLE_AND_WORLD_FILTER + sizeof(PHYSIC_SELECTABLE_AND_WORLD_FILTER) / sizeof(PHYSIC_SELECTABLE_AND_WORLD_FILTER[0])));

		if(getEntity && getEntity->isPlayer() && _leftClickTypes[getEntity->getType()]) 
		{
			if(!selectAll)
			{
				_selectedUnits->push_back(getEntity);
				activeUnit = _selectedUnits->begin();
				this->sendSelectMessage(true);
			}
			else
			{
				selectUnitsOfType(getEntity->getType());
			}
		}
		else{

			selectedEntities = Physics::CServer::getSingletonPtr()->getPhysicPerceptionManager()->overlapQueries(point + Vector3(-SELECTION_UNIT_OVERLAP_RADIUS,0,-SELECTION_UNIT_OVERLAP_RADIUS),
			point + Vector3(SELECTION_UNIT_OVERLAP_RADIUS,0,-SELECTION_UNIT_OVERLAP_RADIUS),point + Vector3(SELECTION_UNIT_OVERLAP_RADIUS,0,SELECTION_UNIT_OVERLAP_RADIUS),point + Vector3(-SELECTION_UNIT_OVERLAP_RADIUS,0,SELECTION_UNIT_OVERLAP_RADIUS),
			OVERLAP_QUERIES_HEIGHT,&std::vector<Physics::CollisionGroup> (PHYSIC_SELECTABLE_FILTER, PHYSIC_SELECTABLE_FILTER + sizeof(PHYSIC_SELECTABLE_FILTER) / sizeof(PHYSIC_SELECTABLE_FILTER[0])));

			if(selectedEntities && !selectedEntities->empty())
			{				
				float minDistance = SELECTION_UNIT_OVERLAP_RADIUS;
				Logic::CEntity* closerEntity = (*(std::list<Logic::CEntity*>::iterator)selectedEntities->begin());

				for(Logic::CEntity* entity : (*selectedEntities))
				{
					if(entity->isPlayer())
					{
						if(point.distance(entity->getPosition()) < minDistance)
							closerEntity = entity;
					}
				}
				if(_leftClickTypes[closerEntity->getType()])
				{
					if(!selectAll)
					{
						_selectedUnits->push_back(closerEntity);
						activeUnit = _selectedUnits->begin();
						this->sendSelectMessage(true);
					}
					else
					{
						selectUnitsOfType(closerEntity->getType());
					}
				}
				
			}
			else
			{
				_selectedUnits->clear();
				activeUnit = _selectedUnits->end();
				this->sendSelectMessage(false);
			}

			if(selectedEntities != NULL)
				delete selectedEntities;
		}

	};//selectEntity

	void CPlayerController::selectEntity(Logic::CEntity* getEntity)
	{
			//deselect old units
		this->sendSelectMessage(false);
		_selectedUnits->clear();

		_selectedUnits->push_back(getEntity);
		activeUnit = _selectedUnits->begin();
		this->sendSelectMessage(true);
	}
	//--------------------------------------------------------

	Vector3 CPlayerController::selectDestinationPosition(const CMouseState & mouseState){		
		Vector3 point;
		
		 //Obtener el rayo a lanzar
		 //A esta función se le pasa la posición relativa (entre 0 y 1) del ratón en pantalla (es fácil conseguirla)
		Ogre::Ray mouseRay = Graphics::CServer::getSingletonPtr()->getCameraToViewportRay(mouseState.posRelX, mouseState.posRelY);
		//in that case we want the point that intersect the world plane, we pass as parameter the PHYSIC_WORLD_FILTER array as std::vector that contains the right physic_group
		Logic::CEntity* getEntity= Physics::CServer::getSingletonPtr()->getPhysicPerceptionManager()->raycastAdvanced(mouseRay,MAX_RAY_DISTANCE,point,&std::vector<Physics::CollisionGroup> (PHYSIC_WORLD_FILTER, PHYSIC_WORLD_FILTER + sizeof(PHYSIC_WORLD_FILTER) / sizeof(PHYSIC_WORLD_FILTER[0])));
		
		return point;

	};//selectDestination

	//--------------------------------------------------------

	Vector3 CPlayerController::getActualMouseWorldPosition()
	{		
		return selectDestinationPosition(GUI::CInputManager::getSingletonPtr()->getLogicMouseState());
	};

	//--------------------------------------------------------

	Logic::CEntity * CPlayerController::selectDestinationEntity(const CMouseState & mouseState){		
		
		Vector3 point;

		 //Obtener el rayo a lanzar
		 //A esta función se le pasa la posición relativa (entre 0 y 1) del ratón en pantalla (es fácil conseguirla)
		Ogre::Ray mouseRay = Graphics::CServer::getSingletonPtr()->getCameraToViewportRay(mouseState.posRelX, mouseState.posRelY);

		//in that case we want retrive an interactable entity, we pass as parameter the PHYSIC_DESTINATION_ENTITY_FILTER array as std::vector that contains the right physic_group
		Logic::CEntity* getEntity= Physics::CServer::getSingletonPtr()->getPhysicPerceptionManager()->raycastAdvanced(mouseRay,MAX_RAY_DISTANCE,point,
			&std::vector<Physics::CollisionGroup> (PHYSIC_DESTINATION_ENTITY_AND_WORLD_FILTER, PHYSIC_DESTINATION_ENTITY_AND_WORLD_FILTER + sizeof(PHYSIC_DESTINATION_ENTITY_AND_WORLD_FILTER) / sizeof(PHYSIC_DESTINATION_ENTITY_AND_WORLD_FILTER[0])));
		std::list<Logic::CEntity*>* selectedEntities;

		if(getEntity && getEntity->getType() != "Ground")
		{
			return getEntity;
		}
		else{

			selectedEntities = Physics::CServer::getSingletonPtr()->getPhysicPerceptionManager()->overlapQueries(point + Vector3(-SELECTION_UNIT_OVERLAP_RADIUS,0,-SELECTION_UNIT_OVERLAP_RADIUS),
			point + Vector3(SELECTION_UNIT_OVERLAP_RADIUS,0,-SELECTION_UNIT_OVERLAP_RADIUS),point + Vector3(SELECTION_UNIT_OVERLAP_RADIUS,0,SELECTION_UNIT_OVERLAP_RADIUS),point + Vector3(-SELECTION_UNIT_OVERLAP_RADIUS,0,SELECTION_UNIT_OVERLAP_RADIUS),
			OVERLAP_QUERIES_HEIGHT,&std::vector<Physics::CollisionGroup> (PHYSIC_DESTINATION_ENTITY_FILTER, PHYSIC_DESTINATION_ENTITY_FILTER + sizeof(PHYSIC_DESTINATION_ENTITY_FILTER) / sizeof(PHYSIC_DESTINATION_ENTITY_FILTER[0])));

			if(selectedEntities && !selectedEntities->empty())
			{				
				
				Logic::CEntity* closerEntity = (*(std::list<Logic::CEntity*>::iterator)selectedEntities->begin());
				float minDistance = point.distance(closerEntity->getPosition());

				for(Logic::CEntity* entity : (*selectedEntities))
				{
					if(entity->isPlayer())
					{
						if(point.distance(entity->getPosition()) < minDistance)
						{
							closerEntity = entity;
							minDistance = point.distance(entity->getPosition());
						}
					}
				}
				
				getEntity = closerEntity;
			}
			else
				getEntity = NULL;

			if(selectedEntities != NULL)
				delete selectedEntities;
			
		}
		
		return getEntity;

		};//selectDestination

	//--------------------------------------------------------

	void CPlayerController::performSelectionBox(float endMousePosX,float endMousePosY)
	{

		Vector3 thirdVertexSelectionBox;

		Ogre::Ray mouseRay = Graphics::CServer::getSingletonPtr()->getCameraToViewportRay(endMousePosX, endMousePosY);
		//in that case we want the point that intersect the world plane, we pass as parameter the PHYSIC_WORLD_FILTER array as std::vector that contains the right physic_group
		Logic::CEntity* getEntity= Physics::CServer::getSingletonPtr()->getPhysicPerceptionManager()->raycastAdvanced(mouseRay,MAX_RAY_DISTANCE,thirdVertexSelectionBox,&std::vector<Physics::CollisionGroup> (PHYSIC_WORLD_FILTER, PHYSIC_WORLD_FILTER + sizeof(PHYSIC_WORLD_FILTER) / sizeof(PHYSIC_WORLD_FILTER[0])));		

		Vector3 secondVertexSelectionBox;				
		Vector3 fourthVertexSelectionBox;

		secondVertexSelectionBox = Vector3(thirdVertexSelectionBox.x,thirdVertexSelectionBox.y ,(*_firstVertexSelectionBox).z);
		fourthVertexSelectionBox = Vector3((*_firstVertexSelectionBox).x,thirdVertexSelectionBox.y ,thirdVertexSelectionBox.z);
		
		//in that case we want retrive all the selectable entities included in the piramid formed
		//from the camera view point and the drawn rectangle int the world plane,
		//we pass as parameter the PHYSIC_SELECTABLE_FILTER array as std::vector that contains the right physic_group
		std::list<Logic::CEntity*>* selectedEntities = Physics::CServer::getSingletonPtr()->getPhysicPerceptionManager()->overlapQueries(((*_firstVertexSelectionBox)),
			secondVertexSelectionBox,(thirdVertexSelectionBox),fourthVertexSelectionBox,OVERLAP_QUERIES_HEIGHT,&std::vector<Physics::CollisionGroup> (PHYSIC_SELECTABLE_FILTER, PHYSIC_SELECTABLE_FILTER + sizeof(PHYSIC_SELECTABLE_FILTER) / sizeof(PHYSIC_SELECTABLE_FILTER[0])));

		this->sendSelectMessage(false);
		_selectedUnits->clear();

		if(!selectedEntities->empty())
		{

			for(std::list<Logic::CEntity*>::iterator it = selectedEntities->begin();
				it != selectedEntities->end(); it++)
			{
				if((*it)->isPlayer() && _leftClickTypes[(*it)->getType()])
				{
					_selectedUnits->push_back((*it));
					//Nos aseguramos de que si seleccionamos un grupo en el que la princesa está incluída, sea la unidad activa automáticamente

				}				

				if(_selectedUnits->size() == SELECTION_MAX_UNITS)
					break;
			}

				activeUnit = _selectedUnits->begin();
			this->sendSelectMessage(true);
			
		}
		else activeUnit = _selectedUnits->end();

		delete selectedEntities;

	}//performBoxSelection

	//--------------------------------------------------------

	void CPlayerController::drawSelectionBox(float endMousePosX,float endMousePosY)
	{
		_selectionRectangleNode->setVisible(true);
		Vector3 thirdVertexSelectionBox;

		Ogre::Ray mouseRay = Graphics::CServer::getSingletonPtr()->getCameraToViewportRay(endMousePosX, endMousePosY);
			//in that case we want the point that intersect the world plane, we pass as parameter the PHYSIC_WORLD_FILTER array as std::vector that contains the right physic_group
		Logic::CEntity* getEntity= Physics::CServer::getSingletonPtr()->getPhysicPerceptionManager()->raycastAdvanced(mouseRay,MAX_RAY_DISTANCE,thirdVertexSelectionBox,&std::vector<Physics::CollisionGroup> (PHYSIC_WORLD_FILTER, PHYSIC_WORLD_FILTER + sizeof(PHYSIC_WORLD_FILTER) / sizeof(PHYSIC_WORLD_FILTER[0])));
		
		thirdVertexSelectionBox.y += 0.2f;
		_firstVertexSelectionBox->y = thirdVertexSelectionBox.y;

		Vector3 secondVertexSelectionBox;				
		Vector3 fourthVertexSelectionBox;
		
		secondVertexSelectionBox = Vector3(thirdVertexSelectionBox.x,thirdVertexSelectionBox.y,_firstVertexSelectionBox->z);
		fourthVertexSelectionBox = Vector3(_firstVertexSelectionBox->x,thirdVertexSelectionBox.y,thirdVertexSelectionBox.z);

		Logic::CServer::getSingletonPtr()->getMap()->getScene()->getDrawManager()->drawRectangle(*_firstVertexSelectionBox,secondVertexSelectionBox,
			thirdVertexSelectionBox,fourthVertexSelectionBox,_selectionRectangleNode,'w',"SelectionRectangle",true);

	}

	//--------------------------------------------------------

	void CPlayerController::sendSelectMessage(bool isSelect)
	{
		if(isSelect)
			GUI::CServer::getSingletonPtr()->getSelectedUnitsController()->activateElements();
		else
			GUI::CServer::getSingletonPtr()->getSelectedUnitsController()->deactivateElements();

		if(!_selectedUnits->empty()){

			//JLC: Nuevo sistema de mensajes
			auto m = std::make_shared<AvatarSelection>();
			m->selected = isSelect;

			//std::string txt=((isSelect)?"select":"deselect");
			//txt+=" entities = ";



			for(std::list<Logic::CEntity*>::iterator it = _selectedUnits->begin();
				it != _selectedUnits->end(); it++)
			{
				if(it == activeUnit)
				{
					auto m2 = std::make_shared<AvatarActivation>();
					m2->actived = isSelect;
					(*activeUnit)->emitMessageN(m2);
				}
				else
//				if((*it)->isPlayer()){
					(*it)->emitMessageN(m);
					//txt+=" "+(*it)->getName();
//				}
			}
			//std::cout<<txt<<std::endl;
		}


	}

	//--------------------------------------------------------

	void CPlayerController::attachEntityToMouse(Logic::CEntity* entity)
	{
		if(mouseAttachedEntity!=nullptr)
			dettachEntityFromMouse();
		mouseAttachedEntity = entity;
	}

	//--------------------------------------------------------

	void CPlayerController::dettachEntityFromMouse()
	{
		mouseAttachedEntity->setPositionN(Vector3(0,-100,0));
		Logic::CEntityFactory::getSingletonPtr()->deferredDeleteEntity(mouseAttachedEntity);
		mouseAttachedEntity = nullptr;
	}

	//--------------------------------------------------------

	void CPlayerController::removeEntityFromSelectedUnits(Logic::CEntity* entity)
	{
		_selectedUnits->remove(entity);
		
		auto m2 = std::make_shared<AvatarSelection>();
		m2->selected = false;
		entity->emitMessageN(m2);

		//std::cout << "Quitando entidad: " << entity->getName() << std::endl;

		if(_selectedUnits->size() < 1)
		{
			GUI::CServer::getSingletonPtr()->getSelectedUnitsController()->deactivateElements();
			GUI::CServer::getSingletonPtr()->getActiveUnitController()->deactivateElements();

			activeUnit = _selectedUnits->end();
		}
		else
		{
			if(entity == GUI::CServer::getSingletonPtr()->getActiveUnitController()->getActiveUnit())
			{
				auto m3 = std::make_shared<AvatarActivation>();
				m3->actived = false;
				entity->emitMessageN(m3);
				
				std::list<Logic::CEntity*>::iterator findIter = std::find(_selectedUnits->begin(), _selectedUnits->end(), _selectedUnits->front());

				activeUnit = findIter;

				auto m4 = std::make_shared<AvatarActivation>();
				m4->actived = true;
				(*activeUnit)->emitMessageN(m4);

				/*
				GUI::CServer::getSingletonPtr()->getActiveUnitController()->setActiveUnit(_selectedUnits->front());
				*/

				GUI::CServer::getSingletonPtr()->getActiveUnitController()->deactivateElements();
				GUI::CServer::getSingletonPtr()->getActiveUnitController()->activateElements();

				/*
				++activeUnit;
				if (activeUnit == _selectedUnits->end())
					activeUnit = _selectedUnits->begin();

				auto m4 = std::make_shared<AvatarActivation>();
				m4->actived = true;
				(*activeUnit)->emitMessageN(m4);
				*/

				//std::cout << "Cambiando entidad activa de: " << entity->getName() << " a: " << (*activeUnit)->getName() << std::endl;
			}
			
			GUI::CServer::getSingletonPtr()->getSelectedUnitsController()->deactivateElements();
			GUI::CServer::getSingletonPtr()->getSelectedUnitsController()->activateElements();
		}
	}

	//--------------------------------------------------------

	bool CPlayerController::checkMouseAttachedEntityIsAbleForActivate()
	{
		if (!mouseAttachedEntity)
			return false;
		Logic::CSetPhysicEntityHelperComponent* component = 
			(Logic::CSetPhysicEntityHelperComponent*) mouseAttachedEntity->getComponent("CSetPhysicEntityHelperComponent");
		return component->isPhysicEntityOnCorrectPosition();
	}

	//--------------------------------------------------------

	void CPlayerController::changeActiveUnit(Logic::CEntity* newActiveUnit)
	{
		if(_inputManager->isModifierPressed(Modifier::CTRL))
		{
			removeEntityFromSelectedUnits(newActiveUnit);
		}
		else if (activeUnit != _selectedUnits->end())
		{
			auto m = std::make_shared<AvatarActivation>();
			m->actived = false;
			(*activeUnit)->emitMessageN(m);

			auto m2 = std::make_shared<AvatarSelection>();
			m2->selected = true;
			(*activeUnit)->emitMessageN(m2);

			std::list<Logic::CEntity*>::iterator findIter = std::find(_selectedUnits->begin(), _selectedUnits->end(), newActiveUnit);

			activeUnit = findIter;

			auto m3 = std::make_shared<AvatarActivation>();
			m3->actived = true;
			(*activeUnit)->emitMessageN(m3);
		}
		if(mouseAttachedEntity!=nullptr)
				dettachEntityFromMouse();
		
	}

	//--------------------------------------------------------

	void CPlayerController::initClickDictionaries()
	{
		_leftClickTypes["Ground"] = true;
		_leftClickTypes["Princess"] = true;
		_leftClickTypes["WarriorGoblin"] = true;
		_leftClickTypes["RangerGoblin"] = true;
		_leftClickTypes["WorkmanGoblin"] = true;
		_leftClickTypes["Well"] = true;		

		_rightClickTypes["Ground"] = true;
		_rightClickTypes["Tree"] = true;
		_rightClickTypes["Rock"] = true;
		_rightClickTypes["Iron"] = true;
		_rightClickTypes["ThiefEnemy"] = true;
		_rightClickTypes["Knight"] = true;
		_rightClickTypes["Assassin"] = true;
		_rightClickTypes["Chest"] = true;
		_rightClickTypes["Well"] = true;
		_rightClickTypes["Pot"] = true;
	}

	//--------------------------------------------------------

	void CPlayerController::setTypeIsClickable(int button, std::string type, bool clickable)
	{
		if(button == 1)
		{
			if(_leftClickTypes.find(type) != _leftClickTypes.end())
				_leftClickTypes[type] = clickable;
		}
		else if(button == 2)
		{
			if(_rightClickTypes.find(type) != _rightClickTypes.end())
				_rightClickTypes[type] = clickable;
		}
	}

	//--------------------------------------------------------

	void CPlayerController::unselectUnits()
	{
		
		this->sendSelectMessage(false);		
		_selectedUnits->clear();
		activeUnit = _selectedUnits->end();
	}

	//--------------------------------------------------------

	Logic::CEntity* CPlayerController::getEntityUnderMouse(const CMouseState &mouseState, bool checkRemovableWorld)
	{
		Ogre::Ray mouseRay = Graphics::CServer::getSingletonPtr()->getCameraToViewportRay(mouseState.posRelX, mouseState.posRelY);

		Vector3 point;
		Logic::CEntity* getEntity;
		if(!checkRemovableWorld)
		{
			getEntity = Physics::CServer::getSingletonPtr()->getPhysicPerceptionManager()->raycastAdvanced(mouseRay,MAX_RAY_DISTANCE,point,
				&std::vector<Physics::CollisionGroup> (PHYSIC_DESTINATION_ENTITY_FILTER, PHYSIC_DESTINATION_ENTITY_FILTER + sizeof(PHYSIC_DESTINATION_ENTITY_FILTER) / sizeof(PHYSIC_DESTINATION_ENTITY_FILTER[0])));
		}
		else
		{
			getEntity = Physics::CServer::getSingletonPtr()->getPhysicPerceptionManager()->raycastAdvanced(mouseRay,MAX_RAY_DISTANCE,point,
				&std::vector<Physics::CollisionGroup> (PHYSIC_REMOVABLE_WORLD_FILTER, PHYSIC_REMOVABLE_WORLD_FILTER + sizeof(PHYSIC_REMOVABLE_WORLD_FILTER) / sizeof(PHYSIC_REMOVABLE_WORLD_FILTER[0])));
		}


		return getEntity ? getEntity : NULL;

	}
	
	//--------------------------------------------------------

	void CPlayerController::checkRemovableWorld(const CMouseState &mouseState)
	{		
		Logic::CEntity* getEntity= GUI::CServer::getSingletonPtr()->getPlayerController()->getEntityUnderMouse(mouseState,true);

		if(getEntity && !_activeTransparency)
		{
			if (!((Logic::CRemovableWorldComponent*)getEntity->getComponent("CRemovableWorldComponent"))->getActivateTransparancy())
			{
				_activeTransparency = true;
				std::vector<std::string> _materials = ((Logic::CRemovableWorldComponent*)getEntity->getComponent("CRemovableWorldComponent"))->getMaterials();
				
				((Logic::CGraphics*)getEntity->getComponent("CGraphics"))->setMaterialTransparency(_materials,0,0,0,0.2f,true,250);
				
				_transparentEntity = getEntity;			
			}
		}
		else if(!getEntity && _activeTransparency)
		{
			if (_transparentEntity && !((Logic::CRemovableWorldComponent*)_transparentEntity->getComponent("CRemovableWorldComponent"))->getActivateTransparancy())
			{
				_activeTransparency = false;
				std::vector<std::string> _materials = ((Logic::CRemovableWorldComponent*)_transparentEntity->getComponent("CRemovableWorldComponent"))->getMaterials();
		
				((Logic::CGraphics*)_transparentEntity->getComponent("CGraphics"))->setMaterialTransparency(_materials,0,0,0,1.0f,true,250);

				_transparentEntity = NULL;
			}
		}
	}

	//--------------------------------------------------------

	void CPlayerController::selectUnitsOfType(std::string type)
	{		
		for(std::set<Logic::CEntity*>::const_iterator itAllies = AI::CServer::getSingletonPtr()->getEntityManager()->getAlliesList()->begin();
			itAllies != AI::CServer::getSingletonPtr()->getEntityManager()->getAlliesList()->end() ; ++itAllies)
		{	
			if((*itAllies)->getType() == type &&((Logic::CCamera*)_camera->getComponent("CCamera"))->isEntityOnCamera((*itAllies)->getPosition()))
			{
				_selectedUnits->push_back((*itAllies));
			}

			if(_selectedUnits->size() == SELECTION_MAX_UNITS)
				break;
		}	

		activeUnit = _selectedUnits->begin();
		this->sendSelectMessage(true);
	}

} // namespace GUI




	
