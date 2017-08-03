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
**	Filename	: SIAArcAppOptions.cpp
**	Author		: I.Ferguson
**	Version		: 1.000
**	Date		: 26-05-2015
**
** #$$@@$$# */

#include "ConfigReader.h"
#include "SIAArcAppOptions.h"
#include "AppConfig.h"
#include "SAUtils.h"
#include "stdio.h"
#include "SIAArcArgvParser.h"
#include "Environment.h"
#include "EnvFunc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif



using namespace CommandLineProcessing;
namespace simplearchive {

	SIAArcAppOptions *SIAArcAppOptions::m_this = nullptr;

	bool SIAArcAppOptions::m_list = false;
	bool SIAArcAppOptions::m_usingFile = false;
bool SIAArcAppOptions::m_peekOnly = false;
bool SIAArcAppOptions::m_eventsOn = false; // UDP events
bool SIAArcAppOptions::m_serverOn = true; // false;
bool SIAArcAppOptions::m_forceDate = false; // false;
bool SIAArcAppOptions::m_useDate = false;
bool SIAArcAppOptions::m_useFileDate = false;
bool SIAArcAppOptions::m_useDateToday = false;
ExifDate SIAArcAppOptions::m_archiveDate;

std::string SIAArcAppOptions::m_name;
SIAArcAppOptions::CommandMode SIAArcAppOptions::m_commandMode = SIAArcAppOptions::CommandMode::CM_Unknown;
std::string SIAArcAppOptions::m_comment;
std::string SIAArcAppOptions::m_imageAddress;
std::string SIAArcAppOptions::m_distinationPath;
std::string SIAArcAppOptions::m_filePath;
int SIAArcAppOptions::m_udpPortNum = 64321;
std::string SIAArcAppOptions::m_udpAddress = "127.0.0.1";
int SIAArcAppOptions::m_tcpPortNum = 64322;
LogDocument::FormatType SIAArcAppOptions::m_formatType = LogDocument::FormatType::Human;


SIAArcAppOptions::SIAArcAppOptions() {
	m_argvParser.reset(new SIAArcArgvParser);
	m_usingFile = false;
	m_archiveDate.now();
};



#ifdef XXXXXXXX
bool SIAArcAppOptions::initalise(int argc, char **argv) {

	
	/*
	m_error = false;
	m_verbose = false;
	m_quiet = false;
	m_logLevel = "INFO";
	m_dry_run = false;
	*/
	CAppConfig &config = CAppConfig::get();
	config.init();
	/*
	const std::string key = "SIA_HOME";
	std::string temp = SAUtils::GetEnvironment(key);
	std::string homePath = temp;
	*/
	bool found = false;
	std::string homePath;
	// Looking the HKEY_LOCAL_MACHINE first
	if (GetEnv(homePath, true) == true) {
		//printf("Found SIA_HOME in system variables: %s", homePath.c_str());
		found = true;
	} else if (GetEnv(homePath, false) == true) {
		//printf("Found SIA_HOME in user variables: %s", homePath.c_str());
		found = true;
	} else {
		bool found = false;
		homePath = SAUtils::GetPOSIXEnv("ProgramData"); 
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
			homePath = SAUtils::GetPOSIXEnv("USERPROFILE");
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
	if (found = false) {
		printf("SIA Unable to start? No archive found in the default location or"
			   " the environment variable SIA_HOME not set.\nUse siaadmin to initalise an archive.\n");
		m_error = true;
		return false;
	}
	std::string configfile = homePath + "/config/" + "config.dat";
	if (SAUtils::FileExists(configfile.c_str()) == false) {
		m_error = true;
		printf("SIA Unable to start? No config.dat file found in the default location or"
			   " the environment variable SIA_HOME not set.\nUse siaadmin to initalise an archive.\n");
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
	m_argvParser->defineOption("add", "add new images to the archive.", ArgvParser::MasterOption);
	m_argvParser->defineOption("checkout", "Checkout images from archive.", ArgvParser::MasterOption);
	m_argvParser->defineOption("checkin", "Checkin images to archive.", ArgvParser::MasterOption);
	m_argvParser->defineOption("uncheckout", "Un-checkout images to archive.", ArgvParser::MasterOption);
	m_argvParser->defineOption("export", "Export images from archive.", ArgvParser::MasterOption);
	m_argvParser->defineOption("about", "prints the version information", ArgvParser::MasterOption);
	m_argvParser->defineOption("view", "View commands", ArgvParser::MasterOption);
	m_argvParser->defineOption("show", "Show details", ArgvParser::MasterOption);
	m_argvParser->defineOption("prop", "Manage image properties", ArgvParser::MasterOption);
	

	/*
	m_argvParser->defineOption("b", "Goes through the motions of running the subcommand but makes no\nactual changes ether disk or repository.", ArgvParser::NoOptionAttribute);
	m_argvParser->defineOptionAlternative("b", "backup");
	*/
	// Options
	m_argvParser->defineOption("i", "image address.", ArgvParser::OptionRequiresValue);
	m_argvParser->defineOptionAlternative("i", "image-address");

	m_argvParser->defineOption("n", "name of the view.", ArgvParser::OptionRequiresValue);
	m_argvParser->defineOptionAlternative("n", "name");

	m_argvParser->defineOption("q", "no output is sent to the terminal.", ArgvParser::NoOptionAttribute);
	m_argvParser->defineOptionAlternative("q", "quiet");

	m_argvParser->defineOption("e", "Generate progress events.", ArgvParser::NoOptionAttribute);
	m_argvParser->defineOptionAlternative("e", "events");

	m_argvParser->defineOption("s", "source of the images", ArgvParser::OptionRequiresValue);
	m_argvParser->defineOptionAlternative("s", "source-path");

	m_argvParser->defineOption("d", "destination of the images", ArgvParser::OptionRequiresValue);
	m_argvParser->defineOptionAlternative("d", "dist-path");

	m_argvParser->defineOption("S", "size of media", ArgvParser::OptionRequiresValue);
	m_argvParser->defineOptionAlternative("S", "media-size");

	m_argvParser->defineOption("m", "Goes through the motions of running the subcommand but makes no\nactual changes ether disk or repository.", ArgvParser::OptionRequiresValue);
	m_argvParser->defineOptionAlternative("m", "media-path");

	m_argvParser->defineOption("fd", "from date", ArgvParser::OptionRequiresValue);
	m_argvParser->defineOptionAlternative("fd", "from-date");

	m_argvParser->defineOption("f", "Specifies a file", ArgvParser::OptionRequiresValue);
	m_argvParser->defineOptionAlternative("f", "file");

	m_argvParser->defineOption("td", "to date", ArgvParser::OptionRequiresValue);
	m_argvParser->defineOptionAlternative("td", "to-date");

	m_argvParser->defineOption("a", "location of the archive root folder.", ArgvParser::OptionRequiresValue);
	m_argvParser->defineOptionAlternative("a", "archive-path");

	m_argvParser->defineOption("r", "location of the archive root folder.", ArgvParser::NoOptionAttribute);
	m_argvParser->defineOptionAlternative("r", "root");


	m_argvParser->defineOption("l", "Temporarily changes the logging level for the scope of this command session.", ArgvParser::OptionRequiresValue);
	m_argvParser->defineOptionAlternative("l", "logging-level");

	m_argvParser->defineOption("c", "Comment to be included in command", ArgvParser::OptionRequiresValue);
	m_argvParser->defineOptionAlternative("c", "comment");

	m_argvParser->defineOption("li", "List items", ArgvParser::OptionRequiresValue);
	m_argvParser->defineOptionAlternative("li", "list");

	m_argvParser->defineOption("force-date", "Overrides all dates found associated with the images in the selection", ArgvParser::OptionRequiresValue);
	m_argvParser->defineOption("default-date", "Uses this date if none found associated with an image", ArgvParser::OptionRequiresValue);
	
	m_argvParser->defineOption("checked-out", "Show checked out", ArgvParser::OptionRequiresValue); // =all =year{2015}
	m_argvParser->defineOption("unchecked-out", "Show changed images which are not checked out", ArgvParser::OptionRequiresValue);

	m_argvParser->defineCommandOption("add", "comment");
	m_argvParser->defineCommandOption("add", "logging-level");
	m_argvParser->defineCommandOption("add", "archive-path");
	m_argvParser->defineCommandOption("add", "source-path");
	
	m_argvParser->defineCommandOption("export", "comment");
	m_argvParser->defineCommandOption("export", "logging-level");
	m_argvParser->defineCommandOption("export", "dist-path");

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
		printf("%s", m_argvParser->generalHelp(80).c_str());
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
		setCommandMode(SIAArcAppOptions::CM_Version);
		cmdFound = true;
	}
	
	else if (m_argvParser->command("add") == true) {
		
		if (m_argvParser->foundOption("source-path") == true) {			
			std::string opt = m_argvParser->optionValue("source-path");
			//printf(opt.c_str()); printf("\n");
			config.setSourcePath(opt.c_str());			
		}
		if (m_argvParser->foundOption("archive-path") == true) {			
			std::string opt = m_argvParser->optionValue("archive-path");
			//printf(opt.c_str()); printf("\n");
			config.setWorkspacePath(opt.c_str());
		}
		if (m_argvParser->foundOption("file") == true) {
			m_filePath = m_argvParser->optionValue("file");
			m_usingFile = true;
			//printf(m_filePath.c_str()); printf("\n");
			
		}
		if (m_argvParser->foundOption("peek") == true) {
			m_peekOnly = true;
			//printf(m_filePath.c_str()); printf("\n");

		}
		if (m_argvParser->foundOption("force-date") == true) {
			std::string opt = m_argvParser->optionValue("force-date");
			//printf(opt.c_str()); printf("\n");
			if (opt.compare("FileDate") == 0) {
				m_useFileDate = true;
			}

			if (opt.compare("TodaysDate") == 0) {
				m_useDateToday = true;
			}
			else {
				m_archiveDate = ExifDate(opt.c_str());
			}
			m_forceDate = true;

		}
		if (m_argvParser->foundOption("default-date") == true) {
			std::string opt = m_argvParser->optionValue("default-date");
			//printf(opt.c_str()); printf("\n");
			config.setSourcePath(opt.c_str());
			if (opt.compare("FileDate") == 0) {
				m_useFileDate = true;
			}

			if (opt.compare("TodaysDate") == 0) {
				m_useDateToday = true;
			}
			else {
				m_archiveDate = ExifDate(opt.c_str());
			}
			m_useDate = true;
		}
		
		setCommandMode(SIAArcAppOptions::CM_Import);

		Environment::setEnvironment();
		cmdFound = true;
	}
	else if (m_argvParser->command("checkout") == true) {
		if (m_argvParser->foundOption("image-address") == true) {
			m_imageAddress = m_argvParser->optionValue("image-address");
			//printf(m_imageAddress.c_str()); printf("\n");
		}

		if (m_argvParser->foundOption("list") == true) {
			m_list = true;
			//printf(m_comment.c_str()); printf("\n");
		}

		if (m_argvParser->foundOption("file") == true) {
			m_filePath = m_argvParser->optionValue("file");
			m_usingFile = true;
			printf(m_filePath.c_str()); printf("\n");
		}

		if (m_argvParser->foundOption("comment") == true) {
			m_comment = m_argvParser->optionValue("comment");
			printf(m_comment.c_str()); printf("\n");
		}
		if (m_argvParser->foundOption("archive-path") == true) {
			std::string opt = m_argvParser->optionValue("archive-path");
			printf(opt.c_str()); printf("\n");
			config.setWorkspacePath(opt.c_str());
		}

		setCommandMode(SIAArcAppOptions::CM_Checkout);
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
			config.setWorkspacePath(opt.c_str());
		}


		setCommandMode(SIAArcAppOptions::CM_Checkin);
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
			config.setWorkspacePath(opt.c_str());
		}
		
		setCommandMode(SIAArcAppOptions::CM_UnCheckout);
		cmdFound = true;
	}
	else if (m_argvParser->command("show") == true) {
		if (m_argvParser->foundOption("checked-out") == true) {
			m_showCommandOption = ShowCommandOption::SC_ShowCheckedOut;
			m_imageAddress = m_argvParser->optionValue("checked-out");
			printf(m_imageAddress.c_str()); printf("\n");
		}
		if (m_argvParser->foundOption("unchecked-out") == true) {
			m_showCommandOption = ShowCommandOption::SC_ShowUncheckedOutChanges;
			m_imageAddress = m_argvParser->optionValue("checked-out");
			printf(m_imageAddress.c_str()); printf("\n");
		}
		setCommandMode(SIAArcAppOptions::CM_Show);
		cmdFound = true;
	}
	else if (m_argvParser->command("prop") == true) {
		if (m_argvParser->foundOption("list") == true) {
			//m_showCommandOption = ShowCommandOption::SC_ShowCheckedOut;
			m_imageAddress = m_argvParser->optionValue("list");
			printf(m_imageAddress.c_str()); printf("\n");
		}
		setCommandMode(SIAArcAppOptions::CM_Prop);
		cmdFound = true;
	}
	else if (m_argvParser->command("export") == true) {
		if (m_argvParser->foundOption("dist-path") == true) {
			m_distinationPath = m_argvParser->optionValue("dist-path");
			printf(m_distinationPath.c_str()); printf("\n");	
		}
		setCommandMode(SIAArcAppOptions::CM_Export);
		cmdFound = true;
	}
	else if (m_argvParser->command("view") == true) {
		setCommandMode(SIAArcAppOptions::CM_View);
		
		if (m_argvParser->foundOption("archive-path") == true) {

			std::string opt = m_argvParser->optionValue("archive-path");
			printf(opt.c_str()); printf("\n");
			config.setWorkspacePath(opt.c_str());
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

	
	
	
	else if (m_argvParser->command("version") == true) {
		setCommandMode(SIAArcAppOptions::CM_Version);
		cmdFound = true;
	}
	else {
		setCommandMode(SIAArcAppOptions::CM_Unknown);
		cmdFound = true;
	}
	// global options:

	if (m_argvParser->foundOption("events") == true) {
		m_eventsOn = true;
	}

	if (m_argvParser->foundOption("quiet") == true) {
		
		
		printf("quiet=On\n");
		config.setQuiet(true);
	}

	if (m_argvParser->foundOption("logging-level") == true) {

		std::string opt = m_argvParser->optionValue("logging-level");
		printf(opt.c_str()); printf("\n");
		config.setLogLevel(opt.c_str());
	}

	if (m_argvParser->foundOption("dry-run") == true) {
		
		std::string opt = m_argvParser->optionValue("dry-run");
		printf(opt.c_str()); printf("\n");
		
		//config.setDryRun(opt.c_str());
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
#endif

DefaultArgumentsContainer SIAArcAppOptions::defaultArgumentsContainer;


SIAArcAppOptions::~SIAArcAppOptions() {
	// TODO Auto-generated destructor stub
	if (m_this != nullptr) {
		delete m_this;
	}
}

SIAArcAppOptions &SIAArcAppOptions::get() {
	if (m_this == nullptr) {
		m_this = new SIAArcAppOptions;
	}
	return *m_this;
}

SIAArcAppOptions::CommandMode SIAArcAppOptions::getCommandMode() {
	return m_commandMode;
}

bool SIAArcAppOptions::setCommandMode(const char *modeString) {
	std::string mode = modeString;

	if (mode.compare("add") == 0) {
		m_commandMode = CommandMode::CM_Import;
		return true;
	}
	else if (mode.compare("add") == 0) {
		m_commandMode = CommandMode::CM_Export;
		return true;
	}
	else if (mode.compare("add") == 0) {
		m_commandMode = CommandMode::CM_Checkout;
		return true;
	}
	else if (mode.compare("add") == 0) {
		m_commandMode = CommandMode::CM_Checkin;
		return true;
	}
	else if (mode.compare("add") == 0) {
		m_commandMode = CommandMode::CM_Uncheckin;
		return true;
	}
	m_commandMode = CommandMode::CM_Unknown;
	//CommandLineProcessing::AppOptions::setError(12, "Unable to start, Command unknown .");
	return false;
}

void SIAArcAppOptions::setCommandMode(const SIAArcAppOptions::CommandMode mode) {
	m_commandMode = mode;
}

const char *SIAArcAppOptions::getName() {
	return m_name.c_str();
}

void SIAArcAppOptions::setName(const char *name) {
	m_name = name;
}

const char *SIAArcAppOptions::getComment() {
	return m_comment.c_str();
}
const char *SIAArcAppOptions::getImageAddress() {
	return m_imageAddress.c_str();
}
const char *SIAArcAppOptions::getDistinationPath() {
	return m_distinationPath.c_str();
}
const char *SIAArcAppOptions::getFilePath() {
	return m_filePath.c_str();
}

bool SIAArcAppOptions::isEnventsOn() {
	return m_eventsOn;
}

int SIAArcAppOptions::eventPort() {
	return m_udpPortNum;
	
}
const char *SIAArcAppOptions::eventAddress() {
	return m_udpAddress.c_str();
}

bool SIAArcAppOptions::isServerOn() {
	return m_serverOn;
}

int SIAArcAppOptions::serverPort() {
	return m_tcpPortNum;

}

bool SIAArcAppOptions::getUseFileDate() {
	return m_useFileDate;
}

bool SIAArcAppOptions::getUseDateToday() {
	return m_useDateToday;
}

bool SIAArcAppOptions::isDataForced() {
	return m_forceDate;
}

ExifDate &SIAArcAppOptions::getArchiveDate() {
	return m_archiveDate;
}

void SIAArcAppOptions::setDefaultArguments(std::string s) {
	defaultArgumentsContainer.push_back(s);
}

DefaultArgumentsContainer& SIAArcAppOptions::getDefaultArguments() {
	return defaultArgumentsContainer;
}

LogDocument::FormatType& SIAArcAppOptions::getFormatType() {
	return m_formatType;
}

} /* namespace simplearchive */
