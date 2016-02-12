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
#include "AdminAppOptions.h"
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
std::string AppOptions::m_homePath;
std::string AppOptions::m_workspacePath;
std::string AppOptions::m_shadowPath;
std::string AppOptions::m_configPath;
AppOptions::Scope AppOptions::m_scope = AppOptions::Both;
bool AppOptions::m_repair = false;
bool AppOptions::m_users = true;

AppOptions::AppOptions() {
	m_configured = true;
}

bool AppOptions::initaliseConfig() {

	CAppConfig &config = CAppConfig::get();
	const std::string key = "SIA_HOME";
	std::string temp = SAUtils::GetEnvironment(key);
	std::string homePath = temp;
	printf("%s", homePath.c_str());
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
			config.printAll();
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

bool AppOptions::initalise(int argc, char **argv) {

	ArgvParser argvParser;
	

	//define error codes
	argvParser.addErrorCode(0, "Success");
	argvParser.addErrorCode(1, "Error");
	argvParser.setIntroductoryDescription("The Image archive provides an organised place to store images. This archive is"
		"designed to be simple in design and to use. It consists of archiving core that provides the basic archiving"
		" functions but in addition, takes input and provides output from optional external components to provide a"
		"tailored achieving solution and can be extended into a complete achieving system. ");
	argvParser.setHelpOption();
	
	argvParser.setHeader("usage: siaadmin subcommand [options] [args]\n\n"
		"Image archive command line client, version 1.0.0.1\n"
		"Type 'siaadmin help <subcommand>' for help on a specific subcommand.\n\n"
		"siaadmin is the primary command-line interface to administer a Simple Image Archive. This interface is used to manage the control of images"
		" going in and out of the archive software. "
		"It has a rich set of subcommands that control the configuration of the archive and \"export\" images out of the archive,"
		"\n"
		"Note:\n"
		"The administration of the archive is carried out by the siaadmin command-line interface."
		"\n");
	

	// Subcommands

	
	argvParser.defineOption("i", "Create Archive Commands", ArgvParser::MasterOption);
	argvParser.defineOptionAlternative("i", "init");

	argvParser.defineOption("s", "Show settings", ArgvParser::MasterOption);
	argvParser.defineOptionAlternative("s", "show");

	// Arguments
	argvParser.defineOption("about", "prints the version information", ArgvParser::MasterOption);

	
	argvParser.defineOption("validate", "Validate commands", ArgvParser::MasterOption);

	argvParser.defineOption("m", "Mirror commands", ArgvParser::MasterOption);
	argvParser.defineOptionAlternative("m", "mirror");
	/*
	argvParser.defineOption("image-address", "image address", ArgvParser::NoOptionAttribute);

	argvParser.defineOption("m", "Mirror commands", ArgvParser::NoOptionAttribute);
	argvParser.defineOptionAlternative("m", "mirror");

	argvParser.defineOption("b", "Goes through the motions of running the subcommand but makes no\nactual changes ether disk or repository.", ArgvParser::NoOptionAttribute);
	argvParser.defineOptionAlternative("b", "backup");
	*/
	// Options
	argvParser.defineOption("n", "name of the item.", ArgvParser::OptionRequiresValue);
	argvParser.defineOptionAlternative("n", "name");

	argvParser.defineOption("r", "Validate and repair.", ArgvParser::NoOptionAttribute);
	argvParser.defineOptionAlternative("r", "repair");

	argvParser.defineOption("s", "Scope validate. i.e. Workspace/Shadow or both", ArgvParser::OptionRequiresValue);
	argvParser.defineOptionAlternative("s", "scope");

	argvParser.defineOption("u", "Make archive available to you only or everyone with a logon to this computer (Myself/All).", ArgvParser::OptionRequiresValue);
	argvParser.defineOptionAlternative("u", "users");

	/*
	argvParser.defineOption("d", "destination of the images", ArgvParser::OptionRequiresValue);
	argvParser.defineOptionAlternative("d", "dist-path");

	argvParser.defineOption("S", "size of media", ArgvParser::OptionRequiresValue);
	argvParser.defineOptionAlternative("S", "media-size");

	argvParser.defineOption("media-path", "Goes through the motions of running the subcommand but makes no\nactual changes ether disk or repository.", ArgvParser::OptionRequiresValue);

	argvParser.defineOption("F", "from date", ArgvParser::OptionRequiresValue);
	argvParser.defineOptionAlternative("F", "from-date");

	argvParser.defineOption("T", "to date", ArgvParser::OptionRequiresValue);
	argvParser.defineOptionAlternative("T", "to-date");

	argvParser.defineOption("r", "location of the archive root folder.", ArgvParser::OptionRequiresValue);
	argvParser.defineOptionAlternative("r", "archive-path");

	argvParser.defineOption("w", "location of the workspace folder.", ArgvParser::OptionRequiresValue);
	argvParser.defineOptionAlternative("w", "workspace");

	argvParser.defineOption("l", "Temporarily changes the logging level for the scope of this command session.", ArgvParser::OptionRequiresValue);
	argvParser.defineOptionAlternative("r", "logging-level");

	argvParser.defineOption("C", "Comment to be included in command", ArgvParser::OptionRequiresValue);
	argvParser.defineOptionAlternative("C", "comment");

	argvParser.defineOption("w", "Location of the archive Workspace", ArgvParser::OptionRequiresValue);
	argvParser.defineOptionAlternative("w", "workspace-path");
	
	argvParser.defineCommandOption("init", "archive-path");
	argvParser.defineCommandOption("init", "workspace-path");
	*/

	ArgvParser::ParserResults res = argvParser.parse(argc, argv);

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
		errStr = argvParser.parseErrorDescription(res);
		printf("%s", errStr.c_str());
		return false;
	case ArgvParser::GeneralHelpRequested:
		printf("%s", argvParser.usageDescription(80).c_str());
		return false;
	case ArgvParser::TopicHelpRequested:
		printf("%s", argvParser.topicUsageDescription(argvParser.getCurrentCommandId(), 80).c_str());
		return false;
	case ArgvParser::ParserCommandNotFound:
		//printf("%s", argvParser.topicUsageDescription(argvParser.getCurrentCommandId(), 80).c_str());
		return false;
	default:
		return false;
	}

	//testHelpOptionDetection();
	bool cmdFound = false;
	CAppConfig &config = CAppConfig::get();
	
	if (argvParser.command("init") == true) {
		// This command will initalise the configuration.
		// so the the configuration need not to be initalised.
		setCommandMode(AppOptions::CM_InitArchive);
		m_configured = false;
		std::string opt;
		if (argvParser.foundOption("users") == true) {
			std::string users = argvParser.optionValue("users");
			if (users.compare("Myself") == 0) {
				m_users = false;
			}
		}
		if (argvParser.foundOption("archive-path") == true) {
			opt = argvParser.optionValue("archive-path");
		}
		else {
			std::string progPath = SAUtils::GetEnvironment("ProgramData");
			std::string siaPath = "/IDK Software/ImageArchive1.0";
			if (SAUtils::makePath(progPath.c_str(), siaPath.c_str()) == false) {
				return false;
			}
			opt = progPath;
			opt += siaPath;
		}
		if (SAUtils::FileExists(opt.c_str()) == false) {
			return false;
		}
		setHomePath(opt.c_str());

		if (argvParser.foundOption("workspace-path") == true) {
			std::string opt = argvParser.optionValue("workspace-path");
		}
		else {
			std::string temp = SAUtils::GetEnvironment("USERPROFILE");
			opt = temp + "/Documents";
			if (SAUtils::FileExists(temp.c_str()) == false) {
				return false;
			}
			opt = opt + "/SIA Workspace";
			if (SAUtils::FileExists(opt.c_str()) == false) {
				if (SAUtils::mkDir(opt.c_str()) == false) {
					return false;
				}
			}
			
		}
		if (SAUtils::FileExists(opt.c_str()) == false) {
			return false;
		}
		setWorkspacePath(opt.c_str());
		
		if (argvParser.foundOption("repository-path") == true) {
			std::string opt = argvParser.optionValue("repository-path");
		}
		else {
			std::string progPath = SAUtils::GetEnvironment("ProgramData");
			std::string siaPath = "/IDK Software/ImageArchive1.0/shadow";
			if (SAUtils::makePath(progPath.c_str(), siaPath.c_str()) == false) {
				return false;
			}
			opt = progPath;
			opt += siaPath;
		}
		if (SAUtils::FileExists(opt.c_str()) == false) {
			return false;
		}
		setShadowPath(opt.c_str());
		cmdFound = true;
	}
	else {
		if (initaliseConfig() == false) {
			return false;
		}
		
		if (argvParser.command("about") == true) {
			setCommandMode(AppOptions::CM_Version);
			cmdFound = true;
		}
		else if (argvParser.command("show") == true) {
			setCommandMode(AppOptions::CM_Show);
			if (argvParser.foundOption("name") == true) {
				std::string opt = argvParser.optionValue("name");
				printf(opt.c_str()); printf("\n");
				setName(opt.c_str());
			}
			cmdFound = true;
		}
		else if (argvParser.command("validate") == true) {
			setCommandMode(AppOptions::CM_Validate);
			cmdFound = true;
			if (argvParser.foundOption("archive-path") == true) {

				std::string opt = argvParser.optionValue("archive-path");
				printf(opt.c_str()); printf("\n");
				config.setWorkspacePath(opt.c_str());
			}
			if (argvParser.foundOption("scope") == true) {

				std::string opt = argvParser.optionValue("scope");
				printf(opt.c_str()); printf("\n");
				if (opt.compare("Workspace") == 0) {
					m_scope = Workspace;
				}
				else if (opt.compare("Shadow") == 0) {
					m_scope = Shadow;
				}
				else {
					m_scope = Both;
				}
			}
			if (argvParser.foundOption("repair") == true) {
				m_repair = true;
			}
		}
		else if (argvParser.command("mirror") == true) {
			setCommandMode(AppOptions::CM_Mirror);

			if (argvParser.foundOption("archive-path") == true) {

				std::string opt = argvParser.optionValue("archive-path");
				printf(opt.c_str()); printf("\n");
				config.setWorkspacePath(opt.c_str());
			}
			/*
			if (argvParser.foundOption("dist-path") == true) {

			std::string opt = argvParser.optionValue("dist-path");
			printf(opt.c_str()); printf("\n");
			config.setBackupDestinationPath(opt.c_str());

			}
			if (argvParser.foundOption("size") == true) {

			std::string opt = argvParser.optionValue("size");
			printf(opt.c_str()); printf("\n");
			config.setBackupMediaSize(opt.c_str());

			}
			if (argvParser.foundOption("from-date") == true) {

			std::string opt = argvParser.optionValue("from-date");
			printf(opt.c_str()); printf("\n");
			config.setFromDate(opt.c_str());

			}
			*/
			if (argvParser.foundOption("name") == true) {
				std::string opt = argvParser.optionValue("name");
				printf(opt.c_str()); printf("\n");
				setName(opt.c_str());
			}

			cmdFound = true;
		}
		else if (argvParser.foundOption("backup") == true) {
			setCommandMode(AppOptions::CM_Archive);

			if (argvParser.foundOption("archive-path") == true) {

				std::string opt = argvParser.optionValue("archive-path");
				printf(opt.c_str()); printf("\n");
				config.setWorkspacePath(opt.c_str());
			}

			if (argvParser.foundOption("dist-path") == true) {

				std::string opt = argvParser.optionValue("dist-path");
				printf(opt.c_str()); printf("\n");
				config.setBackupDestinationPath(opt.c_str());

			}
			if (argvParser.foundOption("size") == true) {

				std::string opt = argvParser.optionValue("size");
				printf(opt.c_str()); printf("\n");
				config.setBackupMediaSize(opt.c_str());

			}
			if (argvParser.foundOption("from-date") == true) {

				std::string opt = argvParser.optionValue("from-date");
				printf(opt.c_str()); printf("\n");
				config.setFromDate(opt.c_str());

			}
			if (argvParser.foundOption("to-date") == true) {

				std::string opt = argvParser.optionValue("to-date");
				printf(opt.c_str()); printf("\n");
				config.setToDate(opt.c_str());

			}
			cmdFound = true;
		}
		else if (argvParser.foundOption("version") == true) {
			setCommandMode(AppOptions::CM_Version);
			cmdFound = true;
		}
		else {
			setCommandMode(AppOptions::CM_Unknown);
			cmdFound = true;
		}
	}
	if (argvParser.foundOption("logging-level") == true) {
		
		std::string opt = argvParser.optionValue("logging-level");
		printf(opt.c_str()); printf("\n");
		config.setLogLevel(opt.c_str());
	}
	if (argvParser.foundOption("dry-run") == true) {
		
		std::string opt = argvParser.optionValue("dry-run");
		printf(opt.c_str()); printf("\n");
		config.setDryRun(opt.c_str());
	}

	if (argvParser.foundOption("media-path") == true) {

		std::string opt = argvParser.optionValue("media-path");
		printf(opt.c_str()); printf("\n");
		//config.setDryRun(opt.c_str());
	}

	if (argvParser.foundOption("media-size") == true) {

		std::string opt = argvParser.optionValue("media-size");
		printf(opt.c_str()); printf("\n");
		//config.setDryRun(opt.c_str());
	}

	if (res != ArgvParser::NoParserError) {
		printf("%s\n", argvParser.parseErrorDescription(res).c_str());
		//printf("%s\n", argvParser.usageDescription().c_str());
	}
	if (cmdFound == false) {
		printf("Main command required?\n\n");
		printf("%s\n", argvParser.usageDescription().c_str());
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

	/*
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
	*/
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

void AppOptions::setHomePath(const char *homePath) {
	m_homePath = homePath;
}

void AppOptions::setWorkspacePath(const char *workspacePath) {
	m_workspacePath = workspacePath;
}
void AppOptions::setShadowPath(const char *shadowPath) {
	m_shadowPath = shadowPath;
}

void AppOptions::setConfigPath(const char *configPath) {
	m_configPath = configPath;
}

const char *AppOptions::getComment() {
	return m_comment.c_str();
}
const char *AppOptions::getImageAddress() {
	return m_imageAddress.c_str();
}

const char *AppOptions::getHomePath() {
	return m_homePath.c_str();
}

const char *AppOptions::getWorkspacePath() {
	return m_workspacePath.c_str();
}
const char *AppOptions::getShadowPath() {
	return m_shadowPath.c_str();
}
const char *AppOptions::getConfigPath() {
	return m_configPath.c_str();
}
void setConfigPath(const char *configPath);

} /* namespace simplearchive */
