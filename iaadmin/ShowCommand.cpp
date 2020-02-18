#include "ShowCommand.h"
#include "AppConfig.h"
#include "CLogger.h"
#include "siaglobal.h"
#include "SAUtils.h"

#include "SAUtils.h"
#include "CSVArgs.h"
#include <sstream>

namespace simplearchive {

	ShowCommand::ShowCommand() : m_error(Error::Unknown) {}

	bool ShowCommand::parseOptions(const char* str) {
		std::string arg = str;
		if (arg.compare("settup") == 0) {
			return true;
		}
		m_error = Error::ParseError;
		return false;
	}

	bool ShowCommand::process(const char* configOption, const char* configValue)
	{
		std::string arg = configOption;

		if (arg.compare("setting") == 0) {
			return processSettings(configValue);
		}
		if (arg.compare("allowed") == 0) {
			return processAllowed(configValue);
		}
		return false;
	}

	bool ShowCommand::processAllowed(const char* str) {
		std::string arg = str;

		if (arg.compare("raw") == 0) {
			return showAllowedRaw();
		}
		if (arg.compare("picture") == 0) {
			return showAllowedPicture();
		}
		if (arg.compare("all") == 0) {
			return showAllowedAll();
		}
		m_error = Error::ParseError;
		return false;
	}

	bool ShowCommand::showAllowed(SelectionType type)
	{
		AppConfig appConfig;
		std::stringstream str;

		ImageExtentions& imageExtentions = ImageExtentions::get();
		std::vector<std::shared_ptr<ExtentionItem>> list;
		imageExtentions.getList(list, type);
		for (auto ii = list.begin(); ii != list.end(); ++ii) {
			std::shared_ptr<ExtentionItem> data = *ii;
			str << data->toString() << '\n';
		}

		std::string s = str.str();
		std::cout << s;
		return true;
	}

	bool ShowCommand::showAllowedRaw()
	{
		return showAllowed(SelectionType::Raw);
	}

	bool ShowCommand::showAllowedPicture()
	{
		return showAllowed(SelectionType::Picture);
	}

	bool ShowCommand::showAllowedAll()
	{
		return showAllowed(SelectionType::All);
	}

	bool ShowCommand::processSettings(const char* str) {
		std::string arg = str;

		if (arg.compare("general") == 0) {
			return showGeneral();
		}
		if (arg.compare("logging") == 0) {
			return showLogging();
		}
		if (arg.compare("network") == 0) {
			return showNetwork();
		}
		if (arg.compare("folders") == 0) {
			return showFolders();
		}
		if (arg.compare("master") == 0) {
			return showMaster();
		}
		if (arg.compare("derivative") == 0) {
			return showDerivative();
		}
		if (arg.compare("backup") == 0) {
			return showBackup();
		}
		if (arg.compare("exiftool") == 0) {
			return showExiftool();
		}
		m_error = Error::ParseError;
		return false;
	}


	bool ShowCommand::showGeneral()
	{
		AppConfig appConfig;
		std::stringstream str;

		str << "    General" << '\n';
		str << "        Log level:                 " << appConfig.getLogLevel() << '\n';
		str << "        Console level:             " << appConfig.getConsoleLevel() << '\n';
		str << "        SQL database:              " << ((appConfig.isSQL()) ? "True" : "False") << '\n';
		str << "        Silent On:                 " << ((appConfig.isSilent()) ? "True" : "False") << '\n';
		str << "        Quiet On:                  " << ((appConfig.isQuiet()) ? "True" : "False") << '\n';

		std::string s = str.str();
		std::cout << s;
		return true;
	}

	bool ShowCommand::showLogging()
	{
		return false;
	}

	bool ShowCommand::showNetwork()
	{
		return false;
	}

	bool ShowCommand::showFolders()
	{
		AppConfig appConfig;
		std::stringstream str;
		str << "    Application paths" << '\n';
		str << "        System path:               " << appConfig.getSystemPath() << '\n';
		str << "        Log path:                  " << appConfig.getLogPath() << '\n';
		str << "        Master path:               " << appConfig.getMasterPath() << '\n';
		str << "        Derivetive path:           " << appConfig.getDerivativePath() << '\n';
		str << "        Workspace path:            " << appConfig.getWorkspacePath() << '\n';
		str << "        Tools path:                " << appConfig.getToolsPath() << '\n';
		str << "        Hook path:                 " << appConfig.getHookPath() << '\n';
		str << "        History path:              " << appConfig.getHistoryPath() << '\n';
		str << "        Template path:             " << appConfig.getTemplatePath() << '\n';
		str << "        Catalog path:              " << appConfig.getMasterCataloguePath() << '\n';
		str << "        SQL Database path:         " << appConfig.getDatabasePath() << '\n';
		str << "        Temp path:                 " << appConfig.getTempPath() << '\n';

		std::string s = str.str();
		std::cout << s;
		return true;
	}

	bool ShowCommand::showMaster()
	{
		AppConfig appConfig;
		std::stringstream str;
		str << "    Master Archive Backups\n";
		str << "        Backup One Enabled:        " << ((appConfig.isBackup1Enabled()) ? "True" : "False") << '\n';
		str << "        Backup One path:           " << appConfig.getBackup1() << '\n';
		str << "        Backup Two Enabled:        " << ((appConfig.isBackup2Enabled()) ? "True" : "False") << '\n';
		str << "        Backup Two path:           " << appConfig.getBackup2() << '\n';

		std::string s = str.str();
		std::cout << s;
		return true;
	}

	bool ShowCommand::showDerivative()
	{
		return false;
	}

	bool ShowCommand::showBackup()
	{
		return false;
	}

	bool ShowCommand::showExiftool()
	{
		AppConfig appConfig;
		std::stringstream str;
		str << "    External Exif Tool" << '\n';
		str << "        External Exif tool enabled:" << ((appConfig.isExternalExifToolEnabled()) ? "True" : "False") << '\n';
		str << "        Exif map path:             " << appConfig.getExifMapPath() << '\n';
		str << "        Exif map file:             " << appConfig.getExifMapFile() << '\n';
		str << "        Exif Tool:                 " << appConfig.getExternalExifTool() << '\n';
		str << "        Exif command line:         " << appConfig.getExternalCommandLine() << '\n';

		std::string s = str.str();
		std::cout << s;
		return true;
	}

}; // namespace
	
	
