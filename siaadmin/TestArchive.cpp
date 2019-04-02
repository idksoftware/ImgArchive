#include "TestArchive.h"
#include "AppConfig.h"
#include "CLogger.h"
#include "siaglobal.h"
#include "SAUtils.h"

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


		logger.log(LOG_OK, CLogger::Level::INFO, "Reading configuration file ");

		logger.log(LOG_OK, CLogger::Level::INFO, "    General");
		logger.log(LOG_OK, CLogger::Level::INFO, "        Dry run enabled            %s", (config.isDryRun()) ? "True" : "False");
		logger.log(LOG_OK, CLogger::Level::INFO, "        Log level:                 %s", config.getLogLevel());
		logger.log(LOG_OK, CLogger::Level::INFO, "        Console level:             %s", config.getConsoleLevel());
		logger.log(LOG_OK, CLogger::Level::INFO, "        SQL database:              %s", config.isSQL() ? "True" : "False");
		logger.log(LOG_OK, CLogger::Level::INFO, "    System paths");
		logger.log(LOG_OK, CLogger::Level::INFO, "        System path:              \"%s\"", config.getSystemPath());
		logger.log(LOG_OK, CLogger::Level::INFO, "        Log path:                 \"%s\"", config.getLogPath());
		logger.log(LOG_OK, CLogger::Level::INFO, "        Master path:              \"%s\"", config.getMasterPath());
		logger.log(LOG_OK, CLogger::Level::INFO, "        Derivetive path:          \"%s\"", config.getDerivativePath());
		logger.log(LOG_OK, CLogger::Level::INFO, "        Tools path:               \"%s\"", config.getToolsPath());
		logger.log(LOG_OK, CLogger::Level::INFO, "        Hook path:                \"%s\"", config.getHookPath());
		logger.log(LOG_OK, CLogger::Level::INFO, "        History path:             \"%s\"", config.getHistoryPath());
		logger.log(LOG_OK, CLogger::Level::INFO, "        Template path:            \"%s\"", config.getTemplatePath());
		logger.log(LOG_OK, CLogger::Level::INFO, "        Catalog path:             \"%s\"", config.getMasterCataloguePath());
		logger.log(LOG_OK, CLogger::Level::INFO, "        SQL Database path:        \"%s\"", config.getDatabasePath());

		logger.log(LOG_OK, CLogger::Level::INFO, "    External Exif Tool");
		bool isExternalExifTool = false;
		logger.log(LOG_OK, CLogger::Level::INFO, "        External Exif tool enabled:\"%s\"", ((isExternalExifTool = (config.getExternalExifTool() != nullptr))) ? "True" : "False");
		if (isExternalExifTool) {
			logger.log(LOG_OK, CLogger::Level::INFO, "        Exif map path:             \"%s\"", config.getExifMapPath());
			logger.log(LOG_OK, CLogger::Level::INFO, "        Exif map file:             \"%s\"", config.getExifMapFile());
			logger.log(LOG_OK, CLogger::Level::INFO, "        Exif Tool:                 \"%s\"", config.getExternalExifTool());
			logger.log(LOG_OK, CLogger::Level::INFO, "        Exif command line:         \"%s\"", config.getExternalCommandLine());
		}

		return false;
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
		CLogger &logger = CLogger::getLogger();
		AppConfig &config = AppConfig::get();

		
		FoldersList foldersList;
		printf("Testing Archive\n");
		foldersList.init();
		foldersList.process();
		
		if (foldersList.summary()) {
			printf("Summary -- No errors found\n");
		}
		else {
			printf("Summary -- Errors found (see above)\n");
		}
		return false;
	}

	bool FoldersList::init() {
		AppConfig &config = AppConfig::get();
		FolderItem folderItem1("System path:       ", config.getSystemPath());
		push_back(folderItem1);
		FolderItem folderItem2("Log path:          ", config.getLogPath());
		push_back(folderItem2);
		FolderItem folderItem3("Master path:       ", config.getMasterPath());
		push_back(folderItem3);
		FolderItem folderItem4("Derivetive path:   ", config.getDerivativePath());
		push_back(folderItem4);
		FolderItem folderItem5("Tools path:        ", config.getToolsPath());
		push_back(folderItem5);
		FolderItem folderItem6("Hook path:         ", config.getHookPath());
		push_back(folderItem6);
		FolderItem folderItem7("History path:      ", config.getHistoryPath());
		push_back(folderItem7);
		FolderItem folderItem8("Template path:     ", config.getTemplatePath());
		push_back(folderItem8);
		FolderItem folderItem9("Catalog path:      ", config.getMasterCataloguePath());
		push_back(folderItem9);
		FolderItem folderItem10("SQL Database path:", config.getDatabasePath());
		push_back(folderItem10);
		
		return true;
	}

	bool FoldersList::process()
	{
		CLogger &logger = CLogger::getLogger();
		AppConfig &config = AppConfig::get();
		
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
		CLogger &logger = CLogger::getLogger();
		AppConfig &config = AppConfig::get();
		printf("\t\tFolders found\n\n");
		for (auto ii = m_found.begin(); ii != m_found.end(); ii++) {
			std::shared_ptr<FolderItem> item = *ii;
			printf("\t\t\t%s%s\n", item->name.c_str(), item->path.c_str());
		}
		if (m_notFound.size() != 0) {
			printf("\t\tFolders not Found\n\n");
			for (auto ii = m_notFound.begin(); ii != m_notFound.end(); ii++) {
				std::shared_ptr<FolderItem> item = *ii;
				printf("\t\t\t%s%s\n", item->name.c_str(), item->path.c_str());
			}
		}
		else {
			printf("\t\tNo folders not Found\n");
		}

		return true;
	}
} //simplearchive
