#pragma once

#include <string>
#include "ParseOptions.h"

enum class FolderOptions;
enum class EnabledOptions;

class SetEnviromentVariables : public ParseOptions {
	FolderOptions processFolders(std::string& optionString);
	EnabledOptions processEnableOptions(std::string& optionString);
	EnabledOptions processDisableOptions(std::string& optionString);
	std::string m_option;
	std::string m_value;
public:
	SetEnviromentVariables() = default;
	virtual ~SetEnviromentVariables() = default;
	bool parseFolders(const char* optionString);
	bool parseEnableOptions(const char* optionString);
	bool parseDisableOptions(const char* optionString);
};


