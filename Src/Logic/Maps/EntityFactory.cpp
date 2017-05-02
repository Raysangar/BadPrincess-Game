/**
@file EntityFactory.cpp

Contiene la implementación de la clase factoría de entidades
del juego.

@see Logic::CEntityFactory

@author David Llansó García, Marco Antonio Gómez Martín
@date Agosto, 2010
*/
#include "EntityFactory.h"
#include "ComponentFactory.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Component.h"
#include "Logic/EntityMessages.h"
#include "Map.h"

#include "Map/MapEntity.h"
#include "AI/Server.h"
#include "AI/EntityManager.h"

#include <iostream>
#include <fstream>
#include <cassert>

// HACK. Debería leerse de algún fichero de configuración
#define BLUEPRINTS_FILE_PATH "./media/maps/"

/**
Sobrecargamos el operador >> para la lectura de blueprints.
Cada línea equivaldrá a una entidad donde la primera palabra es el tipo
y las siguientes son los componentes que tiene.
*/
std::istream& operator>>(std::istream& is, Logic::CEntityFactory::TBluePrint& blueprint) 
{
	is >> blueprint.type;
	std::string aux;
	getline(is,aux,'\n');
	std::istringstream components(aux);
	while(!components.eof())
	{
		aux.clear();
		components >> aux;
		if(!aux.empty())
			blueprint.components.push_back(aux);
	}
	return is;
}

namespace Logic
{

	CEntityFactory *CEntityFactory::_instance = 0;
	
	//---------------------------------------------------------

	CEntityFactory::CEntityFactory()
	{
		_instance = this;

	} // CEntityFactory
	
	//---------------------------------------------------------

	CEntityFactory::~CEntityFactory()
	{
		_instance = 0;

	} // ~CEntityFactory
	
	//---------------------------------------------------------

	bool CEntityFactory::Init() 
	{
		assert(!_instance && "Segunda inicialización de Logic::CEntityFactory no permitida!");

		new CEntityFactory();

		if (!_instance->open())
		{
			Release();
			return false;
		}

		return true;

	} // Init
	
	//---------------------------------------------------------

	void CEntityFactory::Release() 
	{
		assert(_instance && "Logic::CEntityFactory no está inicializado!");

		if(_instance)
		{
			_instance->close();
			delete _instance;
		}

	} // Release

	//--------------------------------------------------------

	bool CEntityFactory::open()
	{
		return true;

	} // open

	//--------------------------------------------------------

	void CEntityFactory::close() 
	{
		unloadBluePrints();

		destroyAllEntities();
	} // close
	
	//---------------------------------------------------------

	typedef std::pair<std::string,CEntityFactory::TBluePrint> TStringBluePrintPair;

	bool CEntityFactory::loadBluePrints(const std::string &filename)
	{
		// Completamos la ruta con el nombre proporcionado
		std::string completePath(BLUEPRINTS_FILE_PATH);
		completePath = completePath + filename;
		// Abrimos el fichero
		std::ifstream in(completePath.c_str());        
		if(!in)
			return false;

		while(!in.eof())
		{
			// Se lee un TBluePrint del fichero
			TBluePrint b;
			in >> b;
			// Si no era una línea en blanco
			if(!b.type.empty())
			{
				// Si el tipo ya estaba definido lo eliminamos.
				if(_bluePrints.count(b.type))
					_bluePrints.erase(b.type);
				// Añadimos a la tabla
				TStringBluePrintPair elem(b.type,b);
				_bluePrints.insert(elem);
			}
		}

		return true;

	} // loadBluePrints
	
	//---------------------------------------------------------

	void CEntityFactory::unloadBluePrints()
	{
		_bluePrints.clear();

	} // unloadBluePrints
	
	void CEntityFactory::destroyAllEntities()
	{
		TEntitySet::const_iterator it=_deletedEntities.begin();
		for(;it!=_deletedEntities.end();++it)
			delete *it;
		_deletedEntities.clear();
	}
	//---------------------------------------------------------

	Logic::CEntity *CEntityFactory::assembleEntity(const std::string &type) 
	{
		TBluePrintMap::const_iterator it;

		it = _bluePrints.find(type);
		// si el tipo se encuentra registrado.
		if (it != _bluePrints.end()) 
		{
			CEntity* ent = new CEntity(EntityID::NextID());
			std::list<std::string>::const_iterator itc;
			// Añadimos todos sus componentes.
			IComponent* comp;
			for(itc = (*it).second.components.begin(); 
				itc !=(*it).second.components.end(); itc++)
			{
				if(CComponentFactory::getSingletonPtr()->has((*itc)))
				{
					comp = CComponentFactory::getSingletonPtr()->create((*itc));
				}
				else
				{
					assert(!"Nombre erroneo de un componente, Mira a ver si están todos bien escritos en el fichero de blueprints");
					delete ent;
					return 0;
				}
				if(comp)
					ent->addComponent(comp);
			}
			return ent;
		}
		return 0;

	} // assembleEntity
	
	//---------------------------------------------------------


	Logic::CEntity *CEntityFactory::createEntity(
								const Map::CEntity *entityInfo,
								Logic::CMap *map, const std::string &newName, Ogre::Vector3 *newPosition)
	{
		CEntity *ret = assembleEntity(entityInfo->getType());	

		if (!ret)
			return 0;

		// Añadimos la nueva entidad en el mapa antes de inicializarla.
		map->addEntity(ret);

		// Y lo inicializamos
		if (ret->spawn(map, entityInfo, newName, *newPosition))
			return ret;
		else {
			map->removeEntity(ret);
			delete ret;
			return 0;
		}

	} // createEntity
	
	//---------------------------------------------------------

	void CEntityFactory::deleteEntity(Logic::CEntity *entity)
	{
		assert(entity);
		// Si la entidad estaba activada se desactiva al sacarla
		// del mapa.
		entity->getMap()->removeEntity(entity);

		//aviso el entity manager que la entidad se destruye
		AI::CServer::getSingletonPtr()->getEntityManager()->unregisterEntity(entity);

		entity->setPositionN(Vector3(0,-100,0));
		_deletedEntities.insert(entity);
//		delete entity;

	} // deleteEntity
	
	//---------------------------------------------------------

	void CEntityFactory::deferredDeleteEntity(Logic::CEntity *entity)
	{
		assert(entity);

		_pendingEntities.insert(entity);
		

	} // deferredDeleteEntity

	//---------------------------------------------------------

	void CEntityFactory::deleteDefferedEntities()
	{
		TEntitySet::const_iterator it(_pendingEntities.begin());
		TEntitySet::const_iterator end(_pendingEntities.end());

		while(it != end)
		{
			CEntity *entity = *it;
			it++;
			deleteEntity(entity);
		}

		if (!_pendingEntities.empty())
			_pendingEntities.clear();

	} // deleteDefferedObjects

	//--------------------------------------------------------

	Logic::CEntity *CEntityFactory::instantiate(CEntity *entity,const std::string &name, Ogre::Vector3 &position)
	{

		Logic::CEntity* e = instantiateEntityFromPool(entity->getMap(),entity->getType());
		if(e==nullptr)
			e = createEntity(entity->getEntityInfo(), entity->getMap(), getNextNameAvailable(entity->getMap(),entity->getName()), &position);	
		else
			e->_map = entity->getMap();
		e->_transform.setTrans(position);
		e->setOrientationN(entity->getOrientation());
		e->activate();	
		e->awake();

		//send to all that an entity is created right now
		auto m=std::make_shared<EntityInstantiated>();
		m->entity=e;
		entity->getMap()->sendMessageToAll(m);

		return e;
	}
	CEntity* CEntityFactory::instantiateEntityFromPool(Logic::CMap *map,std::string entityType)
	{
		TEntitySet::const_iterator it(_deletedEntities.begin());
		TEntitySet::const_iterator end(_deletedEntities.end());
		CEntity* e=nullptr;
		while(it != end)
		{
			if((*it)->getType().compare(entityType)==0)
			{
				e=*it;
				break;
			}
			++it;
		}

		if(e==nullptr)
			return nullptr;

		_deletedEntities.erase(e);

		// Añadimos la nueva entidad en el mapa antes de inicializarla.
		map->addEntity(e);

		std::cout<<"instantiateEntityFromPool add entity "<<e->getName()<<std::endl;
		return e;
	}
	std::string CEntityFactory::getNextNameAvailable(Logic::CMap *map,std::string entityType)
	{
		unsigned short i=0;

		std::string name;
		do
		{
			name = entityType+std::to_string(i);
			++i;
		}while (map->getEntityByName(name)!=nullptr);
		return name;
	}
} // namespace Logic
