#include "IAParseOptions.h"

bool IAParseOptions::parseArchiveOptions(const char* optionString)
{
	if (iequals(optionString, "master")) {
		m_isMaster = true;
	}
	else if (iequals(optionString, "derivative")) {
		m_isMaster = false;
	}
	else if (iequals(optionString, "m")) {
		m_isMaster = true;
	}
	else if (iequals(optionString, "d")) {
		m_isMaster = false;
	}
	else {
		return false;
	}
	return true;
}

bool IAParseOptions::parseCurrentOptions(const char* optionString)
{
	if (iequals(optionString, "current")) {
		m_isCurrent = true;
	}
	else if (iequals(optionString, "base")) {
		m_isCurrent = false;
	}
	else if (iequals(optionString, "c")) {
		m_isCurrent = true;
	}
	else if (iequals(optionString, "b")) {
		m_isCurrent = false;
	}
	else if (iequals(optionString, "y")) {
		m_isCurrent = true;
	}
	else if (iequals(optionString, "n")) {
		m_isCurrent = false;
	}
	else {
		return false;
	}
	return true;
}
