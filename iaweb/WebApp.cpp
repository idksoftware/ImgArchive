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
#include "WebApp.h"
#include "WebArgvParser.h"
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
#include "ViewManager.h"
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
#include "WebAppOptions.h"
#include "EnvFunc.h"
#include "AppConfig.h"

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

bool AdminApp::Show() {
	
	AdminConfig config;
	/*
	const std::string key = "IMGARCHIVE_HOME";
	std::string temp = SAUtils::GetPOSIXEnv(key);
	std::string homePath = temp;
	*/
	bool found = false;
	std::string homePath;
	// Looking the HKEY_LOCAL_MACHINE first
	if (GetEnv(homePath, true) == true) {
		printf("Found IMGARCHIVE_HOME in system variables: %s", homePath.c_str());
		found = true;
	}
	else if (GetEnv(homePath, false) == true) {
		//printf("Found IMGARCHIVE_HOME in user variables: %s", homePath.c_str());
		found = true;
	}
	else {
		bool found = false;
		homePath = SAUtils::GetPOSIXEnv("USERPROFILE");
		if (homePath.empty() == true || homePath.length() == 0) {
			printf("ImgArchive Unable to start? Cannot read user profile.");
			return false;
		}
		else {
			homePath += "/IDK Software/ImageArchive1.0";
			if (SAUtils::DirExists(homePath.c_str()) == true) {
				printf("Found IMGARCHIVE_HOME in user profile: %s", homePath.c_str());
				found = true;
			}
		}
		if (found == false) {
			homePath = SAUtils::GetPOSIXEnv("ProgramData");
			if (homePath.empty() == true || homePath.length() == 0) {
				printf("ImgArchive Unable to start? Cannot read all users profile.");
				return false;
			}
			homePath += "/IDK Software/ImageArchive1.0";
			if (SAUtils::DirExists(homePath.c_str()) == true) {
				printf("Found IMGARCHIVE_HOME in all users profile: %s", homePath.c_str());
				found = true;
			}
		}
	}
	if (found = false) {
		printf("ImgArchive Unable to start? No archive found in the default location or"
			" the environment variable IMGARCHIVE_HOME not set.\nUse siaadmin to initalise an archive.\n");
		
		return false;
	}
	std::string configfile = homePath + "/config/" + "config.dat";
	if (SAUtils::FileExists(configfile.c_str()) == false) {
		
		printf("ImgArchiveUnable to start? No config.dat file found in the default location or"
			" the environment variable IMGARCHIVE_HOME not set.\nUse siaadmin to initalise an archive.\n");
		return false;
	}
	AppConfigReader configReader;
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
	temp = SAUtils::GetPOSIXEnv("IAARCHIVE");
	if (temp.empty() == false) {
		config.setWorkspacePath(temp.c_str());
	}
	temp = SAUtils::GetPOSIXEnv("IASOURCE");
	if (temp.empty() == false) {
		config.setSourcePath(temp.c_str());
	}
	temp = SAUtils::GetPOSIXEnv("IALOGLEVEL");
	if (temp.empty() == false) {
		config.setLogLevel(temp.c_str());
	}
	temp = SAUtils::GetPOSIXEnv("IACONSOLELEVEL");
	if (temp.empty() == false) {
		config.setConsoleLevel(temp.c_str());
	}

	return true;
}


int test(const std::string key) {
	return 0;
}


bool AdminApp::doInitalise(int argc, char **argv) {
	/*
	AppOptions &appOptions = AppOptions::get();
	if (appOptions.initalise(argc, argv) == false) {

		return false;
	}
	*/
	AdminArgvParser adminArgvParser;
	if (adminArgvParser.initalise(argc, argv) == false) {

		return false;
	}
	return true;
}

bool AdminApp::doRun()
{
	// Find if the archive exists
	AppOptions &appOptions = AppOptions::get();

	if (appOptions.isConfiguratedOk() == false) {
		if (appOptions.getCommandMode() == AppOptions::CommandMode::CM_Show) {
			Show();
			return false;
		}
		if (appOptions.getCommandMode() == AppOptions::CommandMode::CM_InitArchive) {
			// const char *archivePath, const char *workspacePath, const char *reposPath, const char *masterPath, const char *derivativePath, bool users
			printf("\nCompleted initalising the Archive\n");
			return true;
		}
		return false;
	}

	
	//CompletedSummary ;

	switch (appOptions.getCommandMode()) {
	case AppOptions::CommandMode::CM_InitArchive:
		break;
	case AppOptions::CommandMode::CM_View:
	{
		SIALib siaLib;
		siaLib.initalise();
		AppConfig &config = AppConfig::get();
		if (siaLib.view(appOptions.getName()) == false) {
			return false;
		}
		siaLib.complete();
		break;
	}
	case AppOptions::CommandMode::CM_Web:
	{
		// make Web
		SIALib siaLib;
		siaLib.initalise();
		AppConfig &config = AppConfig::get();
		//if (siaLib.Web(appOptions.getName()) == false) {
		//	return false;
		//}
		// 

		siaLib.complete();
		break;
	}
	case AppOptions::CommandMode::CM_Validate:
	{
		SIALib siaLib;
		siaLib.initalise();
		AppConfig &config = AppConfig::get();
		if (appOptions.isConfiguratedOk() == false) {
			// Do not create a new archive. The old one needs to be deleted?
			return false;
		}
	
		CompletedSummary completedSummary;
		switch (appOptions.getVerifyOperation()) {
		case AppOptions::VerifyOperation::Workspace:
			if (siaLib.validate(config.getMasterPath(), config.getWorkspacePath(), config.getHomePath(), SIALib::Workspace, appOptions.repair()) == false) {
				return false;
			}
			break;
		case AppOptions::VerifyOperation::Master:			//* Show
			if (siaLib.validate(config.getMasterPath(), config.getWorkspacePath(), config.getHomePath(), SIALib::Master, appOptions.repair()) == false) {
				return false;
			}
			break;
		default:
			if (siaLib.validate(config.getMasterPath(), config.getWorkspacePath(), config.getHomePath(), SIALib::Both, appOptions.repair()) == false) {
				return false;
			}
		}
		siaLib.complete();
		
		break;
	}
	
	case AppOptions::CommandMode::CM_Version:
		printf("Simple Image Archive Administrator\n"
			   "siaadmin version \"%s\" (build %s)\n"
			   "Copyright@(2010-2016) IDK Sftware Ltd.\n", VERSION, BUILD);
		return true;
	case AppOptions::CommandMode::CM_Unknown:
		break;
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

	//AppConfig &config = AppConfig::get();
	AdminConfig config;

	bool found = false;
	std::string homePath;
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
	if (found) {
		// Initalise without the config file i.e. set defaults.
		if (config.init(homePath.c_str()) == false) {
			setError(12, "Cannot find home path? exiting?");
			return false;
		}
	}
	else {
		if (config.init() == false) {
			setError(12, "Cannot find home path? exiting?");
			return false;
		}
	}

	if (SAUtils::DirExists(homePath.c_str()) == false) {
		setError(12, "ImgArchive Unable to start? Archive not found at default location and the environment variable SA_HOME not set.\n"
			"Use siaadmin -i to create an empty archive at the default location (see documentation).\n");
		return false;

	}

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

	}
	else {
		m_configured = false;
	}

	return true;
}

// AppConfig &config = AppConfig::get();
bool AdminApp::initaliseHomePath() {

	SIAARCConfig config;

	bool found = false;
	std::string homePath;
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

	std::string temp;
	temp = SAUtils::GetPOSIXEnv("IAARCHIVE");
	if (temp.empty() == false) {
		config.setWorkspacePath(temp.c_str());
	}
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
	int i = homePath.length();
	if (homePath.empty() == true || homePath.length() == 0) {
		homePath = SAUtils::GetPOSIXEnv("ProgramData");
		//C:\ProgramData\IDK Software\ImageArchive1.0
		homePath += "/IDK Software/ImageArchive1.0";

	}
	return true;
}

bool AdminApp::initaliseArgs(int argc, char **argv) {

	
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


