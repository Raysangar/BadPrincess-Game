#ifndef Sound_h
#define Sound_h

#include <string>

namespace Audio
{
	class CSound
	{
	public:
		CSound(const std::string &soundName) : soundName(soundName) {};

		virtual ~CSound() {};

		virtual void load() = 0;

		virtual void unload() = 0;

		virtual void play() = 0;

		virtual void stop() = 0;

	private:
		std::string soundName;
	};
}

#endif