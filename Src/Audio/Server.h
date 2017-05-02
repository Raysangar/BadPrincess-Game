#ifndef Server_h
#define Server_h

#include "BaseSubsystems\Math.h"

#include "../../dependencies/include/FMOD/fmod_studio.hpp"
#include "../../dependencies/include/FMOD/fmod.hpp"

#include <map>

namespace Logic
{
	class CListenerComponent;
}

namespace Audio
{
	class CListener;

	class CEvent;

	class C3DEvent;

	class CServer
	{
	public:
		static CServer* getInstance();

		static bool Init();

		static void Release();

		void reset();

		static void setFmodVectorWithLogicVector(FMOD_VECTOR &vector, const Vector3 &logicVector);

		void tick(unsigned int msecs);

		FMOD::Studio::System* getStudio();

		CListener* addListenerOwner(Logic::CListenerComponent* listenerComponent);

		void removeListenerOwner(Logic::CListenerComponent* listenerComponent);

		void loadBank(std::string bankName);

		void setGeneralVolume(float volume);

		float getGeneralVolume() const;

		void setSFXVolume(float volume);

		float getSFXVolume() const;

		void registerObserver(CEvent* observer);

		void unregisterObserver(CEvent* observer);

		void unregisterObservers();

		void registerSFXObserver(C3DEvent* observer);

		void unregisterSFXObserver(C3DEvent* observer);

		void unregisterSFXObservers();

		static Audio::CServer* getSingletonPtr(){ return instance; }

		bool addToPlayingSounds(C3DEvent* newSound);

		void removeFromPlayingSounds(C3DEvent* sound);
		
	private:

		static CServer* instance;

		FMOD::System* lowLevelSystem;

		FMOD::Studio::System* studioSystem;

		std::pair<CListener*, Logic::CListenerComponent*> audioListener;

		std::map<std::string, FMOD::Studio::Bank*> loadedBanks;

		CListener* actualListener;

		std::list<CEvent*> observers;

		std::list<C3DEvent*> SFXObservers;

		std::list<std::pair<C3DEvent*, int>> playingSounds;

		int timeSinceLastPlayingSoundsCleaning;

		float generalVolume;

		float SFXVolume;

		CServer();

		~CServer();

		bool open();

		void notifyObservers();

		void notifySFXObservers();

		bool addSoundIfThereIsNoOverFlowOnRegion(C3DEvent* newSound);

	};
}

#endif //Server_h