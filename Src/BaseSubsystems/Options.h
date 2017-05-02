#ifndef __BaseSubsystems_Options_H
#define __BaseSubsystems_Options_H

#include <map>
#include <vector>

namespace BaseSubsystems
{
	class Options
	{
	public:
		typedef std::pair<int, int> Resolution;

		static Options* getInstance();

		static void release();
		
		void saveOptions();

		std::string getAttribute(const std::string &attributeName) const;

		std::vector<Resolution>* getAvailableResolutions();

		void setAttribute(const std::string &attributeName, const std::string &attributeValue);

	private:
		typedef void (Options::*LineProcessor)(char* lineBuffer);

		void readOptionsFile(const std::string &fileName, LineProcessor lineProcessor);

		char* getLine(const std::string &line);

		void insertAvailableResolution(char* resolutionLine);

		void insertAttibuteOnMap(char* attributeLine);

		std::vector<Resolution> availableResolutions;

		std::map<std::string, std::string> configurationAttributes;

		static Options* instance;

		Options();

		virtual ~Options(){};
	};
}

#endif