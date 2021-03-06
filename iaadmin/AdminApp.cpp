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
//#include "History.h"
#include "argvparser.h"
#include "HookCmd.h"
#include "MakeMedia.h"
//#include "MirrorManager.h"
//#include "ViewManager.h"
#include "CreateArchive.h"
#include "TestArchive.h"
#include "SummaryFile.h"
#include "IntegrityManager.h"
#include "SIALib.h"
#include "ShowCommand.h"
#include "UpdateConfig.h"
#include "DefaultEnvironment.h"
#include "SyncCommand.h"

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
#include "HomePaths.h"
#include "SetImageExtentionFile.h"
#include "AboutCommand.h"

constexpr ReturnCode Allow_Command = 4020;

#define DB "c:/temp/test3.db"
/*
java version "1.7.0_51"
Java(TM) SE Runtime Environment (build 1.7.0_51-b13)
Java HotSpot(TM) 64-Bit Server VM (build 24.51-b03, mixed mode)
*/
namespace simplearchive
{
	AdminApp::AdminApp(const char *appName) : AppBase(appName, std::make_shared<AdminArgvParser>())
	{
	};

	bool AdminApp::Allow(const char* cmd, const char* arg)
	{
		SetImageExtentionFile setImageExtentionFile;
		std::string cmdStr = cmd;
		if (cmdStr.compare("add") == 0)
		{
			ExtentionItem extentionItem(arg, ',');
			if (!extentionItem.isValid())
			{
				setError(Allow_Command+1, "Failed to add extention type: \"s\" - %s", arg, extentionItem.getErrorString().c_str());
				return false;
			}
			if (setImageExtentionFile.add(extentionItem) == false) {
				if (setImageExtentionFile.getError() == SetImageExtentionFile::Error::OperationFailed) {
					setError(Allow_Command + 2, "Operation to add extention type failed, possibly extention already included");
				}
				else if (setImageExtentionFile.getError() == SetImageExtentionFile::Error::WriteFailed) {
					setError(Allow_Command + 3, "Operation to add extention type failed writing, possibly need admin permission");
				}
				else {
					setError(Allow_Command + 4, "Operation to add extention type failed, unknown reason");
				}
				return false;
			}
		}
		else if (cmdStr.compare("edit") == 0)
		{
			ExtentionItem extentionItem(arg, ',');
			if (!extentionItem.isValid())
			{
				setError(Allow_Command + 5, "Failed to edit extention type: \"s\" - %s", arg, extentionItem.getErrorString().c_str());
				return false;
			}
			
			if (setImageExtentionFile.update(extentionItem) == false) {
				if (setImageExtentionFile.getError() == SetImageExtentionFile::Error::OperationFailed) {
					setError(Allow_Command + 6, "Operation to update extention type failed, possibly extention does not exist");
				}
				else if (setImageExtentionFile.getError() == SetImageExtentionFile::Error::WriteFailed) {
					setError(Allow_Command + 7, "Operation to update extention type failed writing, possibly need admin permission");
				}
				else {
					setError(Allow_Command + 8, "Operation to update extention type failed, unknown reason");
				}
				return false;
			}
		}
		else if (cmdStr.compare("delete") == 0)
		{
			if (setImageExtentionFile.remove(arg) == false) {
				if (setImageExtentionFile.getError() == SetImageExtentionFile::Error::OperationFailed) {
					setError(Allow_Command + 9, "Operation to delete extention type failed, possibly extention does not exist");
				}
				else if (setImageExtentionFile.getError() == SetImageExtentionFile::Error::WriteFailed) {
					setError(Allow_Command + 10, "Operation to delete extention type failed writing, possibly need admin permission");
				}
				else {
					setError(Allow_Command + 11, "Operation to delete extention type failed, unknown reason");
				}
				return false;
			}
		}
		else
		{
			setError(Allow_Command + 12, "Invalid argument for sub-command: allow --%s", cmdStr.c_str());

			return false;
		}
		return true;
	}

	bool AdminApp::Configure(const char* configOptionBlock, const char* configOption, const char* configValue)
	{
		SIALib siaLib;
		if (siaLib.configure(configOptionBlock, configOption, configValue) == false)
		{
			return false;
		}
		return true;
	}

	bool AdminApp::Show(const char* configOption, const char* configValue, const char* outputType, const char* filename)
	{
		ShowCommand showCommand;
		showCommand.setOutputFile(filename);
		showCommand.setTextOutputType(outputType);

		if (showCommand.process(configOption, configValue) == false)
		{
			return false;
		}
		return true;
	}

	bool AdminApp::Backup(const char* mediaSize, const char* mediaPath, const char* fromDate, const char* toDate)
	{
		return true;
	}

	bool AdminApp::Purge()
	{
		SIALib siaLib;
		return siaLib.purge();
	}


	bool AdminApp::Sync(const char* archive, const char* backups)
	{
		AppOptions& appOptions = AppOptions::get();
		SyncCommand syncCommand;
		if (syncCommand.setArchive(archive) == false)
		{
			return false;
		}
		if (syncCommand.setBackup(backups) == false)
		{
			return false;
		}
		SIALib siaLib;
		siaLib.initalise(APP_NAME);
		//AppConfig& config = AppConfig::get();
		if (appOptions.isConfiguratedOk() == false)
		{
			// Do not create a new archive. The old one needs to be deleted?
			return false;
		}
		bool ret = siaLib.sync(syncCommand.getArchive(), syncCommand.getBackup());
		siaLib.complete();
		return ret;
	}

	bool AdminApp::About(const char* outputType, const char* filename)
	{
		AboutCommand aboutCommand("Administrator", "iaadmin", VERSION, BUILD);
		aboutCommand.setOutputFile(filename);
		aboutCommand.setTextOutputType(outputType);

		if (aboutCommand.process() == false)
		{
			setError(4, "About Command failed? Parsing error\n");
			return false;
		}
		return true;
	}

	int test(const std::string key)
	{
		return 0;
	}


	bool AdminApp::doRun()
	{
		// Find if the archive exists
		AppOptions& appOptions = AppOptions::get();
		if (isConfiguratedOk() == true)
		{
			switch (appOptions.getCommandMode())
			{
			case AppOptions::CommandMode::CM_Allow:
			{
				bool ret = Allow(appOptions.getConfigOption(), appOptions.getConfigValue());
				if (ret == false) {
					setExitCode(ExitCode::Fatal);
				}
				return ret;
			}
			case AppOptions::CommandMode::CM_Config:
				{
					bool ret = Configure(appOptions.getConfigOptionBlock(), appOptions.getConfigOption(),
					                     appOptions.getConfigValue());
					if (ret == false)
					{
						setExitCode(ExitCode::Fatal);
						setError(4, "Command \"Config\" failed? May need to be in admin mode\n");
					}
					setError(4, "Command \"Config\" failed? May need to be in admin mode\n");
					return ret;
				}
			case AppOptions::CommandMode::CM_Show:
				{	bool ret = Show(appOptions.getConfigOption(), appOptions.getConfigValue(), appOptions.getTextOutputType(),
																						appOptions.getOutputFile());
					if (ret == false) {
						setExitCode(ExitCode::Fatal);
					}
					return ret;
				}
			case AppOptions::CommandMode::CM_Test:
				{
					TestArchive testArchive;
					testArchive.readingConfigFile();
					testArchive.testFolders();
					return true;
				}
			case AppOptions::CommandMode::CM_About:
				return About(appOptions.getTextOutputType(), appOptions.getOutputFile());
			case AppOptions::CommandMode::CM_Validate:
				{
					SIALib siaLib;
					siaLib.initalise(APP_NAME);
					//AppConfig& config = AppConfig::get();
					if (appOptions.isConfiguratedOk() == false)
					{
						// Do not create a new archive. The old one needs to be deleted?
						return false;
					}
					SIALib::VerifyBackups slVerifyBackups;
					switch (appOptions.getVerifyBackups())
					{
					case AppOptions::VerifyBackups::Backup_1: slVerifyBackups = SIALib::VerifyBackups::Backup_1;
						break;
					case AppOptions::VerifyBackups::Backup_2: slVerifyBackups = SIALib::VerifyBackups::Backup_1;
						break;
					case AppOptions::VerifyBackups::Both: slVerifyBackups = SIALib::VerifyBackups::Backup_1;
						break;
					case AppOptions::VerifyBackups::None: slVerifyBackups = SIALib::VerifyBackups::Backup_1;
						break;
					}
					//CompletedSummary completedSummary;
					switch (appOptions.getVerifyOperation())
					{
					case AppOptions::VerifyOperation::Workspace:
						return siaLib.validate(SIALib::Scope::Workspace, appOptions.validateMain(), slVerifyBackups,
						                       appOptions.repair());
					case AppOptions::VerifyOperation::Master:
						return siaLib.validate(SIALib::Scope::Master, appOptions.validateMain(), slVerifyBackups,
						                       appOptions.repair());
					case AppOptions::VerifyOperation::Derivative:
						return siaLib.validate(SIALib::Scope::Derivative, appOptions.validateMain(), slVerifyBackups,
						                       appOptions.repair());
					case AppOptions::VerifyOperation::All:
						return siaLib.validate(SIALib::Scope::All, appOptions.validateMain(), slVerifyBackups,
						                       appOptions.repair());
					case AppOptions::VerifyOperation::Main:
						return siaLib.validate(SIALib::Scope::Main, appOptions.validateMain(), slVerifyBackups,
						                       appOptions.repair());
					default:
						return siaLib.validate(SIALib::Scope::All, appOptions.validateMain(), slVerifyBackups,
						                       appOptions.repair());
					}

					siaLib.complete();

					return true;
				}
			case AppOptions::CommandMode::CM_Sync:
				return Sync(appOptions.getConfigOption(), appOptions.getConfigValue());
			case AppOptions::CommandMode::CM_Purge:
				return Purge();
			case AppOptions::CommandMode::CM_Backup:
				return Backup(appOptions.getMediaSize(), appOptions.getMediaPath(), appOptions.getFromDate(),
				              appOptions.getToDate());
			case AppOptions::CommandMode::CM_Unknown:
				break;

			default:
				printf("ImgArchive Administrator\n"
				       "siaadmin version \"%s\" (build %s)\n"
				       "Copyright@(2010-2016) IDK Sftware Ltd. This command is implemented in the version\n", VERSION,
				       BUILD);
			}
		}
		else
		{
			if (appOptions.getCommandMode() == AppOptions::CommandMode::CM_InitArchive)
			{
				// const char *archivePath, const char *workspacePath, const char *reposPath, const char *masterPath, const char *derivativePath, bool users
				//DefaultEnvironment defaultEnvironment;
				//defaultEnvironment.init();

				if (CreateArchive(appOptions.getHomePath(), appOptions.getWorkspacePath(),
				                  appOptions.getRepositoryPath(),
				                  appOptions.getMasterPath(), appOptions.getDerivativePath(),
				                  appOptions.getCataloguePath(),
				                  appOptions.getWWWImagePath(), appOptions.getUsers()) == false)
				{
					return false;
				}
				printf("\nCompleted Creating ImgArchive\n");
				return true;
			}
			return false;
		}
		return true;

		/*
			//CompletedSummary ;
		
			switch (appOptions.getCommandMode()) {
			case AppOptions::CommandMode::CM_InitArchive:
				break;
			case AppOptions::CommandMode::CM_Show:
			{
				//Show();
				ShowCommand show;
				if (show.parseOptions(appOptions.getName())) {
				}
				break;
			}
			case AppOptions::CommandMode::CM_View:
			{
				SIALib siaLib;
				siaLib.initalise();
				//AppConfig &config = AppConfig::get();
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
				//AppConfig &config = AppConfig::get();
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
				case AppOptions::VerifyOperation::Master:			
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
			
			default:
					printf("ImgArchive Administrator\n"
								"siaadmin version \"%s\" (build %s)\n"
								"Copyright@(2010-2016) IDK Sftware Ltd. This command is implemented in the version\n", VERSION, BUILD);
			
			}
			return true;
		*/
	}

	AdminApp::~AdminApp()
	{
		CLogger::Close();
	}

	bool ok()
	{
		cout << "OK" << endl;
		return (true);
	}

	bool failed()
	{
		cout << "Failed" << endl;
		return (false);
	}

	bool AdminApp::CreateArchive(const char* archivePath, const char* workspacePath, const char* reposPath,
	                             const char* masterPath, const char* derivativePath, const char* cataloguePath,
	                             const char* wwwPath, bool users)
	{
		if (users == true)
		{
			if (CreateArchive::IsAdmin() == false)
			{
				std::cout << "Failed? \"All users selected\". You need administrator privileges" << '\n';
				return false;
			}
		}

		std::cout << "Creating ImgArchive Archives...\n";

		std::cout << "Managed directories\n";
		std::cout << "Home Configuration path    = \"" << archivePath << "\"\n";
		std::cout << "Master Repository path     = \"" << masterPath << "\"\n";
		std::cout << "Derivative Repository path = \"" << derivativePath << "\"\n";
		std::cout << "Workspace path             = \"" << workspacePath << "\"\n";
		std::cout << "Picture path               = \"" << cataloguePath << "\"\n";
		std::cout << "WWWW path                  = \"" << wwwPath << "\"\n";

		//  const char* archivePath, const char* workspacePath, const char* reposPath, const char* masterPath, const char* derivativePath, const char* cataloguePath, bool users
		if (CreateArchive::createArchive(archivePath, workspacePath, masterPath, derivativePath, cataloguePath, wwwPath,
		                                 users) == false)
		{
			return false;
		}

		return true;
	}


	bool AdminApp::initaliseConfig()
	{
		AppOptions& appOptions = AppOptions::get();
		if (appOptions.isConfiguratedOk() == false)
		{
			// This is for the time the Init option is in operation
			return true;
		}
		//AppConfig &config = AppConfig::get();
		AdminConfiguration config;

		ImgArchiveHome& imgArchiveHome = ImgArchiveHome::getObject();
		if (imgArchiveHome.isValid() == false)
		{
			ReturnCodeObject::setReturnString(4004, "IMGARCHIVE_HOME not found at loacation: %s.\n", ImgArchiveHome::getImgArchiveHome().c_str());
			return false;
		}

		std::string homePath = ImgArchiveHome::getImgArchiveHome();

		// try to set a systems temp folder 
		std::string tempPath = SAUtils::GetPOSIXEnv("TEMP");
		if (tempPath.empty() == true || tempPath.length() == 0)
		{
			tempPath = SAUtils::GetPOSIXEnv("TMP");
		}

		std::string configfile = homePath + "/config/" + "config.dat";
		std::string configPath = homePath + "/config";
		if (SAUtils::FileExists(configfile.c_str()) == true)
		{
			setConfigPath(configPath.c_str());
			AppConfigReader configReader;
			configReader.setNoLogging();
			if (configReader.read(configfile.c_str(), config) == false)
			{
				ReturnCodeObject::setReturnString(4004, "Error found at line %d in the configuration file.",
				         configReader.getCurrentLineNumber());
				return false;
			}
			config.fileBasedValues(homePath.c_str(), tempPath.c_str());
			setConfigured();
		}
		AppConfig appConfig = AppConfig::get();
		if (ImageExtentions::setExtentionsFilePath(appConfig.getConfigPath()) == false)
		{
			ReturnCodeObject::setReturnString(4004, "Unable to find image extensions file path: \"%s\"", appConfig.getConfigPath());
			return false;
		}
		return true;
	}

	// AppConfig &config = AppConfig::get();
	bool AdminApp::initaliseHomePath()
	{
		SIAARCConfig config;


		std::string homePath;
#ifdef WIN32
		bool found = false;
		// Looking the HKEY_LOCAL_MACHINE first
		if (GetEnv(homePath, true) == true)
		{
			//printf("Found IMGARCHIVE_HOME in system variables: %s", homePath.c_str());
			found = true;
		}
			// Looking the HKEY_CURRENT_USER
		else if (GetEnv(homePath, false) == true)
		{
			//printf("Found IMGARCHIVE_HOME in user variables: %s", homePath.c_str());
			found = true;
		}
		else
		{
			bool found = false;
			homePath = SAUtils::GetPOSIXEnv("ProgramData");
			if (homePath.empty() == true || homePath.length() == 0)
			{
				printf("ImgArchive Unable to start? Cannot read user profile.");
				setError(12, "ImgArchive Unable to start? Cannot read user profile.");
				return false;
			}
			homePath += "/IDK Software/ImageArchive1.0";
			if (SAUtils::DirExists(homePath.c_str()) == true)
			{
				//printf("Found IMGARCHIVE_HOME in user profile: %s", homePath.c_str());
				found = true;
			}
			if (found == false)
			{
				homePath = SAUtils::GetPOSIXEnv("USERPROFILE");
				if (homePath.empty() == true || homePath.length() == 0)
				{
					printf("ImgArchive Unable to start? Cannot read all users profile.");
					setError(12, "ImgArchive Unable to start? Cannot read all users profile.");
					return false;
				}
				homePath += "/IDK Software/ImageArchive1.0";
				if (SAUtils::DirExists(homePath.c_str()) == true)
				{
					//printf("Found IMGARCHIVE_HOME in all users profile: %s", homePath.c_str());
					found = true;
				}
			}
		}
#else
#endif
		std::string temp;

		temp = SAUtils::GetPOSIXEnv("IASOURCE");
		if (temp.empty() == false)
		{
			config.setSourcePath(temp.c_str());
		}
		temp = SAUtils::GetPOSIXEnv("IALOGLEVEL");
		if (temp.empty() == false)
		{
			config.setLogLevel(temp.c_str());
		}


		const std::string key = "IMGARCHIVE_HOME";
		temp = SAUtils::GetPOSIXEnv(key);
		homePath = temp;
		//printf("%s", homePath.c_str());
		//int i = homePath.length();
		if (homePath.empty() == true || homePath.length() == 0)
		{
			homePath = SAUtils::GetPOSIXEnv("ProgramData");
			//C:\ProgramData\IDK Software\ImageArchive1.0
			homePath += "/IDK Software/ImageArchive1.0";
		}
		return true;
	}

	bool AdminApp::initaliseArgs(int argc, char** argv)
	{
		DefaultEnvironment defaultEnvironment;
		defaultEnvironment.init();

		//defaultEnvironment.print();
		//AppOptions& appOptions = AppOptions::get();

		if (m_argvParser->initalise(argc, argv) == false)
		{
			setExitCode(ExitCode::Fatal);
			return false;
		}
		return true;
	}
} // simplearchive
/**
 *
 *
 */

int main(int argc, char** argv)
{
	simplearchive::AdminApp app("ImgArchive Admin");
	ExitCode exitCode = app.RunApp(argc, argv);
	return (static_cast<int>(exitCode));
}
