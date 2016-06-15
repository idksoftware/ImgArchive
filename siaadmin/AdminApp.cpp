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

#include <stdio.h>
#include <iostream>
#include "AdminApp.h"
#include "SAUtils.h"
#include "CDate.h"
#include "AppConfig.h"
#include "CLogger.h"
#include "ImageExtentions.h"
#include "TargetsList.h"
#include "CIDKDate.h"
#include "ImagePath.h"
#include "History.h"
#include "argvparser.h"
#include "HookCmd.h"
#include "MakeMedia.h"
#include "MirrorManager.h"
#include "ViewManager.h"
#include "CreateArchive.h"

#include "SummaryFile.h"
#include "IntegrityManager.h"
#include "SIALib.h"

#ifdef _WIN32
#pragma comment(lib, "ws2_32.lib")
#endif

#include <iostream>
using namespace std;

#include <string>
#include <vector>
#include "AdminAppOptions.h"
#include "EnvFunc.h"

#define VERSION	"1.00"
#define BUILD	"040115.1749"
#define DB "c:/temp/test3.db"
/*
java version "1.7.0_51"
Java(TM) SE Runtime Environment (build 1.7.0_51-b13)
Java HotSpot(TM) 64-Bit Server VM (build 24.51-b03, mixed mode)
*/
namespace simplearchive {
	
bool App::Show() {
	/*
	m_error = false;
	m_verbose = false;
	m_quiet = false;
	m_logLevel = "INFO";
	m_dry_run = false;
	*/
	CAppConfig &config = CAppConfig::get();
	/*
	const std::string key = "SIA_HOME";
	std::string temp = SAUtils::GetEnvironment(key);
	std::string homePath = temp;
	*/
	bool found = false;
	std::string homePath;
	// Looking the HKEY_LOCAL_MACHINE first
	if (GetEnv(homePath, true) == true) {
		printf("Found SIA_HOME in system variables: %s", homePath.c_str());
		found = true;
	}
	else if (GetEnv(homePath, false) == true) {
		//printf("Found SIA_HOME in user variables: %s", homePath.c_str());
		found = true;
	}
	else {
		bool found = false;
		homePath = SAUtils::GetEnvironment("USERPROFILE");
		if (homePath.empty() == true || homePath.length() == 0) {
			printf("SIA Unable to start? Cannot read user profile.");
			return false;
		}
		else {
			homePath += "/IDK Software/ImageArchive1.0";
			if (SAUtils::DirExists(homePath.c_str()) == true) {
				printf("Found SIA_HOME in user profile: %s", homePath.c_str());
				found = true;
			}
		}
		if (found == false) {
			homePath = SAUtils::GetEnvironment("ProgramData");
			if (homePath.empty() == true || homePath.length() == 0) {
				printf("SIA Unable to start? Cannot read all users profile.");
				return false;
			}
			homePath += "/IDK Software/ImageArchive1.0";
			if (SAUtils::DirExists(homePath.c_str()) == true) {
				printf("Found SIA_HOME in all users profile: %s", homePath.c_str());
				found = true;
			}
		}
	}
	if (found = false) {
		printf("SIA Unable to start? No archive found in the default location or"
			" the environment variable SIA_HOME not set.\nUse siaadmin to initalise an archive.\n");
		
		return false;
	}
	std::string configfile = homePath + "/config/" + "config.dat";
	if (SAUtils::FileExists(configfile.c_str()) == false) {
		
		printf("SIA Unable to start? No config.dat file found in the default location or"
			" the environment variable SIA_HOME not set.\nUse siaadmin to initalise an archive.\n");
		return false;
	}
	ConfigReader configReader;
	configReader.setNoLogging();
	configReader.read(configfile.c_str(), config);
	config.printAll();
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
	std::string temp;
	temp = SAUtils::GetEnvironment("SIA_ARCHIVE");
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
	CreateArchive::checkFolders(homePath.c_str());
}


int test(const std::string key) {
	return 0;
}

App::App() {
}

bool App::initalise(int argc, char **argv) {
	
	AppOptions &appOptions = AppOptions::get();	
	if (appOptions.initalise(argc, argv) == false) {
		
		return false;
	}
	
	return true;
};

bool App::Run()
{
	// Find if the archive exists
	AppOptions &appOptions = AppOptions::get();
	if (appOptions.isConfiguratedOk() == false) {
		if (appOptions.getCommandMode() == AppOptions::CM_Show) {
			Show();
			return false;
		}
		if (appOptions.getCommandMode() == AppOptions::CM_InitArchive) {

			if (CreateArchive(appOptions.getHomePath(), appOptions.getWorkspacePath(), appOptions.getShadowPath(), appOptions.getUsers()) == false) {
				return false;
			}
			printf("\n\Completed initalising the Archive\n");
			return true;
		}
		return false;
	}

	
	

	switch (appOptions.getCommandMode()) {
	case AppOptions::CM_InitArchive:
		break;
	case AppOptions::CM_Show:
	{
		CAppConfig &config = CAppConfig::get();
		config.printAll();
		break;
	}
	case AppOptions::CM_View:
	{
		SIALib siaLib;
		siaLib.initalise();
		CAppConfig &config = CAppConfig::get();
		if (siaLib.view(appOptions.getName()) == false) {
			return false;
		}
		siaLib.complete();
		break;
	}
	case AppOptions::CM_Mirror:
	{
		// make mirror
		SIALib siaLib;
		siaLib.initalise();
		CAppConfig &config = CAppConfig::get();
		if (siaLib.mirror(appOptions.getName()) == false) {
			return false;
		}
		// 

		siaLib.complete();
		break;
	}
	case AppOptions::CM_Validate:
	{
		SIALib siaLib;
		siaLib.initalise();
		CAppConfig &config = CAppConfig::get();
		if (appOptions.isConfiguratedOk() == false) {
			// Do not create a new archive. The old one needs to be deleted?
			return false;
		}
		
		switch (appOptions.getScope()) {
		case AppOptions::Workspace:
			if (siaLib.validate(config.getShadowPath(), config.getWorkspacePath(), config.getHomePath(), SIALib::Workspace, appOptions.repair()) == false) {
				return false;
			}

		case AppOptions::Shadow:			//* Show
			if (siaLib.validate(config.getShadowPath(), config.getWorkspacePath(), config.getHomePath(), SIALib::Shadow, appOptions.repair()) == false) {
				return false;
			}
		default:
			if (siaLib.validate(config.getShadowPath(), config.getWorkspacePath(), config.getHomePath(), SIALib::Both, appOptions.repair()) == false) {
				return false;
			}
		}
		siaLib.complete();
		break;
	}
	
	case AppOptions::CM_Version:
		printf("Simple Image Archive Administrator\n"
			   "siaadmin version \"%s\" (build %s)\n"
			   "Copyright@(2010-2016) IDK Sftware Ltd.\n", VERSION, BUILD);
		return true;
	case AppOptions::CM_Unknown:
		break;
	}

	
	
	return true;
}

App::~App() {
	CLogger::Close();
}

bool ok()
{
	cout << "OK" << endl;
	return(true);
}

bool failed()
{
	cout << "Failed" << endl;
	return(false);
}

bool App::CreateArchive(const char *archivePath, const char *workspacePath, const char *shadowPath, bool users) {

	if (users == true) {
		if (CreateArchive::IsAdmin() == false) {
			std::cout << "Failed you need administrator privileges" << '\n';
			return false;
		}
	}
		
	if (CreateArchive::createSystem(users, archivePath, workspacePath, shadowPath) == false) {
		std::cout << "Failed creating root folders" << '\n';
		return false;
	}
	if (CreateArchive::createHomeEnvVar(CreateArchive::getArchivePath().c_str(), users) == false) {
		std::cout << "Failed creating enviroment variable SIA_HOME" << '\n';
		return false;
	}
	
	if (CreateArchive::makeFolders(CreateArchive::getArchivePath().c_str()) == false) {
		std::cout << "Failed creating folders" << '\n';
		return false;
	}

	if (CreateArchive::createHookFiles(CreateArchive::getArchivePath().c_str(), HOOKS_PATH) == false) {
		std::cout << "Failed creating hook files" << '\n';
		return false;
	}

	if (CreateArchive::createConfigFiles(CreateArchive::getArchivePath().c_str(), CONFIG_PATH, CreateArchive::getWorkspace().c_str(), CreateArchive::getShadow().c_str()) == false) {
		std::cout << "Failed creating configuration files" << '\n';
		return false;
	}
	return true;
}

} // simplearchive


/**
 *
 *
 */

int main(int argc, char **argv)
{
	simplearchive::App app;
	if (app.initalise(argc, argv) == false) {
		return false;
	}
	if (app.Run() == false) {
		return 0;
	}
	return 1;

}


