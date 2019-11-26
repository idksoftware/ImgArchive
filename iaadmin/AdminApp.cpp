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
#include "AdminArgvParser.h"
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
#include "CreateArchive.h"
#include "TestArchive.h"
#include "SummaryFile.h"
#include "IntegrityManager.h"
#include "SIALib.h"
#include "ShowCommand.h"

#ifdef _WIN32
#pragma comment(lib, "ws2_32.lib")
#endif

#include <iostream>
using namespace std;

#include <string>
#include <vector>
#include "AdminAppOptions.h"
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
#ifdef WIN32
	/*
	m_error = false;
	m_verbose = false;
	m_quiet = false;
	m_logLevel = "INFO";
	m_dry_run = false;
	*/
	AdminConfig config;
	/*
	const std::string key = "SIA_HOME";
	std::string temp = SAUtils::GetPOSIXEnv(key);
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
		homePath = SAUtils::GetPOSIXEnv("USERPROFILE");
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
			homePath = SAUtils::GetPOSIXEnv("ProgramData");
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
	AppConfigReader configReader;
	configReader.setNoLogging();
	configReader.read(configfile.c_str(), config);
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
	std::string temp;
	temp = SAUtils::GetPOSIXEnv("SIA_ARCHIVE");
	if (temp.empty() == false) {
		config.setWorkspacePath(temp.c_str());
	}
	temp = SAUtils::GetPOSIXEnv("SIA_SOURCE");
	if (temp.empty() == false) {
		config.setSourcePath(temp.c_str());
	}
	temp = SAUtils::GetPOSIXEnv("SIA_LOGLEVEL");
	if (temp.empty() == false) {
		config.setLogLevel(temp.c_str());
	}
	temp = SAUtils::GetPOSIXEnv("SIA_CONSOLELEVEL");
	if (temp.empty() == false) {
		config.setConsoleLevel(temp.c_str());
	}
	CreateArchive::checkFolders(homePath.c_str());
	return true;
#elseif
#endif
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
	switch (appOptions.getCommandMode()) {
	case AppOptions::CommandMode::CM_Show:
		Show();
		return true;
	case AppOptions::CommandMode::CM_Version:
		printf("Simple Image Archive Administrator\n"
			"siaadmin version \"%s\" (build %s)\n"
			"Copyright@(2010-2016) IDK Sftware Ltd.\n", VERSION, BUILD);
		return true;
	case AppOptions::CommandMode::CM_Test:
		{

		TestArchive testArchive;
		testArchive.readingConfigFile();
		testArchive.testFolders();
		return true;

		}
	}
	if (isConfiguratedOk() == false) {
		
		if (appOptions.getCommandMode() == AppOptions::CommandMode::CM_InitArchive) {
			// const char *archivePath, const char *workspacePath, const char *reposPath, const char *masterPath, const char *derivativePath, bool users
			if (CreateArchive(appOptions.getHomePath(), appOptions.getWorkspacePath(), appOptions.getRepositoryPath(), appOptions.getMasterPath(), appOptions.getDerivativePath(), appOptions.getCataloguePath(), appOptions.getUsers()) == false) {
				return false;
			}
			printf("\nCompleted initalising the Archive\n");
			return true;
		}
		return false;
	}

	
	//CompletedSummary ;

	switch (appOptions.getCommandMode()) {
	case AppOptions::CommandMode::CM_InitArchive:
		break;
	case AppOptions::CommandMode::CM_Show:
	{
		//Show();
		ShowCommand show;
		if (show.process(appOptions.getName())) {
		}
		break;
	}
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
	case AppOptions::CommandMode::CM_Mirror:
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

bool AdminApp::CreateArchive(const char *archivePath, const char *workspacePath, const char *reposPath, const char *masterPath, const char *derivativePath, const char *cataloguePath, bool users) {

	if (users == true) {
		if (CreateArchive::IsAdmin() == false) {
			std::cout << "Failed you need administrator privileges" << '\n';
			return false;
		}
	}
	
	AppConfig &config = AppConfig::get();
	
	std::string archivePathStr = archivePath;
	std::string workspacePathStr = workspacePath;
	std::string reposPathStr = reposPath;
	std::string masterPathStr = masterPath;
	std::string derivativePathStr = derivativePath;
	std::string cataloguePathStr = cataloguePath;

	std::string masterPathOpt = masterPath;
	std::string derivativePathOpt = derivativePath;
	
	if (archivePathStr.empty()) {
		archivePathStr = config.getHomePath();
	}
	else {
		// The root has be changed as an option i.e c:/MyPhotoArchive
		archivePathStr = archivePath;
	}

	masterPathStr = archivePathStr;
	masterPathStr += MASTER_PATH;
	derivativePathStr = archivePathStr;
	derivativePathStr += DERIVATIVE_PATH;

	
	if (!reposPathStr.empty()) {
		masterPathStr = reposPathStr;
		masterPathStr += MASTER_PATH;
		derivativePathStr = reposPathStr;
		derivativePathStr += DERIVATIVE_PATH;
	}
	else if (masterPathOpt.empty() && !derivativePathOpt.empty()) {
		masterPathStr = masterPathOpt;
		derivativePathStr = derivativePathOpt;
	}

	if (masterPathStr.empty()) {
		masterPathStr = config.getMasterPath();
	}
	if (derivativePathStr.empty()) {
		derivativePathStr = config.getDerivativePath();
	}
	
	if (workspacePathStr.empty()) {
		workspacePathStr = config.getWorkspacePath();
	}

	if (cataloguePathStr.empty()) {
		cataloguePathStr = config.getMasterCataloguePath();
	}
	std::cout << "Using archive path \"" << archivePathStr << "\"\n";
	std::cout << "Using workspace path \"" << workspacePathStr << "\"\n";
	std::cout << "Using master path \"" << masterPathStr << "\"\n";
	std::cout << "Using derivative path \"" << derivativePathStr << "\"\n";
	std::cout << "Using catalogue path \"" << cataloguePathStr << "\"\n";
	
	AdminConfig adminConfig;
	adminConfig.setArchivePath(archivePathStr.c_str());
	adminConfig.setMasterPath(masterPathStr.c_str());
	adminConfig.setDerivativePath(derivativePathStr.c_str());
	adminConfig.setWorkspacePath(workspacePathStr.c_str());
	adminConfig.setMasterCataloguePath(cataloguePathStr.c_str());

	if (CreateArchive::createSystem(users, archivePathStr.c_str(), workspacePathStr.c_str(), masterPathStr.c_str(), derivativePathStr.c_str(), cataloguePathStr.c_str()) == false) {
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
	
	if (CreateArchive::createConfigFiles(CreateArchive::getArchivePath().c_str(), CONFIG_PATH, CreateArchive::getWorkspace().c_str(), CreateArchive::getMaster().c_str(), derivativePathStr.c_str(), cataloguePathStr.c_str()) == false) {
		std::cout << "Failed creating configuration files" << '\n';
		return false;
	}
	return true;
}


bool AdminApp::initaliseConfig() {

	//AppConfig &config = AppConfig::get();
	AdminConfig config;
	m_configured = false;
	bool found = false;
	std::string homePath;
#ifdef WIN32
		// Looking the HKEY_LOCAL_MACHINE first
		if (GetEnv(homePath, true) == true) {
			//printf("Found SIA_HOME in system variables: %s", homePath.c_str());
			found = true;
		}
		// Looking the HKEY_CURRENT_USER
		else if (GetEnv(homePath, false) == true) {
			//printf("Found SIA_HOME in user variables: %s", homePath.c_str());
			found = true;
		}
#else
		homePath = SAUtils::GetPOSIXEnv("IMGARCHIVE_HOME");
		if (homePath.empty() == true || homePath.length() == 0) {
			printf("SIA Unable to start? Cannot read user profile.");
			setError(12, "SIA Unable to start? Cannot read user profile.");
			return false;
		}
		found = true;
#endif
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
		setError(12, "ImgArchive Unable to start? Archive not found at default location and the environment variable IAHOME not set.\n"
			"Use siaadmin -i to create an empty archive at the default location (see documentation).\n");
		return false;

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
		config.fileBasedValues(homePath.c_str());
		m_configured = true;
	}
	

	return true;
}

// AppConfig &config = AppConfig::get();
bool AdminApp::initaliseHomePath() {

	SIAARCConfig config;

	bool found = false;
	std::string homePath;
#ifdef WIN32
	// Looking the HKEY_LOCAL_MACHINE first
	if (GetEnv(homePath, true) == true) {
		//printf("Found SIA_HOME in system variables: %s", homePath.c_str());
		found = true;
	}
	// Looking the HKEY_CURRENT_USER
	else if (GetEnv(homePath, false) == true) {
		//printf("Found SIA_HOME in user variables: %s", homePath.c_str());
		found = true;
	}
	else {
		bool found = false;
		homePath = SAUtils::GetPOSIXEnv("ProgramData");
		if (homePath.empty() == true || homePath.length() == 0) {
			printf("SIA Unable to start? Cannot read user profile.");
			setError(12, "SIA Unable to start? Cannot read user profile.");
			return false;
		}
		else {
			homePath += "/IDK Software/ImageArchive1.0";
			if (SAUtils::DirExists(homePath.c_str()) == true) {
				//printf("Found SIA_HOME in user profile: %s", homePath.c_str());
				found = true;
			}
		}
		if (found == false) {
			homePath = SAUtils::GetPOSIXEnv("USERPROFILE");
			if (homePath.empty() == true || homePath.length() == 0) {
				printf("SIA Unable to start? Cannot read all users profile.");
				setError(12, "SIA Unable to start? Cannot read all users profile.");
				return false;
			}
			homePath += "/IDK Software/ImageArchive1.0";
			if (SAUtils::DirExists(homePath.c_str()) == true) {
				//printf("Found SIA_HOME in all users profile: %s", homePath.c_str());
				found = true;
			}
		}
	}
#elseif
#endif
	std::string temp;
	temp = SAUtils::GetPOSIXEnv("SIA_ARCHIVE");
	if (temp.empty() == false) {
		config.setWorkspacePath(temp.c_str());
	}
	temp = SAUtils::GetPOSIXEnv("SIA_SOURCE");
	if (temp.empty() == false) {
		config.setSourcePath(temp.c_str());
	}
	temp = SAUtils::GetPOSIXEnv("SIA_LOGLEVEL");
	if (temp.empty() == false) {
		config.setLogLevel(temp.c_str());
	}


	const std::string key = "SIA_HOME";
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


