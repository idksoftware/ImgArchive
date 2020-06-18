#include <sstream>
#include <iostream>
//#include <filesystem>
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
#include "IAParseOptions.h"

// beyond compare
using namespace CommandLineProcessing;
namespace simplearchive {


bool SIAArcArgvParser::doInitalise(int argc, char **argv) {

	SIAArcAppOptions &appOptions = SIAArcAppOptions::get();
	
	SIAARCConfig config;
	
	addErrorCode(0, "Success");
	addErrorCode(1, "Warnings");
	addErrorCode(2, "Errors");
	addErrorCode(3, "Fatal");

	setIntroductoryDescription("iaarc - Image archive client provides the means to operate ImgArchive from the command line");

	setHelpOption();

	// Subcommands
	defineOption("import", "import new images to the archive.", ArgvParser::MasterOption);
	defineCommandSyntax("import", "iaarc import [--source-path=<path>]\n\t"
		"[--comment=<comment text>]\n\t[--lightroom=<On|Off>]");
	
	defineOption("get", "get images from the archive.", ArgvParser::MasterOption);
	defineCommandSyntax("get", "iaarc get [--target-path=<path>]\n\t"
		"[--comment=<comment text>]\n\t[--scope=<scope-address]\n\t[--force=<yes|No>]\n\t[--version=<vesion-num>");

	defineOption("checkout", "Checkout images from archive.", ArgvParser::MasterOption);
	defineCommandSyntax("checkout", "iaarc checkout [--target-path=<path>]\n\t"
		"[--comment=<comment text>]\n\t[--scope=<scope-address]\n\t[--force=<yes|No>]\n\t[--version=<vesion-num>");

	defineOption("checkin", "Checkin images in to archive.", ArgvParser::MasterOption);
	defineCommandSyntax("checkin", "iaarc checkin [--target-path=<path>]\n\t[--logging-level=<level>]"
		"[--comment=<comment text>]\n\t[--scope=<scope-address]\n\t[--force=<yes|No>]\n\t[--version=<vesion-num>");

	defineOption("uncheckout", "Un-checkout images in to archive.", ArgvParser::MasterOption);
	defineCommandSyntax("uncheckout", "iaarc uncheckout [--target-path=<path>]\n\t[--logging-level=<level>]"
		"[--comment=<comment text>]\n\t[--scope=<scope-address]\n\t[--force=<yes|No>]\n\t[--version=<vesion-num>");

	defineOption("export", "Export images from archive.", ArgvParser::MasterOption);
	defineCommandSyntax("uncheckout", "iaarc uncheckout [--target-path=<path>]\n\t[--logging-level=<level>]"
		"[--comment=<comment text>]\n\t[--scope=<scope-address]\n\t[--force=<yes|No>]\n\t[--version=<vesion-num>");

	defineOption("workspace", "Manages the workspace.", ArgvParser::MasterOption);
	defineCommandSyntax("workspace", "iaarc workspace [--sync]\n\t[--logging-level=<level>]"
		"[--comment=<comment text>]\n\t[--scope=<scope-address]\n\t[--force=<yes|No>]");

	defineOption("prop", "Manage image properties", ArgvParser::MasterOption);
	defineCommandSyntax("prop", "iaarc prop [--s]\n\t[--logging-level=<level>]"
		"[--comment=<comment text>]\n\t[--scope=<scope-address]\n\t[--force=<yes|No>]");

	defineOption("template", "Manage metadata template", ArgvParser::MasterOption);
	defineCommandSyntax("template", "iaarc template [--current=<yes|no>]\n\t[--logging-level=<level>]"
		"[--current=<yes|no>]\n\t[--comment=<comment text>]\n\t[--scope=<scope-address]\n\t[--force=<yes|No>]");

	defineOption("status", "show check in/out status", ArgvParser::MasterOption);
	defineOption("view", "View commands", ArgvParser::MasterOption);

	defineOption("show", "Show details", ArgvParser::MasterOption);
	defineCommandSyntax("show", "iaarc show [--history=<image-address>]\n\t");

	defineOption("log", "Show logs", ArgvParser::MasterOption);
	defineCommandSyntax("log", "iaarc log [--image=<image-address]");

	//defineOption("mode", "Sets the mode in which imgarchive will be operating", ArgvParser::MasterOption);

	defineOption("about", "prints this version information", ArgvParser::MasterOption);
	defineCommandSyntax("about", "about [--out]\n\t[--file]\n");

	/*
	defineOption("b", "Goes through the motions of running the subcommand but makes no\nactual changes ether disk or repository.", ArgvParser::NoOptionAttribute);
	defineOptionAlternative("b", "backup");
	*/
	// Options

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

	defineOption("A", "address scope", ArgvParser::OptionRequiresValue);
	defineOptionAlternative("A", "scope");

	defineOption("R", "Remode server mode", ArgvParser::NoOptionAttribute);
	defineOptionAlternative("R", "remote-server");

	defineOption("d", "destination of the images", ArgvParser::OptionRequiresValue);
	defineOptionAlternative("d", "dist-path");

	defineOption("a", "archive", ArgvParser::NoOptionAttribute);
	defineOptionAlternative("a", "archive");

	defineOption("S", "sync workspace with archive", ArgvParser::NoOptionAttribute);
	defineOptionAlternative("S", "sync");
	

	defineOption("image", "Specifies a image address in the form \"<date>/<image name>", ArgvParser::OptionRequiresValue);
	//defineOptionAlternative("i", "image");

	defineOption("F", "no output is sent to the terminal.", ArgvParser::NoOptionAttribute);
	defineOptionAlternative("F", "force");

	defineOption("C", "current template (master or derivative)", ArgvParser::OptionRequiresValue);
	defineOptionAlternative("C", "current");



	defineOption("v", "specifies a version.", ArgvParser::OptionRequiresValue);
	defineOptionAlternative("v", "version");


	defineOption("td", "to date", ArgvParser::OptionRequiresValue);
	defineOptionAlternative("td", "to-date");

	defineOption("fd", "From date", ArgvParser::OptionRequiresValue);
	defineOptionAlternative("fd", "from-date");

	

	defineOption("r", "location of the archive root folder.", ArgvParser::NoOptionAttribute);
	defineOptionAlternative("r", "root");

	defineOption("format-type", "text output format type. Can be \"Humam\", \"XML\" \"Json\" or \"cvs\" i.e format-type=XML.", ArgvParser::OptionRequiresValue);
	//defineOptionAlternative("ft", "format-type");
	defineCommandSyntax("format-type", "format-type=[Human] | [xml] | [json] | [html] | [csv]\n");

	defineOption("l", "Temporarily changes the logging level for the scope of this command session.", ArgvParser::OptionRequiresValue);
	defineOptionAlternative("l", "logging-level");

	defineOption("c", "Comment to be included in command", ArgvParser::OptionRequiresValue);
	defineOptionAlternative("c", "comment");

	defineOption("li", "List items", ArgvParser::OptionRequiresValue);
	defineOptionAlternative("li", "list");

	defineOption("U", "Show settup", ArgvParser::NoOptionAttribute);
	defineOptionAlternative("U", "settup");

	defineOption("out", "Output type: text, xml, json or html.", ArgvParser::OptionRequiresValue);
	defineCommandSyntax("out", "out=[plain] | [xml] | [json] | [html]\n");
	//defineOptionAlternative("u", "users");

	defineOption("file", "output file name.", ArgvParser::OptionRequiresValue);
	defineCommandSyntax("file", "file=<filename>\n");

	defineOption("force-date", "Overrides all dates found associated with the images in the selection", ArgvParser::OptionRequiresValue);
	defineOption("default-date", "Uses this date if none found associated with an image", ArgvParser::OptionRequiresValue);

	defineOption("checked-out", "Show checked out images", ArgvParser::OptionRequiresValue); // =all =year{2015}
	defineOption("unchecked-out", "Show changed images which are not checked out", ArgvParser::OptionRequiresValue);

	defineCommandOption("import", "comment");
	
	defineCommandOption("import", "source-path");
	defineCommandOption("import", "lightroom");

	defineCommandOption("get", "comment");
	defineCommandOption("get", "scope");
	defineCommandOption("get", "force");
	defineCommandOption("get", "version");

	defineCommandOption("checkin", "comment");
	defineCommandOption("checkin", "scope");
	defineCommandOption("checkin", "force");
	defineCommandOption("checkin", "image");

	defineCommandOption("checkout", "comment");
	defineCommandOption("checkout", "scope");
	defineCommandOption("checkout", "force");
	defineCommandOption("checkout", "image");

	defineCommandOption("uncheckout", "comment");
	defineCommandOption("uncheckout", "scope");
	defineCommandOption("uncheckout", "force");
	defineCommandOption("uncheckout", "image");

	defineCommandOption("export", "comment");
	defineCommandOption("export", "logging-level");
	defineCommandOption("export", "dist-path");

	defineCommandOption("status", "scope");
	defineCommandOption("status", "checked-out");
	defineCommandOption("status", "unchecked-out");
	
	defineCommandOption("status", "unchecked-out");

	defineCommandOption("show", "settup");

	defineCommandOption("template", "current");
	defineCommandOption("template", "archive");

	defineCommandOption("log", "image");
	defineCommandOption("log", "format-type");
	defineCommandOption("log", "file");
	defineCommandOption("log", "from-date");
	defineCommandOption("log", "to-date");
//	defineCommandOption("mode", "remote-server");

	defineCommandOption("workspace", "sync");


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
		printf("%s", topicUsageDescription(getCurrentCommandId(), 80).c_str());
		return false;
	case ArgvParser::GeneralHelpRequested:
		std::cout << generalHelp(80); // this is printf("%s", usageDescription(80).c_str()); in iaadmin
		return false;
	case ArgvParser::TopicHelpRequested:
		std::cout << topicUsageDescription(getCurrentCommandId(), 80);
		return false;
	case ArgvParser::ParserCommandNotFound:
		printf("Invalid command: %s\n\n", getCurrentCommand().c_str());
		printf("%s", usageDescription(80).c_str());
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

	else if (command("import") == true) {

		bool isSourePathSet = false;
		// Source of images
		if (foundOption("source-path") == true) {
			std::string opt = optionValue("source-path");
			config.setSourcePath(opt.c_str());
			isSourePathSet = true;
		}
		if (isSourePathSet == false) {
			config.setSourcePath(SAUtils::getCurrentDirectory().c_str());
		}
		if (foundOption("lightroom") == true) {
			std::string opt = optionValue("lightroom");
			config.setLightroom();
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

		bool gotImageAddress = false;
		if (foundOption("image") == true) {
			appOptions.m_imageAddress = optionValue("image");
			gotImageAddress = true;
		}

		if (foundOption("comment") == true) {
			appOptions.m_comment = optionValue("comment");
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
	
	else if (command("template") == true) {
		IAParseOptions iaParseOptions;
		bool res = false;
		std::string opt;
		appOptions.m_current = true;
		appOptions.m_master = true;
		if (foundOption("current") == true) {
			std::string opt = optionValue("current");
			res = iaParseOptions.parseCurrentOptions(opt.c_str());
			if (!res) {
				printf("Invalid argument for \"current\" \"%s\"\n\n", opt.c_str());
				printf("%s", topicUsageDescription(getCurrentCommandId(), 80).c_str());
				return false;
			}
			appOptions.m_current = iaParseOptions.isCurrent();
		}
		else if (foundOption("archive") == true) {
			std::string opt = optionValue("archive");
			res = iaParseOptions.parseArchiveOptions(opt.c_str());
			if (!res) {
				printf("Invalid argument for \"archive\" \"%s\"\n\n", opt.c_str());
				printf("%s", topicUsageDescription(getCurrentCommandId(), 80).c_str());
				return false;
			}
			appOptions.m_master = iaParseOptions.isMaster();
			
		}
		
		appOptions.setCommandMode(SIAArcAppOptions::CommandMode::CM_Template);
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

		bool gotImageAddress = false;
		if (foundOption("image") == true) {
			appOptions.m_imageAddress = optionValue("image");
			gotImageAddress = true;
		}
		
		if (foundOption("file") == true) {
			std::string opt = optionValue("file");
			appOptions.m_filePath = opt;
		}

		if (foundOption("format-type") == true) {
			std::string opt = optionValue("format-type");
			appOptions.m_formatType = LogDocument::parse(opt.c_str());
		}

		if (gotImageAddress == false) {
			// must be system history
		}

		appOptions.setCommandMode(SIAArcAppOptions::CommandMode::CM_Log);
		cmdFound = true;
	}
	else if (command("workspace") == true) {

		if (foundOption("sync") == true) {
			appOptions.m_sync = true;
		}
		appOptions.setCommandMode(SIAArcAppOptions::CommandMode::CM_Workspace);
		cmdFound = true;
	}
	else if (command("status") == true) {

	if (foundOption("scope") == true) {
		appOptions.m_imageAddress = optionValue("scope");
	}
	if (foundOption("checked-out") == true) {
		appOptions.m_showCommandOption = SIAArcAppOptions::ShowCommandOption::SC_ShowUncheckedOutChanges;
	}
	if (foundOption("unchecked-out") == true) {
		appOptions.m_showCommandOption = SIAArcAppOptions::ShowCommandOption::SC_ShowUncheckedOutChanges;
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
		printf("%s\n",opt.c_str());
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
	/*
	usage += "usage: imgarc subcommand [options] [args]\n\n";
	usage += "ImgArchive command line client, version 1.0.0.1\n";
	usage += "Type 'iaarc help <subcommand>' for help on a specific subcommand.\n\n";
	*/
	// setIntroductoryDescription("iaarc - Image archive client provides the means to operate ImgArchive from the command line");
	std::string tmp = "iaarc - Image archive client provides the means to operate ImgArchive from the command line";
	usage += '\n';
	usage += formatString(tmp, _width);
	usage += '\n';
	usage += '\n';
	usage += "Note:\n";
	usage += formatString("The administration of the archive is carried out by the \"iaadmin\" command-line interface.", _width) + "\n";

	return usage;
}

std::string SIAArcArgvParser::commandUsage(unsigned int width) const
{
	std::string usage; // the usage description text
	usage = formatString("usage: iaarc[--version][--help] <command>[<args>]\n", width);
	usage += '\n';

	return usage;
}



std::string SIAArcArgvParser::generalHelp(unsigned int _width) const
{
	std::string usage; // the usage description text
	usage = commandUsage(_width);
	/*
	if (intro_description.length())
		usage += formatString(intro_description, _width) + "\n";

	if (max_key <= 1) {// if we have some options

		usage += formatString("No options available\n", _width) + "\n\n";
		return(usage);
	}

	*/
	usage += '\n';
	/*
	usage += "usage: sia subcommand [options] [args]\n\n";
	usage += "Image archive command line client, version 1.0.0.1\n";
	usage += "Type 'sia help <subcommand>' for help on a specific subcommand.\n\n";
	*/
	std::string tmp = "The command imgarc is the primary command-line interface to ImgArchive. This interface is used to manage the control of images going in and out of the archive software. ";
	tmp += "It has a rich set of subcommands that \"add/import\" images to the archive and \"export\" images out of the archive, In addition manages the controlled modification of images";
	tmp += " using the \"check-in/check-out\" command set";
	usage += '\n';
	usage += formatString(tmp, _width);
	usage += '\n';

	usage += "Note:\n";
	usage += formatString("The administration of the archive is carried out by the imgadmin command-line interface.", _width) + "\n";

	usage += formatString(command_header, _width) + "\n";
	usage += '\n';
	usage += AVAILABLE_COMMANDS;
	usage += "-\n";
	usage += "\n";
	for (auto it = option2attribute.begin(); it != option2attribute.end(); ++it)
	{
		std::string _os; // temp string for the option
		if (option2attribute.find(it->first)->second != MasterOption) {
			continue;
		}
		std::string _longOpt;
		std::string _shortOpt;
		std::list<std::string> alternatives = getAllOptionAlternatives(it->first);
		for (auto alt = alternatives.begin();
			alt != alternatives.end();
			++alt)
		{
			if (option2attribute.find(it->first)->second == MasterOption) {
				int option = option2attribute.find(it->first)->second;
				_os.clear();
				if (alt->length() > 1) {
					_longOpt += *alt;
				}
				else {
					_shortOpt += *alt;
				}


			}
		}

		if (!_longOpt.empty()) {
			_os += ' ';
			_os += _longOpt;
		}
		if (!_shortOpt.empty()) {
			_os += " (";
			_os += _shortOpt;
			_os += ')';
		}
		//_os += " : ";
		usage += formatLine(_os, _width, 0, 20);
		_os.clear();
		_longOpt.clear();
		_shortOpt.clear();
		if (option2descr.find(it->first) != option2descr.end())
			usage += formatString(option2descr.find(it->first)->second, _width, 4) + "\n";
		else
			usage += formatString("(no description)", _width, 4) + "\n";

	}
	usage += "\n";
	/*
	//printf("%s\n", usage.c_str());
	// loop over all option attribute entries (which equals looping over all
	// different options (not option names)
	for (Key2AttributeMap::const_iterator it = option2attribute.begin();
	it != option2attribute.end();
	++it)
	{
	string os; // temp string for the option

	// get the list of alternative names for this option
	list<string> alternatives = getAllOptionAlternatives(it->first);

	unsigned int count = 0;
	for( list<string>::const_iterator alt = alternatives.begin();
	alt != alternatives.end();
	++alt )
	{
	++count;
	if (option2attribute.find(it->first)->second == MasterOption) {
	continue;
	}
	// additional '-' for long options
	if (alt->length() > 1)
	os += "-";

	os += "-" + *alt;

	// note if the option requires a value
	if (option2attribute.find(it->first)->second & OptionRequiresValue)
	os += " <value>";

	// alternatives to come?
	if (count < alternatives.size())
	os += ", "; // add separator
	}

	// note if the option is required
	if (option2attribute.find(it->first)->second & OptionRequired)
	os += " [required]";

	usage += formatString(os, _width) + "\n";

	if (option2descr.find(it->first) != option2descr.end())
	usage += formatString(option2descr.find(it->first)->second, _width, 4);
	else
	usage += formatString("(no description)", _width, 4);

	// finally a little gap
	usage += "\n\n";
	}
	*/
	if (!errorcode2descr.size()) // if have no errorcodes
		return(usage);

	usage += formatString("\n\nReturn codes:\n", _width) + "\n";

	//   map<int, string>::const_iterator eit;
	for (auto alt = errorcode2descr.begin();
		alt != errorcode2descr.end();
		++alt)
	{
		std::ostringstream codeStr;
		codeStr << alt->first;
		std::string label = formatString(codeStr.str(), _width, 4);
		std::string descr = formatString(alt->second, _width, 10);
		usage += label + descr.substr(label.length()) + "\n";
	}
	usage += '\n';
	usage += "Image Archive is a tool for image archiving and version control system.\n";
	usage += "For additional information, see \"http:/www.idk-software.com/\"";
	usage += '\n';
	return(usage);
}





} /* name*/
