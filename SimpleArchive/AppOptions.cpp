/*
 * AppOptions.cpp
 *
 *  Created on: Oct 27, 2014
 *      Author: wzw7yn
 */
#include "ConfigReader.h"
#include "AppOptions.h"
#include "AppConfig.h"
#include "SAUtils.h"
#include "stdio.h"
#include "argvparser.h"
#include "Environment.h"

using namespace CommandLineProcessing;
namespace simplearchive {

AppOptions *AppOptions::m_this = 0;
std::string AppOptions::m_name;
AppOptions::CommandMode AppOptions::m_commandMode = AppOptions::CM_Unknown;
std::string AppOptions::m_comment;
std::string AppOptions::m_imageAddress;

AppOptions::AppOptions() {
}

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
	printf("%s", homePath.c_str());
	int i = homePath.length();
	if (homePath.empty() == true || homePath.length() == 0) {
		homePath = SAUtils::GetEnvironment("ProgramData");
		//C:\ProgramData\IDK Software\ImageArchive1.0
		homePath += "/IDK Software/ImageArchive1.0";
		if (SAUtils::DirExists(homePath.c_str()) == false) {
			printf("SIA Unable to start?\nEnvironment SA_HOME not set and configuration not in default location.");
			m_error = true;
			return false;
		}
	}
	std::string configfile = homePath + "/conf/" + "config.conf";
	if (SAUtils::FileExists(configfile.c_str()) == false) {
		m_error = true;
		return false;
	}
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
	ArgvParser argvParser;
	

	//define error codes
	argvParser.addErrorCode(0, "Success");
	argvParser.addErrorCode(1, "Error");
	argvParser.setIntroductoryDescription("The Image archive provides an organised place to store images. This archive is"
		"designed to be simple in design and to use. It consists of archiving core that provides the basic archiving"
		" functions but in addition, takes input and provides output from optional external components to provide a"
		"tailored achieving solution and can be extended into a complete achieving system. ");
	argvParser.setHelpOption();
	//parser.defineOption("a", "Adds new images to the archive.", ArgvParser::OptionRequiresValue);
	//parser.defineOptionAlternative("a", "add");

	

	// Subcommands
	argvParser.defineOption("a", "add new images to the archive.", ArgvParser::NoOptionAttribute);
	argvParser.defineOptionAlternative("a", "add");

	argvParser.defineOption("o", "Checkout images from archive.", ArgvParser::NoOptionAttribute);
	argvParser.defineOptionAlternative("o", "checkout");

	argvParser.defineOption("i", "Checkin images to archive.", ArgvParser::NoOptionAttribute);
	argvParser.defineOptionAlternative("i", "checkin");

	argvParser.defineOption("u", "Un-checkout images to archive.", ArgvParser::NoOptionAttribute);
	argvParser.defineOptionAlternative("u", "uncheckout");

	argvParser.defineOption("x", "Export images from archive.", ArgvParser::NoOptionAttribute);
	argvParser.defineOptionAlternative("x", "export");

	argvParser.defineOption("about", "prints the version information", ArgvParser::NoOptionAttribute);

	argvParser.defineOption("v", "View commands", ArgvParser::NoOptionAttribute);
	argvParser.defineOptionAlternative("v", "view");

	argvParser.defineOption("validate", "Validate commands", ArgvParser::NoOptionAttribute);
	
	argvParser.defineOption("image-address", "image address", ArgvParser::NoOptionAttribute);

	argvParser.defineOption("m", "Mirror commands", ArgvParser::NoOptionAttribute);
	argvParser.defineOptionAlternative("m", "mirror");

	argvParser.defineOption("b", "Goes through the motions of running the subcommand but makes no\nactual changes ether disk or repository.", ArgvParser::NoOptionAttribute);
	argvParser.defineOptionAlternative("b", "backup");
	// Options
	argvParser.defineOption("n", "name of the view.", ArgvParser::OptionRequiresValue);
	argvParser.defineOptionAlternative("n", "name");

	argvParser.defineOption("q", "no output is sent to the terminal.", ArgvParser::NoOptionAttribute);
	argvParser.defineOptionAlternative("q", "quiet");

	argvParser.defineOption("s", "source of the images", ArgvParser::OptionRequiresValue);
	argvParser.defineOptionAlternative("s", "source-path");

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

	argvParser.defineOption("l", "Temporarily changes the logging level for the scope of this command session.", ArgvParser::OptionRequiresValue);
	argvParser.defineOptionAlternative("r", "logging-level");

	argvParser.defineOption("C", "Comment to be included in command", ArgvParser::OptionRequiresValue);
	argvParser.defineOptionAlternative("C", "comment");

	
	

	
	
	ArgvParser::ParserResults res = argvParser.parse(argc, argv);
	//testHelpOptionDetection();
	bool cmdFound = false;
	if (argvParser.foundOption("about") == true) {
		setCommandMode(AppOptions::CM_Version);
		cmdFound = true;
	}
	else if (argvParser.foundOption("add") == true) {
		
		if (argvParser.foundOption("source-path") == true) {			
			std::string opt = argvParser.optionValue("source-path");
			printf(opt.c_str()); printf("\n");
			config.setSourcePath(opt.c_str());			
		}
		if (argvParser.foundOption("archive-path") == true) {			
			std::string opt = argvParser.optionValue("archive-path");
			printf(opt.c_str()); printf("\n");
			config.setArchivePath(opt.c_str());
		}
		setCommandMode(AppOptions::CM_Import);

		Environment::setEnvironment();
		cmdFound = true;
	}
	else if (argvParser.foundOption("checkout") == true) {
		if (argvParser.foundOption("image-address") == true) {
			m_imageAddress = argvParser.optionValue("image-address");
			printf(m_imageAddress.c_str()); printf("\n");
		}
		if (argvParser.foundOption("comment") == true) {
			m_comment = argvParser.optionValue("comment");
			printf(m_comment.c_str()); printf("\n");
		}
		if (argvParser.foundOption("archive-path") == true) {
			std::string opt = argvParser.optionValue("archive-path");
			printf(opt.c_str()); printf("\n");
			config.setArchivePath(opt.c_str());
		}

		setCommandMode(AppOptions::CM_Checkout);
		cmdFound = true;
	}
	else if (argvParser.foundOption("checkin") == true) {
		
		if (argvParser.foundOption("image-address") == true) {
			m_imageAddress = argvParser.optionValue("image-address");
			printf(m_imageAddress.c_str()); printf("\n");
		}
		if (argvParser.foundOption("comment") == true) {
			m_comment = argvParser.optionValue("comment");
			printf(m_comment.c_str()); printf("\n");
		}
		if (argvParser.foundOption("archive-path") == true) {
			std::string opt = argvParser.optionValue("archive-path");
			printf(opt.c_str()); printf("\n");
			config.setArchivePath(opt.c_str());
		}


		setCommandMode(AppOptions::CM_Checkin);
		cmdFound = true;
	}
	else if (argvParser.foundOption("uncheckout") == true) {
		if (argvParser.foundOption("image-address") == true) {
			m_imageAddress = argvParser.optionValue("image-address");
			printf(m_imageAddress.c_str()); printf("\n");
		}
		if (argvParser.foundOption("comment") == true) {
			m_comment = argvParser.optionValue("comment");
			printf(m_comment.c_str()); printf("\n");
		}
		if (argvParser.foundOption("archive-path") == true) {
			std::string opt = argvParser.optionValue("archive-path");
			printf(opt.c_str()); printf("\n");
			config.setArchivePath(opt.c_str());
		}
		
		setCommandMode(AppOptions::CM_UnCheckout);
		cmdFound = true;
	}
	else if (argvParser.foundOption("export") == true) {
		setCommandMode(AppOptions::CM_Export);
		cmdFound = true;
	}
	else if (argvParser.foundOption("view") == true) {
		setCommandMode(AppOptions::CM_View);
		
		if (argvParser.foundOption("archive-path") == true) {

			std::string opt = argvParser.optionValue("archive-path");
			printf(opt.c_str()); printf("\n");
			config.setArchivePath(opt.c_str());
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

	else if (argvParser.foundOption("validate") == true) {
		setCommandMode(AppOptions::CM_Validate);
		cmdFound = true;
		if (argvParser.foundOption("archive-path") == true) {

			std::string opt = argvParser.optionValue("archive-path");
			printf(opt.c_str()); printf("\n");
			config.setArchivePath(opt.c_str());
		}
	}
	else if (argvParser.foundOption("mirror") == true) {
		setCommandMode(AppOptions::CM_Mirror);

		if (argvParser.foundOption("archive-path") == true) {

			std::string opt = argvParser.optionValue("archive-path");
			printf(opt.c_str()); printf("\n");
			config.setArchivePath(opt.c_str());
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
			config.setArchivePath(opt.c_str());
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
		printf("%s\n", argvParser.usageDescription().c_str());
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
