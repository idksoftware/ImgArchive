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

#include "ConfigReader.h"
#include "AppOptions.h"
#include "AppConfig.h"
#include "SAUtils.h"
#include "stdio.h"
#include "argvparser.h"
#include "Environment.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

using namespace CommandLineProcessing;
namespace simplearchive {

AppOptions *AppOptions::m_this = 0;
std::string AppOptions::m_name;
AppOptions::CommandMode AppOptions::m_commandMode = AppOptions::CM_Unknown;
std::string AppOptions::m_comment;
std::string AppOptions::m_imageAddress;

AppOptions::AppOptions() {
	m_argvParser.reset(new ArgvParser);
};



bool AppOptions::initalise(int argc, char **argv) {

	
	/*
	m_error = false;
	m_verbose = false;
	m_quiet = false;
	m_logLevel = "INFO";
	m_dry_run = false;
	*/
	CAppConfig &config = CAppConfig::get();
	const std::string key = "SA_HOME";
	std::string temp = SAUtils::GetEnvironment(key);
	std::string homePath = temp;
//	printf("%s", homePath.c_str());
	int i = homePath.length();
	if (homePath.empty() == true || homePath.length() == 0) {
		homePath = SAUtils::GetEnvironment("ProgramData");
		//C:\ProgramData\IDK Software\ImageArchive1.0
		homePath += "/IDK Software/ImageArchive1.0";
		
	}
	if (SAUtils::DirExists(homePath.c_str()) == false) {
		printf("SIA Unable to start? No archive found in the default location or the environment variable SA_HOME not set.\nUse siaadmin to initalise an archive.");
		m_error = true;
		return false;
	}
	std::string configfile = homePath + "/conf/" + "config.dat";
	if (SAUtils::FileExists(configfile.c_str()) == false) {
		m_error = true;
		return false;
	}
	ConfigReader configReader;
	configReader.setNoLogging();
	configReader.read(configfile.c_str(), config);
	// test only config.printAll();
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
	temp = SAUtils::GetEnvironment("SIA_ARCHIVE");
	if (temp.empty() == false) {
		config.setArchivePath(temp.c_str());
	}
	temp = SAUtils::GetEnvironment("SIA_SOURCE");
	if (temp.empty() == false) {
		config.setSourcePath(temp.c_str());
	}
	temp = SAUtils::GetEnvironment("SIA_LOGLEVEL");
	if (temp.empty() == false) {
		config.setLogLevel(temp.c_str());
	}
	
	

	//define error codes
	//define error codes
	m_argvParser->addErrorCode(0, "Success");
	m_argvParser->addErrorCode(1, "Warnings");
	m_argvParser->addErrorCode(2, "Errors");
	m_argvParser->addErrorCode(3, "Fatal");
	m_argvParser->setIntroductoryDescription("Image archive provides an organized place to store images. This archive is"
		" designed to be simple in design and to use. It consists of archiving core that provides the basic archiving"
		" functions but in addition, takes input and provides output from optional external components to provide a"
		" tailored achieving solution and can be extended into a complete achieving system. ");
	m_argvParser->setHelpOption();

	


	// Subcommands
	m_argvParser->defineOption("a", "add new images to the archive.", ArgvParser::MasterOption);
	m_argvParser->defineOptionAlternative("a", "add");

	m_argvParser->defineOption("o", "Checkout images from archive.", ArgvParser::MasterOption);
	m_argvParser->defineOptionAlternative("o", "checkout");

	m_argvParser->defineOption("i", "Checkin images to archive.", ArgvParser::MasterOption);
	m_argvParser->defineOptionAlternative("i", "checkin");

	m_argvParser->defineOption("u", "Un-checkout images to archive.", ArgvParser::MasterOption);
	m_argvParser->defineOptionAlternative("u", "uncheckout");

	m_argvParser->defineOption("x", "Export images from archive.", ArgvParser::MasterOption);
	m_argvParser->defineOptionAlternative("x", "export");

	m_argvParser->defineOption("about", "prints the version information", ArgvParser::MasterOption);

	m_argvParser->defineOption("v", "View commands", ArgvParser::NoOptionAttribute);
	m_argvParser->defineOptionAlternative("v", "view");

	m_argvParser->defineOption("image-address", "image address", ArgvParser::NoOptionAttribute);

	/*
	m_argvParser->defineOption("I", "Create Archive Commands", ArgvParser::NoOptionAttribute);
	m_argvParser->defineOptionAlternative("I", "init");

	m_argvParser->defineOption("validate", "Validate commands", ArgvParser::NoOptionAttribute);

	
	m_argvParser->defineOption("m", "Mirror commands", ArgvParser::NoOptionAttribute);
	m_argvParser->defineOptionAlternative("m", "mirror");

	m_argvParser->defineOption("b", "Goes through the motions of running the subcommand but makes no\nactual changes ether disk or repository.", ArgvParser::NoOptionAttribute);
	m_argvParser->defineOptionAlternative("b", "backup");
	*/
	// Options
	m_argvParser->defineOption("n", "name of the view.", ArgvParser::OptionRequiresValue);
	m_argvParser->defineOptionAlternative("n", "name");

	m_argvParser->defineOption("q", "no output is sent to the terminal.", ArgvParser::NoOptionAttribute);
	m_argvParser->defineOptionAlternative("q", "quiet");

	m_argvParser->defineOption("s", "source of the images", ArgvParser::OptionRequiresValue);
	m_argvParser->defineOptionAlternative("s", "source-path");

	m_argvParser->defineOption("d", "destination of the images", ArgvParser::OptionRequiresValue);
	m_argvParser->defineOptionAlternative("d", "dist-path");

	m_argvParser->defineOption("S", "size of media", ArgvParser::OptionRequiresValue);
	m_argvParser->defineOptionAlternative("S", "media-size");

	m_argvParser->defineOption("media-path", "Goes through the motions of running the subcommand but makes no\nactual changes ether disk or repository.", ArgvParser::OptionRequiresValue);

	m_argvParser->defineOption("F", "from date", ArgvParser::OptionRequiresValue);
	m_argvParser->defineOptionAlternative("F", "from-date");

	m_argvParser->defineOption("T", "to date", ArgvParser::OptionRequiresValue);
	m_argvParser->defineOptionAlternative("T", "to-date");

	m_argvParser->defineOption("p", "location of the archive root folder.", ArgvParser::OptionRequiresValue);
	m_argvParser->defineOptionAlternative("p", "archive-path");

	m_argvParser->defineOption("l", "Temporarily changes the logging level for the scope of this command session.", ArgvParser::OptionRequiresValue);
	m_argvParser->defineOptionAlternative("l", "logging-level");

	m_argvParser->defineOption("C", "Comment to be included in command", ArgvParser::OptionRequiresValue);
	m_argvParser->defineOptionAlternative("C", "comment");

	

	m_argvParser->defineCommandOption("add", "comment");
	m_argvParser->defineCommandOption("add", "logging-level");
	m_argvParser->defineCommandOption("add", "archive-path");
	m_argvParser->defineCommandOption("add", "source-path");
	

	
	
	ArgvParser::ParserResults res = m_argvParser->parse(argc, argv);

	std::string errStr;
	switch (res) {
	case ArgvParser::NoParserError:
		break;
	case ArgvParser::ParserUnknownOption:
	case ArgvParser::ParserMissingValue:
	case ArgvParser::ParserOptionAfterArgument:
	case ArgvParser::ParserMalformedMultipleShortOption:
	case ArgvParser::ParserRequiredOptionMissing:
	case ArgvParser::ParserHelpRequested:
		errStr = m_argvParser->parseErrorDescription(res);
		printf("%s", errStr.c_str());
		return false;
	case ArgvParser::GeneralHelpRequested:
		printf("%s", m_argvParser->usageDescription(80).c_str());
		return false;
	case ArgvParser::TopicHelpRequested:
		printf("%s", m_argvParser->topicUsageDescription(m_argvParser->getCurrentCommandId(), 80).c_str());
		return false;
	default:
		return false;
	}

	//testHelpOptionDetection();
	bool cmdFound = false;

	if (m_argvParser->command("about") == true) {
		setCommandMode(AppOptions::CM_Version);
		cmdFound = true;
	}
	else if (m_argvParser->command("init") == true) {
		setCommandMode(AppOptions::CM_InitArchive);
		cmdFound = true;
	}
	else if (m_argvParser->command("add") == true) {
		
		if (m_argvParser->foundOption("source-path") == true) {			
			std::string opt = m_argvParser->optionValue("source-path");
			printf(opt.c_str()); printf("\n");
			config.setSourcePath(opt.c_str());			
		}
		if (m_argvParser->foundOption("archive-path") == true) {			
			std::string opt = m_argvParser->optionValue("archive-path");
			printf(opt.c_str()); printf("\n");
			config.setArchivePath(opt.c_str());
		}
		setCommandMode(AppOptions::CM_Import);

		Environment::setEnvironment();
		cmdFound = true;
	}
	else if (m_argvParser->command("checkout") == true) {
		if (m_argvParser->foundOption("image-address") == true) {
			m_imageAddress = m_argvParser->optionValue("image-address");
			printf(m_imageAddress.c_str()); printf("\n");
		}
		if (m_argvParser->foundOption("comment") == true) {
			m_comment = m_argvParser->optionValue("comment");
			printf(m_comment.c_str()); printf("\n");
		}
		if (m_argvParser->foundOption("archive-path") == true) {
			std::string opt = m_argvParser->optionValue("archive-path");
			printf(opt.c_str()); printf("\n");
			config.setArchivePath(opt.c_str());
		}

		setCommandMode(AppOptions::CM_Checkout);
		cmdFound = true;
	}
	else if (m_argvParser->command("checkin") == true) {
		
		if (m_argvParser->foundOption("image-address") == true) {
			m_imageAddress = m_argvParser->optionValue("image-address");
			printf(m_imageAddress.c_str()); printf("\n");
		}
		if (m_argvParser->foundOption("comment") == true) {
			m_comment = m_argvParser->optionValue("comment");
			printf(m_comment.c_str()); printf("\n");
		}
		if (m_argvParser->foundOption("archive-path") == true) {
			std::string opt = m_argvParser->optionValue("archive-path");
			printf(opt.c_str()); printf("\n");
			config.setArchivePath(opt.c_str());
		}


		setCommandMode(AppOptions::CM_Checkin);
		cmdFound = true;
	}
	else if (m_argvParser->command("uncheckout") == true) {
		if (m_argvParser->foundOption("image-address") == true) {
			m_imageAddress = m_argvParser->optionValue("image-address");
			printf(m_imageAddress.c_str()); printf("\n");
		}
		if (m_argvParser->foundOption("comment") == true) {
			m_comment = m_argvParser->optionValue("comment");
			printf(m_comment.c_str()); printf("\n");
		}
		if (m_argvParser->foundOption("archive-path") == true) {
			std::string opt = m_argvParser->optionValue("archive-path");
			printf(opt.c_str()); printf("\n");
			config.setArchivePath(opt.c_str());
		}
		
		setCommandMode(AppOptions::CM_UnCheckout);
		cmdFound = true;
	}
	else if (m_argvParser->command("export") == true) {
		setCommandMode(AppOptions::CM_Export);
		cmdFound = true;
	}
	else if (m_argvParser->command("view") == true) {
		setCommandMode(AppOptions::CM_View);
		
		if (m_argvParser->foundOption("archive-path") == true) {

			std::string opt = m_argvParser->optionValue("archive-path");
			printf(opt.c_str()); printf("\n");
			config.setArchivePath(opt.c_str());
		}
		/*
		if (m_argvParser->foundOption("dist-path") == true) {

			std::string opt = m_argvParser->optionValue("dist-path");
			printf(opt.c_str()); printf("\n");
			config.setBackupDestinationPath(opt.c_str());

		}
		if (m_argvParser->foundOption("size") == true) {

			std::string opt = m_argvParser->optionValue("size");
			printf(opt.c_str()); printf("\n");
			config.setBackupMediaSize(opt.c_str());

		}
		if (m_argvParser->foundOption("from-date") == true) {

			std::string opt = m_argvParser->optionValue("from-date");
			printf(opt.c_str()); printf("\n");
			config.setFromDate(opt.c_str());

		}
		*/
		if (m_argvParser->foundOption("name") == true) {
			std::string opt = m_argvParser->optionValue("name");
			printf(opt.c_str()); printf("\n");
			setName(opt.c_str());
		}
		
		cmdFound = true;
	}

	else if (m_argvParser->command("validate") == true) {
		setCommandMode(AppOptions::CM_Validate);
		cmdFound = true;
		if (m_argvParser->foundOption("archive-path") == true) {

			std::string opt = m_argvParser->optionValue("archive-path");
			printf(opt.c_str()); printf("\n");
			config.setArchivePath(opt.c_str());
		}
	}
	else if (m_argvParser->command("mirror") == true) {
		setCommandMode(AppOptions::CM_Mirror);

		if (m_argvParser->foundOption("archive-path") == true) {

			std::string opt = m_argvParser->optionValue("archive-path");
			printf(opt.c_str()); printf("\n");
			config.setArchivePath(opt.c_str());
		}
		/*
		if (m_argvParser->foundOption("dist-path") == true) {

		std::string opt = m_argvParser->optionValue("dist-path");
		printf(opt.c_str()); printf("\n");
		config.setBackupDestinationPath(opt.c_str());

		}
		if (m_argvParser->foundOption("size") == true) {

		std::string opt = m_argvParser->optionValue("size");
		printf(opt.c_str()); printf("\n");
		config.setBackupMediaSize(opt.c_str());

		}
		if (m_argvParser->foundOption("from-date") == true) {

		std::string opt = m_argvParser->optionValue("from-date");
		printf(opt.c_str()); printf("\n");
		config.setFromDate(opt.c_str());

		}
		*/
		if (m_argvParser->foundOption("name") == true) {
			std::string opt = m_argvParser->optionValue("name");
			printf(opt.c_str()); printf("\n");
			setName(opt.c_str());
		}

		cmdFound = true;
	}
	else if (m_argvParser->command("backup") == true) {
		setCommandMode(AppOptions::CM_Archive);
		
		if (m_argvParser->foundOption("archive-path") == true) {

			std::string opt = m_argvParser->optionValue("archive-path");
			printf(opt.c_str()); printf("\n");
			config.setArchivePath(opt.c_str());
		}

		if (m_argvParser->foundOption("dist-path") == true) {

			std::string opt = m_argvParser->optionValue("dist-path");
			printf(opt.c_str()); printf("\n");
			config.setBackupDestinationPath(opt.c_str());

		}
		if (m_argvParser->foundOption("size") == true) {

			std::string opt = m_argvParser->optionValue("size");
			printf(opt.c_str()); printf("\n");
			config.setBackupMediaSize(opt.c_str());

		}
		if (m_argvParser->foundOption("from-date") == true) {

			std::string opt = m_argvParser->optionValue("from-date");
			printf(opt.c_str()); printf("\n");
			config.setFromDate(opt.c_str());

		}
		if (m_argvParser->foundOption("to-date") == true) {

			std::string opt = m_argvParser->optionValue("to-date");
			printf(opt.c_str()); printf("\n");
			config.setToDate(opt.c_str());

		}
		cmdFound = true;
	}
	else if (m_argvParser->command("version") == true) {
		setCommandMode(AppOptions::CM_Version);
		cmdFound = true;
	}
	else {
		setCommandMode(AppOptions::CM_Unknown);
		cmdFound = true;
	}
	if (m_argvParser->foundOption("logging-level") == true) {
		
		std::string opt = m_argvParser->optionValue("logging-level");
		printf(opt.c_str()); printf("\n");
		config.setLogLevel(opt.c_str());
	}
	if (m_argvParser->foundOption("dry-run") == true) {
		
		std::string opt = m_argvParser->optionValue("dry-run");
		printf(opt.c_str()); printf("\n");
		
		config.setDryRun(opt.c_str());
	}

	if (m_argvParser->foundOption("media-path") == true) {

		std::string opt = m_argvParser->optionValue("media-path");
		printf(opt.c_str()); printf("\n");
		//config.setDryRun(opt.c_str());
	}

	if (m_argvParser->foundOption("media-size") == true) {

		std::string opt = m_argvParser->optionValue("media-size");
		printf(opt.c_str()); printf("\n");
		//config.setDryRun(opt.c_str());
	}

	if (res != ArgvParser::NoParserError) {
		printf("%s\n", m_argvParser->parseErrorDescription(res).c_str());
		printf("%s\n", m_argvParser->usageDescription().c_str());
	}
	if (cmdFound == false) {
		printf("Main command required?\n\n");
		printf("%s\n", m_argvParser->usageDescription().c_str());
		return false;
	}
	return true;
}

AppOptions::~AppOptions() {
	// TODO Auto-generated destructor stub
}

AppOptions &AppOptions::get() {
	if (m_this == NULL) {
		m_this = new AppOptions;
	}
	return *m_this;
}

AppOptions::CommandMode AppOptions::getCommandMode() {
	return m_commandMode;
}

bool AppOptions::setCommandMode(const char *modeString) {
	std::string mode = modeString;

	if (mode.compare("add") == 0) {
		m_commandMode = CM_Import;
		return true;
	}
	else if (mode.compare("add") == 0) {
		m_commandMode = CM_Export;
		return true;
	}
	else if (mode.compare("add") == 0) {
		m_commandMode = CM_Checkout;
		return true;
	}
	else if (mode.compare("add") == 0) {
		m_commandMode = CM_Checkin;
		return true;
	}
	else if (mode.compare("add") == 0) {
		m_commandMode = CM_Uncheckin;
		return true;
	}
	m_commandMode = CM_Unknown;
	return false;
}

void AppOptions::setCommandMode(const AppOptions::CommandMode mode) {
	m_commandMode = mode;
}

const char *AppOptions::getName() {
	return m_name.c_str();
}

void AppOptions::setName(const char *name) {
	m_name = name;
}

const char *AppOptions::getComment() {
	return m_comment.c_str();
}
const char *AppOptions::getImageAddress() {
	return m_imageAddress.c_str();
}

} /* namespace simplearchive */
