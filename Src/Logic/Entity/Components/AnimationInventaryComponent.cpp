/**
@file AnimationInventaryComponent.cpp
*/
#include "AnimationInventaryComponent.h"

#include "BaseSubsystems\ScriptManager.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"

#include "Logic/AnimationMessages.h"
#include "Logic/Entity/Components/AnimatedGraphics.h"

#include "Graphics/MeshEntity.h"



namespace Logic 
{
	IMP_FACTORY(CAnimationInventaryComponent);

	CAnimationInventaryComponent::~CAnimationInventaryComponent()
	{
		detachCurrentInventary();
		_currentInventary = nullptr;

		if(_animationInventary!=nullptr)
			deleteAnimationInventary();
	}
	//---------------------------------------------------------	

	bool CAnimationInventaryComponent::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		assert(_entity->getComponent("CAnimatedGraphics"));
		_graphics = static_cast<Logic::CAnimatedGraphics*>(_entity->getComponent("CAnimatedGraphics"));

		_scene = map->getScene();

		_luaAnimationTable = _entity->getType()+"Animation";

		return true;
	} 

	//---------------------------------------------------------

	void CAnimationInventaryComponent::awake()
	{
		detachCurrentInventary();
		_currentInventary = nullptr;

		if(_animationInventary== nullptr)
			initAnimationInventary();

	}
	
	void CAnimationInventaryComponent::initAnimationInventary()
	{
		_animationInventary= new std::map<std::string,AttachableObjectList*>();

		std::vector<std::string> animations;
		//get all animation name
		ScriptManager::CScriptManager::GetPtrSingleton()->getTableIndex(_luaAnimationTable.c_str(),animations);

		//foreach animation
		for(std::string idx : animations)
		{
			//get the number of item that we have to attach
			int size = ScriptManager::CScriptManager::GetPtrSingleton()->getTableSize(_luaAnimationTable.c_str(),idx.c_str());
			AttachableObjectList *list = new AttachableObjectList();
			if(size>0)
			{
				//foreach item to attach
				for(int i=1;i<=size;++i)
				{
					//get name of item,mesh, and bone where i have to attach
					std::string bone = ScriptManager::CScriptManager::GetPtrSingleton()->getField(_luaAnimationTable.c_str(),idx.c_str(),i,"bone","");
					std::string mesh = ScriptManager::CScriptManager::GetPtrSingleton()->getField(_luaAnimationTable.c_str(),idx.c_str(),i,"mesh","");
					std::string name = ScriptManager::CScriptManager::GetPtrSingleton()->getField(_luaAnimationTable.c_str(),idx.c_str(),i,"name","");
					//instantiate the mesh item
					Graphics::CMeshEntity* meshObj=new Graphics::CMeshEntity(_entity->getName()+name,mesh);

					meshObj->instantiateEntity(_scene);
					//instantiate the pair bone->mesh
					AttachableObject *obj=new AttachableObject(bone,meshObj);
					//add to a list
					list->push_back(obj);
				}
			}
			//put the list into a map animationName->listOfAttachObject
			(*_animationInventary)[idx]=list;
		}
	}

	void CAnimationInventaryComponent::deleteAnimationInventary()
	{
		//free the memory
		std::map<std::string,AttachableObjectList*>::const_iterator cit=_animationInventary->begin();
		for(;cit!=_animationInventary->end();++cit)
		{
			for(AttachableObject*item:*cit->second)
			{
				delete item->second;
				delete item;
				item=nullptr;
			}
			delete cit->second;
		}
		delete _animationInventary;
		_animationInventary=nullptr;
	}
	//---------------------------------------------------------

	bool CAnimationInventaryComponent::acceptN(const std::shared_ptr<NMessage> &message)
	{
		// La acción no acepta mensajes
		return (message->type.compare("SetAnimation") == 0);
	}

	//---------------------------------------------------------

	void CAnimationInventaryComponent::processN(const std::shared_ptr<NMessage> &message)
	{		
		if(message->type.compare("SetAnimation") == 0)
		{			
			auto m = std::static_pointer_cast<SetAnimation>(message);
			changeAnimation(m->name);
		}
		
	}	

	void CAnimationInventaryComponent::changeAnimation(std::string name)
	{
		//detach current object
		detachCurrentInventary();
		_currentInventary = nullptr;
		//if we have to attach something for this new animation
		std::map<std::string,AttachableObjectList*>::const_iterator it=_animationInventary->find(name);
		if(it!=_animationInventary->end())
			attachInventary(it->second);
	}

	void  CAnimationInventaryComponent::attachInventary(const AttachableObjectList *listToActivate)
	{
		for(AttachableObject* item : *listToActivate)
		{
			//if hte item it isn't already attached to something
			if(!item->second->isAttached())
				_graphics->getGraphicsEntity()->attachObjectToBone(item->first, item->second);		
		}
		_currentInventary=listToActivate;
	}

	void  CAnimationInventaryComponent::detachCurrentInventary()
	{
		if(_currentInventary!=nullptr)
		{
			for(AttachableObject* item : *_currentInventary)
				_graphics->getGraphicsEntity()->detachObjectFromBone(item->second);	
		}
	}

	bool CAnimationInventaryComponent::activate(){
		if(_currentInventary!=nullptr)
			attachInventary(_currentInventary);
		return true;
	}

	void CAnimationInventaryComponent::deactivate(){
		detachCurrentInventary();
	}
}


