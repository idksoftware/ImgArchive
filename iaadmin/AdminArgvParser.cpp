
#include <sstream>
#include "AdminArgvParser.h"
#include "ConfigReader.h"
#include "AdminAppOptions.h"
#include "AppConfig.h"
#include "SAUtils.h"
#include "stdio.h"
#include "SIAArgvParser.h"
#include "Environment.h"
#include "SetConfig.h"
#include "DefaultEnvironment.h"
#include "HomePath.h"

using namespace CommandLineProcessing;
namespace simplearchive {

	bool AdminArgvParser::doInitalise(int argc, char **argv) {

		AppOptions &appOptions = AppOptions::get();


		//define error codes
		addErrorCode(0, "Success");
		addErrorCode(1, "Error");
		setIntroductoryDescription("iaadmin - Tool provides administration house keeping support for ImgArchive.");
		setHelpOption();

		setHeader("usage: imgadmin subcommand [options] [args]\n\n"
			"ImgArchive command line administrator, version 1.0.0.1\n"
			"Type 'iaadmin help <subcommand>' for help on a specific subcommand.\n\n"
			"iaadmin is the primary command-line interface to administer ImgArchive."
			"\n");


		// Subcommands


		defineOption("init", "Create ImgArchive's working enviroment", ArgvParser::MasterOption);
		defineOption("config", "Configure ImgArchive's parameters", ArgvParser::MasterOption);
			/*
				"isadmin config [-q | --quiet][ --general <Option=Value>]\n"
				" [--Logging <Option=Value>][ --Network <Option=Value>]\n"
				" [ --Folders <Option=Value>][ --Master <Option=Value>]\n"
				" [ --Derivative <Option=Value>][ --Backup <Option=Value>]\n"
				" [ --ExifTool <Option=Value>]\n"
				*/


				
		defineOption("show", "Show settings", ArgvParser::MasterOption);
		defineOption("version", "prints the version information", ArgvParser::MasterOption);
		defineOption("validate", "Validate commands", ArgvParser::MasterOption);
		//defineOption("mirror", "Mirror commands", ArgvParser::MasterOption);
		//defineOption("test", "test commands", ArgvParser::MasterOption);

		// Init Command
		defineOption("archive-path", "location of the archive root folder.", ArgvParser::OptionRequiresValue);
		defineOption("workspace-path", "location of the workspace folder.", ArgvParser::OptionRequiresValue);
		defineOption("master-path", "location of the master repository folder.", ArgvParser::OptionRequiresValue);
		defineOption("derivative-path", "location of the derivative repository folder.", ArgvParser::OptionRequiresValue);
		defineOption("picture-path", "location of the picture folder.", ArgvParser::OptionRequiresValue);
		defineOption("www-image-path", "location of the www image folder.", ArgvParser::OptionRequiresValue);
		
		defineOption("set-home-env", "Set the Home environment variable.", ArgvParser::OptionRequiresValue);
		// Configure Command
		defineOption("general", "General options that may be used generally in commands", ArgvParser::OptionRequiresValue);
		defineOptionAlternative("general", "G");

		defineOption("logging", "Logging option that control the logging carried out by applications", ArgvParser::OptionRequiresValue);
		defineOptionAlternative("logging", "L");
		
		defineOption("folders", "These control the folder paths that the system uses.", ArgvParser::OptionRequiresValue);
		defineOptionAlternative("folders", "F");

		defineOption("exiftool", "Configure exit look intergration", ArgvParser::OptionRequiresValue);
		defineOptionAlternative("exiftool", "E");

		defineOption("master", "This section controls the master archive", ArgvParser::OptionRequiresValue);
								/*	"admin config --master BackupOneEnabled=<Enabled|Disabled>\n"
									"- Enables backup one.\n"
									"admin config --master BackupTwoEnabled=<Enabled|Disabled>\n"
									"- Enables backup two.\n"
									"admin config --master BackupOne=<path>\n"
									"- Sets the path used by backup one\n"
									"admin config --master BackupTwo=<path>\n"
									"- Sets the path used by backup two\n" */
		defineOptionAlternative("master", "M");

		defineOption("derivative", "This section controls the derivative archive.", ArgvParser::OptionRequiresValue);
									/* "admin config --derivative BackupOneEnabled=<Enabled|Disabled>\n"
									"- Enables backup one.\n"
									"admin config --derivative BackupTwoEnabled=<Enabled|Disabled>\n"
									"- Enables backup two.\n"
									"admin config --derivative BackupOne=<path>\n"
									"- Sets the path used by backup one\n"
									"admin config --derivative BackupTwo=<path>\n"
									"- Sets the path used by backup two\n", ArgvParser::OptionRequiresValue); */
		defineOptionAlternative("derivative", "D");

		defineOption("network", "Configure network parameters", ArgvParser::OptionRequiresValue);
		defineOptionAlternative("network", "N");

		defineOption("backup", "These options control the backup activities.", ArgvParser::OptionRequiresValue);
		defineOptionAlternative("backup", "B");

		defineOption("m", "Mirror commands", ArgvParser::NoOptionAttribute);
		defineOptionAlternative("m", "mirror");

		defineOption("b", "Goes through the motions of running the subcommand but makes no\nactual changes ether disk or repository.", ArgvParser::NoOptionAttribute);
		defineOptionAlternative("b", "backup-archive");
		
		

		/* Commented to for testing
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

		

		defineOption("logging-level", "Temporarily changes the logging level for the scope of this command session.", ArgvParser::OptionRequiresValue);
		//defineOptionAlternative("r", "logging-level");

		defineOption("comment", "Comment to be included in command", ArgvParser::OptionRequiresValue);
		//defineOptionAlternative("C", "comment");
		*/
		//defineOption("workspace-path", "Location of the archive Workspace", ArgvParser::OptionRequiresValue);
		//defineOptionAlternative("workspace-path", "w");
		
		defineOption("settup", "Show settup", ArgvParser::NoOptionAttribute);

		defineCommandOption("init", "archive-path");
		defineCommandOption("init", "workspace-path");
		defineCommandOption("show", "settup");
		//defineCommandOption("show", "checkedOut");

		defineCommandOption("validate", "scope");
		defineCommandOption("validate", "repair");
		defineCommandOption("validate", "archive-path");
		
		//defineCommandOption("test", "settup");
		
		defineCommandOption("config", "general");
		defineCommandOption("config", "logging");
		defineCommandOption("config", "folders");
		defineCommandOption("config", "master");
		defineCommandOption("config", "derivative");
		defineCommandOption("config", "backup");
		defineCommandOption("config", "exiftool");
		defineCommandOption("config", "network");
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
			appOptions.m_users = SAUtils::IsAdminMode();
			
		
			appOptions.m_configured = false;
			std::string opt;
			if (foundOption("users") == true) {
				std::string users = optionValue("users");
				if (users.compare("myself") == 0) {
					appOptions.setAllUsers(false);
					DefaultEnvironment::setLocalDefaultLocations();
				}
				else if (users.compare("all") == 0) {
					if (DefaultEnvironment::isInAdminMode() == false) {
						// Not in admin mode so cannot be initalised in admin mode so return with error
						printf("Invalid operation? Not in admin mode so cannot be initalised in admin mode\n\n");
						printf("%s", usageDescription(80).c_str());
						return false;
					}
					appOptions.setAllUsers(true);
					DefaultEnvironment::setLocalDefaultLocations();
				}
				else {
					// Invalid option
					printf("%s", topicUsageDescription(getCurrentCommandId(), 80).c_str());
					return false;
				}
			}
			else {
				DefaultEnvironment::setDefaultLocations();
			}
			BoolOption setHomeEnv = BoolOption::Invalid;
			if (foundOption("set-home-env") == true) {
				std::string value = optionValue("set-home-env");
				if ((setHomeEnv = SAUtils::isTrueFalse(value)) == BoolOption::Invalid) {
					printf("%s", topicUsageDescription(getCurrentCommandId(), 80).c_str());
					return false;
				}
				bool homeEnv = (setHomeEnv == BoolOption::True) ? true : false;
				appOptions.setHomeEnv(homeEnv);
			}
			AppOptions::m_homePath = HomePath::get();
			AppOptions::m_workspacePath = WorkspacePath::get();
			AppOptions::m_masterPath = MasterPath::get();
			AppOptions::m_configPath = HomePath::get();
			AppOptions::m_derivativePath = DerivativePath::get();
			AppOptions::m_repositoryPath = HomePath::get();
			AppOptions::m_cataloguePath = PicturePath::get();

			if (foundOption("archive-path") == true) {
				opt = optionValue("archive-path");
				appOptions.setHomePath(opt.c_str());
			}

			if (foundOption("workspace-path") == true) {
				opt = optionValue("workspace-path");
				appOptions.setWorkspacePath(opt.c_str());
			}
			
			if (foundOption("master-path") == true) {
				opt = optionValue("master-path");
				appOptions.setMasterPath(opt.c_str());
			}
			
			if (foundOption("derivative-path") == true) {
				opt = optionValue("derivative-path");
				appOptions.setDerivativePath(opt.c_str());
			}

			//if (foundOption("catalogue-path") == true) {
			//	opt = optionValue("catalogue-path");
			//	appOptions.setCataloguePath(opt.c_str());
			//}

			if (foundOption("picture-path") == true) {
				opt = optionValue("picture-path");
				appOptions.setPicturePath(opt.c_str());
			}

			if (foundOption("www-image-path") == true) {
				opt = optionValue("www-image-path");
				appOptions.setWWWImagePath(opt.c_str());
			}

			cmdFound = true;
		}
		else if (command("version") == true) {
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
		}
		else if (command("config") == true) {
			SetConfig setConfig;
			if (foundOption("general") == true) {
				std::string opt = optionValue("general");
					
				if (setConfig.parseGeneralOptions(opt.c_str()) == false) {
					printf("Invalid argument for sub-command: %s general \"%s\"\n\n", getCurrentCommand().c_str(), opt.c_str());
					printf("%s", usageDescription(80).c_str());
					return false;
				}
					
			}
			if (foundOption("logging") == true) {
				std::string opt = optionValue("logging");

				if (setConfig.parseLoggingOptions(opt.c_str()) == false) {
					printf("Invalid argument for sub-command: %s general \"%s\"\n\n", getCurrentCommand().c_str(), opt.c_str());
					printf("%s", usageDescription(80).c_str());
					return false;
				}

			}
			if (foundOption("folders") == true) {
				std::string opt = optionValue("folders");
					
				if (setConfig.parseFolderOptions(opt.c_str()) == false) {
					printf("Invalid argument for sub-command: %s folders \"%s\"\n\n", getCurrentCommand().c_str(), opt.c_str());
					printf("%s", usageDescription(80).c_str());
					return false;
				}
					
			}
			if (foundOption("exiftool") == true) {
				std::string opt = optionValue("exiftool");
				
				if (setConfig.parseExifToolOptions(opt.c_str()) == false) {
					printf("Invalid argument for sub-command: %s exiftool \"%s\"\n\n", getCurrentCommand().c_str(), opt.c_str());
					printf("%s", usageDescription(80).c_str());
					return false;
				}
			}
			if (foundOption("master") == true) {
				std::string opt = optionValue("master");
				
				if (setConfig.parseMasterOptions(opt.c_str()) == false) {
					printf("Invalid argument for sub-command: %s master \"%s\"\n\n", getCurrentCommand().c_str(), opt.c_str());
					printf("%s", usageDescription(80).c_str());
					return false;
				}
					
			}
			if (foundOption("derivative") == true) {
				std::string opt = optionValue("derivative");

				if (setConfig.parseDerivativeOptions(opt.c_str()) == false) {
					printf("Invalid argument for sub-command: %s derivative \"%s\"\n\n", getCurrentCommand().c_str(), opt.c_str());
					printf("%s", usageDescription(80).c_str());
					return false;
				}

			}
			if (foundOption("backup") == true) {
				std::string opt = optionValue("backup");

				if (setConfig.parseBackupOptions(opt.c_str()) == false) {
					printf("Invalid argument for sub-command: %s derivative \"%s\"\n\n", getCurrentCommand().c_str(), opt.c_str());
					printf("%s", usageDescription(80).c_str());
					return false;
				}

			}
			if (foundOption("network") == true) {
				std::string opt = optionValue("network");
					
				if (setConfig.parseNetworkOptions(opt.c_str()) == false) {
					printf("Invalid argument for sub-command: %s network \"%s\"\n\n", getCurrentCommand().c_str(), opt.c_str());
					printf("%s", usageDescription(80).c_str());
					return false;
				}
					
			}
			appOptions.setConfigOptionBlock(setConfig.getOptionBlock().c_str());
			appOptions.setConfigOption(setConfig.getOption().c_str());
			appOptions.setConfigValue(setConfig.getValue().c_str());
			appOptions.setCommandMode(AppOptions::CommandMode::CM_CONFIG);
			cmdFound = true;
		}
		else if (command("test") == true) {
			if (foundOption("settup") == true) {

				appOptions.setName("settup");
			}
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

	std::string AdminArgvParser::usageDescriptionHeader(unsigned int _width) const
	{
		std::string usage;

		usage += "usage: iaadmin subcommand [options] [args]\n\n";
		usage += "ImgArchive command line Admin client, version 1.0.0.1\n";
		usage += "Type 'iaadmin help <subcommand>' for help on a specific subcommand.\n\n";

		std::string tmp = "iaadmin Tool provides administration house keeping support for ImgArchive.";
		usage += '\n';
		usage += formatString(tmp, _width);
		usage += '\n';
		usage += '\n';

		return usage;
	}

	std::string AdminArgvParser::commandUsage(unsigned int width) const
	{
		std::string usage; // the usage description text
		usage = formatString("usage: imgarc[--version][--help] <command>[<args>]\n", width);
		usage += '\n';

		return usage;
	}



	std::string AdminArgvParser::generalHelp(unsigned int _width) const
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
		usage += ":\n";
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



	std::string AdminArgvParser::usageDescription(unsigned int _width) const
	{
		std::string usage; // the usage description text

		if (intro_description.length())
			usage += formatString(intro_description, _width) + "\n";

		if (max_key <= 1) {// if we have some options

			usage += formatString("No options available\n", _width) + "\n\n";
			return(usage);
		}


		usage += '\n';

		usage += usageDescriptionHeader(_width);

		usage += formatString(command_header, _width) + "\n";
		usage += '\n';
		usage += AVAILABLE_COMMANDS;
		usage += ":\n\n";

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
			_os += " : ";
			usage += formatLine(_os, _width, 0, 20);
			_os.clear();
			_longOpt.clear();
			_shortOpt.clear();
			if (option2descr.find(it->first) != option2descr.end())
				//usage += formatString(option2descr.find(it->first)->second, _width, _os.length() + 2) + "\n";
				usage += formatString(option2descr.find(it->first)->second, _width) + "\n";
			else
				usage += formatString("(no description)", _width, 4) + "\n";

		}


		if (!errorcode2descr.size()) // if have no errorcodes
			return(usage);

		usage += formatString("\n\nReturn codes:\n", _width) + "\n";

		//   map<int, string>::const_iterator eit;
		for (auto alt = errorcode2descr.begin();
			alt != errorcode2descr.end();
			++alt)
		{
			std::ostringstream code;
			code << alt->first;
			std::string label = formatString(code.str(), _width, 4);
			std::string descr = formatString(alt->second, _width, 10);
			usage += label + descr.substr(label.length()) + "\n";
		}
		usage += '\n';
		usage += "Image Archive is a tool for image archiving and version control system.\n";
		usage += "For additional information, see \"http://www.idk-software.com/\"";
		usage += '\n';
		return(usage);
	}
	/*



	*/
	std::string AdminArgvParser::topicUsageDescription(unsigned int topic, unsigned int _width) const
	{
		std::string usage; // the usage description text

		std::string _os; // temp string for the option
		if (option2attribute.find(topic)->second != MasterOption) {
			usage = "error";
			return usage;
		}
		std::string _longOpt;
		std::string _shortOpt;

		std::list<std::string> alternatives = getAllOptionAlternatives(topic);
		for (auto alt = alternatives.begin();
			alt != alternatives.end();
			++alt)
		{
			if (option2attribute.find(topic)->second == MasterOption) {
				int option = option2attribute.find(topic)->second;
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
		_os += " : ";
		usage += formatString(_os, _width) + "\n";
		_os.clear();
		_longOpt.clear();
		_shortOpt.clear();

		if (option2descr.find(topic) != option2descr.end())
			usage += formatString(option2descr.find(topic)->second, _width, 4) + "\n\n";
		else
			usage += formatString("(no description)", _width, 4) + "\n\n";

		usage += "Valid options :\n\n";
		ArgumentContainer ac = command_set.find(topic)->second;

		for (auto opt = ac.begin(); opt != ac.end(); opt++) {
			//printf("%s\n", opt->c_str());
			unsigned int key = option2key.find(opt->c_str())->second;
			std::list<std::string> alternatives = getAllOptionAlternatives(key);
			for (auto alt = alternatives.begin();
				alt != alternatives.end();
				++alt)
			{
				int option = option2attribute.find(key)->second;
				_os.clear();
				if (alt->length() > 1) {

					_longOpt += *alt;
				}
				else {
					_shortOpt += *alt;
				}
			}

			if (!_longOpt.empty()) {
				_os += "\t--";
				_os += _longOpt;
			}
			if (!_shortOpt.empty()) {
				_os += " (-";
				_os += _shortOpt;
				_os += ')';
			}
			_os += " : ";
			usage += formatString(_os, _width) + "\n";
			_os.clear();
			_longOpt.clear();
			_shortOpt.clear();

			if (option2descr.find(key) != option2descr.end()) {
				std::string desc = "\t";
				desc += option2descr.find(key)->second;
				usage += formatString(desc, _width, 4) + "\n\n";
			}
			else
				usage += formatString("\t(no description)", _width, 4) + "\n\n";
		}
		return(usage);
	}


} /* namespace simplearchive */
