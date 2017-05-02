#include "Server.h"

#include "Listener.h"
#include "Event.h"
#include "3DEvent.h"

#include "BaseSubsystems\Server.h"
#include "BaseSubsystems\Options.h"

#include <cassert>

namespace Audio
{

	CServer* CServer::instance = nullptr;

	//---------------------------------------------------------

	CServer* CServer::getInstance()
	{
		return instance;
	}

	//---------------------------------------------------------

	bool CServer::Init()
	{
		if (!instance)
		{
			instance = new CServer();
			if (!instance->open())
			{
				Release();
				return false;
			}
		}
		return true;
	}

	//---------------------------------------------------------

	void CServer::Release()
	{
		if (instance)
			delete instance;
		instance = nullptr;
	}

	//---------------------------------------------------------

	void CServer::tick(unsigned int msecs)
	{
		studioSystem->update();
		std::list<std::pair<C3DEvent*, int>>::iterator it = playingSounds.begin();
		while (it != playingSounds.end())
		{
			it->second += msecs;
			if (it->second >= 8000 || it->first->isPaused())
				playingSounds.erase(it++);
			else
				++it;
		}
	}

	//---------------------------------------------------------

	FMOD::Studio::System* CServer::getStudio()
	{
		return studioSystem;
	}

	//---------------------------------------------------------

	CListener* CServer::addListenerOwner(Logic::CListenerComponent* listenerComponent)
	{
		audioListener.second = listenerComponent;
		return audioListener.first;
	}

	//---------------------------------------------------------

	void CServer::removeListenerOwner(Logic::CListenerComponent* listenerComponent)
	{
		assert(audioListener.second == listenerComponent);
		audioListener.second = nullptr;
	}

	//---------------------------------------------------------

	void CServer::loadBank(std::string bankName)
	{
		FMOD_RESULT result = FMOD_OK;
		if (loadedBanks.find(bankName) == loadedBanks.end())
			result = studioSystem->loadBankFile(("media/audio/banks/" + bankName).c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &loadedBanks[bankName]);
		assert(result == FMOD_OK);
	}

	//---------------------------------------------------------

	CServer::CServer() : studioSystem(nullptr), lowLevelSystem(nullptr), actualListener(nullptr), generalVolume(1), timeSinceLastPlayingSoundsCleaning(0) {}

	//---------------------------------------------------------

	CServer::~CServer()
	{
		delete audioListener.first;

		for(std::map<std::string, FMOD::Studio::Bank*>::iterator it = loadedBanks.begin(); it != loadedBanks.end(); ++it)
		{
			it->second->unloadSampleData();
			it->second->unload();
		}

		studioSystem->unloadAll();
		studioSystem->release();

		studioSystem = nullptr;
		lowLevelSystem = nullptr;
	}

	//---------------------------------------------------------

	bool CServer::open()
	{

		if (FMOD::Studio::System::create(&studioSystem) != FMOD_OK)
			return false;

		studioSystem->getLowLevelSystem(&lowLevelSystem);

		if (lowLevelSystem->setSoftwareFormat(0, FMOD_SPEAKERMODE_5POINT1, 0) != FMOD_OK)
			return false;

		lowLevelSystem->set3DSettings(1.0f, 1.0f, 0.1f);

		if (studioSystem->initialize(32,  FMOD_STUDIO_INIT_SYNCHRONOUS_UPDATE, FMOD_INIT_NORMAL, nullptr) != FMOD_OK)
			return false;
		
		studioSystem->setNumListeners(1);

		audioListener.first = new CListener();
		audioListener.second = nullptr;

		BaseSubsystems::Options* gameOptions = BaseSubsystems::CServer::getSingletonPtr()->getGameOptions();

		generalVolume = (float) std::atof(gameOptions->getAttribute("volume").c_str());

		SFXVolume = (float) std::atof(gameOptions->getAttribute("sfx").c_str());

		return true;
	}

	//---------------------------------------------------------

	void CServer::setFmodVectorWithLogicVector(FMOD_VECTOR &vector, const Vector3 &logicVector)
	{
		vector.x = -logicVector.x;
		vector.y = logicVector.y;
		vector.z = logicVector.z;
	}

	//---------------------------------------------------------

	void CServer::setGeneralVolume(float volume)
	{
		generalVolume = volume;
		notifyObservers();
	}

	//---------------------------------------------------------

	float CServer::getGeneralVolume() const
	{
		return generalVolume;
	}

	//---------------------------------------------------------

	void CServer::setSFXVolume(float volume)
	{
		SFXVolume = volume;
		notifySFXObservers();
	}

	//---------------------------------------------------------

	float CServer::getSFXVolume() const
	{
		return SFXVolume;
	}

	//---------------------------------------------------------

	void CServer::notifyObservers()
	{
		for (CEvent* observer : observers)
			observer->volumeChanged();
	}

	//---------------------------------------------------------

	void CServer::notifySFXObservers()
	{
		for (C3DEvent* observer : SFXObservers)
			observer->volumeChanged();
	}

	//---------------------------------------------------------

	void CServer::registerObserver(CEvent* observer)
	{
		observers.push_back(observer);
		observer->volumeChanged();
	}

	//---------------------------------------------------------

	void CServer::unregisterObserver(CEvent* observer)
	{
		observers.remove(observer);
	}
	
	//---------------------------------------------------------

	void CServer::unregisterObservers()
	{
		observers.clear();
	}

	//---------------------------------------------------------

	void CServer::registerSFXObserver(C3DEvent* observer)
	{
		SFXObservers.push_back(observer);
		observer->volumeChanged();
	}

	//---------------------------------------------------------

	void CServer::unregisterSFXObservers()
	{
		SFXObservers.clear();
	}

	//---------------------------------------------------------

	void CServer::unregisterSFXObserver(C3DEvent* observer)
	{
		SFXObservers.remove(observer);
	}

	//---------------------------------------------------------

	void CServer::reset()
	{
		unregisterObservers();
		unregisterSFXObservers();
	}

	//---------------------------------------------------------

	void CServer::removeFromPlayingSounds(C3DEvent* sound)
	{
		std::list<std::pair<C3DEvent*, int>>::iterator it = playingSounds.begin();
		while (it != playingSounds.end() && it->first != sound)
			++it;
		if (it != playingSounds.end())
			playingSounds.erase(it);
	}

	//---------------------------------------------------------

	bool CServer::addToPlayingSounds(C3DEvent* newSound)
	{
		std::list<std::pair<C3DEvent*, int>>::iterator it = playingSounds.begin();
		while (it != playingSounds.end() && it->first != newSound)
			++it;
		if (it == playingSounds.end())
		{
			playingSounds.push_back(std::pair<C3DEvent*, int>(newSound, 0));
			return true;
		}
		return addSoundIfThereIsNoOverFlowOnRegion(newSound);
	}

	//---------------------------------------------------------

	bool CServer::addSoundIfThereIsNoOverFlowOnRegion(C3DEvent* newSound)
	{
		int numberOfSameSoundsPlayingInArea = 0;
		std::list<std::pair<C3DEvent*, int>>::iterator it = playingSounds.begin();
		while (it != playingSounds.end() && numberOfSameSoundsPlayingInArea < 4)
		{
			if (it->first->getName() == newSound->getName())
			{
				if (it->first->isPaused())
					playingSounds.erase(it++);
				else
				{
					if (it->first->getPosition().distance(newSound->getPosition()) < 80)
						++numberOfSameSoundsPlayingInArea;
					++it;
				}
			}
			else
				++it;
		}
		if (numberOfSameSoundsPlayingInArea >= 4)
			return false;
		playingSounds.push_back(std::pair<C3DEvent*, int>(newSound, 0));
		return true;
	}

} // namespace Audio
