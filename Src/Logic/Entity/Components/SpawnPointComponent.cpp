/**
@file SpawnPointComponent.cpp

Contiene la declaracion de la clase que gestiona l'aparecer de los enemigos

@see Logic::CSpawnPointComponent
@see Logic::IComponent

@author Guidi Giacomo
@date March, 2015
*/

#include "SpawnPointComponent.h"

#include "Map/MapEntity.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"



namespace Logic 
{
	IMP_FACTORY(CSpawnPointComponent);

	//--------------------------------------------------------

	CSpawnPointComponent::CSpawnPointComponent(std::string componentName) : IComponent(componentName),_boxDimension(Vector3::ZERO),_circleRadius(0.0f),_spawnType("")
	{			
	}
	
	//---------------------------------------------------------

	bool CSpawnPointComponent::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;
		_scene = map;

		assert(entityInfo->hasAttribute("spawn_type"));
		_spawnType = entityInfo->getStringAttribute("spawn_type");
		//the type of spawn could be only circle or box
		assert(_spawnType.compare("circle")==0 || _spawnType.compare("box")==0);
	
//		assert(entityInfo->hasAttribute("dimension"));
		if(entityInfo->hasAttribute("spawn_circle_radius"))
			_circleRadius = entityInfo->getFloatAttribute("spawn_circle_radius");

		if(entityInfo->hasAttribute("spawn_box_dimension"))
			_boxDimension = entityInfo->getVector3Attribute("spawn_box_dimension");

		return true;

	} // spawn
	
	//---------------------------------------------------------

	void CSpawnPointComponent::awake(){}

	//---------------------------------------------------------

	bool CSpawnPointComponent::activate()
	{			
		return true;
	} // activate
	
	//---------------------------------------------------------

	void CSpawnPointComponent::deactivate()
	{

	} // deactivate
	
	//---------------------------------------------------------

	bool CSpawnPointComponent::acceptN(const std::shared_ptr<NMessage> &message)
	{
		return false;
	} // accept
	//---------------------------------------------------------

	void CSpawnPointComponent::processN(const std::shared_ptr<NMessage> &message)
	{
		
	} // process

	void CSpawnPointComponent::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);

	}
	CEntityPtr CSpawnPointComponent::createEntity(Logic::CEntity * e)
	{
		Vector3 pos;
		if(_spawnType.compare("box")==0)
			pos = generateBoxSpawnPosition();
		else
			pos = generateCircleSpawnPosition();

		CEntityPtr spawnedEntity = Logic::CEntityFactory::getSingletonPtr()->instantiate(e, "",pos);
		std::cout << spawnedEntity->getName() << " created" << std::endl;
		return spawnedEntity;
	}

	Vector3 CSpawnPointComponent::generateBoxSpawnPosition()
	{
		Vector3 pos = this->getEntity()->getPosition();
		float rx =((float)rand()/(float)(RAND_MAX)) * _boxDimension.x;
		float rz = ((float)rand()/(float)(RAND_MAX))* _boxDimension.z;
		pos.x+= rx -_boxDimension.x/2;
		pos.z+= rz -_boxDimension.y/2;
		
		return pos;
	}

	Vector3 CSpawnPointComponent::generateCircleSpawnPosition()
	{
		Vector3 pos = this->getEntity()->getPosition();
		float randomNormalNumber=((float)rand()/(float)(RAND_MAX));
		float angle = randomNormalNumber * 2*Math::PI;
//		this is the correct expression but the vector is unit_x so		
//		Vector3 dir(Vector3::UNIT_X);
//		dir.x = dir.x*cos(angle)-dir.y*sin(angle);
//		dir.z = dir.x*sin(angle)+dir.y*cos(angle);
		Vector3 dir(cos(angle),0,sin(angle));
		dir.normalise();

		pos+= dir*_circleRadius;
		return pos;
	}

} // namespace Logic

