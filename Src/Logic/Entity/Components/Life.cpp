/**
@file Life.cpp

Contiene la implementación del componente que controla la vida de una entidad.
 
@see Logic::CLife
@see Logic::IComponent

@author David Llansó
@date Octubre, 2010
*/

#include "Life.h"

#include "Logic/UnitMessages.h"
#include "Logic/CombatMessages.h"
#include "Logic/EntityMessages.h"
#include "Logic/GraphicsMessages.h"
#include "Logic/AllyUnitMessages.h"
#include "GUI\EventsController.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Logic/Maps/EntityFactory.h"

#include "Map/MapEntity.h"

#include "GUI/Server.h"
#include "GUI/PlayerController.h"
#include "GUI/MinimapController.h"

#include "Graphics/BillboardSetEntity.h"
#include "Graphics/Scene.h"

#define LIFEBAR_LENGHT 5
#define BILLBOARD_START_X (-(LIFEBAR_LENGHT/2))
#define LIFEBAR_HEIGHT 1
#define LIFEBAR_DECREASE_UNIT 0.5
#define LIFEBAR_DECREASE_FRAMERATE 25

namespace Logic 
{

	IMP_FACTORY(CLife);

	CLife::~CLife() 
	{
		deactivate();
		if(_lifebarBg)
		{
//			deactivate();
			delete _lifebarBg;
			_lifebarBg = 0;
		}
		if(_lifebarFg)
		{
//			deactivate();
			delete _lifebarFg;
			_lifebarFg = 0;
		}

	} // ~CGraphics
	
	//---------------------------------------------------------

	bool CLife::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		if(entityInfo->hasAttribute("life"))
			_maxLife = entityInfo->getFloatAttribute("life");
		if(entityInfo->hasAttribute("lifebar_position"))
			_lifebarPosition = entityInfo->getVector3Attribute("lifebar_position");
		if(entityInfo->hasAttribute("lifebar_color_r"))
			_lifebarColorR = entityInfo->getFloatAttribute("lifebar_color_r");
		if(entityInfo->hasAttribute("lifebar_color_g"))
			_lifebarColorG = entityInfo->getFloatAttribute("lifebar_color_g");
		if(entityInfo->hasAttribute("lifebar_color_b"))
			_lifebarColorB = entityInfo->getFloatAttribute("lifebar_color_b");

		_currentLife=_maxLife;
		_visualLife = _maxLife;

		//create two billboard set with one billboard 
		_lifebarBg = new Graphics::CBillboardSetEntity(entity->getName()+"BG",Graphics::BillboardType::BBT_POINT, 1);
		_lifebarFg = new Graphics::CBillboardSetEntity(entity->getName()+"FG",Graphics::BillboardType::BBT_POINT,1);

		_scene = _entity->getMap()->getScene();

		return true;

	} // spawn
	void CLife::awake()
	{
		_isInvincible=false;
		_currentLife = _maxLife;
		_visualLife = _maxLife;
		_animationTime=0;
		this->updateLifebarLife();
		this->updateLifebarDamage();

	}

	bool CLife::activate()
	{
		
		if(!_scene->addEntity(_lifebarBg))
			return 0;
		_lifebarBg->setMaterial("LifebarBg");
		_lifebarBg->setBilboardSetOrigin(Graphics::BillboardSetOrigin::BBO_TOP_LEFT);
		_lifebarBg->addBillboard(Vector3(BILLBOARD_START_X,-(LIFEBAR_HEIGHT/2),0.f),Vector2(LIFEBAR_LENGHT,LIFEBAR_HEIGHT),1.0f,1.0f,1.0f);

		if(!_scene->addEntity(_lifebarFg))
			return 0;
		_lifebarFg->setMaterial("LifebarFg");
		_lifebarFg->setBilboardSetOrigin(Graphics::BillboardSetOrigin::BBO_TOP_LEFT);
		_lifebarFg->addBillboard(Vector3(BILLBOARD_START_X,-(LIFEBAR_HEIGHT/2),0.01f),Vector2(LIFEBAR_LENGHT,LIFEBAR_HEIGHT),_lifebarColorR,_lifebarColorG,_lifebarColorB);
		_lifebarFg->addBillboard(Vector3(BILLBOARD_START_X,-(LIFEBAR_HEIGHT/2),0.01f),Vector2(LIFEBAR_LENGHT,LIFEBAR_HEIGHT),1.0f,1.0f,0.0f);
		this->updateLifebarLife();
		this->updateLifebarDamage();

		return true;
	}

	void CLife::deactivate()
	{
		_scene->removeEntity(_lifebarBg);
		_scene->removeEntity(_lifebarFg);
		
	}

	//---------------------------------------------------------

	bool CLife::acceptN(const std::shared_ptr<NMessage> &message)
	{
		return ((!_isInvincible && (message->type.compare("DamageMessage") == 0)) || (message->type.compare("HealUnit") == 0)
			|| (message->type.compare("SetInvicible") == 0) || message->type == "SetBerserkerMode");
	}

	//---------------------------------------------------------

	void CLife::processN(const std::shared_ptr<NMessage> &message)
	{
		if (message->type.compare("DamageMessage") == 0)
		{

			if(_entity->isPlayer())
			{
				if(_entity->getType() == "Princess")
					GUI::CServer::getSingletonPtr()->getEventscontroller()->princessDamaged(_entity);
				else
					GUI::CServer::getSingletonPtr()->getEventscontroller()->underAttack(_entity);
			}
			//to avoid that recive 2 message into same tick and send 2 times the entityDestroying message 
			if(_currentLife >0)
			{
				auto damageMessage = std::static_pointer_cast<DamageMessage>(message);
				_currentLife -= damageMessage->damage;

				if (_currentLife <= 0 && !_berserkerMode)
				{
					_currentLife = 0;
					_visualLife = _currentLife;

					auto dieMessage = std::make_shared<EntityDying>();
					dieMessage->entity = _entity;
					_entity->emitMessageN(dieMessage, this);

					if(_entity->getType() != "Princess")
					{

						auto destroyMessage = std::make_shared<EntityDestroying>();
						destroyMessage->entity = _entity;
						_entity->getMap()->sendMessageToAll(destroyMessage);

						if (_entity->isPlayer())
							GUI::CServer::getSingletonPtr()->getPlayerController()->removeEntityFromSelectedUnits(_entity);

					}
					else
						_entity->getMap()->sendMessageToAll(std::make_shared<PrincessHasFallen>());

				}
				else if (_currentLife <= 0)
					_currentLife = 1;

				this->updateLifebarLife();
				this->updateLifebarDamage();
			}
		}
		else if (message->type.compare("HealUnit") == 0)
		{
			auto healingMessage = std::static_pointer_cast<HealUnit>(message);
			_currentLife += healingMessage->healing;

			if(_currentLife > _maxLife)
				_currentLife = _maxLife;

			_visualLife = _currentLife;
			this->updateLifebarLife();
			this->updateLifebarDamage();


		}
		else if (message->type.compare("SetInvicible") == 0)
		{
			auto invincibleMessage = std::static_pointer_cast<SetInvicible>(message);
			_isInvincible=invincibleMessage->isInvicible;
		}
		else if (message->type == "SetBerserkerMode")
			_berserkerMode = std::static_pointer_cast<SetBerserkerMode>(message)->active;
	}

	//---------------------------------------------------------

	void CLife::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);
		_animationTime+=msecs;
		//if there is a gap between real life value and visual life value and
		//the time from last animation tick is greather then the time of the animation speed 
		if(_visualLife>_currentLife && _animationTime>=LIFEBAR_DECREASE_FRAMERATE)
		{
			_visualLife-=LIFEBAR_DECREASE_UNIT;
			this->updateLifebarDamage();
			_animationTime=0;
		}
		//update the position
		Vector3 pos=_entity->getPosition() + _lifebarPosition;
		_lifebarBg->setPosition(pos);
		pos.z+=0.01f;
		_lifebarFg->setPosition(pos);
	}
		

	//---------------------------------------------------------

	void CLife::updateLifebarDamage()
	{
		float startX = (_currentLife/_maxLife)*LIFEBAR_LENGHT;
		float len = ((_visualLife-_currentLife)/_maxLife)*LIFEBAR_LENGHT;

		_lifebarFg->setBilboardPosition(1,Vector3(BILLBOARD_START_X+startX,-(LIFEBAR_HEIGHT/2),0.01f));
			//set the lenght of foreground billboard dependent on visual life value
		_lifebarFg->setBilboardDimension(1,len,LIFEBAR_HEIGHT);
		_lifebarFg->setBilboardTexCoordRect(1,0.f,0.f,(len/LIFEBAR_LENGHT),1.f);
	}

	//---------------------------------------------------------

	void CLife::updateLifebarLife()
	{
		_lifebarFg->setBilboardDimension(0,(_currentLife/_maxLife)*LIFEBAR_LENGHT,LIFEBAR_HEIGHT);
		_lifebarFg->setBilboardTexCoordRect(0,0.f,0.f,(_currentLife/_maxLife),1.f);
		//set the color of foreground billboard dependent on visual life value
//		_lifebarFg->setBilboardColor(0,1-(_currentLife/_maxLife),(_currentLife/_maxLife),0.0f);

	}

	//---------------------------------------------------------

	void CLife::setMaxLife(float newMaxLife)
	{
		float prevMaxLife = _maxLife;
		_maxLife = newMaxLife;
		if(newMaxLife > prevMaxLife)
		{
			float currentLifeProportion = _currentLife / prevMaxLife;
			_currentLife = 	_maxLife * currentLifeProportion;
			this->updateLifebarLife();
			this->updateLifebarDamage();
		}
				
		
	}

	//---------------------------------------------------------

	void CLife::setLifebarVisible(bool isVisible)
	{
		_lifebarBg->setVisible(isVisible);
		_lifebarFg->setVisible(isVisible);
	}

	void CLife::setLife(float life)
	{
		_currentLife = life;
		_visualLife = life;
		updateLifebarLife();
		updateLifebarDamage();
	}

} // namespace Logic

