/**
@file WaveManagerComponent.cpp

Contiene la implementación del componente que controla el movimiento 
de la entidad.
 
@see Logic::CWaveManagerComponent
@see Logic::IComponent

@author Guidi Giacomo
@date March, 2015
*/

#include "WaveManagerComponent.h"
#include "Map/MapEntity.h"

#include "AI/Server.h"
#include "AI/WaveManager.h"



namespace Logic 
{
	IMP_FACTORY(CWaveManagerComponent);

	//--------------------------------------------------------

	CWaveManagerComponent::CWaveManagerComponent(std::string componentName) : IComponent(componentName) 
		{			
		}
	
	//---------------------------------------------------------

	bool CWaveManagerComponent::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		assert(entityInfo->hasAttribute("loadWaveLevel"));
		std::string file = entityInfo->getStringAttribute("loadWaveLevel");
		AI::CServer::getSingletonPtr()->getWaveManger()->loadWaveLevel(file);

		return true;

	} // spawn
	
	//---------------------------------------------------------

	void CWaveManagerComponent::awake(){}

	//---------------------------------------------------------

	bool CWaveManagerComponent::activate()
	{			
		return true;
	} // activate
	
	//---------------------------------------------------------

	void CWaveManagerComponent::deactivate()
	{

	} // deactivate
	
	//---------------------------------------------------------

	bool CWaveManagerComponent::acceptN(const std::shared_ptr<NMessage> &message)
	{
		return AI::CServer::getSingletonPtr()->getWaveManger()->acceptN(message);
	} // accept
	//---------------------------------------------------------

	void CWaveManagerComponent::processN(const std::shared_ptr<NMessage> &message)
	{
		//Accept here all message that could interest to wave manager		
		AI::CServer::getSingletonPtr()->getWaveManger()->processN(message);
	} // process

	void CWaveManagerComponent::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);
		AI::CServer::getSingletonPtr()->getWaveManger()->tick(msecs);

	}

} // namespace Logic

