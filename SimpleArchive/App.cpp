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

#ifdef _WIN32
#pragma comment(lib, "ws2_32.lib")
#endif

#include <iostream>
using namespace std;

#include <string>
#include <vector>
#include "AppOptions.h"
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

	SIALib siaLib;
	siaLib.initalise();

	
	AppOptions &appOptions = AppOptions::get();
	CAppConfig &config = CAppConfig::get();
	
	switch (appOptions.getCommandMode()) {
	case AppOptions::CM_InitArchive:
		printf("\n\nInit Archive\n");
		break;
	case AppOptions::CM_Import:
		siaLib.Import();
		break;
	case AppOptions::CM_Export:
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
	case AppOptions::CM_Mirror:
	{

		if (siaLib.mirror(appOptions.getName()) == false) {
			return false;
		}
		break;
	}
	case AppOptions::CM_Validate:
	{
		
		IntegrityManager &integrityManager = IntegrityManager::get(config.getShadowPath());
		
		if (integrityManager.makeList() == false) {
			return false;
		}
		//}
		break;
	}
	case AppOptions::CM_Uncheckin:
		break;
	case AppOptions::CM_Archive:
		if (siaLib.archive() == false) {
			return false;
		}
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
		return 0;
	}
	return 1;

}


