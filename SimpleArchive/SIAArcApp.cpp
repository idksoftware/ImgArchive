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
**	Filename	: App.cpp
**	Author		: I.Ferguson
**	Version		: 1.000
**	Date		: 26-05-2015
**
** #$$@@$$# */

#include <stdio.h>
#include <iostream>
#include "EnvFunc.h"
#include "SIAArcApp.h"
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

#include "UDPOut.h"

#ifdef _WIN32
#pragma comment(lib, "ws2_32.lib")
#endif

#include <iostream>
using namespace std;

#include <string>
#include <vector>
#include "SIAArcAppOptions.h"
#include "SIAArcArgvParser.h"
#include "Threads.h"

#define VERSION	"1.00"
#define BUILD	"040115.1749"
#define DB "c:/temp/test3.db"
/*
java version "1.7.0_51"
Java(TM) SE Runtime Environment (build 1.7.0_51-b13)
Java HotSpot(TM) 64-Bit Server VM (build 24.51-b03, mixed mode)
*/


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif


namespace simplearchive {
	
	SIAArcApp::SIAArcApp() : AppBase(new SIAArcArgvParser) {};



bool SIAArcApp::doInitalise(int argc, char **argv) {
	SIAArcAppOptions &appOptions = SIAArcAppOptions::get();
	if (m_argvParser->doInitalise(argc, argv) == false) {
		return false;
	}

	return true;
}


bool SIAArcApp::initaliseHomePath() {

	CAppConfig &config = CAppConfig::get();

	bool found = false;
	std::string homePath;
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
		homePath = SAUtils::GetEnvironment("ProgramData");
		if (homePath.empty() == true || homePath.length() == 0) {
			printf("SIA Unable to start? Cannot read user profile.");
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
			homePath = SAUtils::GetEnvironment("USERPROFILE");
			if (homePath.empty() == true || homePath.length() == 0) {
				printf("SIA Unable to start? Cannot read all users profile.");
				return false;
			}
			homePath += "/IDK Software/ImageArchive1.0";
			if (SAUtils::DirExists(homePath.c_str()) == true) {
				//printf("Found SIA_HOME in all users profile: %s", homePath.c_str());
				found = true;
			}
		}
	}

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


	const std::string key = "SIA_HOME";
	temp = SAUtils::GetEnvironment(key);
	homePath = temp;
	//printf("%s", homePath.c_str());
	int i = homePath.length();
	if (homePath.empty() == true || homePath.length() == 0) {
		homePath = SAUtils::GetEnvironment("ProgramData");
		//C:\ProgramData\IDK Software\ImageArchive1.0
		homePath += "/IDK Software/ImageArchive1.0";

	}
	return true;
}

bool SIAArcApp::initaliseConfig() {
	
	CAppConfig &config = CAppConfig::get();
	config.init();
	
	//const std::string key = "SIA_HOME";
	//std::string temp = SAUtils::GetEnvironment(key);
	//std::string homePath = temp;
	
	bool found = false;
	std::string homePath;
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
		homePath = SAUtils::GetEnvironment("ProgramData");
		if (homePath.empty() == true || homePath.length() == 0) {
			printf("SIA Unable to start? Cannot read user profile.");
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
			homePath = SAUtils::GetEnvironment("USERPROFILE");
			if (homePath.empty() == true || homePath.length() == 0) {
				printf("SIA Unable to start? Cannot read all users profile.");
				return false;
			}
			homePath += "/IDK Software/ImageArchive1.0";
			if (SAUtils::DirExists(homePath.c_str()) == true) {
				//printf("Found SIA_HOME in all users profile: %s", homePath.c_str());
				found = true;
			}
		}
	}
	

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
	
	
	const std::string key = "SIA_HOME";
	temp = SAUtils::GetEnvironment(key);
	homePath = temp;
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

bool SIAArcApp::Run()
{
	SIAArcAppOptions &appOptions = SIAArcAppOptions::get();
	CAppConfig &config = CAppConfig::get();

	SIALib siaLib;
	// Set global options
	if (appOptions.isEnventsOn() == true) {

		siaLib.enableEvents(appOptions.eventAddress(), appOptions.eventPort());
	}

	if (appOptions.isServerOn() == true) {

		siaLib.enableServer(appOptions.eventPort());
	}
	printf("%s", config.toString().c_str());
	if (siaLib.initalise() < 0) {
		return false;
	}

	switch (appOptions.getCommandMode()) {

		// run sub-comand
	case SIAArcAppOptions::CM_Import:

		if (appOptions.isDataForced() == true) {
			siaLib.setForceDate();
		}
		if (appOptions.getUseFileDate() == true) {
			siaLib.setUseFileDate();
		}
		else if (appOptions.getUseDateToday() == true) {
			siaLib.setUseDateToday();
		}
		
		if (appOptions.isUsingFile()) {
			siaLib.ImportFile(appOptions.getFilePath());
		}
		else {
			if (siaLib.Import() == false) {
				return false;
			}
		}
		break;
	case SIAArcAppOptions::CM_Export:
		if (siaLib.exportImage(appOptions.getDistinationPath()) == false) {
			return false;
		}
		break;
	case SIAArcAppOptions::CM_Show:
		switch (appOptions.getShowCommandOption()) {
		case SIAArcAppOptions::SC_ShowCheckedOut:
			if (siaLib.showCheckedOut(appOptions.getImageAddress()) == false) {
				return false;
			}
			
		case SIAArcAppOptions::SC_ShowUncheckedOutChanges:
			if (siaLib.showUncheckedOutChanges(appOptions.getImageAddress()) == false) {
				return false;
			}
			return true;
		case SIAArcAppOptions::SC_Unknown:
			return false;	
		}
		break;
	case SIAArcAppOptions::CM_Checkout:
		if (siaLib.checkout(appOptions.getImageAddress(), appOptions.getComment()) == false) {
			return false;
		}
		break;
	case SIAArcAppOptions::CM_Checkin:
		if (siaLib.checkin(appOptions.getImageAddress(), appOptions.getComment()) == false) {
			return false;
		}
		break;
	case SIAArcAppOptions::CM_UnCheckout:
		if (siaLib.uncheckout(appOptions.getImageAddress(), appOptions.getComment()) == false) {
			return false;
		}
		break;
	case SIAArcAppOptions::CM_View:
	{
		if (siaLib.view(appOptions.getName()) == false) {
			return false;
		}
		break;
	}
	case SIAArcAppOptions::CM_Prop:
	{
		if (siaLib.listContents(appOptions.getImageAddress()) == false) {
			return false;
		}
		break;
	}

	case SIAArcAppOptions::CM_Uncheckin:
		break;
	
	case SIAArcAppOptions::CM_Version:
		printf("\n\nSia version \"%s\" (build %s)\n", VERSION, BUILD);
		return true;
	case SIAArcAppOptions::CM_Unknown:
		break;
	}
	
	//m_ArchiveBuilder.checkout("/2014/2014-6-3/_DSC0001.jpg", "Some changes");
	//m_ArchiveBuilder.checkin("/2014/2014-6-3/GB-wp6.jpg", "Some changes");
	siaLib.complete();
	/*
	int i = 0;
	while (i < 1000) {
		SIASleep(500);
		printf("waiting\n");
		i++;
	}
	*/
	return true;
}

SIAArcApp::~SIAArcApp() {
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



} // simplearchive


/**
 *
 *
 */


int main(int argc, char **argv)
{
	simplearchive::SIAArcApp app;
	if (app.initaliseConfig() == false) {
		return false;
	}
	if (app.initaliseArgs(argc, argv) == false) {
		return false;
	}
	if (app.Run() == false) {
		int code = simplearchive::SIALib::getLastCode();
		return code;
	}
	
	return 1;

}


