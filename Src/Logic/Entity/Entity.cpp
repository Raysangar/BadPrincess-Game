/**
@file Entity.cpp

Contiene la implementación de la clase Entity, que representa una entidad
de juego. Es una colección de componentes.

@see Logic::CEntity
@see Logic::IComponent

@author David Llansó
@date Julio, 2010
*/

#include "Entity.h"

// Componentes
#include "Component.h"

#include "Logic/MovementMessages.h"
#include "Logic/Server.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include "GUI/Server.h"
#include "GUI/PlayerController.h"

#include "Logic/Entity/Components/Discover.h"

#include "AI/Server.h"
#include "AI/EntityManager.h"



namespace Logic 
{
	CEntity::CEntity(TEntityID entityID) : _entityID(entityID), 
				_map(0), _type(""), _name(""), _transform(Matrix4::IDENTITY),
				_isPlayer(false), _activated(false),_entityInfo(0),_isGenerator(false),_isEnemy(false)
	{

	} // CEntity
	
	//---------------------------------------------------------

	CEntity::~CEntity()
	{
		assert(!_map && "¡¡Antes de destruir la entidad debe desacoplarse del mapa!!");

		destroyAllComponents();

	} // ~CEntity
	
	//---------------------------------------------------------

	bool CEntity::spawn(CMap *map, const Map::CEntity *entityInfo, const std::string &newName, const Ogre::Vector3 &newPosition) 
	{
		// Leemos las propiedades comunes
		_map = map;
		_type = entityInfo->getType();
		_entityInfo = entityInfo;

		if(!newName.empty())
		{
			_name = newName;
			_transform.setTrans(newPosition);
		}
		else if(entityInfo->hasAttribute("name"))
		{
			_name = entityInfo->getStringAttribute("name");		
			if(entityInfo->hasAttribute("isGenerator"))
				_isGenerator = entityInfo->getBoolAttribute("isGenerator");
			if(entityInfo->hasAttribute("position"))
			{
				Vector3 position = entityInfo->getVector3Attribute("position");
				_transform.setTrans(position);
			}
		}

		#ifdef _DEBUG
			std::cout << _name << std::endl;
		#endif

		// Por comodidad en el mapa escribimos los ángulos en grados.
		if(entityInfo->hasAttribute("orientation"))
		{
			float yaw = Math::fromDegreesToRadians(entityInfo->getFloatAttribute("orientation"));
			Math::yaw(yaw,_transform);
		}

		if(entityInfo->hasAttribute("isPlayer"))
			_isPlayer = entityInfo->getBoolAttribute("isPlayer");

		if(entityInfo->hasAttribute("isEnemy"))
			_isEnemy = entityInfo->getBoolAttribute("isEnemy");
		

		// Inicializamos los componentes
		TComponentList::const_iterator it;

		bool correct = true;

		for( it = deactiveComponents.begin(); it != deactiveComponents.end() && correct; ++it )
			correct = (*it)->spawn(this,map,entityInfo) && correct;

		return correct;

	} // spawn

	//---------------------------------------------------------

	void CEntity::awake()
	{
		//aviso el entity manager que la entidad pasa a ser activa
		if(!_isGenerator) 
			AI::CServer::getSingletonPtr()->getEntityManager()->registerEntity(this);

		TComponentList::const_iterator it;
		for( it = activeComponents.begin(); it != activeComponents.end(); ++it )
			(*it)->awake();
	}

	//---------------------------------------------------------

	bool CEntity::activate() 
	{

		// Si somos jugador, se lo decimos al servidor
		// y nos registramos para que nos informen
		// de los movimientos que debemos realizar
		if (isPlayer())
		{
			CServer::getSingletonPtr()->setPlayer(this);
			
		}
		
		if (_type == "Camera")
			GUI::CServer::getSingletonPtr()->getPlayerController()->setCamera(this);


		// Activamos los componentes
		TComponentList::const_iterator it = deactiveComponents.begin();

		// Solo si se activan todos los componentes correctamente nos
		// consideraremos activados.
		_activated = true;

		while (it != deactiveComponents.end())
		{
			if ((*it)->activate())
			{
				activeComponents.push_back((*it));
				deactiveComponents.erase(it++);
			}
			else
			{
				_activated = false;
				++it;
			}
		}

		return _activated;

	} // activate

	//---------------------------------------------------------

	bool CEntity::restart()
	{
		bool restartSuccesful = true;
		_activated = true;
		TComponentList::const_iterator it = activeComponents.begin();
		while (it != activeComponents.end())
			(*it++)->restart();
		return restartSuccesful;
	}

	//---------------------------------------------------------

	void CEntity::pause()
	{
		TComponentList::const_iterator it = activeComponents.begin();
		while (it != activeComponents.end())
				(*it++)->pause();
		_activated = false;
	}

	//---------------------------------------------------------

	void CEntity::deactivate() 
	{
		// Si éramos el jugador, le decimos al servidor que ya no hay.
		// y evitamos que se nos siga informando de los movimientos que 
		// debemos realizar
		if (isPlayer())
		{
			//GUI::CServer::getSingletonPtr()->getPlayerController()->setControlledAvatar(0);			
			CServer::getSingletonPtr()->setPlayer(0);
			
		}
		
		if (_type == "Camera")			
			GUI::CServer::getSingletonPtr()->getPlayerController()->setCamera(0);

		TComponentList::const_iterator it = activeComponents.begin();

		// Desactivamos los componentes
		while (it != activeComponents.end())
		{
			(*it)->deactivate();
			deactiveComponents.push_back((*it));
			activeComponents.erase(it++);
		}
		_activated = false;

	} // deactivate

	//---------------------------------------------------------

	void CEntity::tick(unsigned int msecs) 
	{
		TComponentList::iterator it;

		for( it = activeComponents.begin(); it != activeComponents.end(); ++it )
			(*it)->tick(msecs);

	} // tick

	//---------------------------------------------------------

	void CEntity::addComponent(IComponent* component)
	{
		deactiveComponents.push_back(component);
		component->setEntity(this);

	} // addComponent

	//---------------------------------------------------------

	IComponent* CEntity::getComponent(std::string type)
	{
		IComponent* component = searchComponentByType(type, activeComponents);
		
		if (component)
			return component;
		
		return searchComponentByType(type, deactiveComponents);
	} // getComponent

	//---------------------------------------------------------

	void CEntity::deactivate(std::string componentType)
	{
		IComponent* component = searchComponentByType(componentType,  activeComponents);

		if (component)
		{
			component->deactivate();
			activeComponents.remove(component);
			deactiveComponents.push_back(component);
		}
	}

	//---------------------------------------------------------

	void CEntity::activate(std::string componentType)
	{
		IComponent* component = searchComponentByType(componentType,  deactiveComponents);

		if (component)
		{
			deactiveComponents.remove(component);
			activeComponents.push_back(component);
			component->activate();
		}
	}

	//---------------------------------------------------------

	bool CEntity::removeComponent(IComponent* component)
	{
		if (removeComponent(component, activeComponents))
			return true;

		return removeComponent(component, deactiveComponents);

	} // removeComponent

	//---------------------------------------------------------

	void CEntity::destroyAllComponents()
	{
		IComponent* c;
		while(!activeComponents.empty())
		{
			c = activeComponents.back();
			activeComponents.pop_back();
			delete c;
		}

		while(!deactiveComponents.empty())
		{
			c = deactiveComponents.back();
			deactiveComponents.pop_back();
			delete c;
		}

	} // destroyAllComponents

	//---------------------------------------------------------

	bool CEntity::emitMessageN(const std::shared_ptr<NMessage> &message, IComponent* emitter)
	{
		// Interceptamos los mensajes que además de al resto de los
		// componentes, interesan a la propia entidad.
		if(emitter)
			message->sender=emitter->getEntity();
		if (message->type.compare("TransformMessage") == 0)
		{
			std::shared_ptr<TransformMessage> tMessage = std::static_pointer_cast<TransformMessage>(message);
			_transform = tMessage->transform;
		}

		TComponentList::const_iterator it;
		// Para saber si alguien quiso el mensaje.
		bool anyReceiver = false;
		for( it = activeComponents.begin(); it != activeComponents.end(); ++it )
		{
			// Al emisor no se le envia el mensaje.
			if( emitter != (*it) )
				anyReceiver = (*it)->setN(message) || anyReceiver;
		}
		return anyReceiver;

	} // emitMessageN

	//---------------------------------------------------------

	void CEntity::setTransformN(const Matrix4& transform) 
	{
		_transform = transform;

		// Avisamos a los componentes del cambio.

		auto tMessage = std::make_shared<TransformMessage>();
		tMessage->transform = _transform;
		emitMessageN(tMessage);

	} // setTransformN


	//---------------------------------------------------------

	void CEntity::setPositionN(const Vector3 &position, IComponent* invoker)  
	{
		_transform.setTrans(position);

		// Avisamos a los componentes del cambio.

		auto tMessage = std::make_shared<TransformMessage>();
		tMessage->transform = _transform;
		emitMessageN(tMessage,invoker);

	} // setPositionN

	//---------------------------------------------------------

	void CEntity::setOrientationN(const Matrix3& orientation) 
	{
		_transform = orientation;

		// Avisamos a los componentes del cambio.
		auto tMessage = std::make_shared<TransformMessage>();
		tMessage->transform = _transform;
		emitMessageN(tMessage);

	} // setOrientationN

	//---------------------------------------------------------

	Matrix3 CEntity::getOrientation() const
	{
		Matrix3 orientation;
		_transform.extract3x3Matrix(orientation);
		return orientation;

	} // getOrientation

	//---------------------------------------------------------

	void CEntity::setYawN(float yaw) 
	{
		Math::setYaw(yaw,_transform);

		// Avisamos a los componentes del cambio.
		auto tMessage = std::make_shared<TransformMessage>();
		tMessage->transform = _transform;
		emitMessageN(tMessage);

	} // setYawN

	//---------------------------------------------------------

	void CEntity::yawN(float yaw) 
	{
		Math::yaw(yaw,_transform);

		// Avisamos a los componentes del cambio.
		auto tMessage = std::make_shared<TransformMessage>();
		tMessage->transform = _transform;
		emitMessageN(tMessage);

	} // yawN

	//---------------------------------------------------------

	IComponent* CEntity::searchComponentByType(std::string type, const TComponentList &componentList)
	{
		TComponentList::const_iterator componentIt;

		// Lambda expression
		std::function<bool(const IComponent* component)> componentTypeComparator = 
			[&type] (const IComponent* component ) { return component->getType() == type; };

		componentIt = std::find_if(componentList.begin(), componentList.end(), componentTypeComparator);
		
		return (componentIt != componentList.end()) ? (*componentIt) : nullptr;
	}

	//---------------------------------------------------------

	bool CEntity::removeComponent(IComponent* component, TComponentList &componentList)
	{
		TComponentList::iterator componentIt = std::find(componentList.begin(), componentList.end(), component);

		if (componentIt == componentList.end())
			return false;

		componentList.erase(componentIt);
		component->setEntity(nullptr);
		return true;
	}

} // namespace Logic
