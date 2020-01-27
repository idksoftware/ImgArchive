#pragma once
#include <string>

enum class Option;



class SetConfig
{
	std::string m_optionBlock;
	std::string m_option;
	std::string m_value;
	Option processGeneralOptions(std::string& optionString);
	Option processFolderOptions(std::string& optionString);
	Option processExifToolOptions(std::string& optionString);
	Option processMasterOptions(std::string& optionString);
	Option processNetworkOptions(std::string& optionString);

	bool processArgs(const char* ov);
public:
	SetConfig() = default;
	bool parseGeneralOptions(const char* optionString);
	bool parseFolderOptions(const char* optionString);
	bool parseExifToolOptions(const char* optionString);
	bool parseMasterOptions(const char* optionString);
	bool parseNetworkOptions(const char* optionString);
	std::string& getOption() {
		return m_option;
	}
	std::string& getValue() {
		return m_value;
	}
	std::string& getOptionBlock() {
		return m_optionBlock;
	}
};

