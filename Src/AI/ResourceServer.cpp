#include "ResourceServer.h"

#include <iostream>

#include "GUI/Server.h"
#include "GUI/ResourceCountController.h"
#include "GUI\EventsController.h"
#include "BaseSubsystems\ScriptManager.h"

namespace AI 
{
	void ResourceServer::addResource(ResourceType type, int quantity)
	{
		if(type!=ResourceType::none)
			resources[type] += quantity;
		switch (type)
		{
		case Wood:
			GUI::CServer::getSingletonPtr()->getResourceCountController()->setWoodQuantity(resources[type]);
			break;
		case Iron:
			GUI::CServer::getSingletonPtr()->getResourceCountController()->setIronQuantity(resources[type]);
			break;
		case Stone:
			GUI::CServer::getSingletonPtr()->getResourceCountController()->setStoneQuantity(resources[type]);
			break;
		case none:
			break;
		default:
			break;
		}
	}

	int ResourceServer::getResource(ResourceType type)
	{
		return resources[type];
	}

	ResourceServer::ResourceServer(int iron = 0, int stone = 0, int wood = 0) 
	{
		resources[ResourceType::Iron] = iron;
		//addResource(ResourceType::Iron,1000);

		resources[ResourceType::Wood] = wood;
		//addResource(ResourceType::Wood,1000);

		resources[ResourceType::Stone] = stone;
		//addResource(ResourceType::Stone,1000);
	}

	ResourceServer::~ResourceServer(){	
	}
	void ResourceServer::Release(){
		if(instance!=nullptr)
			delete instance;
	}

	ResourceServer* ResourceServer::getInstance()
	{
		if (instance == nullptr)
		{
			instance = new ResourceServer();
			instance->init();
		}
		return instance;
	}

	void ResourceServer::init()
	{
		ScriptManager::CScriptManager::GetPtrSingleton()->loadScript("media/lua/EntityResourceCost.lua");
	}

	ResourceServer *ResourceServer::instance = nullptr;
	bool ResourceServer::checkResourceAvailability(const char* fieldName)
	{
		return this->checkResourceAvailability(fieldName,1.0f);
	}
	bool ResourceServer::checkResourceAvailability(const char* fieldName,float percentage, bool showWarning)
	{
		int w = (int)((float)ScriptManager::CScriptManager::GetPtrSingleton()->getField(fieldName,"Wood",0) * percentage);
		int s = (int)((float)ScriptManager::CScriptManager::GetPtrSingleton()->getField(fieldName,"Stone",0) * percentage);
		int i = (int)((float)ScriptManager::CScriptManager::GetPtrSingleton()->getField(fieldName,"Iron",0) * percentage);

		int wAvailable = getResource(AI::ResourceType::Wood);
		int sAvailable = getResource(AI::ResourceType::Stone);
		int iAvailable = getResource(AI::ResourceType::Iron);

		if(wAvailable>=w && sAvailable>=s && iAvailable>=i)
			return true;
		else if (showWarning)
		{
			if(wAvailable < w)
				GUI::CServer::getSingletonPtr()->getEventscontroller()->missingResources(GUI::ResourceType::WOOD);
			if(sAvailable < s)
				GUI::CServer::getSingletonPtr()->getEventscontroller()->missingResources(GUI::ResourceType::STONE);
			if(iAvailable < i)
				GUI::CServer::getSingletonPtr()->getEventscontroller()->missingResources(GUI::ResourceType::IRON);
		}
		
		return false;


	}

	int ResourceServer::getRequiredQuantity(const char* fieldName,char* type)
	{
		return (int)((float)ScriptManager::CScriptManager::GetPtrSingleton()->getField(fieldName,type,0) * 1.0);
	}

	void ResourceServer::useResource(const char* fieldName)
	{
		this->useResource(fieldName,1.0f);
	}

	void ResourceServer::useResource(const char* fieldName,float percentage)
	{
		int w = (int)((float)ScriptManager::CScriptManager::GetPtrSingleton()->getField(fieldName,"Wood",0)*percentage);
		int s = (int)((float)ScriptManager::CScriptManager::GetPtrSingleton()->getField(fieldName,"Stone",0)*percentage);
		int i = (int)((float)ScriptManager::CScriptManager::GetPtrSingleton()->getField(fieldName,"Iron",0)*percentage);

		addResource(AI::ResourceType::Wood,-w);
		addResource(AI::ResourceType::Stone,-s);
		addResource(AI::ResourceType::Iron,-i);
	}

	void ResourceServer::reset()
	{
		resources[ResourceType::Iron] = 0;

		resources[ResourceType::Wood] = 0;

		resources[ResourceType::Stone] = 0;

		init();
	}
}