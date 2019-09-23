#include "SIAArcArgvParser.h"
#include "ConfigReader.h"
#include "SIAArcAppOptions.h"
#include "AppConfig.h"
#include "pathcontroller.h"
#include "SAUtils.h"
#include "stdio.h"
#include "argvparser.h"
#include "Environment.h"
#include "EnvFunc.h"

using namespace CommandLineProcessing;
namespace simplearchive {


bool SIAArcArgvParser::doInitalise(int argc, char **argv) {

	SIAArcAppOptions &appOptions = SIAArcAppOptions::get();
	/*
	m_error = false;
	m_verbose = false;
	m_quiet = false;
	m_logLevel = "INFO";
	m_dry_run = false;
	*/
	//AppConfig &config = AppConfig::get();
	SIAARCConfig config;
		


	//define error codes
	//define error codes
	addErrorCode(0, "Success");
	addErrorCode(1, "Warnings");
	addErrorCode(2, "Errors");
	addErrorCode(3, "Fatal");

	//setIntroductoryDescription("Image archive provides an organized place to store images. This archive is"
	//	" designed to be simple in design and to use. It consists of archiving core that provides the basic archiving"
	//	" functions but in addition, takes input and provides output from optional external components to provide a"
	//	" tailored achieving solution and can be extended into a complete achieving system. ");

	setHelpOption();




	// Subcommands
	defineOption("add", "add new images to the archive.", ArgvParser::MasterOption);
	defineOption("get", "add new images to the archive.", ArgvParser::MasterOption);
	defineOption("checkout", "Checkout images from archive.", ArgvParser::MasterOption);
	defineOption("checkin", "Checkin images to archive.", ArgvParser::MasterOption);
	defineOption("uncheckout", "Un-checkout images to archive.", ArgvParser::MasterOption);
	defineOption("export", "Export images from archive.", ArgvParser::MasterOption);
	defineOption("about", "prints the version information", ArgvParser::MasterOption);
	defineOption("status", "show check in/out status", ArgvParser::MasterOption);
	defineOption("view", "View commands", ArgvParser::MasterOption);
	defineOption("show", "Show details", ArgvParser::MasterOption);
	defineOption("prop", "Manage image properties", ArgvParser::MasterOption);
	defineOption("log", "Show history log", ArgvParser::MasterOption);
	defineOption("mode", "Sets the mode in which imgarchive will be operating", ArgvParser::MasterOption);
	/*
	defineOption("b", "Goes through the motions of running the subcommand but makes no\nactual changes ether disk or repository.", ArgvParser::NoOptionAttribute);
	defineOptionAlternative("b", "backup");
	*/
	// Options
	defineOption("i", "image address.", ArgvParser::OptionRequiresValue);
	defineOptionAlternative("i", "image-address");

	defineOption("n", "name of the view.", ArgvParser::OptionRequiresValue);
	defineOptionAlternative("n", "name");

	defineOption("q", "quiet output is sent to the terminal.", ArgvParser::NoOptionAttribute);
	defineOptionAlternative("q", "quiet");

	defineOption("s", "no output is sent to the terminal.", ArgvParser::NoOptionAttribute);
	defineOptionAlternative("s", "silent");

	defineOption("e", "Generate progress events.", ArgvParser::NoOptionAttribute);
	defineOptionAlternative("e", "events");

	defineOption("p", "source of the images", ArgvParser::OptionRequiresValue);
	defineOptionAlternative("p", "source-path");

	defineOption("L", "import from lightroom", ArgvParser::NoOptionAttribute);
	defineOptionAlternative("L", "lightroom");

	defineOption("S", "address scope", ArgvParser::OptionRequiresValue);
	defineOptionAlternative("S", "scope");

	defineOption("R", "Remode server mode", ArgvParser::NoOptionAttribute);
	defineOptionAlternative("R", "remote-server");

	defineOption("d", "destination of the images", ArgvParser::OptionRequiresValue);
	defineOptionAlternative("d", "dist-path");

	defineOption("M", "size of media", ArgvParser::OptionRequiresValue);
	defineOptionAlternative("S", "media-size");

	defineOption("m", "Goes through the motions of running the subcommand but makes no\nactual changes ether disk or repository.", ArgvParser::OptionRequiresValue);
	defineOptionAlternative("m", "media-path");

	defineOption("fd", "from date", ArgvParser::OptionRequiresValue);
	defineOptionAlternative("fd", "from-date");

	defineOption("f", "Specifies a file", ArgvParser::OptionRequiresValue);
	defineOptionAlternative("f", "file");

	defineOption("F", "no output is sent to the terminal.", ArgvParser::NoOptionAttribute);
	defineOptionAlternative("F", "force");

	defineOption("v", "specifies a version.", ArgvParser::OptionRequiresValue);
	defineOptionAlternative("v", "version");


	defineOption("td", "to date", ArgvParser::OptionRequiresValue);
	defineOptionAlternative("td", "to-date");

	defineOption("a", "location of the archive root folder.", ArgvParser::OptionRequiresValue);
	defineOptionAlternative("a", "archive-path");

	defineOption("r", "location of the archive root folder.", ArgvParser::NoOptionAttribute);
	defineOptionAlternative("r", "root");

	defineOption("ft", "text output format type. Can be \"Humam\", \"XML\" \"Json\" or \"cvs\" i.e format-type=XML.", ArgvParser::OptionRequiresValue);
	defineOptionAlternative("ft", "format-type");

	defineOption("l", "Temporarily changes the logging level for the scope of this command session.", ArgvParser::OptionRequiresValue);
	defineOptionAlternative("l", "logging-level");

	defineOption("c", "Comment to be included in command", ArgvParser::OptionRequiresValue);
	defineOptionAlternative("c", "comment");

	defineOption("li", "List items", ArgvParser::OptionRequiresValue);
	defineOptionAlternative("li", "list");

	defineOption("U", "Show setup", ArgvParser::NoOptionAttribute);
	defineOptionAlternative("U", "setup");

	defineOption("force-date", "Overrides all dates found associated with the images in the selection", ArgvParser::OptionRequiresValue);
	defineOption("default-date", "Uses this date if none found associated with an image", ArgvParser::OptionRequiresValue);

	//defineOption("checked-out", "Show checked out", ArgvParser::OptionRequiresValue); // =all =year{2015}
	//defineOption("unchecked-out", "Show changed images which are not checked out", ArgvParser::OptionRequiresValue);

	defineCommandOption("add", "comment");
	defineCommandOption("add", "logging-level");
	defineCommandOption("add", "archive-path");
	defineCommandOption("add", "source-path");
	defineCommandOption("add", "lightroom");

	defineCommandOption("get", "comment");
	defineCommandOption("get", "scope");
	defineCommandOption("get", "force");
	defineCommandOption("get", "version");

	defineCommandOption("checkin", "comment");
	defineCommandOption("checkin", "scope");
	defineCommandOption("checkin", "force");
	defineCommandOption("checkin", "file");

	defineCommandOption("checkout", "comment");
	defineCommandOption("checkout", "scope");
	defineCommandOption("checkout", "force");
	defineCommandOption("checkout", "file");

	defineCommandOption("uncheckout", "comment");
	defineCommandOption("uncheckout", "scope");
	defineCommandOption("uncheckout", "force");
	defineCommandOption("uncheckout", "file");

	defineCommandOption("export", "comment");
	defineCommandOption("export", "logging-level");
	defineCommandOption("export", "dist-path");

	defineCommandOption("status", "scope");
	
	defineCommandOption("show", "setup");

	defineCommandOption("log", "image-address");
	defineCommandOption("log", "format-type");
	defineCommandOption("mode", "remote-server");
	ArgvParser::ParserResults res = parse(argc, argv);

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
		errStr = parseErrorDescription(res);
		printf("%s", errStr.c_str());
		return false;
	case ArgvParser::GeneralHelpRequested:
		printf("%s", generalHelp(80).c_str());
		return false;
	case ArgvParser::TopicHelpRequested:
		printf("%s", topicUsageDescription(getCurrentCommandId(), 80).c_str());
		return false;
	default:
		return false;
	}

	//testHelpOptionDetection();
	bool cmdFound = false;

	if (command("about") == true) {
		appOptions.setCommandMode(SIAArcAppOptions::CommandMode::CM_Version);
		cmdFound = true;
	}

	else if (command("add") == true) {

		if (foundOption("source-path") == true) {
			std::string opt = optionValue("source-path");
			config.setSourcePath(opt.c_str());
		}

		if (foundOption("lightroom") == true) {
			std::string opt = optionValue("lightroom");
			config.setLightroom();
		}

		if (foundOption("archive-path") == true) {
			std::string opt = optionValue("archive-path");
			config.setWorkspacePath(opt.c_str());
		}
		if (foundOption("file") == true) {
			appOptions.m_filePath = optionValue("file");
			appOptions.m_usingFile = true;
		}
		if (foundOption("peek") == true) {
			appOptions.m_peekOnly = true;
		}
		if (foundOption("force-date") == true) {
			std::string opt = optionValue("force-date");
			if (opt.compare("FileDate") == 0) {
				appOptions.m_useFileDate = true;
			}

			if (opt.compare("TodaysDate") == 0) {
				appOptions.m_useDateToday = true;
			}
			else {
				appOptions.m_archiveDate = ExifDate(opt.c_str());
			}
			appOptions.m_forceDate = true;

		}
		if (foundOption("default-date") == true) {
			std::string opt = optionValue("default-date");
			config.setSourcePath(opt.c_str());
			if (opt.compare("FileDate") == 0) {
				appOptions.m_useFileDate = true;
			}

			if (opt.compare("TodaysDate") == 0) {
				appOptions.m_useDateToday = true;
			}
			else {
				appOptions.m_archiveDate = ExifDate(opt.c_str());
			}
			appOptions.m_useDate = true;
		}

		appOptions.setCommandMode(SIAArcAppOptions::CommandMode::CM_Import);

		Environment::setEnvironment();
		cmdFound = true;
	}
	else if (command("get") == true) {
		if (foundOption("scope") == true) {
			appOptions.m_imageAddress = optionValue("scope");
		}

		if (foundOption("comment") == true) {
			appOptions.m_comment = optionValue("comment");
		}
		if (foundOption("archive-path") == true) {
			std::string opt = optionValue("archive-path");
			config.setWorkspacePath(opt.c_str());
		}

		if (foundOption("force") == true) {
			appOptions.m_force = true;
		}

		if (foundOption("version") == true) {
			std::string opt = optionValue("version");
			appOptions.m_version = opt.c_str();
		}

		appOptions.setCommandMode(SIAArcAppOptions::CommandMode::CM_Get);
		cmdFound = true;
	}
	else if (command("checkout") == true) {
		if (foundOption("scope") == true) {
			appOptions.m_imageAddress = optionValue("scope");
		}

		if (foundOption("list") == true) {
			appOptions.m_list = true;
		}

		if (foundOption("file") == true) {
			appOptions.m_filePath = optionValue("file");
			appOptions.m_usingFile = true;
		}

		if (foundOption("comment") == true) {
			appOptions.m_comment = optionValue("comment");
		}
		if (foundOption("archive-path") == true) {
			std::string opt = optionValue("archive-path");
			config.setWorkspacePath(opt.c_str());
		}

		if (foundOption("force") == true) {
			appOptions.m_force = true;
		}

		appOptions.setCommandMode(SIAArcAppOptions::CommandMode::CM_Checkout);
		cmdFound = true;
	}
	else if (command("checkin") == true) {

		if (foundOption("scope") == true) {
			appOptions.m_imageAddress = optionValue("scope");
		}
		if (foundOption("comment") == true) {
			appOptions.m_comment = optionValue("comment");
		}
		if (foundOption("archive-path") == true) {
			std::string opt = optionValue("archive-path");
			config.setWorkspacePath(opt.c_str());
		}

		if (foundOption("force") == true) {
			appOptions.m_force = true;
		}

		const auto& args = ArgvParser::getDefaultArgumentsContainer();
		for (auto i = args.begin(); i != args.end(); i++) {
			appOptions.setDefaultArguments(*i);
		}

		appOptions.setCommandMode(SIAArcAppOptions::CommandMode::CM_Checkin);
		cmdFound = true;
	}
	else if (command("uncheckout") == true) {
		if (foundOption("image-address") == true) {
			appOptions.m_imageAddress = optionValue("image-address");
		}
		if (foundOption("comment") == true) {
			appOptions.m_comment = optionValue("comment");
		}
		if (foundOption("archive-path") == true) {
			std::string opt = optionValue("archive-path");
			config.setWorkspacePath(opt.c_str());
		}

		if (foundOption("force") == true) {
			appOptions.m_force = true;
		}


		appOptions.setCommandMode(SIAArcAppOptions::CommandMode::CM_UnCheckout);
		cmdFound = true;
	}
	else if (command("show") == true) {
		
		appOptions.setCommandMode(SIAArcAppOptions::CommandMode::CM_Show);
		cmdFound = true;
	}
	else if (command("prop") == true) {
		if (foundOption("list") == true) {
			appOptions.m_imageAddress = optionValue("list");
		}
		appOptions.setCommandMode(SIAArcAppOptions::CommandMode::CM_Prop);
		cmdFound = true;
	}
	else if (command("export") == true) {
		if (foundOption("dist-path") == true) {
			appOptions.m_distinationPath = optionValue("dist-path");
		}
		appOptions.setCommandMode(SIAArcAppOptions::CommandMode::CM_Export);
		cmdFound = true;
	}
	else if (command("view") == true) {
		appOptions.setCommandMode(SIAArcAppOptions::CommandMode::CM_View);

		if (foundOption("archive-path") == true) {

			std::string opt = optionValue("archive-path");
			config.setWorkspacePath(opt.c_str());
		}
		/*
		if (foundOption("dist-path") == true) {

		std::string opt = optionValue("dist-path");
		//printf(opt.c_str()); printf("\n");
		config.setBackupDestinationPath(opt.c_str());

		}
		if (foundOption("size") == true) {

		std::string opt = optionValue("size");
		//printf(opt.c_str()); printf("\n");
		config.setBackupMediaSize(opt.c_str());

		}
		if (foundOption("from-date") == true) {

		std::string opt = optionValue("from-date");
		//printf(opt.c_str()); printf("\n");
		config.setFromDate(opt.c_str());

		}
		*/
		if (foundOption("name") == true) {
			std::string opt = optionValue("name");
			appOptions.setName(opt.c_str());
		}

		cmdFound = true;
	}

	else if (command("log") == true) {

		if (foundOption("image-address") == true) {
			appOptions.m_imageAddress = optionValue("image-address");
		}
		
		if (foundOption("archive-path") == true) {
			std::string opt = optionValue("archive-path");
			config.setWorkspacePath(opt.c_str());
		}

		if (foundOption("format-type") == true) {
			std::string opt = optionValue("format-type");
			appOptions.m_formatType = LogDocument::parse(opt.c_str());
		}

		const auto& args = ArgvParser::getDefaultArgumentsContainer();
		for (auto i = args.begin(); i != args.end(); i++) {
			appOptions.setDefaultArguments(*i);
		}

		appOptions.setCommandMode(SIAArcAppOptions::CommandMode::CM_Log);
		cmdFound = true;
	}
	else if (command("status") == true) {

		if (foundOption("scope") == true) {
			appOptions.m_imageAddress = optionValue("scope");
		}
		if (foundOption("checked-out") == true) {
			appOptions.m_showCommandOption = SIAArcAppOptions::ShowCommandOption::SC_ShowUncheckedOutChanges;
			appOptions.m_imageAddress = optionValue("checked-out");
		}
		if (foundOption("unchecked-out") == true) {
			appOptions.m_showCommandOption = SIAArcAppOptions::ShowCommandOption::SC_ShowUncheckedOutChanges;
			appOptions.m_imageAddress = optionValue("checked-out");
		}
		appOptions.setCommandMode(SIAArcAppOptions::CommandMode::CM_Status);
		cmdFound = true;
	}
	else if (command("version") == true) {
		appOptions.setCommandMode(SIAArcAppOptions::CommandMode::CM_Version);
		cmdFound = true;
	}
	else if (command("mode") == true) {
		if (foundOption("remote-server") == true) {
			config.setServerModeON();
		}
		appOptions.setCommandMode(SIAArcAppOptions::CommandMode::CM_Mode);
		cmdFound = true;
	}
	else {
		appOptions.setCommandMode(SIAArcAppOptions::CommandMode::CM_Unknown);
		cmdFound = true;
	}
	// global options:

	if (foundOption("events") == true) {
		appOptions.m_eventsOn = true;
	}

	if (foundOption("quiet") == true) {
		config.setQuiet(true);
	}

	if (foundOption("silent") == true) {
		config.setSilent(true);
	}

	if (foundOption("logging-level") == true) {

		std::string opt = optionValue("logging-level");
		config.setLogLevel(opt.c_str());
	}
	if (foundOption("console-level") == true) {

		std::string opt = optionValue("console-level");
		printf(opt.c_str()); printf("\n");
		config.setConsoleLevel(opt.c_str());
	}
	if (foundOption("dry-run") == true) {
		std::string opt = optionValue("dry-run");
	}

	if (foundOption("media-path") == true) {
		std::string opt = optionValue("media-path");
	}

	if (foundOption("media-size") == true) {
		std::string opt = optionValue("media-size");
	}

	if (res != ArgvParser::NoParserError) {
		printf("%s\n", parseErrorDescription(res).c_str());
		printf("%s\n", usageDescription().c_str());
	}
	if (cmdFound == false) {
		printf("Main command required?\n\n");
		printf("%s\n", usageDescription().c_str());
		return false;
	}
	return true;
}

std::string SIAArcArgvParser::usageDescriptionHeader(unsigned int _width) const
{
	std::string usage;
		
	usage += "usage: siaarc subcommand [options] [args]\n\n";
	usage += "Image archive command line client, version 1.0.0.1\n";
	usage += "Type 'sia help <subcommand>' for help on a specific subcommand.\n\n";

	std::string tmp = "siaarc is the primary command-line interface to Simple Image Archive. This interface is used to manage the control of images going in and out of the archive software. ";
	tmp += "It has a rich set of subcommands that \"add/import\" images to the archive and \"export\" images out of the archive, In addition manages the controlled modification of images";
	tmp += " using the \"check-in/check-out\" command set";
	usage += '\n';
	usage += formatString(tmp, _width);
	usage += '\n';
	usage += '\n';
	usage += "Note:\n";
	usage += formatString("The administration of the archive is carried out by the siaadmin command-line interface.", _width) + "\n";

	return usage;
}

} /* name*/