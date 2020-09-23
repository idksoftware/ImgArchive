#include "JsonCommand.h"
#include <sstream>
#include <iterator>
#include <numeric>
#include <iostream>

JsonCommand::JsonCommand(std::string& inputString)
{
	std::istringstream stringStream(inputString);
	m_tokens = std::make_shared<std::vector<std::string>>((std::istream_iterator<std::string>(stringStream)), std::istream_iterator<std::string>());

	m_str = std::accumulate(std::next(m_tokens->begin()), m_tokens->end(),
		(*m_tokens)[0], // start with first element
		[](std::string a, std::string b) { return a + "" + b; });

	for (auto i = m_tokens->begin(); i != m_tokens->end(); i++) {
		std::cout << *i << std::endl;
	}
}

bool JsonCommand::trimBlock(std::string& str)
{
	if (trim('{', '}', str) == false) {
		return false;
	}

	return true;
}

void JsonCommand::trimString(std::string& str)
{
	if (trim('"', '"', str) == false) {
		return;
	}

	return;
}


bool JsonCommand::trim(char left, char right, std::string& str)
{
	if (str[0] != left || str[str.size() - 1] != right) {
		return false;
	}
	str = str.substr(1, str.length() - 2);
	return true;
}

bool JsonCommand::expression(std::string& str)
{
	std::size_t pos = str.find_first_of(":");
	if (pos == std::string::npos) {
		return false;
	}
	m_eLeft = str.substr(0, pos);
	m_eRight = str.substr(pos + 1, str.length() - pos + 1);
	return true;
}

JsonCommand::ExprType JsonCommand::expressionType(std::string& str)
{
	if (str[0] == '{' || str[str.size() - 1] == '}') {
		return JsonCommand::ExprType::Block;
	}
	if (str[0] == '[' || str[str.size() - 1] == ']') {
		return JsonCommand::ExprType::Array;
	}
	return JsonCommand::ExprType::Unknown;
}

bool JsonCommand::parseBlock(std::string& str)
{
	if (trimBlock(str) == false) {
		return false;
	}
	if (expression(str) == false) {
		return false;
	}
	trimString(m_eLeft);
	trimString(m_eRight);
	m_argumentList.push_back(Argument(m_eLeft, m_eRight));
	return true;
}

bool JsonCommand::splitArray(std::string& str)
{
	std::size_t e = 0;
	std::size_t s = 0;
	while (1) {
		if (str[e] != '{') {
			return false;
		}
		s = e;
		s = str.find_first_of("}", s);
		if (s == std::string::npos) {
			return false;
		}
		std::string exp = str.substr(e, s + 1);
		if (parseBlock(exp) == false) {
			return false;
		}
		s = str.find_first_of(",", s);
		if (s == std::string::npos) {
			return true;
		}
		e = ++s;
		s++;
	}
	return true;
}

bool JsonCommand::parseArray(std::string& str)
{
	if (trim('[', ']', str) == false) {
		return false;
	}
	splitArray(str);
	//trimString(m_eLeft);
	//trimString(m_eRight);
	//m_argumentList.push_back(Argument(m_eLeft, m_eRight));
}


bool JsonCommand::parse() {
	if (trimBlock(m_str) == false) {
		return false;
	}
	if (expression(m_str) == false) {
		return false;
	}
	trimString(m_eLeft);
	m_command = m_eLeft;
	std::string args = m_eRight;
	JsonCommand::ExprType type = expressionType(args);
	switch (type) {
	case JsonCommand::ExprType::Block:
		parseBlock(args);
		break;
	case JsonCommand::ExprType::Array:
		parseArray(args);
		break;
	case JsonCommand::ExprType::Unknown:
	default:
		return false;
	}
}