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
#include "App.h"
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
#include "AppOptions.h"
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




int test(const std::string key) {
	return 0;
}

App::App() {
}

#define BUFLEN 512	//Max length of buffer

bool App::initalise(int argc, char **argv) {

	AppOptions &appOptions = AppOptions::get();	
	if (appOptions.initalise(argc, argv) == false) {
		
		return false;
	}
	return true;
};




bool App::Run()
{
	AppOptions &appOptions = AppOptions::get();
	CAppConfig &config = CAppConfig::get();

	SIALib siaLib;
	// Set global options
	if (appOptions.isEnventsOn() == true) {

		siaLib.enableEvents(appOptions.eventAddress(), appOptions.eventPort());
	}

	if (appOptions.isServerOn() == true) {

		siaLib.enableServer(appOptions.eventPort());
	}

	if (siaLib.initalise() < 0) {
		return false;
	}

	switch (appOptions.getCommandMode()) {

		// run sub-comand
	case AppOptions::CM_Import:

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
	case AppOptions::CM_Export:
		if (siaLib.exportImage(appOptions.getImageAddress()) == false) {
			return false;
		}
		break;
	case AppOptions::CM_Show:
		switch (appOptions.getShowCommandOption()) {
		case AppOptions::SC_ShowCheckedOut:
			if (siaLib.showCheckedOut(appOptions.getImageAddress()) == false) {
				return false;
			}
			
		case AppOptions::SC_ShowUncheckedOutChanges:
			if (siaLib.showUncheckedOutChanges(appOptions.getImageAddress()) == false) {
				return false;
			}
			return true;
		case AppOptions::SC_Unknown:
			return false;	
		}
		break;
	case AppOptions::CM_Checkout:
		if (siaLib.checkout(appOptions.getImageAddress(), appOptions.getComment()) == false) {
			return false;
		}
		break;
	case AppOptions::CM_Checkin:
		if (siaLib.checkin(appOptions.getImageAddress(), appOptions.getComment()) == false) {
			return false;
		}
		break;
	case AppOptions::CM_UnCheckout:
		if (siaLib.uncheckout(appOptions.getImageAddress(), appOptions.getComment()) == false) {
			return false;
		}
		break;
	case AppOptions::CM_View:
	{
		if (siaLib.view(appOptions.getName()) == false) {
			return false;
		}
		break;
	}
	case AppOptions::CM_Prop:
	{
		if (siaLib.listContents(appOptions.getImageAddress()) == false) {
			return false;
		}
		break;
	}

	case AppOptions::CM_Uncheckin:
		break;
	
	case AppOptions::CM_Version:
		printf("\n\nSia version \"%s\" (build %s)\n", VERSION, BUILD);
		return true;
	case AppOptions::CM_Unknown:
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
		int code = simplearchive::SIALib::getLastCode();
		return code;
	}
	
	return 1;

}


