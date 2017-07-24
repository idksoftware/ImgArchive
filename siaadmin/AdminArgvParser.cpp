
#include "AdminArgvParser.h"
#include "ConfigReader.h"
#include "AdminAppOptions.h"
#include "AppConfig.h"
#include "SAUtils.h"
#include "stdio.h"
#include "SIAArgvParser.h"
#include "Environment.h"



using namespace CommandLineProcessing;
namespace simplearchive {

	bool AdminArgvParser::doInitalise(int argc, char **argv) {

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
			"siaadmin is the primary command-line interface to administer Simple Image Archive (sia)."
			"\n");


		// Subcommands


		defineOption("init", "Create Archive enviroment", ArgvParser::MasterOption);
		//defineOptionAlternative("i", "init");

		defineOption("show", "Show settings", ArgvParser::MasterOption);
		// Arguments
		defineOption("version", "prints the version information", ArgvParser::MasterOption);


		defineOption("validate", "Validate commands", ArgvParser::MasterOption);

		defineOption("m", "Mirror commands", ArgvParser::MasterOption);
		defineOptionAlternative("m", "mirror");
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

		defineOption("repository-path", "location of the repository folder.", ArgvParser::OptionRequiresValue);
		//defineOptionAlternative("w", "workspace");

		defineOption("logging-level", "Temporarily changes the logging level for the scope of this command session.", ArgvParser::OptionRequiresValue);
		//defineOptionAlternative("r", "logging-level");

		defineOption("comment", "Comment to be included in command", ArgvParser::OptionRequiresValue);
		//defineOptionAlternative("C", "comment");

		defineOption("workspace-path", "Location of the archive Workspace", ArgvParser::OptionRequiresValue);
		//defineOptionAlternative("w", "workspace-path");

		/*
		defineCommandOption("init", "archive-path");
		defineCommandOption("init", "workspace-path");
		*/

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
		CAppConfig &config = CAppConfig::get();

		if (command("init") == true) {
			// This command will initalise the configuration.
			// so the the configuration need not to be initalised.
			appOptions.setCommandMode(AppOptions::CM_InitArchive);

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


			/*
			else {
			std::string progPath = SAUtils::GetPOSIXEnv("ProgramData");
			std::string siaPath = "/IDK Software/ImageArchive1.0";
			//if (SAUtils::makePath(progPath.c_str(), siaPath.c_str()) == false) {
			//	return false;
			//}
			opt = progPath;
			opt += siaPath;
			}
			if (SAUtils::FileExists(opt.c_str()) == false) {
			return false;
			}
			*/


			if (foundOption("workspace-path") == true) {
				opt = optionValue("workspace-path");
				appOptions.setWorkspacePath(opt.c_str());
			}
			/*
			else {
			std::string temp = SAUtils::GetPOSIXEnv("USERPROFILE");
			opt = temp + "/Documents";
			if (SAUtils::FileExists(temp.c_str()) == false) {
			return false;
			}
			opt = opt + "/SIA Workspace";
			//if (SAUtils::FileExists(opt.c_str()) == false) {
			//	if (SAUtils::mkDir(opt.c_str()) == false) {
			//		return false;
			//	}
			//}

			}
			*/
			//if (SAUtils::FileExists(opt.c_str()) == false) {
			//	return false;
			//}


			if (foundOption("repository-path") == true) {
				opt = optionValue("repository-path");
				appOptions.setMasterPath(opt.c_str());
			}
			/*
			else {
			std::string progPath = SAUtils::GetPOSIXEnv("ProgramData");
			std::string siaPath = "/IDK Software/ImageArchive1.0/Master";
			//if (SAUtils::makePath(progPath.c_str(), siaPath.c_str()) == false) {
			//	return false;
			//}
			opt = progPath;
			opt += siaPath;
			}
			//if (SAUtils::FileExists(opt.c_str()) == false) {
			//	return false;
			//}
			*/

			cmdFound = true;
		}
		else {
			if (appOptions.initaliseConfig() == false) {
				return false;
			}

			if (command("version") == true) {
				appOptions.setCommandMode(AppOptions::CM_Version);
				cmdFound = true;
			}
			else if (command("show") == true) {
				appOptions.setCommandMode(AppOptions::CM_Show);
				if (foundOption("name") == true) {
					std::string opt = optionValue("name");
					printf(opt.c_str()); printf("\n");
					appOptions.setName(opt.c_str());
				}
				cmdFound = true;
			}
			else if (command("validate") == true) {
				appOptions.setCommandMode(AppOptions::CM_Validate);
				cmdFound = true;
				if (foundOption("archive-path") == true) {

					std::string opt = optionValue("archive-path");
					printf(opt.c_str()); printf("\n");
					config.setWorkspacePath(opt.c_str());
				}
				if (foundOption("scope") == true) {

					std::string opt = optionValue("scope");
					printf(opt.c_str()); printf("\n");
					if (opt.compare("Workspace") == 0) {
						appOptions.m_scope = appOptions.Workspace;
					}
					else if (opt.compare("Master") == 0) {
						appOptions.m_scope = appOptions.Master;
					}
					else {
						appOptions.m_scope = appOptions.Both;
					}
				}
				if (foundOption("repair") == true) {
					appOptions.m_repair = true;
				}
			}
			else if (command("mirror") == true) {
				appOptions.setCommandMode(AppOptions::CM_Mirror);

				if (foundOption("archive-path") == true) {

					std::string opt = optionValue("archive-path");
					printf(opt.c_str()); printf("\n");
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
					printf(opt.c_str()); printf("\n");
					appOptions.setName(opt.c_str());
				}

				cmdFound = true;
			}
			else if (foundOption("backup") == true) {
				appOptions.setCommandMode(AppOptions::CM_Archive);

				if (foundOption("archive-path") == true) {

					std::string opt = optionValue("archive-path");
					printf(opt.c_str()); printf("\n");
					config.setWorkspacePath(opt.c_str());
				}

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
				if (foundOption("to-date") == true) {

					std::string opt = optionValue("to-date");
					printf(opt.c_str()); printf("\n");
					config.setToDate(opt.c_str());

				}
				cmdFound = true;
			}
			else if (foundOption("version") == true) {
				appOptions.setCommandMode(AppOptions::CM_Version);
				cmdFound = true;
			}
			else {
				appOptions.setCommandMode(AppOptions::CM_Unknown);
				cmdFound = true;
			}
		}
		if (foundOption("logging-level") == true) {

			std::string opt = optionValue("logging-level");
			printf(opt.c_str()); printf("\n");
			config.setLogLevel(opt.c_str());
		}
		if (foundOption("dry-run") == true) {

			std::string opt = optionValue("dry-run");
			printf(opt.c_str()); printf("\n");
			config.setDryRun(opt.c_str());
		}

		if (foundOption("media-path") == true) {

			std::string opt = optionValue("media-path");
			printf(opt.c_str()); printf("\n");
			//config.setDryRun(opt.c_str());
		}

		if (foundOption("media-size") == true) {

			std::string opt = optionValue("media-size");
			printf(opt.c_str()); printf("\n");
			//config.setDryRun(opt.c_str());
		}

		if (res != ArgvParser::NoParserError) {
			printf("%s\n", parseErrorDescription(res).c_str());
			//printf("%s\n", usageDescription().c_str());
		}
		if (cmdFound == false) {
			printf("Main command required?\n\n");
			printf("%s\n", usageDescription().c_str());
			return false;
		}
		return true;
	}
} /* namespace simplearchive */