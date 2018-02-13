/* **************************************************
**
**    III                DDD  KKK
**    III                DDD  KKK
**                       DDD  KKK
**    III   DDDDDDDDDDD  DDD  KKK            KKK
**    III  DDD           DDD  KKK            KKK
**    III  DDD           DDD  KKK           KKK
**    III  DDD           DDD  KKK        KKKKKK
**    III  DDD           DDD  KKK   KKKKKKKKK
**    III  DDD           DDD  KKK        KKKKKK
**    III  DDD           DDD  KKK           KKK
**    III  DDD           DDD  KKK            KKK
**    III   DDDDDDDDDDDDDDDD  KKK            KKK
**
**
**     SSS         FF
**    S           F   T
**     SSS   OO   FF  TTT W   W  AAA  R RR   EEE
**        S O  O  F   T   W W W  AAAA RR  R EEEEE
**    S   S O  O  F   T   W W W A   A R     E
**     SSS   OO  FFF   TT  W W   AAAA R      EEE
**
**    Copyright: (c) 2015 IDK Software Ltd
**
****************************************************
**
**	Filename	: CRegString.cpp
**	Author		: I.Ferguson
**	Version		: 1.000
**	Date		: 26-05-2015
**
** #$$@@$$# */

#include <exception>
//#include <stdio.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <streambuf>
#include <sstream>
#include <algorithm>
#include <string>
#include <map>
#include <cstdlib>
#include "ConfigReader.h"
#include "siaglobal.h"
#include "CLogger.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif


namespace simplearchive {

void ConfigBlock::printAll() {
	int size = this->size();
	std::cout << m_name << '\n';
	// &logger = CLogger::getLogger();
	for (std::map<std::string, std::string>::iterator ii = begin(); ii != end(); ++ii) {
		
		std::cout << "   " << ii->first << " " << ii->second << '\n';
			//printf("cmd:\"%s\" opt:\"%s\"\n", (*ii).first.c_str(), (*ii).second.c_str());
		
	}
	
}

std::shared_ptr<ConfigBlock> AppConfigBase::getConfigBlocks(const char *name) {
	std::map<std::string, std::shared_ptr<ConfigBlock>>::iterator it;
	if ((it = find(name)) != end()) {
		std::shared_ptr<ConfigBlock> configBlock = it->second;
		return configBlock;
	}
	return nullptr;
}


bool AppConfigBase::setConfigBlock(const char* name, std::string &value, std::string &defaultValue, const char *configBlock) {
	iterator tmp = find(configBlock);
	if (tmp != end()) {
		std::shared_ptr<ConfigBlock> cb = find(configBlock)->second;
		if (cb != nullptr && (cb->value(name, value) == true)) {
			return true;
		}
	}
	value = defaultValue;
	return false;

}

void AppConfigBase::printAll() {
	for (auto ii = begin(); ii != end(); ++ii) {
		ConfigBlock &configBlock = *ii->second;
		configBlock.printAll();
	}
}

bool ConfigBlock::value(const char *key, std::string &value) {
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
	m_currentLineNumber = 0;
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

bool ConfigReader::read(const char *datafile, ConfigBlock &config) {


	std::string text;
	m_path = datafile;
	std::ifstream file(datafile);
	if (file.is_open() == false) {
		return false;
	}

	while (std::getline(file, text)) {
		//m_dataContainer->push_back(*(new std::string(text)));
		switch(parse(text.c_str(), config)) {
		case KeyValue:
			continue;
		case Comment:
			continue;
		case Include:
			m_includeCnt++;
			if (m_includeCnt >= 10) {
				if (m_logging) {
					CLogger::getLogger().log(LOG_OK, CLogger::Level::ERR, "include files more than ten deep \"%s\"", m_path.c_str());
				}
				return false;
			}
			if (read(m_path.c_str(), config) == false) {
				m_includeCnt--;
				if (m_logging) {
					CLogger::getLogger().log(LOG_OK, CLogger::Level::WARNING, "Cannot include file \"%s\"", m_path.c_str());
				}
				return false;
			}
			continue;
		case Error:
			return false;
		default:
			if (m_logging) {
				CLogger::getLogger().log(LOG_OK, CLogger::Level::WARNING, "Cannot read token in config file \"%s\"", m_path.c_str());
			}
			return false;
		}
	}
	//std::cout << config.size() << std::endl;
	file.close();

	return true;
}

bool ConfigReader::readExif(const std::string &str, ConfigBlock &config) {

	std::stringstream ss(str);
	std::string text;
	while (std::getline(ss, text, '\n')){

		//m_dataContainer->push_back(*(new std::string(text)));
		switch (parseExif(text.c_str(), config)) {
		case KeyValue:
			continue;
		case Comment:
			continue;
		//case Error:
		//	return false;
		default:
			if (m_logging) {
				CLogger::getLogger().log(LOG_OK, CLogger::Level::WARNING, "Cannot read token in config file \"%s\"", m_path.c_str());
			}
			return false;
		}
	}
	return true;
}

bool ConfigReader::read(const std::string &str, ConfigBlock &config) {

	std::stringstream ss(str);
	std::string text;


	//while (ss.getline(text, 2 * 1024)) {
	while (std::getline(ss, text, '\n')){

		//m_dataContainer->push_back(*(new std::string(text)));
		switch (parse(text.c_str(), config)) {
		case KeyValue:
			continue;
		case Comment:
			continue;
		case Include:
			m_includeCnt++;
			if (m_includeCnt >= 10) {
				if (m_logging) {
					CLogger::getLogger().log(LOG_OK, CLogger::Level::ERR, "include files more than ten deep \"%s\"", m_path.c_str());
				}
				return false;
			}
			if (read(m_path.c_str(), config) == false) {
				m_includeCnt--;
				if (m_logging) {
					CLogger::getLogger().log(LOG_OK, CLogger::Level::WARNING, "Cannot include file \"%s\"", m_path.c_str());
				}
				return false;
			}
			continue;
		case Error:
			return false;
		default:
			if (m_logging) {
				CLogger::getLogger().log(LOG_OK, CLogger::Level::WARNING, "Cannot read token in config file \"%s\"", m_path.c_str());
			}
			return false;
		}
	}
	

	return true;
}

bool AppConfigReader::read(const char *datafile, AppConfigBase &config) {


	std::string text;
	m_path = datafile;
	std::ifstream file(datafile);

	if (file.is_open() == false) {
		return false;
	}

	m_current = config.find(ROOT_BLOCK)->second;
	while (std::getline(file, text)) {
		//m_dataContainer->push_back(*(new std::string(text)));
		m_currentLineNumber++;
		switch (parse(text.c_str(), (*m_current))) {
		case KeyValue:
			continue;
		case Comment:
			continue;
		case Include:
			m_includeCnt++;
			if (m_includeCnt >= 10) {
				if (m_logging) {
					CLogger::getLogger().log(LOG_OK, CLogger::Level::ERR, "include files more than ten deep \"%s\"", m_path.c_str());
				}
				return false;
			}
			if (read(m_path.c_str(), config) == false) {
				m_includeCnt--;
				if (m_logging) {
					CLogger::getLogger().log(LOG_OK, CLogger::Level::WARNING, "Cannot include file \"%s\"", m_path.c_str());
				}
				return false;
			}
			continue;
		case NewBlock:
			m_current = std::make_shared<ConfigBlock>(m_blockName.c_str());
			config.insert(std::pair<std::string, std::shared_ptr<ConfigBlock>>(m_blockName, m_current));
			continue;
		case Error:
			return false;
		default:
			if (m_logging) {
				CLogger::getLogger().log(LOG_OK, CLogger::Level::WARNING, "Cannot read token in config file \"%s\"", m_path.c_str());
			}
			return false;
		}
	}
	//std::cout << config.size() << std::endl;
	file.close();

	return true;
}



static std::string trim(std::string const& str, char c)
{
	std::size_t first = str.find_first_not_of(c);

	// If there is no non-whitespace character, both first and last will be std::string::npos (-1)
	// There is no point in checking both, since if either doesn't work, the
	// other won't work, either.
	if (first == std::string::npos)
		return "";

	std::size_t last = str.find_last_not_of(c);
	std::size_t lastcr = str.find_last_not_of('\r');
	if (lastcr < last) {
		return str.substr(first, lastcr - first + 1);
	}
	return str.substr(first, last - first + 1);
}

static std::string trim(std::string const& str)
{
	std::string tmp = trim(str, ' ');
	return trim(tmp, '\"');
}
/*
static inline std::string &ltrim(std::string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
	return s;
}

static inline std::string &rtrim(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	return s;
}
*/

/*
Error = -1,
		KeyValue,
		Comment,
		Include
 */
ConfigReader::Token ConfigReader::parseExif(const char *text, ConfigBlock &config) {
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
	
	int delimIdx = line.find_first_of(config.getDelimChar());
	if (delimIdx == -1) {
		if (m_logging) {
			CLogger::getLogger().log(LOG_OK, CLogger::Level::WARNING, "Cannot read \"%s\" in config file \"%s\"", line.c_str(), m_path.c_str());
		}
		return Error;
	}
	std::string cmd = line.substr(0, delimIdx);
	std::string option = line.substr(delimIdx + 1, line.length());
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


ConfigReader::Token ConfigReader::parse(const char *text, ConfigBlock &config) {
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
	int blockStartIdx = line.find_first_of('[');
	if (blockStartIdx == 0) {
		//printf("%s\n", line.c_str());
		int blockEndIdx = line.find_last_of(']');
		m_blockName = line.substr(blockStartIdx+1, blockEndIdx-1);
		return NewBlock; // comment before command
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
				CLogger::getLogger().log(LOG_OK, CLogger::Level::WARNING, "Cannot read \"%s\" in config file \"%s\"", line.c_str(), m_path.c_str());
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

/*
ConfigReader::Token AppConfigReader::parse(const char *text, AppConfig &config) {
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
	int blockStartIdx = line.find_first_of('[');
	if (blockStartIdx == 0) {
		//printf("%s\n", line.c_str());
		int blockEndIdx = line.find_last_of(']');
		m_blockName = line.substr(blockStartIdx, blockEndIdx);
		return NewBlock; // comment before command
	}
	// config.getDelimChar()
	int delimIdx = line.find_first_of(config.getDelimChar());
	if (delimIdx == -1) {
		std::string include("include");
		std::size_t  includeIdx = line.find("include");
		if (includeIdx != (std::size_t) - 1) {

			includePath(includeIdx, line);
			return Include;
		}
		else {
			if (m_logging) {
				CLogger::getLogger().log(LOG_OK, CLogger::Level::WARNING, "Cannot read \"%s\" in config file \"%s\"", line.c_str(), m_path.c_str());
			}
			return Error;
		}
	}
	std::string cmd = line.substr(0, delimIdx);
	std::string option = line.substr(delimIdx + 1, line.length());
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
*/

ConfigWriter::ConfigWriter() {}
ConfigWriter::~ConfigWriter() {}

bool ConfigWriter::edit(const char *cmd, const char *options, ConfigBlock &config) {
	for (std::map<std::string, std::string>::iterator ii = config.begin(); ii != config.end(); ++ii) {
		//std::cout << ii->first << '\n';
		if (ii->first.compare(cmd) == 0) {
			ii->second = options;
		}
	}
	return true;
}

bool ConfigWriter::add(const char *cmd, const char *options, ConfigBlock &config) {
	for (std::map<std::string, std::string>::iterator ii = config.begin(); ii != config.end(); ++ii) {
		//std::cout << ii->first << '\n';
		if (ii->first.compare(cmd) == 0) {
			std::cout << ii->first << '\n';
			return false;
		}
	}
	std::string cmdp(cmd);
	std::string optionp(options);
	config[(cmdp)] = (optionp);
	return true;
}

bool ConfigWriter::remove(const char *cmd, ConfigBlock &config) {
	for (std::map<std::string, std::string>::iterator ii = config.begin(); ii != config.end(); ++ii) {
		//std::cout << ii->first << '\n';
		if (ii->first.compare(cmd) == 0) {
			config.erase(ii++);
			return true;
		}
	}
	return false;
}

bool ConfigWriter::write(const char *datafile, ConfigBlock &config) {
	std::ofstream file(datafile, std::ifstream::trunc);
	if (file.is_open() == false) {
		return false;
	}
	for (std::map<std::string, std::string>::iterator ii = config.begin(); ii != config.end(); ++ii) {
		file << ii->first << "=" << ii->second << '\n';
		//std::cout << ii->first << "=" << ii->second << '\n';
	}
	return true;
}

} /* namespace simplearchive */
