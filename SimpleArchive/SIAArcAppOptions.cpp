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
**	Filename	: SIAArcAppOptions.cpp
**	Author		: I.Ferguson
**	Version		: 1.000
**	Date		: 26-05-2015
**
** #$$@@$$# */

#include "ConfigReader.h"
#include "SIAArcAppOptions.h"
#include "AppConfig.h"
#include "SAUtils.h"
#include "stdio.h"
#include "SIAArcArgvParser.h"
#include "Environment.h"
#include "EnvFunc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif


using namespace CommandLineProcessing;
namespace simplearchive {

	

bool SIAArcAppOptions::m_list = false;
bool SIAArcAppOptions::m_usingFile = false;
bool SIAArcAppOptions::m_peekOnly = false;
bool SIAArcAppOptions::m_eventsOn = false; // UDP events
bool SIAArcAppOptions::m_serverOn = true; // false;
bool SIAArcAppOptions::m_forceDate = false; // false;
bool SIAArcAppOptions::m_force = false; // false;
bool SIAArcAppOptions::m_useDate = false;
bool SIAArcAppOptions::m_useFileDate = false;
bool SIAArcAppOptions::m_useDateToday = false;
bool SIAArcAppOptions::m_sync = false;
ExifDate SIAArcAppOptions::m_archiveDate;

std::string SIAArcAppOptions::m_name;
SIAArcAppOptions::CommandMode SIAArcAppOptions::m_commandMode = SIAArcAppOptions::CommandMode::CM_Unknown;
std::string SIAArcAppOptions::m_comment;
std::string SIAArcAppOptions::m_imageAddress;
std::string SIAArcAppOptions::m_distinationPath;
std::string SIAArcAppOptions::m_filePath;
std::string SIAArcAppOptions::m_version = "last";
std::string SIAArcAppOptions::m_FromDate;
std::string SIAArcAppOptions::m_ToDate;

int SIAArcAppOptions::m_udpPortNum = 64321;
std::string SIAArcAppOptions::m_udpAddress = "127.0.0.1";
int SIAArcAppOptions::m_tcpPortNum = 64322;
LogDocument::FormatType SIAArcAppOptions::m_formatType = LogDocument::FormatType::Human;


SIAArcAppOptions::SIAArcAppOptions() : m_argvParser(std::make_unique<SIAArcArgvParser>()) {
	m_usingFile = false;
	m_archiveDate.now();
};


DefaultArgumentsContainer SIAArcAppOptions::defaultArgumentsContainer;


SIAArcAppOptions::~SIAArcAppOptions() {}



SIAArcAppOptions::CommandMode SIAArcAppOptions::getCommandMode() {
	return m_commandMode;
}

bool SIAArcAppOptions::setCommandMode(const char *modeString) {
	std::string mode = modeString;

	if (mode.compare("add") == 0) {
		m_commandMode = CommandMode::CM_Import;
		return true;
	}
	else if (mode.compare("add") == 0) {
		m_commandMode = CommandMode::CM_Export;
		return true;
	}
	else if (mode.compare("add") == 0) {
		m_commandMode = CommandMode::CM_Checkout;
		return true;
	}
	else if (mode.compare("add") == 0) {
		m_commandMode = CommandMode::CM_Checkin;
		return true;
	}
	else if (mode.compare("add") == 0) {
		m_commandMode = CommandMode::CM_Uncheckin;
		return true;
	}
	m_commandMode = CommandMode::CM_Unknown;
	//CommandLineProcessing::AppOptions::setError(12, "Unable to start, Command unknown .");
	return false;
}

void SIAArcAppOptions::setCommandMode(const SIAArcAppOptions::CommandMode mode) {
	m_commandMode = mode;
}

const char *SIAArcAppOptions::getName() {
	return m_name.c_str();
}

void SIAArcAppOptions::setName(const char *name) {
	m_name = name;
}

const char *SIAArcAppOptions::getComment() {
	return m_comment.c_str();
}
const char *SIAArcAppOptions::getImageAddress() {
	return m_imageAddress.c_str();
}
const char *SIAArcAppOptions::getDistinationPath() {
	return m_distinationPath.c_str();
}
const char *SIAArcAppOptions::getFilePath() {
	return m_filePath.c_str();
}

const char* SIAArcAppOptions::getFromDate()
{
	return m_FromDate.c_str();
}

const char* SIAArcAppOptions::getToDate()
{
	return m_ToDate.c_str();
}

bool SIAArcAppOptions::isEnventsOn() {
	return m_eventsOn;
}

int SIAArcAppOptions::eventPort() {
	return m_udpPortNum;
	
}
const char *SIAArcAppOptions::eventAddress() {
	return m_udpAddress.c_str();
}

bool SIAArcAppOptions::isServerOn() {
	return m_serverOn;
}

int SIAArcAppOptions::serverPort() {
	return m_tcpPortNum;

}

bool SIAArcAppOptions::getUseFileDate() {
	return m_useFileDate;
}

bool SIAArcAppOptions::getUseDateToday() {
	return m_useDateToday;
}

bool SIAArcAppOptions::isDataForced() {
	return m_forceDate;
}

bool SIAArcAppOptions::isForced() {
	return m_force;
}

bool SIAArcAppOptions::isSync()
{
	return m_sync;
}

ExifDate &SIAArcAppOptions::getArchiveDate() {
	return m_archiveDate;
}

void SIAArcAppOptions::setDefaultArguments(std::string s) {
	defaultArgumentsContainer.push_back(s);
}

DefaultArgumentsContainer& SIAArcAppOptions::getDefaultArguments() {
	return defaultArgumentsContainer;
}

LogDocument::FormatType& SIAArcAppOptions::getFormatType() {
	return m_formatType;
}

} /* namespace simplearchive */
