#include "IAParseOptions.h"

bool IAParseOptions::parseTemplateOptions(const char* optionString)
{
	if (iequals(optionString, "master")) {
		m_isMaster = true;
	}
	else if (iequals(optionString, "derivative")) {
		m_isMaster = false;
	}
	
	else {
		return false;
	}
	return true;
}

