#pragma once

#include <string>
#include "ParseOptions.h"

enum class FolderOptions;
enum class EnabledOptions;

enum class UserMode
{
	Local,
	All,
	Invalid
};


class SetEnviromentVariables : public ParseOptions
{
	FolderOptions processFolders(std::string& optionString);
	std::string m_option;
	std::string m_value;
	Error m_error{Error::Unknown};
	UserMode m_userMode{UserMode::Invalid};

	bool processArgs(const char* ov);
	bool parseEnableOptions(const char* optionString, const char* value);
public:
	SetEnviromentVariables() = default;
	virtual ~SetEnviromentVariables() = default;
	bool parseFolders(const char* optionString);
	bool parseEnableOptions(const char* optionString);
	bool parseDisableOptions(const char* optionString);

	bool setVariable();

	UserMode userMode(const char* ua);
	UserMode userMode();
};
