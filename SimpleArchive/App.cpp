/*
 * App.cpp
 *
 *  Created on: Feb 11, 2014
 *      Author: wzw7yn
 */

#include <stdio.h>
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
#include "Database.h"

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

	if (MirrorManager::initalise("/home/wzw7yn/test", "/home/wzw7yn/mirror.conf") == false) {
		return false;
	}

	CAppConfig &config = CAppConfig::get();
	AppOptions &appOptions = AppOptions::get();
	if (appOptions.getCommandMode() == AppOptions::CM_Version) {
		printf("\n\nSia version \"%s\" (build %s)\n", VERSION, BUILD);
		return true;
	}
	//CLogger::setLevel(CLogger::INFO);
	CLogger::setLevel(CLogger::FINE);
	CLogger::setLogPath(config.getLogPath());
	//ChangeLog::setLogPath(config.getHistoryPath());

	CLogger &logger = CLogger::getLogger();

	/* Logging started */

	CIDKDate date;
	date.Now();
	logger.log(CLogger::INFO, "=== Application starting at %s ===", date.Print().c_str());
	logger.log(CLogger::INFO, "Home path is \"%s\"", config.getHomePath());

	std::string temp = config.getArchivePath();

	if (SAUtils::DirExists(temp.c_str()) == false) {

		logger.log(CLogger::INFO, "Repository folder not found at location: \"%s\"", temp.c_str());

		if (SAUtils::mkDir(temp.c_str()) == false) {
			logger.log(CLogger::INFO, "Cannot create Repository folder at location: \"%s\"", temp.c_str());
			return false;
		}
		logger.log(CLogger::INFO, "Repository folder created at location: \"%s\"", temp.c_str());
	}

	if (ImagePath::settupMainArchiveFolders(config.getArchivePath()) == false) {

		return false;
	}

	History &history = History::getHistory();
	history.setPath(config.getHistoryPath());
	if (SAUtils::DirExists(temp.c_str()) == false) {
		logger.log(CLogger::INFO, "Hidden .sia folder not found at location: \"%s\"", temp.c_str());

		if (SAUtils::mkDir(temp.c_str()) == false) {
			logger.log(CLogger::INFO, "Cannot create Hidden .sia folder at location: \"%s\"", temp.c_str());
			return false;
		}
		if (SAUtils::setHidden(temp.c_str()) == false) {
			logger.log(CLogger::INFO, "Cannot set Hidden .sia folder to a hidden folder \"%s\"", temp.c_str());
			return false;
		}
		logger.log(CLogger::INFO, "Hidden .sia folder created at location: \"%s\"", temp.c_str());
	}

	logger.log(CLogger::FINE, "Log path \"%s\"", config.getLogPath());
	temp = config.getHistoryPath();
	if (SAUtils::DirExists(temp.c_str()) == false) {
		logger.log(CLogger::INFO, "History folder not found at location: \"%s\"", temp.c_str());

		if (SAUtils::mkDir(temp.c_str()) == false) {
			logger.log(CLogger::INFO, "Cannot create History folder at location: \"%s\"", temp.c_str());
			return false;
		}
		logger.log(CLogger::INFO, "History folder created at location: \"%s\"", temp.c_str());
	}
	temp = config.getIndexPath();
	if (SAUtils::DirExists(temp.c_str()) == false) {
		logger.log(CLogger::INFO, "Index folder not found at location: \"%s\"", temp.c_str());

		if (SAUtils::mkDir(temp.c_str()) == false) {
			logger.log(CLogger::INFO, "Cannot create Index folder at location: \"%s\"", temp.c_str());
			return false;
		}
		logger.log(CLogger::INFO, "Index folder created at location: \"%s\"", temp.c_str());
	}
	if (ImageExtentions::setExtentionsFilePath(config.getConfigPath()) == false) {
		return false;
	}
	HookCmd::setHookPath(config.getHookPath());
	m_ArchiveBuilder.setUseExternalExifTool(true);

	if (m_ArchiveBuilder.Init() == false) {
		return false;
	}

	Database &db = Database::getInstance();

	if (db.open(DB, "", "") == false) {
		printf("database open returned %s", db.getError());
	}


	logger.log(CLogger::INFO, "+++ Initalisation complete +++");
	// C:\output\2014\2014-6-3
	switch (appOptions.getCommandMode()) {
	case AppOptions::CM_Import:
		m_ArchiveBuilder.Import(config.getSourcePath());
		break;
	case AppOptions::CM_Export:
		break;
	case AppOptions::CM_Checkout:
		break;
	case AppOptions::CM_Checkin:
		break;
	case AppOptions::CM_View:
	{
		ViewManager viewManager;
		std::string name = appOptions.getName();
		if (name.compare("Master") == 0) {
			viewManager.initaliseMaster(config.getArchivePath(), config.getMasterViewPath());
			if (viewManager.processMaster() == false) {
				return false;
			}
		}
		else {

			if (viewManager.initalise(config.getArchivePath(), config.getConfigPath()) == false) {
				return false;
			}
			if (viewManager.process() == false) {
				return false;
			}
		}
		break;
	}
	case AppOptions::CM_Uncheckin:
		break;
	case AppOptions::CM_Archive:
		printf("Archive");
		if (config.isToDateSet() || config.isFromDateSet()) {
			//MakeMedia makeMedia(config.getArchivePath().c_str(), config.setBackupDestinationPath(), config.getBackupMediaSize(), CDate startDate, CDate endDate);
		}
		else {
			MakeMedia makeMedia;
			if (makeMedia.initalise(config.getArchivePath(), config.getBackupDestinationPath(), config.getBackupMediaSize()) == false) {
				return false;
			}
			if (makeMedia.process() == false) {
				return false;
			}
		}
		break;
	case AppOptions::CM_Unknown:
		break;
	}
	
	//m_ArchiveBuilder.checkout("/2014/2014-6-3/_DSC0001.jpg", "Some changes");
	//m_ArchiveBuilder.checkin("/2014/2014-6-3/GB-wp6.jpg", "Some changes");
	date.Now();
	logger.log(CLogger::INFO, "+++ Application completed successfully at %s +++", date.Print().c_str());
	return true;
}

App::~App() {
	CLogger::Close();
}

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


/**
 *
 *
 */

int main(int argc, char **argv)
{
	/*
	ArgvParser parser;  // the command line parser
	
	//define error codes
	parser.addErrorCode(0, "Success");
	parser.addErrorCode(1, "Error");
	parser.setIntroductoryDescription("The Image archive provides an organised place to store images. This archive is"
		"designed to be simple in design and to use. It consists of archiving core that provides the basic archiving"
		" functions but in addition, takes input and provides output from optional external components to provide a"
		"tailored achieving solution and can be extended into a complete achieving system. ");
	parser.setHelpOption();
	//parser.defineOption("a", "Adds new images to the archive.", ArgvParser::OptionRequiresValue);
	//parser.defineOptionAlternative("a", "add");

	// Subcommands
	parser.defineOption("a", "add new images to the archive.", ArgvParser::NoOptionAttribute);
	parser.defineOptionAlternative("a", "add");

	parser.defineOption("o", "Checkout images from archive.", ArgvParser::NoOptionAttribute);
	parser.defineOptionAlternative("o", "checkout");

	parser.defineOption("i", "Checkin images to archive.", ArgvParser::NoOptionAttribute);
	parser.defineOptionAlternative("i", "checkin");

	parser.defineOption("u", "Un-checkout images to archive.", ArgvParser::NoOptionAttribute);
	parser.defineOptionAlternative("u", "uncheckout");

	parser.defineOption("x", "Export images from archive.", ArgvParser::NoOptionAttribute);
	parser.defineOptionAlternative("x", "export");

	// Options
	parser.defineOption("v", "Verbose output is sent to the terminal.", ArgvParser::NoOptionAttribute);
	parser.defineOptionAlternative("v", "verbose");

	parser.defineOption("q", "no output is sent to the terminal.", ArgvParser::NoOptionAttribute);
	parser.defineOptionAlternative("q", "quiet");

	parser.defineOption("s", "source of the images", ArgvParser::OptionRequiresValue);
	parser.defineOptionAlternative("s", "source-path");

	parser.defineOption("r", "location of the archive root folder.", ArgvParser::OptionRequiresValue);
	parser.defineOptionAlternative("r", "archive-path");

	parser.defineOption("l", "Temporarily changes the logging level for the scope of this command session.", ArgvParser::OptionRequiresValue);
	parser.defineOptionAlternative("r", "logging-level");

	parser.defineOption("n", "Goes through the motions of running the subcommand but makes no\nactual changes ether disk or repository.", ArgvParser::OptionRequiresValue);
	parser.defineOptionAlternative("n", "dry-run");


	ArgvParser::ParserResults res = parser.parse(argc, argv);

	//testHelpOptionDetection();
	if (parser.foundOption("add") == true) {
		printf("== Found add ==\n");
		if (parser.foundOption("source-path") == true) {
			printf("== Found source ==\n");
			std::string opt = parser.optionValue("source-path");
			printf(opt.c_str()); printf("\n");
		}
		if (parser.foundOption("archive") == true) {
			printf("== Found archive ==\n");
			std::string opt = parser.optionValue("archive-path");
			printf(opt.c_str()); printf("\n");
		}
	}
	else if (parser.foundOption("checkout") == true) {
	}
	else if (parser.foundOption("checkin") == true) {
	}
	else if (parser.foundOption("uncheckout") == true) {
	}
	else if (parser.foundOption("export") == true) {
	}
	else {
		// error
	}


	
	if (res != ArgvParser::NoParserError) {
		printf("%s\n", parser.parseErrorDescription(res).c_str());
		printf("%s\n", parser.usageDescription().c_str());
	}
 	return -1;
	*/
	/*
	if (argc < 5) { // Check the value of argc. If not enough parameters have been passed, inform user and exit.
		std::cout << "Usage is -in <infile> -out <outdir>\n"; // Inform the user of how to use the program
		std::cin.get();
		exit(0);
	}
	else { // if we got enough parameters...
		std::string myFile;
		std::string myPath;
		std::string myOutPath;
		std::cout << argv[0];
		for (int i = 1; i < argc; i++) { // We will iterate over argv[] to get the parameters stored inside.
										 // Note that we're starting on 1 because we don't need to know the
										 // path of the program, which is stored in argv[0]
			if (i + 1 != argc) {// Check that we haven't finished parsing already
				if (argv[i] == "-f") {
					// We know the next argument *should* be the filename:
					myFile = argv[i + 1];
				}
				else if (argv[i] == "-p") {
					myPath = argv[i + 1];
				}
				else if (argv[i] == "-o") {
					myOutPath = argv[i + 1];
				}
				else {
					std::cout << "Not enough or invalid arguments, please try again.\n";
					exit(0);
				}
				std::cout << argv[i] << " ";
			}
		}
		//... some more code
		std::cin.get();
		return 0;
	}
	*/

	simplearchive::App app;
	if (app.initalise(argc, argv) == false) {
		return false;
	}
	if (app.Run() == false) {
		return 0;
	}
	return 1;

}


