
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
#include "HomePaths.h"
#include "SetImageExtentionFile.h"
#include "SyncCommand.h"

// beyond compare

using namespace CommandLineProcessing;
namespace simplearchive {

	bool AdminArgvParser::doInitalise(int argc, char **argv) {

		AppOptions &appOptions = AppOptions::get();
		
		
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
		defineCommandSyntax("show", "isadmin show [--setting=<Option>] | [--allowed=[raw|picture|all]]\n"
			"SettingOption = <[general] | [logging] | [network]\n"
			"| [folders] | [master] | | [derivative] | [backup] | [exiftool]");

		defineOption("archive", "These options control the backup activities.", ArgvParser::MasterOption);
		//defineOptionAlternative("archive", "a");
		defineCommandSyntax("archive", "isadmin archive [--media-size=<number>] | [--media-path=<path>]\n"
			"[--from-date=<date>] | [--to-date=date]");

		defineOption("sync", "This command synchronises the primary archives with there associated backups.", ArgvParser::MasterOption);
		//defineOptionAlternative("archive", "a");
		defineCommandSyntax("sync", "isadmin sync --master-with=[backup1] | [backup2] | [Both]\n"
												"--derivative-with=[backup1] | [backup2] | [Both]\n"
												"--workspace --force\n"
												"--catalogue\n"
												"--www\n");

		defineOption("master-with", "Sync this archive with named backup or both", ArgvParser::OptionRequiresValue);
		defineOption("derivative-with", "Sync this archive with named backup or both", ArgvParser::OptionRequiresValue);

		defineOption("about", "prints the version information", ArgvParser::MasterOption);
		defineCommandSyntax("about", "about [--out] [--file]\n");

		defineOption("purge", "Purges deleted images from archive", ArgvParser::MasterOption);
		defineCommandSyntax("purge", "purge\n");

		defineOption("validate", "Validate commands", ArgvParser::MasterOption);
		defineCommandSyntax("validate", "validate [-q | --quiet]\n"
			"| [--scope=<archive>] | [--validate-backup=<option>]\n"
			"| [--validate-main=<yes|no>]\n"
			"| [--repair=<yesno>|\n");

		defineOption("scope", "Scope of validation. This defines which archives will be validated", ArgvParser::OptionRequiresValue);
		defineCommandSyntax("scope", "--scope=<Value>\n"
			"value=[master] | [derivative] | [workspace] | [all] | [main]");
		
		defineOption("validate-backup", "validate backup. Note: both backups will be validate by default if avalible.", ArgvParser::OptionRequiresValue);
		defineCommandSyntax("validate-backup", "--validate-backup=<Value>\n"
			"value=[1] | [2] | [both]");

		defineOption("validate-main", "validate main archive. Note: The main archive (master/will be validate by default.", ArgvParser::OptionRequiresValue);
		defineCommandSyntax("validate-main", "--validate-main=<yes|no>\n");

		defineOption("repair", "repairs the archive if damaged. Will not be repaired by default", ArgvParser::OptionRequiresValue);
		defineCommandSyntax("repair", "--repair=<yes|no>");

		defineOption("allow", "Controls which image file extensions are allowed into the archive.", ArgvParser::MasterOption);
		defineCommandSyntax("allow", "isadmin allow [-q | --quiet]\n"
			"| [--add = <ext, type, mine, description>] | [--delete = <ext>]\n"
			"| [--edit = <ext, type, mine, description>]\n"
			"ext         - Image type file extension to be include in the archive.\n"
			"type        - The type of image i.e.a picture or raw.\n"
			"mine        - The mine type.\n"
			"description - A description of the image type.");
		
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
									"[ExifFileDelim=<character>]\n"
									"[ExifTool=<path/name>] | [ExifCommandLine=<argumentlist>]");

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

		//defineOption("workspace", "Manages the workspace.", ArgvParser::MasterOption);
		//defineCommandSyntax("workspace", "iaarc workspace [--sync]\n\t[--logging-level=<level>]"
		//	"[--comment=<comment text>]\n\t[--scope=<scope-address]\n\t[--force=<yes|No>]");

		defineOption("network", "Configure network parameters", ArgvParser::OptionRequiresValue);
		defineOptionAlternative("network", "N");
		defineCommandSyntax("network", "--network <Option=Value>\n"
			"[EventsEnabled=<Enabled|Disabled>] | [CommandsEnabled=<Enabled|Disabled>] |\n"
			"[EventsPort=<path>] | [EventsAddress=<path>] |\n"
			"[CommandPort=<path>]");

		defineOption("m", "Mirror commands", ArgvParser::NoOptionAttribute);
		defineOptionAlternative("m", "mirror");

		defineOption("add", "Adds an extension type to file type filter. File of this type can now added to the archive.", ArgvParser::OptionRequiresValue);
		defineOptionAlternative("add", "a");
		defineCommandSyntax("add", "--add=<ext,type,mine,description>");

		defineOption("delete", "Deletes an extension type from file type filter. File of this type will now be excluded from the archive.", ArgvParser::OptionRequiresValue);
		defineOptionAlternative("delete", "d");
		defineCommandSyntax("delete", "--delete=<ext>");

		defineOption("edit", "Edits the extension details.", ArgvParser::OptionRequiresValue);
		defineOptionAlternative("edit", "e");
		defineCommandSyntax("edit", "--edit=<ext,type,mine,description>");
		
		defineOption("media-size", "size of media in GBytes", ArgvParser::OptionRequiresValue);
		//defineOptionAlternative("S", "media-size");
		defineCommandSyntax("media-size", "--media-size=<SizeInGBytes>\n");

		defineOption("media-path", "Path where media is located", ArgvParser::OptionRequiresValue);
		defineCommandSyntax("media-path", "--media-path=<path>\n");

		defineOption("from-date", "from date, Date from which to start the archiving. If none given will be from the bebining of the archive", ArgvParser::OptionRequiresValue);
		defineCommandSyntax("from-date", "--from-date=<<day>/<month>/<year>> example: --from-date=02/09/2011\n");

		defineOption("to-date", "to date, Date to shop archiving. If none given will be to the end (lastest item).", ArgvParser::OptionRequiresValue);
		defineCommandSyntax("to-date", "--to-date=<<day>/<month>/<year>> example: --to-date=05/12/2019\n");

		defineOption("force", "Forces over-write", ArgvParser::NoOptionAttribute);
		defineOption("catalogue", "synchronize catalogue", ArgvParser::NoOptionAttribute);
		defineOption("www", "synchronize www catalogue", ArgvParser::NoOptionAttribute);
		defineOption("workspace", "synchronize workspace", ArgvParser::NoOptionAttribute);
		defineCommandSyntax("workspace", "synchronize workspace");

		defineOption("s", "Show setting", ArgvParser::OptionRequiresValue);
		defineOptionAlternative("s", "setting");
		defineCommandSyntax("setting", "--setting=<[general] | [logging] | [network]\n"
			"| [folders] | [master] | | [derivative] | [backup] | [exiftool]");

		defineOption("allowed", "Shows which file type are allowed into the archive.", ArgvParser::OptionRequiresValue);
		defineCommandSyntax("allowed", "--allowed=[ raw | picture | all ]");

		defineOption("out", "Output type: text, xml, json or html.", ArgvParser::OptionRequiresValue);
		defineCommandSyntax("out", "out=[plain] | [xml] | [json] | [html]\n");
		//defineOptionAlternative("u", "users");

		defineOption("file", "output file name.", ArgvParser::OptionRequiresValue);
		defineCommandSyntax("file", "file=<filename>\n");
		
		defineCommandOption("init", "archive-path");
		defineCommandOption("init", "workspace-path");
		defineCommandOption("init", "master-path");
		defineCommandOption("init", "derivative-path");
		defineCommandOption("init", "picture-path");
		defineCommandOption("init", "www-image-path");
		defineCommandOption("init", "set-home-env");
		defineCommandOption("init", "user");

		defineCommandOption("show", "setting");
		defineCommandOption("show", "allowed");
		defineCommandOption("show", "out");
		defineCommandOption("show", "file");

		defineCommandOption("validate", "scope");
		defineCommandOption("validate", "validate-backup");
		defineCommandOption("validate", "validate-main");
		defineCommandOption("validate", "repair");
		
		
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

		defineCommandOption("sync", "master-with");
		defineCommandOption("sync", "derivative-with");
		defineCommandOption("sync", "workspace");
		defineCommandOption("sync", "force");
		defineCommandOption("sync", "catalogue");
		defineCommandOption("sync", "www");

		defineCommandOption("archive", "media-size");
		defineCommandOption("archive", "media-path");
		defineCommandOption("archive", "from-date");
		defineCommandOption("archive", "to-date");

		defineCommandOption("about", "out");
		defineCommandOption("about", "file");

		//defineCommandOption("workspace", "sync");

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
			printf("%s", usageDescription(80).c_str()); // this may need generalHelp(80);
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
					if (SAUtils::IsAdminMode() == false) {
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
			AppOptions::m_catalogPath = PicturePath::get();

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
		else if (command("about") == true) {
			if (foundOption("out") == true) {
				OutputType outputType;
				std::string outType = optionValue("out");
				if (outputType.parse(optionValue("out").c_str()) == false) {
					printf("Option for argument \"out\" for sub-command: %s is invalid: %s\n\n", getCurrentCommand().c_str(), optionValue("out").c_str());
					printf("%s", topicUsageDescription(getCurrentCommandId(), 80).c_str());
					return false;
				}
				else {
					appOptions.m_textOutputType = optionValue("out");
				}
			}
			if (foundOption("file") == true) {
				appOptions.m_outputFile = optionValue("file");
			}
			appOptions.setCommandMode(AppOptions::CommandMode::CM_About);
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
				if (foundOption("out") == true) {
					OutputType outputType;
					std::string outType = optionValue("out");
					if (outputType.parse(optionValue("out").c_str()) == false) {
						printf("Option for argument \"out\" for sub-command: %s is invalid: %s\n\n", getCurrentCommand().c_str(), optionValue("out").c_str());
						printf("%s", topicUsageDescription(getCurrentCommandId(), 80).c_str());
						return false;
					}
					else {
						appOptions.m_textOutputType = optionValue("out");
					}
				}
				if (foundOption("file") == true) {
					appOptions.m_outputFile = optionValue("file");
				}
			} else if (foundOption("allowed") == true) {
				if (setSettings.parseAllowedOptions(optionValue("allowed").c_str()) == true) {
					subOption = "allowed";
					argFound = true;
				}
				if (foundOption("out") == true) {
					OutputType outputType;
					std::string outType = optionValue("out");
					if (outputType.parse(optionValue("out").c_str()) == false) {
						printf("Option for argument \"out\" for sub-command: %s is invalid: %s\n\n", getCurrentCommand().c_str(), optionValue("out").c_str());
						printf("%s", topicUsageDescription(getCurrentCommandId(), 80).c_str());
						return false;
					}
					else {
						appOptions.m_textOutputType = optionValue("out");
					}
				}
				if (foundOption("file") == true) {
					appOptions.m_outputFile = optionValue("file");
				}
			}
			
			
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
		else if (command("purge") == true) {
			printf("Purge command\n");
			appOptions.setCommandMode(AppOptions::CommandMode::CM_Purge);
			cmdFound = true;
		}
		
		else if (command("validate") == true) {
			appOptions.setCommandMode(AppOptions::CommandMode::CM_Validate);
			cmdFound = true;

			/*
			"value=[master] | [derivative] | [workspace] | [all] | [main]");

		defineOption("validate-backup", "validate backup. Note: both backups will be validate by default if avalible.", ArgvParser::OptionRequiresValue);
		defineCommandSyntax("validate-backup", "--validate-backup=<Value>\n"
			"value=[1] | [2] | [both]");
			*/

			if (foundOption("scope") == true) {

				std::string opt = optionValue("scope");
//					printf(opt.c_str()); printf("\n");
				if (opt.compare("master") == 0) {
					appOptions.m_verifyOperation = AppOptions::VerifyOperation::Master;
				}
				else if (opt.compare("derivative") == 0) {
					appOptions.m_verifyOperation = AppOptions::VerifyOperation::Derivative;
				}
				else if (opt.compare("workspace") == 0) {
					appOptions.m_verifyOperation = AppOptions::VerifyOperation::Workspace;
				}
				else if (opt.compare("all") == 0) {
					appOptions.m_verifyOperation = AppOptions::VerifyOperation::All;
				}
				else if (opt.compare("main") == 0) {
					appOptions.m_verifyOperation = AppOptions::VerifyOperation::Main;
				}
				else {
					printf("Invalid argument for sub-command: %s \"%s\"\n\n", getCurrentCommand().c_str(), opt.c_str());
					printf("%s", usageDescription(80).c_str());
					return false;
				}
			}
			if (foundOption("validate-main") == true) {

				std::string opt = optionValue("validate-main");
				SAUtils::isTrueFalse(opt);
				appOptions.m_validateMain = (opt.compare("True") == 0);
			}
			if (foundOption("validate-backup") == true) {
				std::string opt = optionValue("validate-backup");
				if (opt.compare("1") == 0) {
					appOptions.m_verifyBackups = AppOptions::VerifyBackups::Backup_1;
				}
				else if (opt.compare("2") == 0) {
					appOptions.m_verifyBackups = AppOptions::VerifyBackups::Backup_2;
				}
				else if (opt.compare("both") == 0) {
					appOptions.m_verifyBackups = AppOptions::VerifyBackups::Both;
				}
				else {
					printf("Invalid argument for sub-command: %s \"%s\"\n\n", getCurrentCommand().c_str(), opt.c_str());
					printf("%s", usageDescription(80).c_str());
					return false;
				}
			}
			if (foundOption("repair") == true) {
				std::string opt = optionValue("repair");
				SAUtils::isTrueFalse(opt);
				appOptions.m_repair = (opt.compare("True") == 0);
			}
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
					
			} else if (foundOption("logging") == true) {
				std::string opt = optionValue("logging");

				if (setConfig.parseLoggingOptions(opt.c_str()) == false) {
					printf("Invalid argument for sub-command: %s logging \"%s\"\n\n", getCurrentCommand().c_str(), opt.c_str());
					printf("%s", topicUsageDescription(getCurrentCommandId(), 80).c_str());
					return false;
				}

			} else if (foundOption("folders") == true) {
				std::string opt = optionValue("folders");
					
				if (setConfig.parseFolderOptions(opt.c_str()) == false) {
					printf("Invalid argument for sub-command: %s folders \"%s\" %s\n\n", getCurrentCommand().c_str(), opt.c_str(), setConfig.errorString());
					printf("%s", topicUsageDescription(getCurrentCommandId(), 80).c_str());
					return false;
				}
					
			} else if (foundOption("exiftool") == true) {
				std::string opt = optionValue("exiftool");
				
				if (setConfig.parseExifToolOptions(opt.c_str()) == false) {
					printf("Invalid argument for sub-command: %s exiftool \"%s\"\n\n", getCurrentCommand().c_str(), opt.c_str());
					printf("%s", topicUsageDescription(getCurrentCommandId(), 80).c_str());
					return false;
				}
			} else if (foundOption("master") == true) {
				std::string opt = optionValue("master");
				
				if (setConfig.parseMasterOptions(opt.c_str()) == false) {
					printf("Invalid argument for sub-command: %s master \"%s\" %s\n\n", getCurrentCommand().c_str(), opt.c_str(), setConfig.errorString());
					printf("%s", topicUsageDescription(getCurrentCommandId(), 80).c_str());
					return false;
				}
					
			} else if (foundOption("derivative") == true) {
				std::string opt = optionValue("derivative");

				if (setConfig.parseDerivativeOptions(opt.c_str()) == false) {
					printf("Invalid argument for sub-command: %s derivative \"%s\"\n\n", getCurrentCommand().c_str(), opt.c_str());
					printf("%s", topicUsageDescription(getCurrentCommandId(), 80).c_str());
					return false;
				}

			} else if (foundOption("backup") == true) {
				std::string opt = optionValue("backup");

				if (setConfig.parseBackupOptions(opt.c_str()) == false) {
					printf("Invalid argument for sub-command: %s derivative \"%s\"\n\n", getCurrentCommand().c_str(), opt.c_str());
					printf("%s", topicUsageDescription(getCurrentCommandId(), 80).c_str());
					return false;
				}

			} else if (foundOption("network") == true) {
				std::string opt = optionValue("network");
					
				if (setConfig.parseNetworkOptions(opt.c_str()) == false) {
					printf("Invalid argument for sub-command: %s network \"%s\"\n\n", getCurrentCommand().c_str(), opt.c_str());
					printf("%s", usageDescription(80).c_str());
					return false;
				}
					
			} else {
				printf("No argument for sub-command: %s\n", getCurrentCommand().c_str());
				printf("%s", topicUsageDescription(getCurrentCommandId(), 80).c_str());
				return false;
			}
			appOptions.setConfigOptionBlock(setConfig.getOptionBlock().c_str());
			appOptions.setConfigOption(setConfig.getOption().c_str());
			appOptions.setConfigValue(setConfig.getValue().c_str());
			appOptions.setCommandMode(AppOptions::CommandMode::CM_Config);
			cmdFound = true;
		}
		else if (command("sync") == true) {
			SyncCommand syncCommand;
			bool ok = false;
			std::string opt;
			if (foundOption("master-with") == true) {
				opt = optionValue("master-with");
				appOptions.setConfigOption("master");
				if (syncCommand.setBackup(opt.c_str()) == true) {
					ok = true;
				}
				
			}
			else if (foundOption("derivative-with") == true) {
				opt = optionValue("derivative-with");
				appOptions.setConfigOption("derivative");
				if (syncCommand.setBackup(opt.c_str()) == true) {
					ok = true;
				}
				
			}
			else if (foundOption("both-with") == true) {
				opt = optionValue("both-with");
				appOptions.setConfigOption("both");
				if (syncCommand.setBackup(opt.c_str()) == true) {
					ok = true;
				}
				
			}
			else if (foundOption("workspace") == true) {
				opt = optionValue("workspace");
				if (syncCommand.setBackup(opt.c_str()) == true) {
					ok = true;
				}

			}
			else if (foundOption("catalogue") == true) {
				opt = optionValue("catalogue");
				if (syncCommand.setBackup(opt.c_str()) == true) {
					ok = true;
				}

			}
			else if (foundOption("www") == true) {
				opt = optionValue("www");
				if (syncCommand.setBackup(opt.c_str()) == true) {
					ok = true;
				}

			}
			else {
				printf("Invalid argument for sub-command: %s folders \"%s\" %s\n\n", getCurrentCommand().c_str(), appOptions.getConfigOption(), opt.c_str());
				printf("%s", topicUsageDescription(getCurrentCommandId(), 80).c_str());
				return false;
			}
			if (ok) {
				appOptions.setConfigValue(opt.c_str());
			}
			else {
				printf("Invalid argument for sub-command: %s folders \"%s\"\n\n", getCurrentCommand().c_str(), opt.c_str());
				printf("%s", topicUsageDescription(getCurrentCommandId(), 80).c_str());
				return false;
			}
			appOptions.setCommandMode(AppOptions::CommandMode::CM_Sync);
			cmdFound = true;
		}
		else if (command("test") == true) {
			
			if (foundOption("settings") == true) {
				appOptions.setConfigOption("settings");
			}
			appOptions.setCommandMode(AppOptions::CommandMode::CM_Test);
			cmdFound = true;
		}
		else if (command("archive") == true) {
			appOptions.setCommandMode(AppOptions::CommandMode::CM_Backup);
			if (foundOption("media-path") == true) {

				appOptions.m_mediaPath = optionValue("media-path");
//					printf(opt.c_str()); printf("\n");
				//config.setBackupDestinationPath(opt.c_str());
				//appOptions.m_toDate;
			}
			if (foundOption("media-size") == true) {

				appOptions.m_mediaSize = optionValue("media-size");
//					printf(opt.c_str()); printf("\n");
				//config.setBackupMediaSize(opt.c_str());

			}
			if (foundOption("from-date") == true) {

				appOptions.m_fromDate = optionValue("from-date");
//					printf(opt.c_str()); printf("\n");
				//config.setFromDate(opt.c_str());

			}
			if (foundOption("to-date") == true) {

				appOptions.m_toDate = optionValue("to-date");
//					printf(opt.c_str()); printf("\n");
				//config.setToDate(opt.c_str());

			}
			cmdFound = true;
		}
		else {
			appOptions.setCommandMode(AppOptions::CommandMode::CM_Unknown);
			cmdFound = true;
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
		usage = formatString("usage: iaadmin [--about][--help] <command>[<args>]\n", width);
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
					//int option = option2attribute.find(it->first)->second;
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
