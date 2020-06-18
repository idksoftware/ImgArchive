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
	bool m_isCurrent;
public:
	IAParseOptions() : m_isMaster(true) {};
	virtual ~IAParseOptions() = default;
	bool parseArchiveOptions(const char* optionString);
	bool isMaster() { return m_isMaster; };
	bool parseCurrentOptions(const char* optionString);
	bool isCurrent() { return m_isCurrent; };
};


