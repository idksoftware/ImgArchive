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
#include "CLogger.h"
#include "CDate.h"
#include "AppConfig.h"
#include "CLogger.h"
#include "ImageExtentions.h"
#include "TargetsList.h"
#include "CIDKDate.h"
#include "ImagePath.h"

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
#include "AppOptions.h"
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

	using namespace CommandLineProcessing;

	
	SIAArcApp::SIAArcApp() : AppBase(new SIAArcArgvParser) {};



bool SIAArcApp::initaliseArgs(int argc, char **argv) {
	
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

bool SIAArcApp::initaliseConfig() {
	
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
	if (found) {
		config.init(homePath.c_str());
	}
	else {
		config.init();
	}
	
	if (SAUtils::DirExists(homePath.c_str()) == false) {
		setError(12, "SIA Unable to start?\nArchive not found at default location and the environment variable SA_HOME not set.\n"
			"Use siaadmin -i to create an empty archive at the default location (see documentation).\n");
		return false;
		
	}
	
	std::string configfile = homePath + "/config/" + "config.dat";
	std::string configPath = homePath + "/config";
	if (SAUtils::FileExists(configfile.c_str()) == true) {
		setConfigPath(configPath.c_str());
		ConfigReader configReader;
		configReader.setNoLogging();
		configReader.read(configfile.c_str(), config);
		config.fileBasedValues();
		
	}
	else {
		m_configured = false;
	}
	
	return true;
}

bool SIAArcApp::doRun()
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
	//printf("%s", config.toString().c_str());
	config.settup();
	if (siaLib.initalise() < 0) {
		return false;
	}
	/*
		INITALISING COMLETE ????
	*/
	switch (appOptions.getCommandMode()) {

		// run sub-comand
	case SIAArcAppOptions::CommandMode::CM_Import:

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
				setError(CLogger::getLastCode(), CLogger::getLastMessage());
				return false;
			}
		}
		break;
	case SIAArcAppOptions::CommandMode::CM_Export:
		if (siaLib.exportImage(appOptions.getDistinationPath()) == false) {
			setError(CLogger::getLastCode(), CLogger::getLastMessage());
			return false;
		}
		break;
	case SIAArcAppOptions::CommandMode::CM_Show:
		switch (appOptions.getShowCommandOption()) {
		case SIAArcAppOptions::ShowCommandOption::SC_ShowCheckedOut:
			if (siaLib.showCheckedOut(appOptions.getImageAddress()) == false) {
				setError(CLogger::getLastCode(), CLogger::getLastMessage());
				return false;
			}
			
		case SIAArcAppOptions::ShowCommandOption::SC_ShowUncheckedOutChanges:
			if (siaLib.showUncheckedOutChanges(appOptions.getImageAddress()) == false) {
				setError(CLogger::getLastCode(), CLogger::getLastMessage());
				return false;
			}
			return true;
		case SIAArcAppOptions::ShowCommandOption::SC_Unknown:
			return false;	
		}
		break;
	case SIAArcAppOptions::CommandMode::CM_Get:
		if (siaLib.get(appOptions.getImageAddress(), appOptions.getComment(), appOptions.isForced()) == false) {
			setError(CLogger::getLastCode(), CLogger::getLastMessage());
			return false;
		}
		break;
	case SIAArcAppOptions::CommandMode::CM_Checkout:
		if (siaLib.checkout(appOptions.getImageAddress(), appOptions.getComment(), appOptions.isForced()) == false) {
			setError(CLogger::getLastCode(), CLogger::getLastMessage());
			return false;
		}
		break;
	case SIAArcAppOptions::CommandMode::CM_Checkin:
		if (siaLib.checkin(appOptions.getImageAddress(), appOptions.getComment(), appOptions.isForced()) == false) {
			setError(CLogger::getLastCode(), CLogger::getLastMessage());
			return false;
		}
		break;
	case SIAArcAppOptions::CommandMode::CM_UnCheckout:
		if (siaLib.uncheckout(appOptions.getImageAddress(), appOptions.getComment(), appOptions.isForced()) == false) {
			setError(CLogger::getLastCode(), CLogger::getLastMessage());
			return false;
		}
		break;
	case SIAArcAppOptions::CommandMode::CM_View:
	{
		if (siaLib.view(appOptions.getName()) == false) {
			setError(CLogger::getLastCode(), CLogger::getLastMessage());
			return false;
		}
		break;
	}
	case SIAArcAppOptions::CommandMode::CM_Prop:
	{
		if (siaLib.listContents(appOptions.getImageAddress()) == false) {
			setError(CLogger::getLastCode(), CLogger::getLastMessage());
			return false;
		}
		break;
	}

	case SIAArcAppOptions::CommandMode::CM_Log:
	{
		if (siaLib.log(appOptions.getImageAddress(), appOptions.getFormatType()) == false) {
			setError(CLogger::getLastCode(), CLogger::getLastMessage());
			return false;
		}
		break;
	}


	case SIAArcAppOptions::CommandMode::CM_Uncheckin:
		break;
	
	case SIAArcAppOptions::CommandMode::CM_Version:
		printf("\n\nSia version \"%s\" (build %s)\n", VERSION, BUILD);
		return true;
	case SIAArcAppOptions::CommandMode::CM_Unknown:
		setError(CLogger::getLastCode(), CLogger::getLastMessage());
		break;
	}
	siaLib.complete();
	
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
	bool error = false;
	simplearchive::SIAArcApp app;
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
		return code;
	}
	else {
		return 0;
	}
}


