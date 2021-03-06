
#include <sstream>
#include "CatalogueArgvParser.h"
#include "ConfigReader.h"
#include "CatalogueAppOptions.h"
#include "AppConfig.h"
#include "SAUtils.h"
#include "stdio.h"
#include "SIAArgvParser.h"
#include "Environment.h"



using namespace CommandLineProcessing;
namespace simplearchive {

	bool CatalogueArgvParser::doInitalise(int argc, char **argv) {

		AppOptions &appOptions = AppOptions::get();


		//define error codes
		addErrorCode(0, "Success");
		addErrorCode(1, "Error");
		setIntroductoryDescription("The Image archive provides an organised place to store images. This archive is"
			"designed to be simple in design and to use. It consists of archiving core that provides the basic archiving"
			" functions but in addition, takes input and provides output from optional external components to provide a"
			"tailored achieving solution and can be extended into a complete achieving system. ");
		setHelpOption();

		setHeader("usage: siaadmin subcommand [options] [args]\n\n"
			"Image archive command line administrator, version 1.0.0.1\n"
			"Type 'siaadmin help <subcommand>' for help on a specific subcommand.\n\n"
			"siaadmin is the primary command-line interface to administer Image Archive (imga)."
			"\n");


		// Subcommands


		defineOption("init", "Create Archive enviroment", ArgvParser::MasterOption);
		defineOption("show", "Show settings", ArgvParser::MasterOption);
		defineOption("version", "prints the version information", ArgvParser::MasterOption);
		defineOption("validate", "Validate commands", ArgvParser::MasterOption);
		defineOption("mirror", "Mirror commands", ArgvParser::MasterOption);
		defineOption("test", "test commands", ArgvParser::MasterOption);
		/*
		defineOption("image-address", "image address", ArgvParser::NoOptionAttribute);

		defineOption("m", "Mirror commands", ArgvParser::NoOptionAttribute);
		defineOptionAlternative("m", "mirror");

		defineOption("b", "Goes through the motions of running the subcommand but makes no\nactual changes ether disk or repository.", ArgvParser::NoOptionAttribute);
		defineOptionAlternative("b", "backup");
		*/
		// Options
		defineOption("name", "name of the item.", ArgvParser::OptionRequiresValue);
		//defineOptionAlternative("n", "name");

		defineOption("repair", "Validate and repair.", ArgvParser::NoOptionAttribute);
		//defineOptionAlternative("r", "repair");

		defineOption("checkedOut", "Show images checkedOut.", ArgvParser::NoOptionAttribute);
		defineOptionAlternative("o", "checkedOut");

		defineOption("scope", "Scope validate. i.e. Workspace/Master or both", ArgvParser::OptionRequiresValue);
		//defineOptionAlternative("s", "scope");

		defineOption("users", "Make archive available to you only or everyone with a logon to this computer (Myself/All).", ArgvParser::OptionRequiresValue);
		//defineOptionAlternative("u", "users");


		defineOption("dist-path", "destination of the images", ArgvParser::OptionRequiresValue);
		//defineOptionAlternative("d", "dist-path");

		defineOption("media-size", "size of media", ArgvParser::OptionRequiresValue);
		//defineOptionAlternative("S", "media-size");

		defineOption("media-path", "Goes through the motions of running the subcommand but makes no\nactual changes ether disk or repository.", ArgvParser::OptionRequiresValue);

		defineOption("from-date", "from date", ArgvParser::OptionRequiresValue);
		//defineOptionAlternative("F", "from-date");

		defineOption("to-date", "to date", ArgvParser::OptionRequiresValue);
		//defineOptionAlternative("T", "to-date");

		defineOption("archive-path", "location of the archive root folder.", ArgvParser::OptionRequiresValue);
		//defineOptionAlternative("r", "archive-path");

		defineOption("workspace", "location of the workspace folder.", ArgvParser::OptionRequiresValue);
		//defineOptionAlternative("w", "workspace");

		defineOption("repository-path", "location of the master repository folder.", ArgvParser::OptionRequiresValue);

		defineOption("master-path", "location of the master repository folder.", ArgvParser::OptionRequiresValue);
		//defineOptionAlternative("w", "workspace");

		defineOption("derivative-path", "location of the derivative repository folder.", ArgvParser::OptionRequiresValue);
		//defineOptionAlternative("w", "workspace");
		defineOption("picture-path", "location of the picture folder.", ArgvParser::OptionRequiresValue);

		defineOption("logging-level", "Temporarily changes the logging level for the scope of this command session.", ArgvParser::OptionRequiresValue);
		//defineOptionAlternative("r", "logging-level");

		defineOption("comment", "Comment to be included in command", ArgvParser::OptionRequiresValue);
		//defineOptionAlternative("C", "comment");

		defineOption("workspace-path", "Location of the archive Workspace", ArgvParser::OptionRequiresValue);
		//defineOptionAlternative("w", "workspace-path");
		defineOption("settup", "Show settup", ArgvParser::NoOptionAttribute);
		
		/*
		defineCommandOption("init", "archive-path");
		defineCommandOption("init", "workspace-path");
		*/
		defineCommandOption("show", "settup");
		defineCommandOption("show", "checkedOut");

		defineCommandOption("validate", "scope");
		defineCommandOption("validate", "repair");
		defineCommandOption("validate", "archive-path");
		

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
			printf("%s", usageDescription(80).c_str());
			return false;
		case ArgvParser::TopicHelpRequested:
			printf("%s", topicUsageDescription(getCurrentCommandId(), 80).c_str());
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
		AdminConfig config;

		if (command("init") == true) {
			// This command will initalise the configuration.
			// so the the configuration need not to be initalised.
			appOptions.setCommandMode(AppOptions::CommandMode::CM_InitArchive);

			appOptions.m_configured = false;
			std::string opt;
			if (foundOption("users") == true) {
				std::string users = optionValue("users");
				if (users.compare("Myself") == 0) {
					appOptions.setAllUsers(false);
				}
				else {
					appOptions.setAllUsers(true);
				}
			}
			if (foundOption("archive-path") == true) {
				opt = optionValue("archive-path");
				appOptions.setHomePath(opt.c_str());
			}

			if (foundOption("workspace-path") == true) {
				opt = optionValue("workspace-path");
				appOptions.setWorkspacePath(opt.c_str());
			}
			
			if (foundOption("repository-path") == true) {
				opt = optionValue("derivative-path");
				appOptions.setRepositoryPath(opt.c_str());
			}

			if (foundOption("master-path") == true) {
				opt = optionValue("master-path");
				appOptions.setMasterPath(opt.c_str());
			}
			
			if (foundOption("derivative-path") == true) {
				opt = optionValue("derivative-path");
				appOptions.setDerivativePath(opt.c_str());
			}

			if (foundOption("picture-path") == true) {
				opt = optionValue("picture-path");
				appOptions.setCataloguePath(opt.c_str());
			}

			cmdFound = true;
		}
		else {
			if (appOptions.initaliseConfig() == false) {
				return false;
			}

			if (command("version") == true) {
				appOptions.setCommandMode(AppOptions::CommandMode::CM_Version);
				cmdFound = true;
			}
			else if (command("show") == true) {
				appOptions.setCommandMode(AppOptions::CommandMode::CM_Show);
				if (foundOption("settup") == true) {
					
					appOptions.setName("settup");
				}
				if (foundOption("checkedOut") == true) {

					appOptions.m_showOperation = AppOptions::ShowOperation::CheckedOut;
				}
				cmdFound = true;
			}
			else if (command("validate") == true) {
				appOptions.setCommandMode(AppOptions::CommandMode::CM_Validate);
				cmdFound = true;
				if (foundOption("archive-path") == true) {

					std::string opt = optionValue("archive-path");
//					printf(opt.c_str()); printf("\n");
					config.setWorkspacePath(opt.c_str());
				}
				if (foundOption("scope") == true) {

					std::string opt = optionValue("scope");
//					printf(opt.c_str()); printf("\n");
					if (opt.compare("workspace") == 0) {
						appOptions.m_verifyOperation = AppOptions::VerifyOperation::Workspace;
					}
					else if (opt.compare("master") == 0) {
						appOptions.m_verifyOperation = AppOptions::VerifyOperation::Master;
					}
					else if (opt.compare("both") == 0) {
						appOptions.m_verifyOperation = AppOptions::VerifyOperation::Both;
					}
					else {
						printf("Invalid argument for sub-command: %s \"%s\"\n\n", getCurrentCommand().c_str(), opt.c_str());
						printf("%s", usageDescription(80).c_str());
						return false;
					}
				}
				if (foundOption("repair") == true) {
					appOptions.m_repair = true;
				}
			}
			else if (command("mirror") == true) {
				appOptions.setCommandMode(AppOptions::CommandMode::CM_Mirror);

				if (foundOption("archive-path") == true) {

					std::string opt = optionValue("archive-path");
//					printf(opt.c_str()); printf("\n");
					config.setWorkspacePath(opt.c_str());
				}
				/*
				if (foundOption("dist-path") == true) {

				std::string opt = optionValue("dist-path");
				printf(opt.c_str()); printf("\n");
				config.setBackupDestinationPath(opt.c_str());

				}
				if (foundOption("size") == true) {

				std::string opt = optionValue("size");
				printf(opt.c_str()); printf("\n");
				config.setBackupMediaSize(opt.c_str());

				}
				if (foundOption("from-date") == true) {

				std::string opt = optionValue("from-date");
				printf(opt.c_str()); printf("\n");
				config.setFromDate(opt.c_str());

				}
				*/
				if (foundOption("name") == true) {
					std::string opt = optionValue("name");
//					printf(opt.c_str()); printf("\n");
					appOptions.setName(opt.c_str());
				}

				cmdFound = true;
			} else if (command("test") == true) {
				appOptions.setCommandMode(AppOptions::CommandMode::CM_Test);
				cmdFound = true;
			}
			else if (foundOption("backup") == true) {
				appOptions.setCommandMode(AppOptions::CommandMode::CM_Archive);

				if (foundOption("archive-path") == true) {

					std::string opt = optionValue("archive-path");
//					printf(opt.c_str()); printf("\n");
					config.setWorkspacePath(opt.c_str());
				}

				if (foundOption("dist-path") == true) {

					std::string opt = optionValue("dist-path");
//					printf(opt.c_str()); printf("\n");
					config.setBackupDestinationPath(opt.c_str());

				}
				if (foundOption("size") == true) {

					std::string opt = optionValue("size");
//					printf(opt.c_str()); printf("\n");
					config.setBackupMediaSize(opt.c_str());

				}
				if (foundOption("from-date") == true) {

					std::string opt = optionValue("from-date");
//					printf(opt.c_str()); printf("\n");
					config.setFromDate(opt.c_str());

				}
				if (foundOption("to-date") == true) {

					std::string opt = optionValue("to-date");
//					printf(opt.c_str()); printf("\n");
					config.setToDate(opt.c_str());

				}
				cmdFound = true;
			}
			else if (foundOption("version") == true) {
				appOptions.setCommandMode(AppOptions::CommandMode::CM_Version);
				cmdFound = true;
			}
			else {
				appOptions.setCommandMode(AppOptions::CommandMode::CM_Unknown);
				cmdFound = true;
			}
		}
		
		if (foundOption("logging-level") == true) {

			std::string opt = optionValue("logging-level");
//			printf(opt.c_str()); printf("\n");
			config.setLogLevel(opt.c_str());
		}
		if (foundOption("console-level") == true) {

			std::string opt = optionValue("console-level");
//			printf(opt.c_str()); printf("\n");
			config.setConsoleLevel(opt.c_str());
		}
		if (foundOption("dry-run") == true) {

			std::string opt = optionValue("dry-run");
//			printf(opt.c_str()); printf("\n");
			config.setDryRun(opt.c_str());
		}

		if (foundOption("media-path") == true) {

			std::string opt = optionValue("media-path");
//			printf(opt.c_str()); printf("\n");
			//config.setDryRun(opt.c_str());
		}

		if (foundOption("media-size") == true) {

			std::string opt = optionValue("media-size");
//			printf(opt.c_str()); printf("\n");
			//config.setDryRun(opt.c_str());
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

	std::string CatalogueArgvParser::usageDescriptionHeader(unsigned int _width) const
	{
		std::string usage;

		usage += "usage: imgarc subcommand [options] [args]\n\n";
		usage += "Image archive command line client, version 1.0.0.1\n";
		usage += "Type 'imgarc help <subcommand>' for help on a specific subcommand.\n\n";

		std::string tmp = "imgarc is the primary command-line interface to ImgArchive. This interface is used to manage the control of images going in and out of the archive software. ";
		tmp += "It has a rich set of subcommands that \"add/import\" images to the archive and \"export\" images out of the archive, In addition manages the controlled modification of images";
		tmp += " using the \"check-in/check-out\" command set";
		usage += '\n';
		usage += formatString(tmp, _width);
		usage += '\n';
		usage += '\n';
		usage += "Note:\n";
		usage += formatString("The administration of the archive is carried out by the imgadmin command-line interface.", _width) + "\n";

		return usage;
	}

	std::string CatalogueArgvParser::commandUsage(unsigned int width) const
	{
		std::string usage; // the usage description text
		usage = formatString("usage: iaarc[--version][--help] <command>[<args>]\n", width);
		usage += '\n';

		return usage;
	}

	std::string CatalogueArgvParser::generalHelp(unsigned int _width) const
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
		usage += "usage: imga subcommand [options] [args]\n\n";
		usage += "Image archive command line client, version 1.0.0.1\n";
		usage += "Type 'imga help <subcommand>' for help on a specific subcommand.\n\n";
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

} /* namespace simplearchive */