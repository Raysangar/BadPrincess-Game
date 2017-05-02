#include "Options.h"

#include <fstream>
#include <sstream>

namespace BaseSubsystems
{
	Options* Options::instance = nullptr;

	Options* Options::getInstance()
	{
		if (instance == nullptr)
			instance = new Options();
		return instance;
	}

	void Options::release()
	{
		if (instance)
		{
			instance->saveOptions();
			delete instance;
		}
	}

	void Options::saveOptions()
	{
		std::ofstream configFile("BadPrincess.cfg");
		for (std::map<std::string, std::string>::const_iterator it = configurationAttributes.begin(); it != configurationAttributes.end(); ++it)
			configFile << it->first << " = " << it->second << "\n";
		configFile.close();
	}

	Options::Options()
	{
		readOptionsFile("BadPrincess.cfg", &Options::insertAttibuteOnMap);
		readOptionsFile("AvailableResolutions.cfg", &Options::insertAvailableResolution);
	}

	std::string Options::getAttribute(const std::string &attributeName) const
	{
		std::map<std::string, std::string>::const_iterator it = configurationAttributes.find(attributeName);
		return (it == configurationAttributes.end()) ? "" : it->second;
	}

	std::vector<Options::Resolution>* Options::getAvailableResolutions()
	{
		return &availableResolutions;
	}

	void Options::setAttribute(const std::string &attributeName, const std::string &attributeValue)
	{
		configurationAttributes[attributeName] = attributeValue;
	}

	void Options::readOptionsFile(const std::string &fileName, LineProcessor lineProcessor)
	{
		std::ifstream configFile(fileName);
		std::string line;
		while (std::getline(configFile, line))
			(*this.*lineProcessor)(getLine(line));
	}

	char* Options::getLine(const std::string &line)
	{
		char * lineBuffer = new char[line.size() + 1];
		std::copy(line.begin(), line.end(), lineBuffer);
		lineBuffer[line.size()] = '\0';
		return lineBuffer;
	}

	void Options::insertAttibuteOnMap(char* attributeLine)
	{
		char* context = NULL;
		char* attributeName = strtok_s(attributeLine, " =", &context);
		char *attributeValue = strtok_s(NULL, " =", &context);
		if (attributeName != NULL && attributeValue != NULL)
			configurationAttributes[std::string(attributeName)] = std::string(attributeValue);
		delete attributeLine;
	}

	void Options::insertAvailableResolution(char* attributeLine)
	{
		char* context = NULL;
		char* resolutionWidth = strtok_s(attributeLine, " x", &context);
		char *resolutionHeight = strtok_s(NULL, " x", &context);
		if (resolutionWidth != NULL && resolutionHeight != NULL)
			availableResolutions.push_back(std::pair<int, int>(std::atoi(resolutionWidth), std::atoi(resolutionHeight)));
		delete attributeLine;
	}
}