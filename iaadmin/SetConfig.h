#pragma once
class SetConfig
{
	bool processGeneralOptions(std::string& option);
public:
	SetConfig() = default;
	bool parseGeneralOptions(const char* optionString);
};

