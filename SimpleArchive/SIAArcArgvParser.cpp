#include "SIAArcArgvParser.h"
#include "ConfigReader.h"
#include "SIAArcAppOptions.h"
#include "AppConfig.h"
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
		}
		else if (GetEnv(homePath, false) == true) {
			//printf("Found SIA_HOME in user variables: %s", homePath.c_str());
			found = true;
		}
		else {
			bool found = false;
			homePath = SAUtils::GetEnvironment("ProgramData");
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
				homePath = SAUtils::GetEnvironment("USERPROFILE");
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
		temp = SAUtils::GetEnvironment("SIA_ARCHIVE");
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



		//define error codes
		//define error codes
		addErrorCode(0, "Success");
		addErrorCode(1, "Warnings");
		addErrorCode(2, "Errors");
		addErrorCode(3, "Fatal");
		setIntroductoryDescription("Image archive provides an organized place to store images. This archive is"
			" designed to be simple in design and to use. It consists of archiving core that provides the basic archiving"
			" functions but in addition, takes input and provides output from optional external components to provide a"
			" tailored achieving solution and can be extended into a complete achieving system. ");
		setHelpOption();




		// Subcommands
		defineOption("add", "add new images to the archive.", ArgvParser::MasterOption);
		defineOption("checkout", "Checkout images from archive.", ArgvParser::MasterOption);
		defineOption("checkin", "Checkin images to archive.", ArgvParser::MasterOption);
		defineOption("uncheckout", "Un-checkout images to archive.", ArgvParser::MasterOption);
		defineOption("export", "Export images from archive.", ArgvParser::MasterOption);
		defineOption("about", "prints the version information", ArgvParser::MasterOption);
		defineOption("view", "View commands", ArgvParser::MasterOption);
		defineOption("show", "Show details", ArgvParser::MasterOption);
		defineOption("prop", "Manage image properties", ArgvParser::MasterOption);


		/*
		defineOption("b", "Goes through the motions of running the subcommand but makes no\nactual changes ether disk or repository.", ArgvParser::NoOptionAttribute);
		defineOptionAlternative("b", "backup");
		*/
		// Options
		defineOption("i", "image address.", ArgvParser::OptionRequiresValue);
		defineOptionAlternative("i", "image-address");

		defineOption("n", "name of the view.", ArgvParser::OptionRequiresValue);
		defineOptionAlternative("n", "name");

		defineOption("q", "no output is sent to the terminal.", ArgvParser::NoOptionAttribute);
		defineOptionAlternative("q", "quiet");

		defineOption("e", "Generate progress events.", ArgvParser::NoOptionAttribute);
		defineOptionAlternative("e", "events");

		defineOption("s", "source of the images", ArgvParser::OptionRequiresValue);
		defineOptionAlternative("s", "source-path");

		defineOption("d", "destination of the images", ArgvParser::OptionRequiresValue);
		defineOptionAlternative("d", "dist-path");

		defineOption("S", "size of media", ArgvParser::OptionRequiresValue);
		defineOptionAlternative("S", "media-size");

		defineOption("m", "Goes through the motions of running the subcommand but makes no\nactual changes ether disk or repository.", ArgvParser::OptionRequiresValue);
		defineOptionAlternative("m", "media-path");

		defineOption("fd", "from date", ArgvParser::OptionRequiresValue);
		defineOptionAlternative("fd", "from-date");

		defineOption("f", "Specifies a file", ArgvParser::OptionRequiresValue);
		defineOptionAlternative("f", "file");

		defineOption("td", "to date", ArgvParser::OptionRequiresValue);
		defineOptionAlternative("td", "to-date");

		defineOption("a", "location of the archive root folder.", ArgvParser::OptionRequiresValue);
		defineOptionAlternative("a", "archive-path");

		defineOption("r", "location of the archive root folder.", ArgvParser::NoOptionAttribute);
		defineOptionAlternative("r", "root");


		defineOption("l", "Temporarily changes the logging level for the scope of this command session.", ArgvParser::OptionRequiresValue);
		defineOptionAlternative("l", "logging-level");

		defineOption("c", "Comment to be included in command", ArgvParser::OptionRequiresValue);
		defineOptionAlternative("c", "comment");

		defineOption("li", "List items", ArgvParser::OptionRequiresValue);
		defineOptionAlternative("li", "list");

		defineOption("force-date", "Overrides all dates found associated with the images in the selection", ArgvParser::OptionRequiresValue);
		defineOption("default-date", "Uses this date if none found associated with an image", ArgvParser::OptionRequiresValue);

		defineOption("checked-out", "Show checked out", ArgvParser::OptionRequiresValue); // =all =year{2015}
		defineOption("unchecked-out", "Show changed images which are not checked out", ArgvParser::OptionRequiresValue);

		defineCommandOption("add", "comment");
		defineCommandOption("add", "logging-level");
		defineCommandOption("add", "archive-path");
		defineCommandOption("add", "source-path");

		defineCommandOption("export", "comment");
		defineCommandOption("export", "logging-level");
		defineCommandOption("export", "dist-path");

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
			appOptions.setCommandMode(SIAArcAppOptions::CM_Version);
			cmdFound = true;
		}

		else if (command("add") == true) {

			if (foundOption("source-path") == true) {
				std::string opt = optionValue("source-path");
				//printf(opt.c_str()); printf("\n");
				config.setSourcePath(opt.c_str());
			}
			if (foundOption("archive-path") == true) {
				std::string opt = optionValue("archive-path");
				//printf(opt.c_str()); printf("\n");
				config.setWorkspacePath(opt.c_str());
			}
			if (foundOption("file") == true) {
				appOptions.m_filePath = optionValue("file");
				appOptions.m_usingFile = true;
				//printf(m_filePath.c_str()); printf("\n");

			}
			if (foundOption("peek") == true) {
				appOptions.m_peekOnly = true;
				//printf(m_filePath.c_str()); printf("\n");

			}
			if (foundOption("force-date") == true) {
				std::string opt = optionValue("force-date");
				//printf(opt.c_str()); printf("\n");
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
				//printf(opt.c_str()); printf("\n");
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

			appOptions.setCommandMode(SIAArcAppOptions::CM_Import);

			Environment::setEnvironment();
			cmdFound = true;
		}
		else if (command("checkout") == true) {
			if (foundOption("image-address") == true) {
				appOptions.m_imageAddress = optionValue("image-address");
				//printf(m_imageAddress.c_str()); printf("\n");
			}

			if (foundOption("list") == true) {
				appOptions.m_list = true;
				//printf(m_comment.c_str()); printf("\n");
			}

			if (foundOption("file") == true) {
				appOptions.m_filePath = optionValue("file");
				appOptions.m_usingFile = true;
				printf(appOptions.m_filePath.c_str()); printf("\n");
			}

			if (foundOption("comment") == true) {
				appOptions.m_comment = optionValue("comment");
				printf(appOptions.m_comment.c_str()); printf("\n");
			}
			if (foundOption("archive-path") == true) {
				std::string opt = optionValue("archive-path");
				printf(opt.c_str()); printf("\n");
				config.setWorkspacePath(opt.c_str());
			}

			appOptions.setCommandMode(SIAArcAppOptions::CM_Checkout);
			cmdFound = true;
		}
		else if (command("checkin") == true) {

			if (foundOption("image-address") == true) {
				appOptions.m_imageAddress = optionValue("image-address");
				printf(appOptions.m_imageAddress.c_str()); printf("\n");
			}
			if (foundOption("comment") == true) {
				appOptions.m_comment = optionValue("comment");
				printf(appOptions.m_comment.c_str()); printf("\n");
			}
			if (foundOption("archive-path") == true) {
				std::string opt = optionValue("archive-path");
				printf(opt.c_str()); printf("\n");
				config.setWorkspacePath(opt.c_str());
			}


			appOptions.setCommandMode(SIAArcAppOptions::CM_Checkin);
			cmdFound = true;
		}
		else if (command("uncheckout") == true) {
			if (foundOption("image-address") == true) {
				appOptions.m_imageAddress = optionValue("image-address");
				printf(appOptions.m_imageAddress.c_str()); printf("\n");
			}
			if (foundOption("comment") == true) {
				appOptions.m_comment = optionValue("comment");
				printf(appOptions.m_comment.c_str()); printf("\n");
			}
			if (foundOption("archive-path") == true) {
				std::string opt = optionValue("archive-path");
				printf(opt.c_str()); printf("\n");
				config.setWorkspacePath(opt.c_str());
			}

			appOptions.setCommandMode(SIAArcAppOptions::CM_UnCheckout);
			cmdFound = true;
		}
		else if (command("show") == true) {
			if (foundOption("checked-out") == true) {
				appOptions.m_showCommandOption = appOptions.ShowCommandOption::SC_ShowCheckedOut;
				appOptions.m_imageAddress = optionValue("checked-out");
				printf(appOptions.m_imageAddress.c_str()); printf("\n");
			}
			if (foundOption("unchecked-out") == true) {
				appOptions.m_showCommandOption = appOptions.ShowCommandOption::SC_ShowUncheckedOutChanges;
				appOptions.m_imageAddress = optionValue("checked-out");
				printf(appOptions.m_imageAddress.c_str()); printf("\n");
			}
			appOptions.setCommandMode(SIAArcAppOptions::CM_Show);
			cmdFound = true;
		}
		else if (command("prop") == true) {
			if (foundOption("list") == true) {
				//m_showCommandOption = ShowCommandOption::SC_ShowCheckedOut;
				appOptions.m_imageAddress = optionValue("list");
				printf(appOptions.m_imageAddress.c_str()); printf("\n");
			}
			appOptions.setCommandMode(SIAArcAppOptions::CM_Prop);
			cmdFound = true;
		}
		else if (command("export") == true) {
			if (foundOption("dist-path") == true) {
				appOptions.m_distinationPath = optionValue("dist-path");
				printf(appOptions.m_distinationPath.c_str()); printf("\n");
			}
			appOptions.setCommandMode(SIAArcAppOptions::CM_Export);
			cmdFound = true;
		}
		else if (command("view") == true) {
			appOptions.setCommandMode(SIAArcAppOptions::CM_View);

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




		else if (command("version") == true) {
			appOptions.setCommandMode(SIAArcAppOptions::CM_Version);
			cmdFound = true;
		}
		else {
			appOptions.setCommandMode(SIAArcAppOptions::CM_Unknown);
			cmdFound = true;
		}
		// global options:

		if (foundOption("events") == true) {
			appOptions.m_eventsOn = true;
		}

		if (foundOption("quiet") == true) {


			printf("quiet=On\n");
			config.setQuiet(true);
		}

		if (foundOption("logging-level") == true) {

			std::string opt = optionValue("logging-level");
			printf(opt.c_str()); printf("\n");
			config.setLogLevel(opt.c_str());
		}

		if (foundOption("dry-run") == true) {

			std::string opt = optionValue("dry-run");
			printf(opt.c_str()); printf("\n");

			//config.setDryRun(opt.c_str());
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
			printf("%s\n", usageDescription().c_str());
		}
		if (cmdFound == false) {
			printf("Main command required?\n\n");
			printf("%s\n", usageDescription().c_str());
			return false;
		}
		return true;
	}

} /* name*/