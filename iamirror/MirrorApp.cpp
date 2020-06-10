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
#include "MirrorApp.h"
#include "MirrorArgvParser.h"
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
#include "MirrorAppOptions.h"
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
	
	MirrorApp::MirrorApp() : AppBase(std::make_shared<MirrorArgvParser>()) {};


bool MirrorApp::doInitalise(int argc, char **argv) {
	
	MirrorArgvParser mirrorArgvParser;
	if (mirrorArgvParser.initalise(argc, argv) == false) {

		return false;
	}
	return true;
}

bool MirrorApp::doRun()
{
	// Find if the archive exists
	AppOptions& appOptions = AppOptions::get();
	AppConfig& config = AppConfig::get();

	SIALib siaLib;
	

	//printf("%s", config.toString().c_str());
	config.settup();

	if (isConfiguratedOk() == false) {
		setError(12, "ImgArchive Unable to start? Archive not found at the specified location \"%s\".\n"
			"Use iaadmin -i to create an empty archive at the default location (see documentation).\n", config.getHomePath());
		return false;
	}


	if (siaLib.initalise() < 0) {
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
	case AppOptions::CommandMode::CM_Process:
	{
		// make mirror
		SIALib siaLib;
		siaLib.initalise();
		AppConfig &config = AppConfig::get();
		if (siaLib.mirror(appOptions.getName()) == false) {
			return false;
		}
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
	
		/*
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
		*/
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

MirrorApp::~MirrorApp() {
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

void MirrorApp::Show() {
	return;
}

bool MirrorApp::initaliseConfig() {


	SIAARCConfig config;

	bool res = HomePath::init();
	HomePathType homePathType = HomePath::type();

	switch (homePathType) {
	case HomePathType::LocalEnv:	// Local Environment set
		if (res == false) {
			//printf("Found IMGARCHIVE_HOME as system profile: %s but archive not found at loacation", HomePath::get().c_str());
			return false;
		}
		//printf("Found IMGARCHIVE_HOME as system profile: %s. Archive found at that loacation", HomePath::get().c_str());
		break;
	case HomePathType::SystemEnv:	// System Environment set
		if (res == false) {
			//printf("Found IMGARCHIVE_HOME as system profile: %s. Archive found at that loacation", HomePath::get().c_str());
			return false;
		}
		break;
	case HomePathType::UserOnly:	// user only archive
		if (res == false) {
			//printf("Archive not found at default loacation");
			return false;
		}
		//printf("Archive found at default user loacation: %s.", HomePath::get().c_str());
		break;
	case HomePathType::AllUsers:	// all users archive
		if (res == false) {
			//printf("Archive not found at default loacation");
			return false;
		}
		//printf("Archive found at default system loacation: %s.", HomePath::get().c_str());
		break;
	case HomePathType::Unknown:
	default:
		//printf("Unknown error");
		return false;
	}
	std::string homePath = HomePath::get();

	// try to set a systems temp folder 
	std::string tempPath = SAUtils::GetPOSIXEnv("TEMP");
	if (tempPath.empty() == true || tempPath.length() == 0) {
		tempPath = SAUtils::GetPOSIXEnv("TMP");
	}

	AppConfig::setDefaultLocations();
	std::string configfile = homePath + "/config/" + "config.dat";
	std::string configPath = homePath + "/config";
	// Now set the file based configuration with the possablity of overrighting defaults set prevously. 
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
	else {
		m_configured = false;
	}

	return true;
}

// AppConfig &config = AppConfig::get();
bool MirrorApp::initaliseHomePath() {

	SIAARCConfig config;

	bool found = false;
	std::string homePath;
#ifdef WIN32
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
	temp = SAUtils::GetPOSIXEnv("IAARCHIVE");
	if (temp.empty() == false) {
		config.setWorkspacePath(temp.c_str());
	}
	temp = SAUtils::GetPOSIXEnv("IA_SOURCE");
	if (temp.empty() == false) {
		config.setSourcePath(temp.c_str());
	}
	temp = SAUtils::GetPOSIXEnv("IA_LOGLEVEL");
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

bool MirrorApp::initaliseArgs(int argc, char **argv) {

	
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
	simplearchive::MirrorApp app;
	bool error = false;
	if (app.initalise(argc, argv) == false) {

		error = true;
	}
	else {
		if (app.Run() == false) {
			error = true;
		}
	}
	if (error) {
		int code = CommandLineProcessing::AppBase::getError();
		cout << "\n\n" << CommandLineProcessing::AppBase::getFullErrorString() << '\n';
		return code;
	}
	else {
		return 0;
	}
}


