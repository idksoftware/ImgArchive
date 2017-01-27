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

#include "ConfigReader.h"
#include "AdminAppOptions.h"
#include "AppConfig.h"
#include "SAUtils.h"
#include "stdio.h"
#include "SIAArgvParser.h"
#include "Environment.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

using namespace CommandLineProcessing;
namespace simplearchive {

AppOptions *AppOptions::m_this = 0;
std::string AppOptions::m_name;
AppOptions::CommandMode AppOptions::m_commandMode = AppOptions::CM_Unknown;
std::string AppOptions::m_comment;
std::string AppOptions::m_imageAddress;
std::string AppOptions::m_homePath;
std::string AppOptions::m_workspacePath;
std::string AppOptions::m_shadowPath;
std::string AppOptions::m_configPath;
AppOptions::Scope AppOptions::m_scope = AppOptions::Both;
bool AppOptions::m_repair = false;
bool AppOptions::m_users = true;


AppOptions &AppOptions::get() {
	if (m_this == nullptr) {
		m_this = new AppOptions;
	}
	return *m_this;
}

AppOptions::AppOptions() {
	m_configured = true;
}

bool AppOptions::initaliseConfig() {

	CAppConfig &config = CAppConfig::get();
	const std::string key = "SIA_HOME";
	std::string temp = SAUtils::GetEnvironment(key);
	std::string homePath = temp;
	//printf("%s", homePath.c_str());
	int i = homePath.length();
	if (homePath.empty() == true || homePath.length() == 0) {
		homePath = SAUtils::GetEnvironment("ProgramData");
		//C:\ProgramData\IDK Software\ImageArchive1.0
		homePath += "/IDK Software/ImageArchive1.0";

	}
	std::string configfile = homePath + "/config/" + "config.dat";
	std::string configPath = homePath + "/config";
	if (SAUtils::DirExists(homePath.c_str()) == false) {
		//printf("SIA Unable to start?\nArchive not found at default location and the environment variable SA_HOME not set.\n"
		//	"Use siaadmin -i to create an empty archive at the default location (see documentation).\n");
		//m_error = true;
		//return false;
		m_configured = false;
	}
	else {
		
		if (SAUtils::FileExists(configfile.c_str()) == true) {
			setConfigPath(configPath.c_str());
			ConfigReader configReader;
			configReader.setNoLogging();
			configReader.read(configfile.c_str(), config);
			// This is usfull to print the config
			//config.printAll();
			/*
			if (config.value("SourcePath", temp) == true) {
			m_sourcePath = temp;
			}
			if (config.value("ArchivePath", temp) == true) {
			m_archivePath = temp;
			}
			if (config.value("LogLevel", temp) == true) {
			m_logLevel = temp;
			}
			*/
			config.setHomePath(homePath.c_str());
			temp = SAUtils::GetEnvironment("SIA_WORKSPACE");
			if (temp.empty() == false) {
				config.setWorkspacePath(temp.c_str());
			}
			temp = SAUtils::GetEnvironment("SIA_SOURCE");
			if (temp.empty() == false) {
				config.setSourcePath(temp.c_str());
			}
			temp = SAUtils::GetEnvironment("SIA_LOGLEVEL");
			if (temp.empty() == false) {
				config.setLogLevel(temp.c_str());
			}
		}
		else {
			m_configured = false;
		}
	}
	return true;
}



AppOptions::~AppOptions() {
	// TODO Auto-generated destructor stub
}


AppOptions::CommandMode AppOptions::getCommandMode() {
	return m_commandMode;
}

bool AppOptions::setCommandMode(const char *modeString) {
	std::string mode = modeString;

	/*
	if (mode.compare("add") == 0) {
		m_commandMode = CM_Import;
		return true;
	}
	else if (mode.compare("add") == 0) {
		m_commandMode = CM_Export;
		return true;
	}
	else if (mode.compare("add") == 0) {
		m_commandMode = CM_Checkout;
		return true;
	}
	else if (mode.compare("add") == 0) {
		m_commandMode = CM_Checkin;
		return true;
	}
	else if (mode.compare("add") == 0) {
		m_commandMode = CM_Uncheckin;
		return true;
	}
	*/
	m_commandMode = CM_Unknown;
	return false;
}

void AppOptions::setCommandMode(const AppOptions::CommandMode mode) {
	m_commandMode = mode;
}

bool AppOptions::isAllUsers() {
	return m_users;
}

void AppOptions::setAllUsers(bool allUsers) {
	m_users = allUsers;
}

const char *AppOptions::getName() {
	return m_name.c_str();
}

void AppOptions::setName(const char *name) {
	m_name = name;
}

void AppOptions::setHomePath(const char *homePath) {
	m_homePath = homePath;
}

void AppOptions::setWorkspacePath(const char *workspacePath) {
	m_workspacePath = workspacePath;
}
void AppOptions::setShadowPath(const char *shadowPath) {
	m_shadowPath = shadowPath;
}

void AppOptions::setConfigPath(const char *configPath) {
	m_configPath = configPath;
}

const char *AppOptions::getComment() {
	return m_comment.c_str();
}
const char *AppOptions::getImageAddress() {
	return m_imageAddress.c_str();
}

const char *AppOptions::getHomePath() {
	return m_homePath.c_str();
}

const char *AppOptions::getWorkspacePath() {
	return m_workspacePath.c_str();
}
const char *AppOptions::getShadowPath() {
	return m_shadowPath.c_str();
}
const char *AppOptions::getConfigPath() {
	return m_configPath.c_str();
}
void setConfigPath(const char *configPath);

} /* namespace simplearchive */
