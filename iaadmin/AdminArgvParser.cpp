
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
#include "SetImageExtentionFile.h"

using namespace CommandLineProcessing;
namespace simplearchive {

	bool AdminArgvParser::doInitalise(int argc, char **argv) {

		AppOptions &appOptions = AppOptions::get();


		//define error codes
		addErrorCode(0, "Success");
		addErrorCode(1, "Warnings");
		addErrorCode(2, "Errors");
		addErrorCode(3, "Fatal");

		setIntroductoryDescription("iaadmin - Tool provides administrative house keeping support for ImgArchive.");
		setHelpOption();

		
		setHeader("usage: iaadmin subcommand [options] [args]\n\n"
			"ImgArchive command line administrator, version 1.0.0.1\n"
			"Type 'iaadmin help <subcommand>' for help on a specific subcommand.\n\n"
			"iaadmin is the primary command-line interface to administer ImgArchive."
			"\n");


		// Subcommands


		defineOption("init", "Create ImgArchive's working enviroment", ArgvParser::MasterOption);
		defineCommandSyntax("init", "isadmin init [-q | --quiet] | [--user <all|self>]\n"
			"| [--set-home-env <yes | no>] | [ --archive-path <path>]\n"
			"| [ --workspace-path <path>] | [ --master-path <path>]\n"
			"| [ --derivative-path <path>] | [ --catalogue-path <path>]\n"
			"| [ --picture-path <path>] | [ --www-image-path <path>]");
		
		defineOption("config", "Configure ImgArchive's parameters", ArgvParser::MasterOption);
		defineCommandSyntax("config", "isadmin config [-q | --quiet] | [ --general <Option=Value>]\n"
			"| [--logging <Option=Value>] | [--network <Option=Value>]\n"
			"| [--folders <Option=Value>] | [--master <Option=Value>]\n"
			"| [--derivative <Option=Value>] | [--backup <Option=Value>]\n"
			"| [--exiftool <Option=Value>]");
				
		defineOption("show", "Show how the system is configured.", ArgvParser::MasterOption);
		defineCommandSyntax("show", "--show <Option>\n"
			"Option = <[general] | [logging] | [network]\n"
			"| [folders] | [master] | | [derivative] | [backup] | [exiftool]");


		defineOption("version", "prints the version information", ArgvParser::MasterOption);
		defineOption("validate", "Validate commands", ArgvParser::MasterOption);
		defineOption("allow", "Controls which image file extensions are allowed into the archive.", ArgvParser::MasterOption);
		defineCommandSyntax("allow", "isadmin allow [-q | --quiet]\n"
			"| [--add = <ext, type, mine, description>] | [--delete = <ext>]\n"
			"| [--edit = <ext, type, mine, description>]\n"
			"ext         - Image type file extension to be include in the archive.\n"
			"type        - The type of image i.e.a picture or raw.\n"
			"mine        - The mine type.\n"
			"description - A description of the image type.");
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
		defineOption("user", "Sets user to local or all users.", ArgvParser::OptionRequiresValue);
		// Configure Command
		defineOption("general", "General options that may be used generally in commands", ArgvParser::OptionRequiresValue);
		defineOptionAlternative("general", "G");
		defineCommandSyntax("general", "--general <Option=Value>\n"
			"[Quiet=<On|Off>] | [Silent=<On|Off>] |\n"
			"[FileCatalogue=<path>] | [WWWCatalogue=<path>] |\n"
			"[Lightroom=<On|Off>]");

		defineOption("logging", "Logging option that control the logging carried out by applications", ArgvParser::OptionRequiresValue);
		defineOptionAlternative("logging", "L");
		defineCommandSyntax("logging", "--logging <Option=Value>\n"
			"[LogLevel=<level>] | [ConsoleLevel=<level>]\n"
			"[LogPath=<path>]");

		defineOption("folders", "These control the folder paths that the system uses.", ArgvParser::OptionRequiresValue);
		defineOptionAlternative("folders", "F");
		defineCommandSyntax("folders", "--folders <Option=Value>\n"
			"[HookScripsPath=<path>] | [ToolsPath=<path>]\n"
			"[TempPath=<path>] | [SystemPath=<path>]\n"
			"[MasterPath=<path>] | [MasterCataloguePath=<path>]\n"
			"[DerivativePath=<path>] | [SQLDatabasePath=<path>]\n"
			"[LogPath=<path>] | [HistoryPath=<path>]\n"
			"[LightroomPath=<path>]");

		defineOption("exiftool", "Configure exit look intergration", ArgvParser::OptionRequiresValue);
		defineOptionAlternative("exiftool", "E");
		defineCommandSyntax("exiftool", "--exiftool <Option=Value>\n"
			"[ExifMapPath=<path>] | [ExifMapFile=<filename>]\n"
			"[ExifTool=<path>] | [ExifCommandLine=<argumentlist>]");

		defineOption("master", "This section controls the master archive", ArgvParser::OptionRequiresValue);					
		defineOptionAlternative("master", "M");
		defineCommandSyntax("master", "--master <Option=Value>\n"
			"[BackupOneEnabled=<Enabled|Disabled>] | [BackupTwoEnabled=<Enabled|Disabled>] |\n"
			"[BackupOne=<path>] | [BackupTwo=<path>]");

		defineOption("derivative", "This section controls the derivative archive.", ArgvParser::OptionRequiresValue);						
		defineOptionAlternative("derivative", "D");
		defineCommandSyntax("derivative", "--derivative <Option=Value>\n"
			"[BackupOneEnabled=<Enabled|Disabled>] | [BackupTwoEnabled=<Enabled|Disabled>] |\n"
			"[BackupOne=<path>] | [BackupTwo=<path>]");

		defineOption("network", "Configure network parameters", ArgvParser::OptionRequiresValue);
		defineOptionAlternative("network", "N");
		defineCommandSyntax("network", "--network <Option=Value>\n"
			"[EventsEnabled=<Enabled|Disabled>] | [CommandsEnabled=<Enabled|Disabled>] |\n"
			"[EventsPort=<path>] | [EventsAddress=<path>] |\n"
			"[CommandPort=<path>]");

		defineOption("backup", "These options control the backup activities.", ArgvParser::OptionRequiresValue);
		defineOptionAlternative("backup", "B");
		defineCommandSyntax("backup", "--backup <Option=Value>\n"
			"[media-size=<number>] | [media-path=<path>]\n"
		    "[from-date=<date>] | [to-date=date]");


		defineOption("m", "Mirror commands", ArgvParser::NoOptionAttribute);
		defineOptionAlternative("m", "mirror");

		defineOption("b", "Goes through the motions of running the subcommand but makes no\nactual changes ether disk or repository.", ArgvParser::NoOptionAttribute);
		defineOptionAlternative("b", "backup-archive");
		
		defineOption("add", "Adds an extension type to file type filter. File of this type can now added to the archive.", ArgvParser::OptionRequiresValue);
		defineOptionAlternative("add", "a");
		defineCommandSyntax("add", "--add=<ext,type,mine,description>");

		defineOption("delete", "Deletes an extension type from file type filter. File of this type will now be excluded from the archive.", ArgvParser::OptionRequiresValue);
		defineOptionAlternative("delete", "d");
		defineCommandSyntax("delete", "--delete=<ext>");

		defineOption("edit", "Edits the extension details.", ArgvParser::OptionRequiresValue);
		defineOptionAlternative("edit", "e");
		defineCommandSyntax("edit", "--edit=<ext,type,mine,description>");
		
		//defineOption("dist-path", "destination of the images", ArgvParser::OptionRequiresValue);
		//defineOptionAlternative("d", "dist-path");
		// backup
		
		defineOption("media-size", "size of media", ArgvParser::OptionRequiresValue);
		//defineOptionAlternative("S", "media-size");

		defineOption("media-path", "Goes through the motions of running the subcommand but makes no\nactual changes ether disk or repository.", ArgvParser::OptionRequiresValue);

		defineOption("from-date", "from date", ArgvParser::OptionRequiresValue);
		//defineOptionAlternative("F", "from-date");

		defineOption("to-date", "to date", ArgvParser::OptionRequiresValue);
		//defineOptionAlternative("T", "to-date");

		/* Commented to for testing
		// Options
		defineOption("name", "name of the item.", ArgvParser::OptionRequiresValue);
		//defineOptionAlternative("n", "name");

		defineOption("repair", "Validate and repair.", ArgvParser::NoOptionAttribute);
		//defineOptionAlternative("r", "repair");

		

		defineOption("users", "Make archive available to you only or everyone with a logon to this computer (Myself/All).", ArgvParser::OptionRequiresValue);
		//defineOptionAlternative("u", "users");


		

		

		defineOption("logging-level", "Temporarily changes the logging level for the scope of this command session.", ArgvParser::OptionRequiresValue);
		//defineOptionAlternative("r", "logging-level");

		defineOption("comment", "Comment to be included in command", ArgvParser::OptionRequiresValue);
		//defineOptionAlternative("C", "comment");
		*/
		//defineOption("workspace-path", "Location of the archive Workspace", ArgvParser::OptionRequiresValue);
		//defineOptionAlternative("workspace-path", "w");
		
		defineOption("s", "Show setting", ArgvParser::OptionRequiresValue);
		defineOptionAlternative("s", "setting");

		defineOption("scope", "Scope of validation.", ArgvParser::OptionRequiresValue);
		//defineOptionAlternative("u", "users");

		defineOption("out", "Output type: text, xml, json or html.", ArgvParser::OptionRequiresValue);
		//defineOptionAlternative("u", "users");

		defineCommandOption("init", "archive-path");
		defineCommandOption("init", "workspace-path");
		defineCommandOption("init", "master-path");
		defineCommandOption("init", "derivative-path");
		defineCommandOption("init", "picture-path");
		defineCommandOption("init", "www-image-path");
		defineCommandOption("init", "set-home-env");
		defineCommandOption("init", "user");

		defineCommandOption("show", "setting");
		defineCommandOption("show", "out");

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

		defineCommandOption("allow", "add");
		defineCommandOption("allow", "edit");
		defineCommandOption("allow", "delete");
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
		if (command("allow") == true) {
			std::string opt;
			std::string value;
			if (foundOption("add") == true) {
				opt = "add";
				value = optionValue("add");
				if (!SetImageExtentionFile::validateOptions(value.c_str())) {
					printf("Invalid argument for sub-command: %s \"%s=%s\"\n\n", getCurrentCommand().c_str(), opt.c_str(), value.c_str());
					printf("%s", usageDescription(80).c_str());
					return false;
				}
			} else if (foundOption("edit") == true) {
				opt = "edit";
				value = optionValue("edit");
				if (!SetImageExtentionFile::validateOptions(value.c_str())) {
					printf("Invalid argument for sub-command: %s \"%s=%s\"\n\n", getCurrentCommand().c_str(), opt.c_str(), value.c_str());
					printf("%s", usageDescription(80).c_str());
					return false;
				}
			}
			else if (foundOption("delete") == true) {
				opt = "delete";
				value = optionValue("delete");
			}
			else {
				printf("Invalid argument for sub-command: %s \"%s\"\n\n", getCurrentCommand().c_str(), opt.c_str());
				printf("%s", usageDescription(80).c_str());
				return false;
			}

			appOptions.setConfigOption(opt.c_str());
			appOptions.setConfigValue(value.c_str());
			appOptions.setCommandMode(AppOptions::CommandMode::CM_Allow);
			cmdFound = true;
		} else if (command("init") == true) {
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
			bool argFound = false;
			std::string subOption;
			appOptions.setCommandMode(AppOptions::CommandMode::CM_Show);
			/*
			 general logging network folders master derivative backup exiftool
			*/
			SetSettings setSettings;
			if (foundOption("setting") == true) {
				std::string opt = optionValue("setting");
				if (setSettings.parseSettingsOptions(optionValue("setting").c_str()) == true) {
					subOption = "setting";
					argFound = true;
				}
				OutputType outputType;
				std::string outType = optionValue("out");
				if (outputType.parse(optionValue("out").c_str()) == true) {
					printf("Option for argument \"out\" for sub-command: %s is invalid: %s\n\n", getCurrentCommand().c_str(), optionValue("out").c_str());
					printf("%s", topicUsageDescription(getCurrentCommandId(), 80).c_str());
					return false;
				}
			}
			/*
			if (foundOption("logging") == true) {
				appOptions.setName("general");
				argFound = true;
			}
			if (foundOption("network") == true) {
				appOptions.setName("general");
				argFound = true;
			}
			if (foundOption("folders") == true) {
				appOptions.setName("general");
				argFound = true;
			}
			if (foundOption("master") == true) {
				appOptions.setName("master");
				argFound = true;
			}
			if (foundOption("derivative") == true) {
				appOptions.setName("derivative");
				argFound = true;
			}
			if (foundOption("backup") == true) {
				appOptions.setName("backup");
				argFound = true;
			}
			if (foundOption("exiftool") == true) {
				appOptions.setName("exiftool");
				argFound = true;
			}
			*/
			if (argFound == false) {
				printf("No argument for sub-command: %s\n\n", getCurrentCommand().c_str());
				printf("%s", topicUsageDescription(getCurrentCommandId(), 80).c_str());
				return false;
			}
			
			if (argFound == true) {
				appOptions.setConfigOption(subOption.c_str());
				appOptions.setConfigValue(setSettings.getValue().c_str());
				cmdFound = true;
			}
			
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
					printf("%s", topicUsageDescription(getCurrentCommandId(), 80).c_str());
					return false;
				}
					
			}
			if (foundOption("logging") == true) {
				std::string opt = optionValue("logging");

				if (setConfig.parseLoggingOptions(opt.c_str()) == false) {
					printf("Invalid argument for sub-command: %s general \"%s\"\n\n", getCurrentCommand().c_str(), opt.c_str());
					printf("%s", topicUsageDescription(getCurrentCommandId(), 80).c_str());
					return false;
				}

			}
			if (foundOption("folders") == true) {
				std::string opt = optionValue("folders");
					
				if (setConfig.parseFolderOptions(opt.c_str()) == false) {
					printf("Invalid argument for sub-command: %s folders \"%s\" %s\n\n", getCurrentCommand().c_str(), opt.c_str(), setConfig.errorString());
					printf("%s", topicUsageDescription(getCurrentCommandId(), 80).c_str());
					return false;
				}
					
			}
			if (foundOption("exiftool") == true) {
				std::string opt = optionValue("exiftool");
				
				if (setConfig.parseExifToolOptions(opt.c_str()) == false) {
					printf("Invalid argument for sub-command: %s exiftool \"%s\"\n\n", getCurrentCommand().c_str(), opt.c_str());
					printf("%s", topicUsageDescription(getCurrentCommandId(), 80).c_str());
					return false;
				}
			}
			if (foundOption("master") == true) {
				std::string opt = optionValue("master");
				
				if (setConfig.parseMasterOptions(opt.c_str()) == false) {
					printf("Invalid argument for sub-command: %s master \"%s\" %s\n\n", getCurrentCommand().c_str(), opt.c_str(), setConfig.errorString());
					printf("%s", topicUsageDescription(getCurrentCommandId(), 80).c_str());
					return false;
				}
					
			}
			if (foundOption("derivative") == true) {
				std::string opt = optionValue("derivative");

				if (setConfig.parseDerivativeOptions(opt.c_str()) == false) {
					printf("Invalid argument for sub-command: %s derivative \"%s\"\n\n", getCurrentCommand().c_str(), opt.c_str());
					printf("%s", topicUsageDescription(getCurrentCommandId(), 80).c_str());
					return false;
				}

			}
			if (foundOption("backup") == true) {
				std::string opt = optionValue("backup");

				if (setConfig.parseBackupOptions(opt.c_str()) == false) {
					printf("Invalid argument for sub-command: %s derivative \"%s\"\n\n", getCurrentCommand().c_str(), opt.c_str());
					printf("%s", topicUsageDescription(getCurrentCommandId(), 80).c_str());
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
			appOptions.setCommandMode(AppOptions::CommandMode::CM_Config);
			cmdFound = true;
		}
		else if (command("test") == true) {
			
			if (foundOption("settings") == true) {
				appOptions.setConfigOption("settings");
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

		/*
		usage += "usage: iaadmin subcommand [options] [args]\n\n";
		usage += "ImgArchive command line Admin client, version 1.0.0.1\n";
		usage += "Type 'iaadmin help <subcommand>' for help on a specific subcommand.\n\n";
		*/
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
		usage = formatString("usage: iaadmin [--version][--help] <command>[<args>]\n", width);
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

} /* namespace simplearchive */
