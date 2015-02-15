/*
 * ConfigReader.cpp
 *
 *  Created on: May 27, 2014
 *      Author: wzw7yn
 */
#include <exception>
//#include <stdio.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <streambuf>
#include <algorithm>
#include <string>
#include <map>
#include <cstdlib>
#include "ConfigReader.h"
#include "CLogger.h"


namespace simplearchive {

void Config::printAll() {
	int size = this->size();
	// &logger = CLogger::getLogger();
	for (std::map<std::string, std::string>::iterator ii = begin(); ii != end(); ++ii) {
		//logger.log(CLogger::FINE, "Key: \"%s\" Value: \"%s\"\n", ii->first, ii->second);
		//std::cout << ii->first << " " << ii->second << '\n';
	}
}

bool Config::value(const char *key, std::string &value) {
	std::map<std::string, std::string>::iterator it;

	if ((it = find(key)) == end()) {
		return false;
	}
	value = it->second;
	return true;
}

ConfigReader::ConfigReader() {
	m_includeCnt = 0;
	m_delimChar = '=';
	m_logging = true;
}

ConfigReader::~ConfigReader() {
	// TODO Auto-generated destructor stub
}

std::string ConfigReader::includePath(int pos, std::string line) {

	int s = line.find_first_of("\"");
	int e = line.find_last_of("\"");
	m_path = line.substr(s+1,e-(s+1));
	//printf("path:%s\n", m_path.c_str());
	return m_path;
}

bool ConfigReader::read(const char *datafile, Config &config) {

	
	char text[256];
	m_path = datafile;
	std::ifstream file(datafile);
	if (file.is_open() == false) {
		return false;
	}

	while (file.getline(text, 1000)) {
		//m_dataContainer->push_back(*(new std::string(text)));
		switch(parse(text, config)) {
		case KeyValue:
			continue;
		case Comment:
			continue;
		case Include:
			m_includeCnt++;
			if (m_includeCnt >= 10) {
				if (m_logging) {
					CLogger::getLogger().log(CLogger::ERROR, "include files more than ten deep \"%s\"", m_path.c_str());
				}
				return false;
			}
			if (read(m_path.c_str(), config) == false) {
				m_includeCnt--;
				if (m_logging) {
					CLogger::getLogger().log(CLogger::WARNING, "Cannot include file \"%s\"", m_path.c_str());
				}
				return false;
			}
			continue;
		case Error:
			return false;
		default:
			if (m_logging) {
				CLogger::getLogger().log(CLogger::WARNING, "Cannot read token in config file \"%s\"", m_path.c_str());
			}
			return false;
		}
	}
	//std::cout << config.size() << std::endl;
	file.close();

	return true;
}

#ifdef WIN32
static std::string trim(std::string const& str)
{
	std::size_t first = str.find_first_not_of(' ');

	// If there is no non-whitespace character, both first and last will be std::string::npos (-1)
	// There is no point in checking both, since if either doesn't work, the
	// other won't work, either.
	if (first == std::string::npos)
		return "";

	std::size_t last = str.find_last_not_of(' ');

	return str.substr(first, last - first + 1);
}
#else
static inline std::string &ltrim(std::string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
	return s;
}

static inline std::string &rtrim(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	return s;
}
#endif

/*
Error = -1,
		KeyValue,
		Comment,
		Include
 */



ConfigReader::Token ConfigReader::parse(const char *text, Config &config) {
	std::string line = text;
	line = trim(line);

	if (line.empty()) {
		return Comment;
	}
	int commentIdx = line.find_first_of('#');
	if (commentIdx == 0) {
		//printf("%s\n", line.c_str());
		return Comment; // comment before command
	}
	
	// config.getDelimChar()
	int delimIdx = line.find_first_of(config.getDelimChar());
	if (delimIdx == -1) {
		std::string include("include");
		std::size_t  includeIdx = line.find("include");
		if (includeIdx != (std::size_t)-1) {

			includePath(includeIdx, line);
			return Include;
		} else {
			if (m_logging) {
				CLogger::getLogger().log(CLogger::WARNING, "Cannot read \"%s\" in config file \"%s\"", line.c_str(), m_path.c_str());
			}
			return Error;
		}
	}
	std::string cmd = line.substr(0, delimIdx);
	std::string option = line.substr(delimIdx+1, line.length());
	if ((commentIdx = option.find_first_of('#')) != -1) {
		option = option.substr(0, commentIdx);
	}

	cmd = trim(cmd);
	option = trim(option);
	//printf("cmd:\"%s\" opt:\"%s\"\n", cmd.c_str(), option.c_str());
	std::string cmdp(cmd);
	std::string optionp(option);
	config[(cmdp)] = (optionp);

	return KeyValue;
}



} /* namespace simplearchive */
