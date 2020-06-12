
#include <vector>
#include "ParseOptions.h"
#include "SAUtils.h"

OutputType::Type OutputType::get() {
	return m_type;
}

bool OutputType::parse(const char* str) {
	std::string arg = str;

	if (arg.compare("plain") == 0) {
		m_type = Type::plain;
		return true;
	}
	if (arg.compare("xml") == 0) {
		m_type = Type::xml;
		return true;
	}
	if (arg.compare("json") == 0) {
		m_type = Type::json;
		return true;
	}
	if (arg.compare("html") == 0) {
		m_type = Type::html;
		return true;
	}
	m_type = Type::unknown;
	return false;
}

bool ParseOptions::iequals(const std::string& a, const std::string& b)
{
	return std::equal(a.begin(), a.end(),
		b.begin(), b.end(),
		[](char a, char b) {
			return tolower(a) == tolower(b);
		});
}

// check if given string is a numeric string or not
bool ParseOptions::isNumber(const std::string& str)
{
	// std::find_first_not_of searches the string for the first character 
	// that does not match any of the characters specified in its arguments
	return !str.empty() &&
		(str.find_first_not_of("[0123456789]") == std::string::npos);
}

// Function to split string str using given delimiter
std::vector<std::string> ParseOptions::split(const std::string& str, char delim)
{
	auto i = 0;
	std::vector<std::string> list;

	auto pos = str.find(delim);

	while (pos != std::string::npos)
	{
		list.push_back(str.substr(i, pos - i));
		i = ++pos;
		pos = str.find(delim, pos);
	}

	list.push_back(str.substr(i, str.length()));

	return list;
}

// Function to validate an IP address
bool ParseOptions::validateIP(std::string ip)
{
	// split the string into tokens
	std::vector<std::string> list = split(ip, '.');

	// if token size is not equal to four
	if (list.size() != 4)
		return false;

	// validate each token
	for (std::string str : list)
	{
		// verify that string is number or not and the numbers 
		// are in the valid range
		if (!isNumber(str) || stoi(str) > 255 || stoi(str) < 0)
			return false;
	}

	return true;
}

bool ParseOptions::checkIPAddress(const char* ip) {

	if (validateIP(ip) == false) {
		return false;
	}
	return true;
}


bool ParseOptions::checkPath(const char* path) {

	if (SAUtils::FileExists(path) == false) {
		return false;
	}
	return true;
}

bool ParseOptions::checkPath() {

	if (SAUtils::FileExists(m_value.c_str()) == false) {
		m_error = Error::Path_not_found;
		return false;
	}
	return true;
}

bool ParseOptions::checkBool() {
	bool ret = (SAUtils::isTrueFalse(m_value) != BoolOption::Invalid);
	if (ret) {
		m_error = Error::Invalid_argument;
	}
	return ret;
}

bool ParseOptions::checkNumber() {
	bool ret = SAUtils::isNumber(m_value);
	if (!ret) {
		m_error = Error::Invalid_argument;
	}
	return ret;
}

bool ParseOptions::checkIPAddress()
{
	bool ret = validateIP(m_value);
	if (ret) {
		m_error = Error::Invalid_argument;
	}
	return ret;
}

bool ParseOptions::processArgs(const char* ov) {
	std::string optionValueString = ov;
	size_t pos = optionValueString.find_first_of('=');
	if (pos == std::string::npos) {
		return false;
	}
	m_option = optionValueString.substr(0, pos);
	m_value = optionValueString.substr(pos + 1, optionValueString.length() - 1);

	return true;
}
