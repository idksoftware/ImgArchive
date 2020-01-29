#include "TestArchive.h"
#include "AppConfig.h"
#include "CLogger.h"
#include "siaglobal.h"
#include "SAUtils.h"
#include "EnvFunc.h"

namespace simplearchive {

	TestArchive::TestArchive()
	{
		AppConfig &config = AppConfig::get();
		CLogger::setLogLevel(config.getLogLevel());
		CLogger::setConsoleLevel(config.getConsoleLevel());
		CLogger::setLogPath(config.getLogPath());
		CLogger::setSilent(config.isSilent());
	}


	TestArchive::~TestArchive()
	{
	}

	bool TestArchive::readingConfigFile()
	{
		CLogger &logger = CLogger::getLogger();
		AppConfig &config = AppConfig::get();

		
		logger.log(LOG_OK, CLogger::Level::STATUS, "Reading configuration");

		logger.log(LOG_OK, CLogger::Level::STATUS, "    General");
		logger.log(LOG_OK, CLogger::Level::STATUS, "        Dry run enabled            %s", (config.isDryRun()) ? "True" : "False");
		logger.log(LOG_OK, CLogger::Level::STATUS, "        Log level:                 %s", config.getLogLevel());
		logger.log(LOG_OK, CLogger::Level::STATUS, "        Console level:             %s", config.getConsoleLevel());
		logger.log(LOG_OK, CLogger::Level::INFO, "        SQL database:              %s", config.isSQL() ? "True" : "False");
		logger.log(LOG_OK, CLogger::Level::INFO, "        Silent On:                 %s", config.isSilent() ? "True" : "False");
		logger.log(LOG_OK, CLogger::Level::INFO, "        Quiet On:                  %s", config.isQuiet() ? "True" : "False");
		logger.log(LOG_OK, CLogger::Level::INFO, "    System paths");
		logger.log(LOG_OK, CLogger::Level::INFO, "        System path:              \"%s\"", config.getSystemPath());
		logger.log(LOG_OK, CLogger::Level::INFO, "        Log path:                 \"%s\"", config.getLogPath());
		logger.log(LOG_OK, CLogger::Level::INFO, "        Master path:              \"%s\"", config.getMasterPath());
		logger.log(LOG_OK, CLogger::Level::INFO, "        Derivetive path:          \"%s\"", config.getDerivativePath());
		logger.log(LOG_OK, CLogger::Level::INFO, "        Workspace path:           \"%s\"", config.getWorkspacePath());
		logger.log(LOG_OK, CLogger::Level::INFO, "        Tools path:               \"%s\"", config.getToolsPath());
		logger.log(LOG_OK, CLogger::Level::INFO, "        Hook path:                \"%s\"", config.getHookPath());
		logger.log(LOG_OK, CLogger::Level::INFO, "        History path:             \"%s\"", config.getHistoryPath());
		logger.log(LOG_OK, CLogger::Level::INFO, "        Template path:            \"%s\"", config.getTemplatePath());
		logger.log(LOG_OK, CLogger::Level::INFO, "        Catalog path:             \"%s\"", config.getMasterCataloguePath());
		logger.log(LOG_OK, CLogger::Level::INFO, "        SQL Database path:        \"%s\"", config.getDatabasePath());
		logger.log(LOG_OK, CLogger::Level::INFO, "        Temp path:                \"%s\"", config.getTempPath());
		logger.log(LOG_OK, CLogger::Level::INFO, "    Master Archive Backups");
		logger.log(LOG_OK, CLogger::Level::INFO, "        Backup One Enabled:       \"%s\"", ((config.isBackup1Enabled()) ? "True" : "False"));
		logger.log(LOG_OK, CLogger::Level::INFO, "        Backup One path:          \"%s\"", config.getBackup1());
		logger.log(LOG_OK, CLogger::Level::INFO, "        Backup Two Enabled:       \"%s\"", ((config.isBackup2Enabled()) ? "True" : "False"));
		logger.log(LOG_OK, CLogger::Level::INFO, "        Backup Two path:          \"%s\"", config.getBackup2());
		logger.log(LOG_OK, CLogger::Level::INFO, "    External Exif Tool");
		bool isExternalExifTool = (config.getExternalExifTool() != nullptr)? true : false;
		logger.log(LOG_OK, CLogger::Level::INFO, "        External Exif tool enabled:\"%s\"", isExternalExifTool ? "True" : "False");
		if (isExternalExifTool) {
			logger.log(LOG_OK, CLogger::Level::INFO, "        Exif map path:             \"%s\"", config.getExifMapPath());
			logger.log(LOG_OK, CLogger::Level::INFO, "        Exif map file:             \"%s\"", config.getExifMapFile());
			logger.log(LOG_OK, CLogger::Level::INFO, "        Exif Tool path:            \"%s\"", config.getExternalExifTool());
			logger.log(LOG_OK, CLogger::Level::INFO, "        Exif command line:         \"%s\"", config.getExternalCommandLine());
		}

		return false;
	}

	bool TestArchive::Show() {

		/*
		m_error = false;
		m_verbose = false;
		m_quiet = false;
		m_logLevel = "INFO";
		m_dry_run = false;
		*/
		AdminConfig config;

		/*
		const std::string key = "IMGARCHIVE_HOME";
		std::string temp = SAUtils::GetPOSIXEnv(key);
		std::string homePath = temp;
		*/
		bool found = false;
		std::string homePath;
#ifdef WIN32
		// Looking the HKEY_LOCAL_MACHINE first
		if (GetEnv(homePath, true) == true) {
			printf("Found IAHOME in system variables: %s", homePath.c_str());
			found = true;
		}
		else if (GetEnv(homePath, false) == true) {
			//printf("Found IMGARCHIVE_HOME in user variables: %s", homePath.c_str());
			found = true;
		}
		else {
			bool found = false;
			homePath = SAUtils::GetPOSIXEnv("USERPROFILE");
			if (homePath.empty() == true || homePath.length() == 0) {
				printf("ImgArchive Unable to start? Cannot read user profile.");
				return false;
			}
			else {
				homePath += "/IDK Software/ImageArchive1.0";
				if (SAUtils::DirExists(homePath.c_str()) == true) {
					printf("Found IMGARCHIVE_HOME in user profile: %s", homePath.c_str());
					found = true;
				}
			}
			if (found == false) {
				homePath = SAUtils::GetPOSIXEnv("ProgramData");
				if (homePath.empty() == true || homePath.length() == 0) {
					printf("ImgArchive Unable to start? Cannot read all users profile.");
					return false;
				}
				homePath += "/IDK Software/ImageArchive1.0";
				if (SAUtils::DirExists(homePath.c_str()) == true) {
					printf("Found IMGARCHIVE_HOME in all users profile: %s", homePath.c_str());
					found = true;
				}
			}
		}
#else
#endif
		if (found == false) {
			printf("ImgArchive Unable to start? No archive found in the default location or"
				" the environment variable IMGARCHIVE_HOME not set.\nUse siaadmin to initalise an archive.\n");

			return false;
		}
		std::string configfile = homePath + "/config/" + "config.dat";
		if (SAUtils::FileExists(configfile.c_str()) == false) {

			printf("ImgArchive Unable to start? No config.dat file found in the default location or"
				" the environment variable IMGARCHIVE_HOME not set.\nUse siaadmin to initalise an archive.\n");
			return false;
		}
		AppConfigReader configReader;
		configReader.setNoLogging();
		configReader.read(configfile.c_str(), config);
		//config.printAll();
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
		temp = SAUtils::GetPOSIXEnv("IAARCHIVE");
		if (temp.empty() == false) {
			config.setWorkspacePath(temp.c_str());
		}
		temp = SAUtils::GetPOSIXEnv("IASOURCE");
		if (temp.empty() == false) {
			config.setSourcePath(temp.c_str());
		}
		temp = SAUtils::GetPOSIXEnv("IALOGLEVEL");
		if (temp.empty() == false) {
			config.setLogLevel(temp.c_str());
		}
		temp = SAUtils::GetPOSIXEnv("IACONSOLELEVEL");
		if (temp.empty() == false) {
			config.setConsoleLevel(temp.c_str());
		}
		
		return true;
	}


	class FolderItem {
	public:
		FolderItem(const char* n, const char* p) {
			name = n;
			path = p;
			found = false;
		}
		bool found;
		std::string name;
		std::string path;
		
	};

	class FoldersList : public std::vector<FolderItem> {
		std::vector<std::shared_ptr<FolderItem>> m_found;
		std::vector<std::shared_ptr<FolderItem>> m_notFound;
	public:
		bool init();
		bool process();
		bool summary();
	};
	bool TestArchive::testFolders() {
		//CLogger &logger = CLogger::getLogger();
		//AppConfig &config = AppConfig::get();

		
		FoldersList foldersList;
		printf("Testing Archive\n");
		Show();
		foldersList.init();

		foldersList.process();
		
		if (foldersList.summary()) {
			printf("\n\tSummary -- No errors found\n");
		}
		else {
			printf("\n\tSummary -- Errors found (see above)\n");
		}
		return false;
	}

	bool FoldersList::init() {
		AppConfig &config = AppConfig::get();
		printf("\tMaster Archive Backups\n");
		printf("\t\tBackup One Enabled: %s\n", config.isBackup1Enabled() ? "True" : "False");
		if (config.isBackup1Enabled()) {
			FolderItem folderItemBackup1("Backup One path:       ", config.getBackup1());
			push_back(folderItemBackup1);
		}
		printf("\t\tBackup Two Enabled: %s\n", config.isBackup2Enabled() ? "True" : "False");
		if (config.isBackup2Enabled()) {
			FolderItem folderItemBackup2("Backup Two path:       ", config.getBackup2());
			push_back(folderItemBackup2);
		}
		bool isExternalExifTool = (config.getExternalExifTool() != nullptr) ? true : false;
		printf("\t\tExternal Exif tool enabled: %s\n", isExternalExifTool ? "True" : "False");
		if (isExternalExifTool) {
			FolderItem folderExifMapPath("Exif map path:     ", config.getExifMapPath());
			push_back(folderExifMapPath);
			FolderItem folderExternalExifTool("Exif Tool path:    ", config.getExternalExifTool());
			push_back(folderExternalExifTool);
		}
		FolderItem folderItem1("System path:       ", config.getSystemPath());
		push_back(folderItem1);
		FolderItem folderItem2("Log path:          ", config.getLogPath());
		push_back(folderItem2);
		FolderItem folderItem3("Master path:       ", config.getMasterPath());
		push_back(folderItem3);
		FolderItem folderItem4("Derivetive path:   ", config.getDerivativePath());
		push_back(folderItem4);
		FolderItem folderItem5("Workspace path:    ", config.getWorkspacePath());
		push_back(folderItem5);
		FolderItem folderItem6("Tools path:        ", config.getToolsPath());
		push_back(folderItem6);
		FolderItem folderItem7("Hook path:         ", config.getHookPath());
		push_back(folderItem7);
		FolderItem folderItem8("History path:      ", config.getHistoryPath());
		push_back(folderItem8);
		FolderItem folderItem9("Template path:     ", config.getTemplatePath());
		push_back(folderItem9);
		FolderItem folderItem10("Catalog path:      ", config.getMasterCataloguePath());
		push_back(folderItem10);
		FolderItem folderItem11("SQL Database path: ", config.getDatabasePath());
		push_back(folderItem11);
		FolderItem folderItem12("Temp path:         ", config.getTempPath());
		push_back(folderItem12);
		return true;
	}

	bool FoldersList::process()
	{
		//CLogger &logger = CLogger::getLogger();
		//AppConfig &config = AppConfig::get();
		
		printf("\tLocating folders...\n");
		for (auto ii = begin(); ii != end(); ii++) {
			FolderItem& item = *ii;
			if (SAUtils::DirExists(item.path.c_str())) {
				item.found = true;
				m_found.push_back(std::make_shared<FolderItem>(item));
			}
			else {
				m_notFound.push_back(std::make_shared<FolderItem>(item));
			}
		}
		return true;
	}

	bool FoldersList::summary() {
		//CLogger &logger = CLogger::getLogger();
		//AppConfig &config = AppConfig::get();

		printf("\t\tFolders found\n\n");
		for (auto ii = m_found.begin(); ii != m_found.end(); ii++) {
			std::shared_ptr<FolderItem> item = *ii;
			printf("\t\t\t%s%s\n", item->name.c_str(), item->path.c_str());
		}
		if (m_notFound.size() != 0) {
			printf("\n\t\tFolders not Found\n\n");
			for (auto ii = m_notFound.begin(); ii != m_notFound.end(); ii++) {
				std::shared_ptr<FolderItem> item = *ii;
				printf("\t\t\t%s%s\n", item->name.c_str(), item->path.c_str());
			}
		}
		else {
			printf("\t\tNo folders not Found\n");
		}

		return (m_notFound.size() == 0);
	}
} //simplearchive
