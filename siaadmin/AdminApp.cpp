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

#define VERSION	"1.00"
#define BUILD	"040115.1749"
#define DB "c:/temp/test3.db"
/*
java version "1.7.0_51"
Java(TM) SE Runtime Environment (build 1.7.0_51-b13)
Java HotSpot(TM) 64-Bit Server VM (build 24.51-b03, mixed mode)
*/
namespace simplearchive {




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
		if (appOptions.getCommandMode() == AppOptions::CM_InitArchive) {

			if (CreateArchive(appOptions.getHomePath(), appOptions.getWorkspacePath(), appOptions.getShadowPath(), appOptions.getUsers()) == false) {
				return false;
			}
			printf("\n\nInit Archive\n");
			return true;
		}
		return false;
	}

	SIALib siaLib;
	siaLib.initalise();

	CAppConfig &config = CAppConfig::get();
	

	switch (appOptions.getCommandMode()) {
	case AppOptions::CM_InitArchive:
		/*
	case AppOptions::CM_InitArchive:
		if (appOptions.isConfiguratedOk() == true) {
			// Do not create a new archive. The old one needs to be deleted?
			return false;
		}
#ifdef _DEBUG
		if (CreateArchive("c:/temp", "c:/output", "c:/shadow") == false) {
#else
		if (CreateArchive(config.getHomePath(), config.getWorkspacePath()) == false) {
#endif
			return false;
		}
		printf("\n\nInit Archive\n");
		break;
		*/

	case AppOptions::CM_Show:
	{
		//if (siaLib.show(appOptions.getName()) == false) {
		//	return false;
		//}
		break;
	}
	case AppOptions::CM_View:
	{
		if (siaLib.view(appOptions.getName()) == false) {
			return false;
		}
		break;
	}
	case AppOptions::CM_Mirror:
	{
		// make mirror
		if (siaLib.mirror(appOptions.getName()) == false) {
			return false;
		}
		// 

		
		break;
	}
	case AppOptions::CM_Validate:
	{
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
		
		break;
	}
	
	case AppOptions::CM_Version:
		printf("\n\nSia version \"%s\" (build %s)\n", VERSION, BUILD);
		return true;
	case AppOptions::CM_Unknown:
		break;
	}

	siaLib.complete();
	
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
		std::cout << "Failed creating enviroment variable SIA_HOME" << '\n';
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


