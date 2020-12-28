#pragma once

#include <string>
#include "ParseOptions.h"

class ParseProperties : public ParseOptions {
	Option process(std::string& optionString);
	
public:
	ParseProperties() = default;
	virtual ~ParseProperties() = default;
	bool parse(const char* optionString);
};


