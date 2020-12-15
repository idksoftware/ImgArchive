#pragma once

#include <string>
#include "ParseOptions.h"

enum class Option {
	MASTER,
	DERIVATIVE
};

class IAParseOptions : public ParseOptions
{
	bool m_isMaster{ true };
	bool m_isCurrent{ true };
public:
	IAParseOptions() = default;
	virtual ~IAParseOptions() = default;
	bool parseArchiveOptions(const char* optionString);
	bool isMaster() const { return m_isMaster; }
	bool parseCurrentOptions(const char* optionString);
	bool isCurrent() const { return m_isCurrent; }
};



