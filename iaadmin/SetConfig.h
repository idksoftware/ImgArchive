#pragma once
#include <string>

enum class Option;

class SetConfig
{
	Option processGeneralOptions(std::string& optionString);
public:
	SetConfig() = default;
	bool parseGeneralOptions(const char* optionString);
};

