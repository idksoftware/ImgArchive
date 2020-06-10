#pragma once

#include <string>
#include "ParseOptions.h"

enum class Option {
	MASTER,
	DERIVATIVE
};

class IAParseOptions : public ParseOptions
{
	bool m_isMaster;
public:
	IAParseOptions() : m_isMaster(true) {};
	virtual ~IAParseOptions() = default;
	bool parseTemplateOptions(const char* optionString);
	bool isMaster() { return m_isMaster; };
};

