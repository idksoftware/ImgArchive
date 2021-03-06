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
#include "ServerApp.h"
#include "ServerArgvParser.h"
#include "ServerAppOptions.h"
#include "AppPaths.h"
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
//#include "ViewManager.h"

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
#include "EnvFunc.h"
#include "AppConfig.h"
#include "HomePath.h"

#define VERSION	"1.00"
#define BUILD	"040115.1749"
#define DB "c:/temp/test3.db"
/*
java version "1.7.0_51"
Java(TM) SE Runtime Environment (build 1.7.0_51-b13)
Java HotSpot(TM) 64-Bit Server VM (build 24.51-b03, mixed mode)
*/
namespace simplearchive {
	
	AdminApp::AdminApp() : AppBase(std::make_shared<AdminArgvParser>()) {};

	

	


bool AdminApp::doRun()
{
	// Find if the archive exists
	AppOptions &appOptions = AppOptions::get();
	switch (appOptions.getCommandMode()) {
	

	default:
		printf("ImgArchive Administrator\n"
					"siaadmin version \"%s\" (build %s)\n"
					"Copyright@(2010-2016) IDK Sftware Ltd. This command is implemented in the version\n", VERSION, BUILD);
	}
	if (isConfiguratedOk() == false) {
		
		if (appOptions.getCommandMode() == AppOptions::CommandMode::CM_InitArchive) {
			// const char *archivePath, const char *workspacePath, const char *reposPath, const char *masterPath, const char *derivativePath, bool users
			
			return true;
		}
		return false;
	}

	
	//CompletedSummary ;

	switch (appOptions.getCommandMode()) {
	
	case AppOptions::CommandMode::CM_Version:
			printf("Simple Image Archive Administrator\n"
				"siaadmin version \"%s\" (build %s)\n"
				"Copyright@(2010-2016) IDK Sftware Ltd.\n", VERSION, BUILD);
			return true;
	
	case AppOptions::CommandMode::CM_Unknown:
		break;
	
	default:
			printf("ImgArchive Administrator\n"
						"siaadmin version \"%s\" (build %s)\n"
						"Copyright@(2010-2016) IDK Sftware Ltd. This command is implemented in the version\n", VERSION, BUILD);
	
	}
	return true;
}

AdminApp::~AdminApp() {
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




bool AdminApp::initaliseConfig() {
	AppOptions& appOptions = AppOptions::get();
	if (appOptions.isConfiguratedOk() == false) {
		// This is for the time the Init option is  in operation
		return true;
	}
	//AppConfig &config = AppConfig::get();
	AdminConfig config;
	m_configured = false;
	
	
	
	bool res = HomePath::init();
	HomePathType homePathType = HomePath::type();

	switch (homePathType) {
	case HomePathType::LocalEnv:	// Local Environment set
		if (res == false) {
			printf("Found IMGARCHIVE_HOME as system profile: %s but archive not found at loacation", HomePath::get().c_str());
			return false;
		}
		printf("Found IMGARCHIVE_HOME as system profile: %s. Archive found at that loacation", HomePath::get().c_str());
		break;
	case HomePathType::SystemEnv:	// System Environment set
		if (res == false) {
			printf("Found IMGARCHIVE_HOME as system profile: %s. Archive found at that loacation", HomePath::get().c_str());
			return false;
		}
		break;
	case HomePathType::UserOnly:	// user only archive
		if (res == false) {
			printf("Archive not found at default loacation");
			return false;
		}
		printf("Archive found at default user loacation: %s.", HomePath::get().c_str());
		break;
	case HomePathType::AllUsers:	// all users archive
		if (res == false) {
			printf("Archive not found at default loacation");
			return false;
		}
		printf("Archive found at default system loacation: %s.", HomePath::get().c_str());
		break;
	case HomePathType::Unknown:
	default:
		printf("Unknown error");
		return false;
	}
	std::string homePath = HomePath::get();
	if (HomePath::isFound()) {
		
		// Initalise without the config file i.e. set defaults.
		if (config.init(homePath.c_str()) == false) {
			setError(12, "Cannot find home path? exiting?");
			return false;
		}
	}
	/*
	else {
		if (config.init() == false) {
			setError(12, "Cannot find home path? exiting?");
			return false;
		}
	}

	if (SAUtils::DirExists(homePath.c_str()) == false) {
		setError(12, "ImgArchive Unable to start? Archive not found at default location and the environment variable IAHOME not set.\n"
			"Use siaadmin -i to create an empty archive at the default location (see documentation).\n");
		return false;

	}
	*/
	// try to set a systems temp folder 
	std::string tempPath = SAUtils::GetPOSIXEnv("TEMP");
	if (tempPath.empty() == true || tempPath.length() == 0) {
		tempPath = SAUtils::GetPOSIXEnv("TMP");
	}
	
	std::string configfile = homePath + "/config/" + "config.dat";
	std::string configPath = homePath + "/config";
	if (SAUtils::FileExists(configfile.c_str()) == true) {
		setConfigPath(configPath.c_str());
		AppConfigReader configReader;
		configReader.setNoLogging();
		if (configReader.read(configfile.c_str(), config) == false) {
			setError(13, "Error found at line %d in the configuration file.\n", configReader.getCurrentLineNumber());
			return false;
		}
		config.fileBasedValues(homePath.c_str(), tempPath.c_str());
		m_configured = true;
	}
	

	return true;
}

// AppConfig &config = AppConfig::get();
bool AdminApp::initaliseHomePath() {

	SIAARCConfig config;


	std::string homePath;
#ifdef WIN32
	bool found = false;
	// Looking the HKEY_LOCAL_MACHINE first
	if (GetEnv(homePath, true) == true) {
		//printf("Found IMGARCHIVE_HOME in system variables: %s", homePath.c_str());
		found = true;
	}
	// Looking the HKEY_CURRENT_USER
	else if (GetEnv(homePath, false) == true) {
		//printf("Found IMGARCHIVE_HOME in user variables: %s", homePath.c_str());
		found = true;
	}
	else {
		bool found = false;
		homePath = SAUtils::GetPOSIXEnv("ProgramData");
		if (homePath.empty() == true || homePath.length() == 0) {
			printf("ImgArchive Unable to start? Cannot read user profile.");
			setError(12, "ImgArchive Unable to start? Cannot read user profile.");
			return false;
		}
		else {
			homePath += "/IDK Software/ImageArchive1.0";
			if (SAUtils::DirExists(homePath.c_str()) == true) {
				//printf("Found IMGARCHIVE_HOME in user profile: %s", homePath.c_str());
				found = true;
			}
		}
		if (found == false) {
			homePath = SAUtils::GetPOSIXEnv("USERPROFILE");
			if (homePath.empty() == true || homePath.length() == 0) {
				printf("ImgArchive Unable to start? Cannot read all users profile.");
				setError(12, "ImgArchive Unable to start? Cannot read all users profile.");
				return false;
			}
			homePath += "/IDK Software/ImageArchive1.0";
			if (SAUtils::DirExists(homePath.c_str()) == true) {
				//printf("Found IMGARCHIVE_HOME in all users profile: %s", homePath.c_str());
				found = true;
			}
		}
	}
#else
#endif
	std::string temp;
	
	temp = SAUtils::GetPOSIXEnv("IASOURCE");
	if (temp.empty() == false) {
		config.setSourcePath(temp.c_str());
	}
	temp = SAUtils::GetPOSIXEnv("IALOGLEVEL");
	if (temp.empty() == false) {
		config.setLogLevel(temp.c_str());
	}


	const std::string key = "IMGARCHIVE_HOME";
	temp = SAUtils::GetPOSIXEnv(key);
	homePath = temp;
	//printf("%s", homePath.c_str());
	//int i = homePath.length();
	if (homePath.empty() == true || homePath.length() == 0) {
		homePath = SAUtils::GetPOSIXEnv("ProgramData");
		//C:\ProgramData\IDK Software\ImageArchive1.0
		homePath += "/IDK Software/ImageArchive1.0";

	}
	return true;
}

bool AdminApp::initaliseArgs(int argc, char **argv) {
	//DefaultEnvironment defaultEnvironment;
	//defaultEnvironment.init();
	//defaultEnvironment.print();
	//AppOptions& appOptions = AppOptions::get();
	
	if (m_argvParser->doInitalise(argc, argv) == false) {
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
	simplearchive::AdminApp app;
	return app.RunApp(argc, argv);
}


