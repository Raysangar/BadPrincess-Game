/**
@file TrackComponent.cpp
*/
#include "TrackComponent.h"

#include "BaseSubsystems\ScriptManager.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Logic/AudioMessages.h"
#include "Logic/AnimationMessages.h"
#include "Logic/Entity/Components/AnimatedGraphics.h"

#include "Graphics/AnimatedEntity.h"



namespace Logic 
{
	IMP_FACTORY(CTrackComponent);

	CTrackComponent::~CTrackComponent()
	{
		deactivateTrack();
		_currentTrack = nullptr;

		if(_animationTrack!=nullptr)
			deleteAnimationTrack();
	}
	//---------------------------------------------------------	

	bool CTrackComponent::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		assert(_entity->getComponent("CAnimatedGraphics"));
		_graphics = static_cast<Logic::CAnimatedGraphics*>(_entity->getComponent("CAnimatedGraphics"));

		_luaAnimationTable = _entity->getType()+"Track";

		return true;
	} 

	//---------------------------------------------------------

	void CTrackComponent::awake()
	{
		deactivateTrack();
		_currentTrack = nullptr;

		if(_animationTrack== nullptr)
			initAnimationTrack();

	}
	
	void CTrackComponent::initAnimationTrack()
	{
		_animationTrack = new std::map<std::string,TrackSoundEventList*>();

		std::vector<std::string> animations;
		//get all animation name
		ScriptManager::CScriptManager::GetPtrSingleton()->getTableIndex(_luaAnimationTable.c_str(),animations);

		//foreach animation
		for(std::string idx : animations)
		{
			//get the number of item that we have to attach
			int size = ScriptManager::CScriptManager::GetPtrSingleton()->getTableSize(_luaAnimationTable.c_str(),idx.c_str());
			TrackSoundEventList *list = new TrackSoundEventList();
			if(size>0)
			{
				//foreach item to attach
				for(int i=1;i<=size;++i)
				{
					//get name of item,mesh, and bone where i have to attach
					float time = ScriptManager::CScriptManager::GetPtrSingleton()->getField(_luaAnimationTable.c_str(),idx.c_str(),i,"time",0.0f);
					std::string sound = ScriptManager::CScriptManager::GetPtrSingleton()->getField(_luaAnimationTable.c_str(),idx.c_str(),i,"soundName","");
					std::string id = ScriptManager::CScriptManager::GetPtrSingleton()->getField(_luaAnimationTable.c_str(),idx.c_str(),i,"id","");
//					std::cout<<idx<<"id= "<<id<<" sound="<<sound<<" time="<<time<<std::endl;
					//instantiate the pair bone->mesh
					TrackSoundEvent *obj=new TrackSoundEvent(id,TrackSound(time,sound));
					//add to a list
					list->push_back(obj);
				}
			}
			//put the list into a map animationName->listOfAttachObject
			(*_animationTrack)[idx]=list;
		}
	}

	void CTrackComponent::deleteAnimationTrack()
	{
		//free the memory
		std::map<std::string,TrackSoundEventList*>::const_iterator cit=_animationTrack->begin();
		for(;cit!=_animationTrack->end();++cit)
		{
			for(TrackSoundEvent*item:*cit->second)
			{
				delete item;
				item=nullptr;
			}
			delete cit->second;
		}
		delete _animationTrack;
		_animationTrack=nullptr;
	}
	//---------------------------------------------------------

	bool CTrackComponent::acceptN(const std::shared_ptr<NMessage> &message)
	{
		// La acción no acepta mensajes
		return (message->type.compare("SetAnimation") == 0);
	}

	//---------------------------------------------------------

	void CTrackComponent::processN(const std::shared_ptr<NMessage> &message)
	{		
		if(message->type.compare("SetAnimation") == 0)
		{			
			auto m = std::static_pointer_cast<SetAnimation>(message);
			changeAnimation(m->name);
		}
		
	}	

	void CTrackComponent::changeAnimation(std::string name)
	{
		//detach current object
		deactivateTrack();
		_currentTrack = nullptr;
		//if we have to play some sounds for this new animation
		std::map<std::string,TrackSoundEventList*>::const_iterator it=_animationTrack->find(name);
		if(it!=_animationTrack->end())
			activateTrack(it->second);
	}

	void  CTrackComponent::activateTrack(const TrackSoundEventList *listToActivate)
	{
		_currentTrack=listToActivate;
	}

	void  CTrackComponent::deactivateTrack()
	{
		if(_currentTrack!=nullptr)
		{
			for(TrackSoundEvent* item : *_currentTrack)
			{
				auto stopAudioMessage = std::make_shared<StopAudio>();
				stopAudioMessage->eventName = item->second._soundName;
				_entity->emitMessageN(stopAudioMessage);
			}
		}
	}

	bool CTrackComponent::activate(){
		if(_currentTrack!=nullptr)
			activateTrack(_currentTrack);
		return true;
	}

	void CTrackComponent::deactivate(){
		deactivateTrack();
	}

	void CTrackComponent::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);
		float _currentAnimTime=_graphics->getAnimatedGraphicsEntity()->getCurrentAnimationTime();
//		std::cout<<"animTime="<<_currentAnimTime<<std::endl;
		if(_currentTrack!=nullptr)
		{
			for(TrackSoundEvent* item : *_currentTrack)
			{
				if(_currentAnimTime>=item->second._time && !item->second._alreadyExecuted)
				{
//					std::cout<<_entity->getName()<<" play audio "<<item->second._soundName<<std::endl;
					item->second._alreadyExecuted = true;
					auto playAudioMessage = std::make_shared<PlayAudio>();
					playAudioMessage->eventName = item->second._soundName;
					_entity->emitMessageN(playAudioMessage);
				}

				if(_currentAnimTime<item->second._time && item->second._alreadyExecuted)
				{
					item->second._alreadyExecuted = false;
//					std::cout<<_entity->getName()<<" reset audio "<<item->second._soundName<<std::endl;
				}
			}
		}

	}

	void CTrackComponent::changeAnimationTrackSoundName(std::string animationName,std::string id,std::string newSoundName)
	{
		std::map<std::string,TrackSoundEventList*>::const_iterator it=_animationTrack->find(animationName);
		if(it!=_animationTrack->end())
		{
			for(TrackSoundEvent* item : *it->second)
			{
				if(item->first.compare(id)==0)
				{
					item->second._soundName = newSoundName;
					break;
				}
			}
		}
	}

}


