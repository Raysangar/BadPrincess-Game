/**
@file Graphics.cpp

Contiene la implementación del componente que controla la representación
gráfica de la entidad.
 
@see Logic::CGraphics
@see Logic::IComponent

@author David Llansó
@date Agosto, 2010
*/

#include "Graphics.h"

#include "Logic/MovementMessages.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include "Graphics/Scene.h"
#include "Graphics/MeshEntity.h"
#include "Graphics/StaticEntity.h"

namespace Logic 
{
	IMP_FACTORY(CGraphics);
	
	//---------------------------------------------------------

	CGraphics::~CGraphics() 
	{
		if(_graphicsEntity)
		{
			deactivate();
			delete _graphicsEntity;
			_graphicsEntity = 0;
		}

	} // ~CGraphics
	
	//---------------------------------------------------------

	bool CGraphics::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;
		
		_scene = _entity->getMap()->getScene();

		if(entityInfo->hasAttribute("model"))
			_model = entityInfo->getStringAttribute("model");
		_currentModel = _model;
		_graphicsEntity = createGraphicsEntity(entityInfo);
		if(!_graphicsEntity)
			return false;

		if (entityInfo->hasAttribute("scale"))
			_scale = entityInfo->getFloatAttribute("scale");
		_currentScale = _scale;

		if (_entity->isGenerator())
			return activateGraphics();
		return true;

	} // spawn
	
	void CGraphics::awake()
	{
		if(_currentModel.compare(_model)!=0)
		{
			//std::cout<<"ATTENTION : graphics change model on awake";
			changeModel(_model,_scale);
		}
	}

	//---------------------------------------------------------

	bool CGraphics::activate()
	{
		if(!_isActive)
			if (!activateGraphics())
				return false;

		return true;
		
	}

	//---------------------------------------------------------

	bool CGraphics::activateGraphics()
	{

		_isActive = true;
		if (isStatic)
		{
			if(!_scene->addStaticEntity((Graphics::CStaticEntity*)_graphicsEntity))
				return 0;
		}
		else
		{
			if(!_scene->addEntity(_graphicsEntity))
				return 0;
		}
		setGraphicsEntityAttributes();

		if (!_entity->isPlayer() && !_entity->isEnemy() && (_entity->getType() != "Chest") && (_entity->getType() != "Well") && !_entity->isGenerator())
			_graphicsEntity->registerSubEntitiesOnFogOfWarController();

		return true;
		
	}

	//---------------------------------------------------------

	void CGraphics::deactivate()
	{
		_isActive = false;
		if (!_entity->isPlayer() && !_entity->isEnemy() && (_entity->getType() != "Chest") && (_entity->getType() != "Well"))
			_graphicsEntity->unregisterSubEntitiesOnFogOfWarController();

		if(isStatic)
			_scene->removeStaticEntity(static_cast<Graphics::CStaticEntity*>(_graphicsEntity));
		else
			_scene->removeEntity(_graphicsEntity);
	}

	//---------------------------------------------------------

	void CGraphics::pause()
	{
		
	}

	//---------------------------------------------------------

	void CGraphics::restart()
	{
		
		if (_entity->getType() == "Ground")
		{
			
			deactivate();
			delete _graphicsEntity;
			_graphicsEntity = new Graphics::CStaticEntity("Ground",_currentModel);
			activate();
			
		}
		
	}

	//---------------------------------------------------------

	void CGraphics::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);

		if(_gradualTransparency)
		{
			if(_maxGradualTransparency == 1.0f)
			{
				_currentTransparency += _transparencyPerMsec * msecs;
				if(_currentTransparency >= 1.0f)
				{
					_currentTransparency = 1.0f;
					_gradualTransparency = false;
				}
				if(_materialNames.size() > 0)
					_graphicsEntity->setMaterialTransparency(_materialNames, _technique, _pass, _textureUnit, _currentTransparency);
				else
					_graphicsEntity->setMaterialTransparency(_materialName, _technique, _pass, _textureUnit, _currentTransparency);
			}
			else
			{
				_currentTransparency -= _transparencyPerMsec * msecs;
				if(_currentTransparency <= _maxGradualTransparency)
				{
					_currentTransparency = _maxGradualTransparency;
					_gradualTransparency = false;
				}
				if(_materialNames.size() > 0)
					_graphicsEntity->setMaterialTransparency(_materialNames, _technique, _pass, _textureUnit, _currentTransparency);
				else
					_graphicsEntity->setMaterialTransparency(_materialName, _technique, _pass, _textureUnit, _currentTransparency);
			}
		}
	}


	//---------------------------------------------------------

	Graphics::CMeshEntity* CGraphics::createGraphicsEntity(const Map::CEntity *entityInfo)
	{		
		if(entityInfo->hasAttribute("isStatic"))
			isStatic = entityInfo->getBoolAttribute("isStatic");

		if(isStatic)
			_graphicsEntity = new Graphics::CStaticEntity(_entity->getName(),_currentModel);
		else
			_graphicsEntity = new Graphics::CMeshEntity(_entity->getName(),_currentModel);

		return _graphicsEntity;

	} // createGraphicsEntity
	
	//---------------------------------------------------------

	void CGraphics::setGraphicsEntityAttributes()
	{
		_graphicsEntity->setScale(_currentScale);
		_graphicsEntity->setTransform(_entity->getTransform());
	}

	//---------------------------------------------------------
	
	bool CGraphics::acceptN(const std::shared_ptr<NMessage> &message)
	{
		return (message->type.compare("TransformMessage") == 0);
	} // acceptN
	
	
	//---------------------------------------------------------

	void CGraphics::processN(const std::shared_ptr<NMessage> &message)
	{
		if (message->type.compare("TransformMessage") == 0)
		{
			std::shared_ptr<TransformMessage> m = std::static_pointer_cast<TransformMessage>(message);
			_graphicsEntity->setTransform(m->transform);
		}
	} // processN
	
	void CGraphics::setMaterialTransparency(std::string name,int tecnique,int pass,int unitState,float tranparency,bool gradual, int gradualMsecs)
	{
		if(!_gradualTransparency && gradual && tranparency != _currentTransparency)
		{
			_materialName = name;
			_maxGradualTransparency = tranparency;
			_transparencyPerMsec = abs(_maxGradualTransparency - _currentTransparency) / gradualMsecs;
			_gradualTransparency = true;			
			_technique = tecnique;
			_pass = pass;
			_textureUnit = unitState;			
		}
		else if(!gradual)
		{
			_graphicsEntity->setMaterialTransparency(name, tecnique, pass, unitState, tranparency);
			_currentTransparency = tranparency;
		}
	}

	void CGraphics::setMaterialTransparency(std::vector<std::string> name,int tecnique,int pass,int unitState,float tranparency,bool gradual, int gradualMsecs)
	{
		if(!_gradualTransparency && gradual && tranparency != _currentTransparency)
		{
			_materialNames = name;
			_maxGradualTransparency = tranparency;
			_transparencyPerMsec = abs(_maxGradualTransparency - _currentTransparency) / gradualMsecs;
			_gradualTransparency = true;			
			_technique = tecnique;
			_pass = pass;
			_textureUnit = unitState;			
		}
		else if(!gradual)
		{
			_graphicsEntity->setMaterialTransparency(_materialNames, tecnique, pass, unitState, tranparency);
			_currentTransparency = tranparency;
		}
	}


	void CGraphics::changeModel(std::string newName,float newScale)
	{
		if(isStatic)
		{
			std::cerr<<"can't change the model "<<_model<<" to "<<newName<<" because it is static"<<std::endl;
			return;
		}
		deactivate();
		delete _graphicsEntity;
		_currentModel=newName;
		_currentScale = newScale;

		_graphicsEntity = new Graphics::CMeshEntity(_entity->getName(),_currentModel);
		activate();

	}
	void CGraphics::setVisible(bool isVisible){
		if(_graphicsEntity!=nullptr)
			_graphicsEntity->setVisible(isVisible);
	}
} // namespace Logic

