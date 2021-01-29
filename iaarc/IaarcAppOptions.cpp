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
**	Filename	: IaarcAppOptions.cpp
**	Author		: I.Ferguson
**	Version		: 1.000
**	Date		: 26-05-2015
**
** #$$@@$$# */

#include "ConfigReader.h"
#include "IaarcAppOptions.h"
#include "AppConfig.h"
#include "SAUtils.h"
#include "stdio.h"
#include "IaarcArgvParser.h"
#include "Environment.h"
#include "EnvFunc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

using namespace CommandLineProcessing;
namespace simplearchive {

bool IaarcAppOptions::m_list = false;
bool IaarcAppOptions::m_usingFile = false;
bool IaarcAppOptions::m_peekOnly = false;
bool IaarcAppOptions::m_eventsOn = false; // UDP events
bool IaarcAppOptions::m_serverOn = true; // false;
bool IaarcAppOptions::m_forceDate = false; // false;
bool IaarcAppOptions::m_force = false; // false;
bool IaarcAppOptions::m_useDate = false;
bool IaarcAppOptions::m_useFileDate = false;
bool IaarcAppOptions::m_useDateToday = false;
bool IaarcAppOptions::m_sync = false;
bool IaarcAppOptions::m_current = false;
bool IaarcAppOptions::m_master = false;

ExifDate IaarcAppOptions::m_archiveDate;

std::string IaarcAppOptions::m_name;
IaarcAppOptions::CommandMode IaarcAppOptions::m_commandMode = IaarcAppOptions::CommandMode::CM_Unknown;
std::string IaarcAppOptions::m_comment;
std::string IaarcAppOptions::m_imageAddress;
std::string IaarcAppOptions::m_distinationPath;
std::string IaarcAppOptions::m_filePath;
std::string IaarcAppOptions::m_version = "last";
std::string IaarcAppOptions::m_FromDate;
std::string IaarcAppOptions::m_ToDate;
std::string IaarcAppOptions::m_option;
std::string IaarcAppOptions::m_value;
std::string IaarcAppOptions::m_outputFile;
std::string IaarcAppOptions::m_textOutputType;
IaarcAppOptions::ShowCommandOption IaarcAppOptions::m_showCommandOption;

int IaarcAppOptions::m_udpPortNum = 64321;
std::string IaarcAppOptions::m_udpAddress = "127.0.0.1";
int IaarcAppOptions::m_tcpPortNum = 64322;
ResultsPresentation::FormatType IaarcAppOptions::m_formatType = ResultsPresentation::FormatType::Human;


IaarcAppOptions::IaarcAppOptions() : m_argvParser(std::make_unique<IaarcArgvParser>()) {
	m_usingFile = false;
	m_archiveDate.now();
};


DefaultArgumentsContainer IaarcAppOptions::defaultArgumentsContainer;


IaarcAppOptions::~IaarcAppOptions() {}



IaarcAppOptions::CommandMode IaarcAppOptions::getCommandMode() {
	return m_commandMode;
}

bool IaarcAppOptions::setCommandMode(const char *modeString) {
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

void IaarcAppOptions::setCommandMode(const IaarcAppOptions::CommandMode mode) {
	m_commandMode = mode;
}

const char *IaarcAppOptions::getName() {
	return m_name.c_str();
}

void IaarcAppOptions::setName(const char *name) {
	m_name = name;
}

const char *IaarcAppOptions::getComment() {
	return m_comment.c_str();
}
const char *IaarcAppOptions::getImageAddress() {
	return m_imageAddress.c_str();
}
const char *IaarcAppOptions::getDistinationPath() {
	return m_distinationPath.c_str();
}
const char *IaarcAppOptions::getFilePath() {
	return m_filePath.c_str();
}

const char* IaarcAppOptions::getFromDate()
{
	return m_FromDate.c_str();
}

const char* IaarcAppOptions::getToDate()
{
	return m_ToDate.c_str();
}

const char* IaarcAppOptions::getOption()
{
	return m_option.c_str();
}

const char* IaarcAppOptions::getValue()
{
	return m_value.c_str();
}

bool IaarcAppOptions::isEnventsOn() {
	return m_eventsOn;
}

int IaarcAppOptions::eventPort() {
	return m_udpPortNum;
	
}
const char *IaarcAppOptions::eventAddress() {
	return m_udpAddress.c_str();
}

bool IaarcAppOptions::isServerOn() {
	return m_serverOn;
}

int IaarcAppOptions::serverPort() {
	return m_tcpPortNum;

}

bool IaarcAppOptions::getUseFileDate() {
	return m_useFileDate;
}

bool IaarcAppOptions::getUseDateToday() {
	return m_useDateToday;
}

bool IaarcAppOptions::isDataForced() {
	return m_forceDate;
}

bool IaarcAppOptions::isForced() {
	return m_force;
}

bool IaarcAppOptions::isSync()
{
	return m_sync;
}

bool IaarcAppOptions::isCurrent()
{
	return m_current;
}

bool IaarcAppOptions::isMaster()
{
	return m_master;
}

const char* IaarcAppOptions::getOutputFile()
{
	return m_outputFile.c_str();
}
const char* IaarcAppOptions::getTextOutputType()
{
	return m_textOutputType.c_str();
}

ExifDate &IaarcAppOptions::getArchiveDate() {
	return m_archiveDate;
}

void IaarcAppOptions::setDefaultArguments(std::string s) {
	defaultArgumentsContainer.push_back(s);
}

DefaultArgumentsContainer& IaarcAppOptions::getDefaultArguments() {
	return defaultArgumentsContainer;
}

ResultsPresentation::FormatType& IaarcAppOptions::getFormatType() {
	return m_formatType;
}


bool StatusOptions::parse(const char* optionString)
{
	if (iequals(optionString, STATUS_CHECKED_OUT)) {
		m_option = STATUS_CHECKED_OUT;
		IaarcAppOptions::m_showCommandOption = IaarcAppOptions::ShowCommandOption::SC_ShowCheckedOut;
		return true;
	}
	else if (iequals(optionString, STATUS_CHECKED_IN)) {
		m_option = STATUS_CHECKED_IN;
		IaarcAppOptions::m_showCommandOption = IaarcAppOptions::ShowCommandOption::SC_ShowCheckedIn;
		return true;
	}
	else if (iequals(optionString, STATUS_DELETED)) {
		m_option = STATUS_DELETED;
		return true;
	}
	return false;
}


} /* namespace simplearchive */
