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
#include "argvparser.h"
#include "HookCmd.h"
#include "MakeMedia.h"
#include "MirrorManager.h"
//#include "ViewManager.h"
#include "AboutCommand.h"

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
#include "HookCmd.h"
#include "ImagePath.h"
#include "HomePaths.h"
#include "UpdateTemplateManager.h"
//#include "DefaultEnvironment.h"


#define MAJORVERSION 0
#define MINORVERSION 9
#define REVISION     91





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

	
	SIAArcApp::SIAArcApp() : AppBase(std::make_shared<SIAArcArgvParser>()) {};


class CheckoutSummary : public SummaryReporter {
protected:
	virtual bool doProcess() {
		std::stringstream str;
		//int warnings = 0;
		//int errors = 0;
		//int competed = 0;
		std::shared_ptr<StatusList> list = getList();
		for (auto i = list->begin(); i != list->end(); i++) {
			//ReporterEvent& item = *i;
			
		}
		setSummary(str.str().c_str());

		return false;
	}
};

bool SIAArcApp::about(const char* outputType, const char* filename) {
	AboutCommand aboutCommand("Administrator", "iaadmin", VERSION, BUILD);
	aboutCommand.setOutputFile(filename);
	aboutCommand.setTextOutputType(outputType);

	if (aboutCommand.process() == false) {
		return false;
	}

	return true;
}

bool SIAArcApp::initaliseArgs(int argc, char **argv) {
	
	if (m_argvParser->doInitalise(argc, argv) == false) {
		return false;
	}

	return true;
}

bool SIAArcApp::initaliseConfig() {


	SIAARCConfig config;

	ImgArchiveHome& imgArchiveHome = ImgArchiveHome::getObject();
	bool res = imgArchiveHome.init();

	HomePath& HomePathObj = HomePath::getObject();
	HomePathType homePathType = HomePathObj.type();
	
	switch (homePathType) {
	case HomePathType::LocalEnv:	// Local Environment set
		if (res == false) {
			printf("Found IMGARCHIVE_HOME as local profile: %s but archive not found at loacation", HomePath::get().c_str());
			return false;
		}
		printf("Found IMGARCHIVE_HOME as local profile: %s. Archive found at that loacation", HomePath::get().c_str());
		break;
	case HomePathType::SystemEnv:	// System Environment set
		if (res == false) {
			printf("Found IMGARCHIVE_HOME as system profile: %s but archive not found at loacation", HomePath::get().c_str());
			return false;
		}
		printf("Found IMGARCHIVE_HOME as system profile: %s. Archive found at that loacation", HomePath::get().c_str());
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
	// Initalise without the config file i.e. set defaults.
	
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


bool SIAArcApp::doRun()
{
	SIAArcAppOptions &appOptions = SIAArcAppOptions::get();
	AppConfig &config = AppConfig::get();
	
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
	
	if (isConfiguratedOk() == false) {
		setError(12, "ImgArchive Unable to start? Archive not found at the specified location \"%s\".\n"
			"Use iaadmin -i to create an empty archive at the default location (see documentation).\n", config.getHomePath());
		return false;
	}
	
	
	if (siaLib.initalise(APP_NAME) < 0) {
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
		
		// using file list
		if (appOptions.isUsingFile()) {
			if (siaLib.ImportFile(appOptions.getFilePath())== false) {
				setError(CLogger::getLastCode(), CLogger::getLastMessage());
				return false;
			}
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
		if (siaLib.show() == false) {
			setError(CLogger::getLastCode(), CLogger::getLastMessage());
			return false;
		}
		break;
		
	case SIAArcAppOptions::CommandMode::CM_Checkout:
	{
		if (siaLib.checkout(appOptions.getImageAddress(), appOptions.getComment(), appOptions.isForced()) == false) {
			setError(CLogger::getLastCode(), CLogger::getLastMessage());
			return false;
		}
		StatusReporter& statusReporter = StatusReporter::get();
		SummaryReporter summaryReporter(statusReporter.getList());
		summaryReporter.process();
		summaryReporter.toConsole();
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
	case SIAArcAppOptions::CommandMode::CM_Get:
		if (siaLib.getImages(appOptions.getImageAddress(), appOptions.getComment(), appOptions.isForced()) == false) {
			setError(CLogger::getLastCode(), CLogger::getLastMessage());
			return false;
		}
		break;
	case SIAArcAppOptions::CommandMode::CM_Delete:
		if (siaLib.deleteImages(appOptions.getImageAddress(), appOptions.getComment(), appOptions.isForced()) == false) {
			setError(CLogger::getLastCode(), CLogger::getLastMessage());
			return false;
		}
		break;
	case SIAArcAppOptions::CommandMode::CM_Undelete:
		if (siaLib.undeleteImages(appOptions.getImageAddress(), appOptions.getComment()) == false) {
			setError(CLogger::getLastCode(), CLogger::getLastMessage());
			return false;
		}
		break;
	case SIAArcAppOptions::CommandMode::CM_History:
	{
		if (siaLib.history(appOptions.getImageAddress(), appOptions.getFormatType(), appOptions.getFilePath()) == false) {
			setError(CLogger::getLastCode(), CLogger::getLastMessage());
			return false;
		}
		break;
	}
	case SIAArcAppOptions::CommandMode::CM_Status:
	{
		//if (siaLib.status(appOptions.getImageAddress()) == false) {
		//	setError(CLogger::getLastCode(), CLogger::getLastMessage());
		//	return false;
		//}
		switch (appOptions.getShowCommandOption()) {
		case SIAArcAppOptions::ShowCommandOption::SC_ShowCheckedOut:
			if (siaLib.showCheckedOut(appOptions.getImageAddress(), appOptions.getFormatType(), appOptions.getFilePath()) == false) {
				setError(CLogger::getLastCode(), CLogger::getLastMessage());
				return false;
			}

		case SIAArcAppOptions::ShowCommandOption::SC_ShowUncheckedOutChanges:
			if (siaLib.showUncheckedOutChanges(appOptions.getImageAddress(), appOptions.getFormatType(), appOptions.getFilePath()) == false) {
				setError(CLogger::getLastCode(), CLogger::getLastMessage());
				return false;
			}
			return true;
		case SIAArcAppOptions::ShowCommandOption::SC_Unknown:
			return false;
		}

		break;
	}
	case SIAArcAppOptions::CommandMode::CM_Metadata:
	{
		std::string formatType = appOptions.getOption();
		if (formatType.empty()) {
			formatType = "human";
		}
		if (siaLib.metadata(appOptions.getImageAddress(), formatType.c_str(), appOptions.getFilePath()) == false) {
			setError(CLogger::getLastCode(), CLogger::getLastMessage());
			return false;
		}
		break;
	}
#ifdef NOT_USED
	case SIAArcAppOptions::CommandMode::CM_View:
	{
		if (siaLib.view(appOptions.getName()) == false) {
			setError(CLogger::getLastCode(), CLogger::getLastMessage());
			return false;
		}
		break;
	}
#endif
	case SIAArcAppOptions::CommandMode::CM_Workspace:
	{
		if (siaLib.workspace(appOptions.isSync())) {
			setError(CLogger::getLastCode(), CLogger::getLastMessage());
			return false;
		}
		break;
	}
	case SIAArcAppOptions::CommandMode::CM_Template:
	{
		std::string option = appOptions.getOption();
		if (!option.empty()) {
			std::string templatePath = config.getTemplatePath();
			UpdateTemplateManager updateTemplateManager(templatePath.c_str());
			if (updateTemplateManager.process(appOptions.isMaster(), appOptions.isCurrent(), option.c_str()) == false) {
				return false;
			}
		}
		if (siaLib.metadataTemplate(appOptions.isMaster(), appOptions.isCurrent()) == false) {
			setError(CLogger::getLastCode(), CLogger::getLastMessage());
			return false;
		}
		break;
	}
	case SIAArcAppOptions::CommandMode::CM_Prop:
	{
		siaLib.setProperty(appOptions.getImageAddress(), appOptions.getOption(), appOptions.getValue());
		/*
		if (siaLib.listContents(appOptions.getImageAddress()) == false) {
			setError(CLogger::getLastCode(), CLogger::getLastMessage());
			return false;
		}
		*/
		break;
	}

	case SIAArcAppOptions::CommandMode::CM_Log:
	{
		if (siaLib.log(appOptions.getImageAddress(), appOptions.getFromDate(), appOptions.getToDate(), appOptions.getFormatType(), appOptions.getFilePath()) == false) {
			setError(CLogger::getLastCode(), CLogger::getLastMessage());
			return false;
		}
		break;
	}

	case SIAArcAppOptions::CommandMode::CM_Mode:
	{

		if (siaLib.remoteServer() == false) {
			setError(CLogger::getLastCode(), CLogger::getLastMessage());
			return false;
		}
		break;
	}
	case SIAArcAppOptions::CommandMode::CM_Uncheckin:
		break;
	
	case SIAArcAppOptions::CommandMode::CM_About:
	{
		return about(appOptions.getTextOutputType(), appOptions.getOutputFile());
	}
	case SIAArcAppOptions::CommandMode::CM_Unknown:
		setError(CLogger::getLastCode(), CLogger::getLastMessage());
		break;
	default:
		setError(15, "Command not implimented at this time");
		return false;
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
 *C:\temp\2017\2017-08-12
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
		cout << "\n\n" << CommandLineProcessing::AppBase::getFullErrorString() << '\n';
		return code;
	}
	else {
		return 0;
	}
}


