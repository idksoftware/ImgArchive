#include "ShowCommand.h"
#include "AppConfig.h"
#include "CLogger.h"
#include "siaglobal.h"
#include "SAUtils.h"
#include "TextOut.h"
#include "SAUtils.h"
#include "HomePaths.h"
#include "CSVArgs.h"
#include <sstream>

namespace simplearchive {

	ShowCommand::ShowCommand() : m_error(Error::Unknown) {}

	void ShowCommand::setOutputFile(const char* s)
	{
		m_outputFile = s;
	}

	void ShowCommand::setTextOutputType(const char* s)
	{
		m_textOutputType = s;
	}

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
		if (arg.compare("env") == 0) {
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

	bool ShowCommand::processEnv(const char* str) {
		std::string arg = str;

		if (arg.compare("folders") == 0) {
			return showAllowedRaw();
		}
		if (arg.compare("enabled") == 0) {
			return showAllowedPicture();
		}
		if (arg.compare("all") == 0) {
			return showAllowedAll();
		}
		m_error = Error::ParseError;
		return false;
	}

	class ShowAllowedTextOut : public TextOut {
		SelectionType m_type;
	public:
		ShowAllowedTextOut(SelectionType type) : m_type(type) {};
		std::string writePlain();
		std::string writeXML();
		std::string writeJson();
		std::string writeHtml();
	};

	std::string ShowAllowedTextOut ::writePlain() {
		AppConfig appConfig;
		std::stringstream str;

		ImageExtentions& imageExtentions = ImageExtentions::get();
		std::vector<std::shared_ptr<ExtentionItem>> list;
		
		str << " AllowedTypes" << std::endl;
		str << " ============" << std::endl;
		imageExtentions.getList(list, m_type);
		for (auto ii = list.begin(); ii != list.end(); ++ii) {
			std::shared_ptr<ExtentionItem> data = *ii;
			str << " Ext:         " << data->getExt() << std::endl;
			str << " ImageType:   " << data->getType().toString() << std::endl;
			str << " Mime:        " << data->getMimeType() << std::endl;
			str << " Description: " << data->getDesciption() << std::endl;
			str << std::endl;
		}
		std::string s = str.str();
		return s;
	}
	std::string ShowAllowedTextOut::writeXML() {
		AppConfig appConfig;
		std::stringstream str;

		ImageExtentions& imageExtentions = ImageExtentions::get();
		std::vector<std::shared_ptr<ExtentionItem>> list;
		str << XML_HEATER << '\n';
		str << "<AllowedTypes>" << std::endl;
		imageExtentions.getList(list, m_type);
		for (auto ii = list.begin(); ii != list.end(); ++ii) {
			std::shared_ptr<ExtentionItem> data = *ii;
			str << "\t" << "<Type>" << std::endl;
			str << "\t\t" << writeXMLTag("Ext", data->getExt());
			str << "\t\t" << writeXMLTag("ImageType", data->getType().toString());
			str << "\t\t" << writeXMLTag("Mime", data->getMimeType());
			str << "\t\t" << writeXMLTag("Description", data->getDesciption());
			str << "\t" << "</Item>" << std::endl;
		}
		str << "</AllowedTypes>" << std::endl;
		std::string s = str.str();
		return s;
	}
	std::string ShowAllowedTextOut::writeJson() {
		AppConfig appConfig;
		std::stringstream str;

		ImageExtentions& imageExtentions = ImageExtentions::get();
		std::vector<std::shared_ptr<ExtentionItem>> list;
		str << "{" << std::endl;
		str << "\t\"AllowedTypes\" : [" << std::endl;
		imageExtentions.getList(list, m_type);
		bool first = true;
		for (auto ii = list.begin(); ii != list.end(); ++ii) {
			if (first) {
				first = false;
			}
			else {
				str << "\t\t" << "}}," << std::endl;
			}
			std::shared_ptr<ExtentionItem> data = *ii;
			//str << "\t\t" << "\"" << data->getExt() << "\" : {" << std::endl;
			str << "\t\t" << "{ \"Type\" : {" << std::endl;
			str << "\t\t\t" << writeJsonTag("Ext", data->getExt());
			str << "\t\t\t" << writeJsonTag("ImageType", data->getType().toString());
			str << "\t\t\t" << writeJsonTag("Mime", data->getMimeType());
			str << "\t\t\t" << writeJsonTag("Description", data->getDesciption(), true);
			
		}
		str << "\t\t" << "}}" << std::endl;
		str << "\t]" << std::endl;
		str << "}" << std::endl;
		std::string s = str.str();
		return s;
	}
	std::string ShowAllowedTextOut::writeHtml() {
		return std::string();
	}

	bool ShowCommand::showAllowed(SelectionType type)
	{
		//m_outputFile;
		
		ShowAllowedTextOut showAllowedTextOut(type);
		if (showAllowedTextOut.parseTextOutType(m_textOutputType.c_str()) == false) {
			return false;
		}
		showAllowedTextOut.process();
		return true;
		/*
		AppConfig appConfig;
		std::stringstream str;

		ImageExtentions& imageExtentions = ImageExtentions::get();
		std::vector<std::shared_ptr<ExtentionItem>> list;
		imageExtentions.getList(list, type);
		for (auto ii = list.begin(); ii != list.end(); ++ii) {
			std::shared_ptr<ExtentionItem> data = *ii;
			str << data->toString() << std::endl;
		}

		std::string s = str.str();
		std::cout << s;
		*/
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
			return showGeneral(m_outputFile.c_str(), m_textOutputType.c_str());
		}
		if (arg.compare("logging") == 0) {
			return showLogging(m_outputFile.c_str(), m_textOutputType.c_str());
		}
		if (arg.compare("network") == 0) {
			return showNetwork(m_outputFile.c_str(), m_textOutputType.c_str());
		}
		if (arg.compare("folders") == 0) {
			return showFolders(m_outputFile.c_str(), m_textOutputType.c_str());
		}
		if (arg.compare("master") == 0) {
			return showMaster(m_outputFile.c_str(), m_textOutputType.c_str());
		}
		if (arg.compare("derivative") == 0) {
			return showDerivative(m_outputFile.c_str(), m_textOutputType.c_str());
		}
		if (arg.compare("workspace") == 0) {
			return showWorkspace(m_outputFile.c_str(), m_textOutputType.c_str());
		}
		if (arg.compare("picture") == 0) {
			return showPicture(m_outputFile.c_str(), m_textOutputType.c_str());
		}
		if (arg.compare("www") == 0) {
			return showWWW(m_outputFile.c_str(), m_textOutputType.c_str());
		}
		if (arg.compare("backup") == 0) {
			return showBackup(m_outputFile.c_str(), m_textOutputType.c_str());
		}
		if (arg.compare("exiftool") == 0) {
			return showExiftool(m_outputFile.c_str(), m_textOutputType.c_str());
		}
		m_error = Error::ParseError;
		return false;
	}

	class GeneralTextOut : public TextOut {
	public:
		std::string writePlain();
		std::string writeXML();
		std::string writeJson();
		std::string writeHtml();
	};

	std::string GeneralTextOut::writePlain() {
		AppConfig appConfig;
		std::stringstream str;

		str << "    General" << std::endl;
		str << "        Log level:                 " << appConfig.getLogLevel() << std::endl;
		str << "        Console level:             " << appConfig.getConsoleLevel() << std::endl;
		str << "        SQL database:              " << ((appConfig.isSQL()) ? "True" : "False") << std::endl;
		str << "        Silent On:                 " << ((appConfig.isSilent()) ? "True" : "False") << std::endl;
		str << "        Quiet On:                  " << ((appConfig.isQuiet()) ? "True" : "False") << std::endl;

		std::string s = str.str();
		return s;
	}
	std::string GeneralTextOut::writeXML() {
		AppConfig appConfig;
		std::stringstream str;

		str << XML_HEATER << std::endl;
		str << "<GeneralSettings>" << std::endl;
		str << writeXMLTag("Loglevel", appConfig.getLogLevel());
		str << writeXMLTag("Consolelevel", appConfig.getConsoleLevel());
		str << writeXMLTag("SQLDatabase", appConfig.isSQL());
		str << writeXMLTag("SilentOn", appConfig.isSilent());
		str << writeXMLTag("QuietOn", appConfig.isQuiet());
		str << "</GeneralSettings>" << std::endl;
		std::string s = str.str();
		return s;
	}
	std::string GeneralTextOut::writeJson() {
		AppConfig appConfig;
		std::stringstream str;
		str << "{" << std::endl;
		str << "\"General\" : {" << std::endl;
		str << writeJsonTag("LogLevel", appConfig.getLogLevel());
		str << writeJsonTag("ConsoleLevel", appConfig.getConsoleLevel());
		str << writeJsonTag("SQLDatabase", ((appConfig.isSQL()) ? "True" : "False"));
		str << writeJsonTag("SilentOn", ((appConfig.isSilent()) ? "True" : "False"));
		str << writeJsonTag("QuietOn", ((appConfig.isQuiet()) ? "True" : "False"), true);
		str << "}}" << std::endl;
		std::string s = str.str();
		return s;
	}
	std::string GeneralTextOut::writeHtml() {
		AppConfig appConfig;
		std::stringstream str;
		/*
		str << "General" << std::endl;
		str << writeHtmlTag("Log level" << appConfig.getLogLevel();
		str << writeHtmlTag("Console level" << appConfig.getConsoleLevel();
		str << writeHtmlTag("SQL database" << appConfig.isSQL();
		str << writeHtmlTag("Silent On" << appConfig.isSilent();
		str << writeHtmlTag("Quiet On" << appConfig.isQuiet();
		*/
		std::string s = str.str();
		return s;
	}

	bool ShowCommand::showGeneral(const char* filename, const char* textOutType)
	{
		GeneralTextOut generalTextOut;
		if (generalTextOut.parseTextOutType(textOutType) == false) {
			return false;
		}
		generalTextOut.process();
		return true;
	}

	bool ShowCommand::showLogging(const char* filename, const char* textOutType)
	{
		return false;
	}

	bool ShowCommand::showNetwork(const char* filename, const char* textOutType)
	{
		return false;
	}

	class FoldersTextOut : public TextOut {
		bool showImaArchiveHome(std::stringstream& str);
	public:
		std::string writePlain();
		std::string writeXML();
		std::string writeJson();
		std::string writeHtml();
	};

	bool ShowCommand::showFolders(const char* filename, const char* textOutType)
	{
		FoldersTextOut foldersTextOut;
		if (foldersTextOut.parseTextOutType(textOutType) == false) {
			return false;
		}
		if (filename != nullptr && filename[0] != '\0') {
			foldersTextOut.setFilename(filename);
		}
		foldersTextOut.process();
		return true;
	}

	std::string FoldersTextOut::writePlain() {
		AppConfig appConfig;
		std::stringstream str;
		str << std::endl;
		str << "Folders" <<std::endl;
		str << "=======" << std::endl;
		if (showImaArchiveHome(str) == false) {
			std::string s = str.str();
			return s;
		}
		str << std::endl;

		str << "Application paths" << std::endl;
		str << "  Configuration path:        " << appConfig.getConfigPath() << std::endl;
		str << "  System path:               " << appConfig.getSystemPath() << std::endl;
		str << "  Log path:                  " << appConfig.getLogPath() << std::endl;
		str << "  Tools path:                " << appConfig.getToolsPath() << std::endl;
		str << "  Hook path:                 " << appConfig.getHookPath() << std::endl;
		str << "  History path:              " << appConfig.getHistoryPath() << std::endl;     
		str << "  SQL Database path:         " << appConfig.getDatabasePath() << std::endl;
		str << "  Templates path:            " << appConfig.getTemplatePath() << std::endl;
		str << "  Duplicates path:           " << ImgArchiveHome::getImgArchiveHome() << "/dups" << std::endl;
		str << std::endl;
		std::string s = str.str();
		return s;
	}

	std::string FoldersTextOut::writeXML()
	{
		AppConfig appConfig;
		std::stringstream str;
		str << "<ApplicationPaths>" << std::endl;
		str << writeXMLTag("ConfigurationPath", appConfig.getConfigPath());
		str << writeXMLTag("SystemPath", appConfig.getSystemPath());
		str << writeXMLTag("LogPath", appConfig.getLogPath());
		str << writeXMLTag("ToolsPath", appConfig.getToolsPath());
		str << writeXMLTag("HookPath", appConfig.getHookPath());
		str << writeXMLTag("HistoryPath", appConfig.getHistoryPath());
		str << writeXMLTag("SQLDatabasePath", appConfig.getDatabasePath());
		str << writeXMLTag("TemplatesPath", appConfig.getTemplatePath());
		//str << writeXMLTag("DuplicatesPath, ImgArchiveHome::getImgArchiveHome() + "/dups");
		str << "</ApplicationPaths>" << std::endl;
		std::string s = str.str();
		return s;
	}

	std::string FoldersTextOut::writeJson()
	{
		AppConfig appConfig;
		std::stringstream str;
		str << "{" << std::endl;
		str << "\"ApplicationPaths\" : {" << std::endl;
		str << writeJsonTag("ConfigurationPath", appConfig.getConfigPath());
		str << writeJsonTag("SystemPath", appConfig.getSystemPath());
		str << writeJsonTag("LogPath", appConfig.getLogPath());
		str << writeJsonTag("ToolsPath", appConfig.getToolsPath());
		str << writeJsonTag("HookPath", appConfig.getHookPath());
		str << writeJsonTag("HistoryPath", appConfig.getHistoryPath());
		str << writeJsonTag("SQLDatabasePath", appConfig.getDatabasePath());
		str << writeJsonTag("TemplatesPath", appConfig.getTemplatePath(), true);
		//str << writeXMLTag("DuplicatesPath, ImgArchiveHome::getImgArchiveHome() + "/dups");
		str << "}" << std::endl;
		str << "}" << std::endl;
		std::string s = str.str();
		return s;
	}

	std::string FoldersTextOut::writeHtml()
	{
		return std::string();
	}

	bool FoldersTextOut::showImaArchiveHome(std::stringstream& str) {
		ImgArchiveHome& imgArchiveHome = ImgArchiveHome::getObject();
		if (imgArchiveHome.isFound() == false) {
			str << "ImgArchive path not set?" << std::endl;
			return false;
		}
		HomePathType homePathType = imgArchiveHome.type();
		str << "ImaArchive Home Location" << std::endl;
		switch (homePathType) {
		case HomePathType::LocalEnv:	// Local Environment set
			str << "  IMGARCHIVE_HOME using Local Environment at loacation: " << std::endl;
			break;
		case HomePathType::SystemEnv:	// System Environment set
			str << "  IMGARCHIVE_HOME using System Environment at loacation: " << std::endl;
			break;
		case HomePathType::UserOnly:	// user only archive
			str << "  Archive found at default user loacation: " << std::endl;
			break;
		case HomePathType::AllUsers:	// all users archive
			str << "  Archive found at default system loacation: " << std::endl;
			break;
		case HomePathType::Unknown:
		default:
			str << "Unknown error" << std::endl;
			return false;
		}
		str << "        " << ImgArchiveHome::getImgArchiveHome() << std::endl;
		if (imgArchiveHome.isValid() == false) {
			str << "  IMGARCHIVE_HOME not found at loacation: " << ImgArchiveHome::getImgArchiveHome();
		}
		return true;
	}

	class MasterTextOut : public TextOut {

	public:
		std::string writePlain();
		std::string writeXML();
		std::string writeJson();
		std::string writeHtml();
	};

	bool ShowCommand::showMaster(const char* filename, const char* textOutType)
	{
		MasterTextOut masterTextOut;
		if (masterTextOut.parseTextOutType(textOutType) == false) {
			return false;
		}
		masterTextOut.process();
		return true;
	}

	std::string MasterTextOut::writePlain() {
		AppConfig appConfig;
		std::stringstream str;
		str << std::endl;
		str << "Master Archive\n";
		str << "==============\n";

		MasterPath masterPath = MasterPath::getObject();
		if (masterPath.isFound() == false) {
			str << "Master Archive path empty" << std::endl;
			str << "Note: This is an Error condition as Master Archive path is mandatory?" << std::endl;
			std::string s = str.str();
			return s;
		}
		if (masterPath.isValid() == false) {
			str << "Master Archive path not found" << std::endl;
			str << "Note: This is an Error condition as Master Archive path is mandatory?" << std::endl;
			std::string s = str.str();
			return s;
		}
		str << "  Master Archive Location" << std::endl;
		str << "        " << masterPath.get() << std::endl;
		str << std::endl;
		str << "  Master Archive Backups\n";
			str << "        Backup One Enabled:        " << ((appConfig.isMasterBackup1Enabled()) ? "True" : "False") << std::endl;
		MasterBackupOnePath masterBackupOnePath = MasterBackupOnePath::getObject();
		if (masterBackupOnePath.isFound() == false) {
			str << "        Path empty" << std::endl;	
		}
		else if (masterBackupOnePath.isValid() == false) {
			str << "        Path not Valid:            " << masterBackupOnePath.get() << std::endl;
		}
		else {
			str << "        Backup One path:           " << masterBackupOnePath.get() << std::endl;
		}
		str << "        Backup Two Enabled:        " << ((appConfig.isMasterBackup2Enabled()) ? "True" : "False") << std::endl;
		MasterBackupTwoPath masterBackupTwoPath = MasterBackupTwoPath::getObject();
		if (masterBackupTwoPath.isFound() == false) {
			str << "        Path empty" << std::endl;
		}
		else if (masterBackupTwoPath.isValid() == false) {
			str << "        Path not Valid:            " << masterBackupTwoPath.get() << std::endl;
		}
		else {
			str << "        Backup Two path:           " << masterBackupTwoPath.get() << std::endl;
		}
		
		str << std::endl;
		std::string s = str.str();
		return s;
	}

	std::string MasterTextOut::writeXML()
	{
		std::string value;
		bool noPath = false;
		std::stringstream str;
		MasterPath masterPath = MasterPath::getObject();
		if (masterPath.isFound() == false) {
			value = "Empty";
			noPath = true;
		}
		if (masterPath.isValid() == false) {
			value = "Not found";
			noPath = true;
		}
		if (noPath == false) {
			value = "Valid";
		}
		str << "<MasterArchive>" << std::endl;
		str << writeXMLTag("Location", value);
		str << writeXMLTag("Path", masterPath.get());

		str << "<BackupOne>" << std::endl;
		MasterBackupOnePath masterBackupOnePath = MasterBackupOnePath::getObject();
		str << writeXMLTag("Enabled", masterBackupOnePath.enabled() ? "True" : "False");
		if (masterBackupOnePath.isFound() == false) {
			value = "Empty";
			noPath = true;
		}
		if (masterBackupOnePath.isValid() == false) {
			value = "Not found";
			noPath = true;
		}
		if (noPath == false) {
			value = "Valid";
		}
		str << writeXMLTag("Location", value);
		str << writeXMLTag("Path", masterBackupOnePath.get());
		str << "</BackupOne>" << std::endl;
		str << "<BackupTwo>" << std::endl;
		MasterBackupTwoPath masterBackupTwoPath = MasterBackupTwoPath::getObject();
		str << writeXMLTag("Enabled", masterBackupTwoPath.enabled() ? "True" : "False");
		if (masterBackupTwoPath.isFound() == false) {
			value = "Empty";
			noPath = true;
		}
		if (masterBackupTwoPath.isValid() == false) {
			value = "Not found";
			noPath = true;
		}
		if (noPath == false) {
			value = "Valid";
		}
		str << writeXMLTag("Location", value);
		str << writeXMLTag("Path", masterBackupTwoPath.get());
		str << "</BackupTwo>" << std::endl;
		str << "</MasterArchive>" << std::endl;
		std::string s = str.str();
		return s;
	}

	std::string MasterTextOut::writeJson()
	{
		std::string value;
		bool noPath = false;
		std::stringstream str;
		MasterPath masterPath = MasterPath::getObject();
		if (masterPath.isFound() == false) {
			value = "Empty";
			noPath = true;
		}
		if (masterPath.isValid() == false) {
			value = "Not found";
			noPath = true;
		}
		if (noPath == false) {
			value = "Valid";
		}
		str << "{" << std::endl;
		str << "\"MasterArchive\" : {" << std::endl;
		str << writeJsonTag("Location", value);
		str << writeJsonTag("Path", masterPath.get());

		str << "\"BackupOne\" : {" << std::endl;
		MasterBackupOnePath masterBackupOnePath = MasterBackupOnePath::getObject();
		str << writeJsonTag("Enabled", masterBackupOnePath.enabled() ? "True" : "False");
		if (masterBackupOnePath.isFound() == false) {
			value = "Empty";
			noPath = true;
		}
		if (masterBackupOnePath.isValid() == false) {
			value = "Not found";
			noPath = true;
		}
		if (noPath == false) {
			value = "Valid";
		}
		str << writeJsonTag("Location", value);
		str << writeJsonTag("Path", masterBackupOnePath.get(), true);
		str << "}," << std::endl;
		str << "\"BackupTwo\" : {" << std::endl;
		MasterBackupTwoPath masterBackupTwoPath = MasterBackupTwoPath::getObject();
		str << writeJsonTag("Enabled", masterBackupTwoPath.enabled() ? "True" : "False");
		if (masterBackupTwoPath.isFound() == false) {
			value = "Empty";
			noPath = true;
		}
		if (masterBackupTwoPath.isValid() == false) {
			value = "Not found";
			noPath = true;
		}
		if (noPath == false) {
			value = "Valid";
		}
		str << writeJsonTag("Location", value);
		str << writeJsonTag("Path", masterBackupTwoPath.get(), true);
		str << "}" << std::endl;
		str << "}" << std::endl;
		str << "}" << std::endl;
		std::string s = str.str();
		return s;
	}

	std::string MasterTextOut::writeHtml()
	{
		return std::string();
	}

	class DerivativeTextOut : public TextOut {
	public:
		std::string writePlain();
		std::string writeXML();
		std::string writeJson();
		std::string writeHtml();
	};

	bool ShowCommand::showDerivative(const char* filename, const char* textOutType)
	{
		DerivativeTextOut derivativeTextOut;
		if (derivativeTextOut.parseTextOutType(textOutType) == false) {
			return false;
		}
		derivativeTextOut.process();
		return true;
	}

	std::string DerivativeTextOut::writePlain() {
		AppConfig appConfig;
		std::stringstream str;
		bool noPath = false;
		str << std::endl;
		str << "Derivative Archive\n";
		str << "==============\n";
		DerivativePath derivativePath = DerivativePath::getObject();
		if (derivativePath.isFound() == false) {
			str << "Derivative path empty" << std::endl;
			str << "Note: This is an Error condition as Derivative Archive path is mandatory?" << std::endl;
			std::string s = str.str();
			return s;
		}
		if (derivativePath.isValid() == false) {
			str << "Derivative path not found" << std::endl;
			str << "Note: This is an Error condition as Derivative Archive path is mandatory?" << std::endl;
			std::string s = str.str();
			return s;
		}

		str << "  Derivative Archive Location" << std::endl;
		str << "        " << derivativePath.get() << std::endl;
		str << std::endl;

		/// Derivative Archive Backups
		
		str << "  Derivative Archive Backups\n";

		
		/// Backup One
		noPath = false;
		str << "        Backup One Enabled:        " << ((appConfig.isDerivativeBackup1Enabled()) ? "True" : "False") << std::endl;
		DerivativeBackupOnePath derivativeBackupOnePath = DerivativeBackupOnePath::getObject();
		if (derivativeBackupOnePath.isFound() == false) {
			str << "        Path empty" << std::endl;
			noPath = true;
		}
		else if (derivativeBackupOnePath.isValid() == false) {
			str << "        Path not Valid:            " << derivativeBackupOnePath.get() << std::endl;
			noPath = true;
		}
		else {
			str << "        Backup One path:           " << derivativeBackupOnePath.get() << std::endl;
		}
		if (appConfig.isDerivativeBackup1Enabled() && noPath) {
			str << "Note: This is an Error condition as Derivative backup 1 Path location is not valid?" << std::endl;
		}

		str << "        Backup Two Enabled:        " << ((appConfig.isDerivativeBackup2Enabled()) ? "True" : "False") << std::endl;
		if (appConfig.isDerivativeBackup2Enabled() && noPath) {
			str << "Note: This is an Error condition as Derivative backup 1 Path location is not valid?" << std::endl;
		}

		/// Backup Two
		noPath = false;
		DerivativeBackupTwoPath derivativeBackupTwoPath = DerivativeBackupTwoPath::getObject();
		if (derivativeBackupTwoPath.isFound() == false) {
			str << "        Path empty" << std::endl;
			noPath = true;
		}
		else if (derivativeBackupTwoPath.isValid() == false) {
			str << "        Path not Valid:            " << derivativeBackupTwoPath.get() << std::endl;
			noPath = true;
		}
		else {
			str << "        Backup Two path:           " << derivativeBackupTwoPath.get() << std::endl;
		}
		if (appConfig.isDerivativeBackup2Enabled() && noPath) {
			str << "Note: This is an Error condition as Derivative backup 2 Path location is not valid?" << std::endl;
		}
		str << std::endl;
		std::string s = str.str();
		return s;
	}

	std::string DerivativeTextOut::writeXML()
	{
		std::string value;
		bool noPath = false;
		std::stringstream str;
		DerivativePath derivativePath = DerivativePath::getObject();
		if (derivativePath.isFound() == false) {
			value = "Empty";
			noPath = true;
		}
		if (derivativePath.isValid() == false) {
			value = "Not found";
			noPath = true;
		}
		if (noPath == false) {
			value = "Valid";
		}
		str << "<DerivativeArchive>" << std::endl;
		str << writeXMLTag("Location", value);
		str << writeXMLTag("Path", derivativePath.get());

		str << "<BackupOne>" << std::endl;
		DerivativeBackupOnePath derivativeBackupOnePath = DerivativeBackupOnePath::getObject();
		str << writeXMLTag("Enabled", derivativeBackupOnePath.enabled() ? "True" : "False");
		if (derivativeBackupOnePath.isFound() == false) {
			value = "Empty";
			noPath = true;
		}
		if (derivativeBackupOnePath.isValid() == false) {
			value = "Not found";
			noPath = true;
		}
		if (noPath == false) {
			value = "Valid";
		}
		str << writeXMLTag("Location", value);
		str << writeXMLTag("Path", derivativeBackupOnePath.get());
		str << "</BackupOne>" << std::endl;
		str << "<BackupTwo>" << std::endl;
		DerivativeBackupTwoPath derivativeBackupTwoPath = DerivativeBackupTwoPath::getObject();
		str << writeXMLTag("Enabled", derivativeBackupTwoPath.enabled() ? "True" : "False");
		if (derivativeBackupTwoPath.isFound() == false) {
			value = "Empty";
			noPath = true;
		}
		if (derivativeBackupTwoPath.isValid() == false) {
			value = "Not found";
			noPath = true;
		}
		if (noPath == false) {
			value = "Valid";
		}
		str << writeXMLTag("Location", value);
		str << writeXMLTag("Path", derivativeBackupTwoPath.get());
		str << "</BackupTwo>" << std::endl;
		str << "</DerivativeArchive>" << std::endl;
		std::string s = str.str();
		return s;
	}

	std::string DerivativeTextOut::writeJson()
	{
		std::string value;
		bool noPath = false;
		std::stringstream str;
		DerivativePath derivativePath = DerivativePath::getObject();
		if (derivativePath.isFound() == false) {
			value = "Empty";
			noPath = true;
		}
		if (derivativePath.isValid() == false) {
			value = "Not found";
			noPath = true;
		}
		if (noPath == false) {
			value = "Valid";
		}
		str << "{" << std::endl;
		str << "\"DerivativeArchive\" : {" << std::endl;
		str << writeJsonTag("Location", value);
		str << writeJsonTag("Path", derivativePath.get());

		str << "\"BackupOne\" : {" << std::endl;
		DerivativeBackupOnePath derivativeBackupOnePath = DerivativeBackupOnePath::getObject();
		str << writeJsonTag("Enabled", derivativeBackupOnePath.enabled() ? "True" : "False");
		if (derivativeBackupOnePath.isFound() == false) {
			value = "Empty";
			noPath = true;
		}
		if (derivativeBackupOnePath.isValid() == false) {
			value = "Not found";
			noPath = true;
		}
		if (noPath == false) {
			value = "Valid";
		}
		str << writeJsonTag("Location", value);
		str << writeJsonTag("Path", derivativeBackupOnePath.get(), true);
		str << "}," << std::endl;
		str << "\"BackupTwo\" : {" << std::endl;
		DerivativeBackupTwoPath derivativeBackupTwoPath = DerivativeBackupTwoPath::getObject();
		str << writeJsonTag("Enabled", derivativeBackupTwoPath.enabled() ? "True" : "False");
		if (derivativeBackupTwoPath.isFound() == false) {
			value = "Empty";
			noPath = true;
		}
		if (derivativeBackupTwoPath.isValid() == false) {
			value = "Not found";
			noPath = true;
		}
		if (noPath == false) {
			value = "Valid";
		}
		str << writeJsonTag("Location", value);
		str << writeJsonTag("Path", derivativeBackupTwoPath.get(), true);
		str << "}" << std::endl;
		str << "}" << std::endl;
		str << "}" << std::endl;
		std::string s = str.str();
		return s;
	}

	std::string DerivativeTextOut::writeHtml()
	{
		return std::string();
	}

	class WorkspaceTextOut : public TextOut {
	public:
		std::string writePlain();
		std::string writeXML();
		std::string writeJson();
		std::string writeHtml();
	};

	bool ShowCommand::showWorkspace(const char* filename, const char* textOutType)
	{
		WorkspaceTextOut workspaceTextOut;
		if (workspaceTextOut.parseTextOutType(textOutType) == false) {
			return false;
		}
		workspaceTextOut.process();
		return true;
	}

	std::string WorkspaceTextOut::writePlain() {
		AppConfig appConfig;
		std::stringstream str;
		str << std::endl;
		str << "Workspace\n";
		str << "=========\n";
		bool noPath = false;
		WorkspacePath workspacePath = WorkspacePath::getObject();
		if (workspacePath.isFound() == false) {
			str << "  path empty" << std::endl;
			noPath = true;
		}
		if (workspacePath.isValid() == false) {
			str << "  path not found" << std::endl;
			noPath = true;
		}
		if (noPath == false) {
			str << "  Workspace Location" << std::endl;
			str << "        " << workspacePath.get() << std::endl;
		}			
		str << "      Auto view: " << ((workspacePath.autoViewOn()) ? "On" : "Off") << std::endl;
		str << "  Auto checkout: " << ((workspacePath.autoCheckout()) ? "On" : "Off") << std::endl;
		if ((workspacePath.autoViewOn() || workspacePath.autoCheckout()) && noPath) {
			str << "Note: This is an Error condition as Workspace path location is not valid?" << std::endl;
		}
		str << std::endl;
		std::string s = str.str();
		return s;
	}

	std::string WorkspaceTextOut::writeXML()
	{
		std::string value;
		bool noPath = false;
		std::stringstream str;
		WorkspacePath workspacePath = WorkspacePath::getObject();
		if (workspacePath.isFound() == false) {
			value = "Empty";
			noPath = true;
		}
		if (workspacePath.isValid() == false) {
			value = "Not found";
			noPath = true;
		}
		if (noPath == false) {
			value = "Valid";
		}
		str << "<Workspace>" << std::endl;
		str << writeXMLTag("Location", value);
		str << writeXMLTag("Path", workspacePath.get());
		str << writeXMLTag("AutoView", ((workspacePath.autoViewOn()) ? "On" : "Off"));
		str << writeXMLTag("AutoCheckout", ((workspacePath.autoCheckout()) ? "On" : "Off"));
		str << "</Workspace>" << std::endl;
		std::string s = str.str();
		return s;
	}

	std::string WorkspaceTextOut::writeJson()
	{
		std::string value;
		bool noPath = false;
		std::stringstream str;
		WorkspacePath workspacePath = WorkspacePath::getObject();
		if (workspacePath.isFound() == false) {
			value = "Empty";
			noPath = true;
		}
		if (workspacePath.isValid() == false) {
			value = "Not found";
			noPath = true;
		}
		if (noPath == false) {
			value = "Valid";
		}
		str << "{" << std::endl;
		str << "\"Workspace\" : {" << std::endl;
		str << writeJsonTag("Location", value);
		str << writeJsonTag("Path", workspacePath.get());
		str << writeJsonTag("AutoView", ((workspacePath.autoViewOn()) ? "On" : "Off"));
		str << writeJsonTag("AutoCheckout", ((workspacePath.autoCheckout()) ? "On" : "Off"), true);
		str << "}" << std::endl;
		str << "}" << std::endl;
		std::string s = str.str();
		return s;
	}

	std::string WorkspaceTextOut::writeHtml()
	{
		return std::string();
	}

	class PictureTextOut : public TextOut {
	public:
		std::string writePlain();
		std::string writeXML();
		std::string writeJson();
		std::string writeHtml();
	};


	bool ShowCommand::showPicture(const char* filename, const char* textOutType)
	{
		PictureTextOut pictureTextOut;
		if (pictureTextOut.parseTextOutType(textOutType) == false) {
			return false;
		}
		pictureTextOut.process();
		return true;
	}

	std::string PictureTextOut::writePlain() {
		AppConfig appConfig;
		std::stringstream str;
		str << std::endl;
		str << "Picture\n";
		str << "=======\n";
		bool noPath = false;
		PicturePath picturePath = PicturePath::getObject();
		if (picturePath.isFound() == false) {
			str << "  path empty" << std::endl;
			noPath = true;
		}
		if (picturePath.isValid() == false) {
			str << "  path not found" << std::endl;
			noPath = true;
		}
		if (noPath == false) {
			str << "  Picture Location" << std::endl;
			str << "        " << picturePath.get() << std::endl;
		}
		str << "  Auto view: " << ((picturePath.autoViewOn()) ? "On" : "Off") << std::endl;
		if (picturePath.autoViewOn() && noPath) {
			str << "Note: This is an Error condition as Picture path location is not valid?" << std::endl;
		}
		str << std::endl;
		std::string s = str.str();
		return s;
	}

	std::string PictureTextOut::writeXML()
	{
		std::string value;
		bool noPath = false;
		std::stringstream str;
		PicturePath picturePath = PicturePath::getObject();
		if (picturePath.isFound() == false) {
			value = "Empty";
			noPath = true;
		}
		if (picturePath.isValid() == false) {
			value = "Not found";
			noPath = true;
		}
		if (noPath == false) {
			value = "Valid";
		}
		str << "<Picture>" << std::endl;
		str << writeXMLTag("Location", value);
		str << writeXMLTag("Path", picturePath.get());
		str << writeXMLTag("AutoView", ((picturePath.autoViewOn()) ? "On" : "Off"));
		str << "</Picture>" << std::endl;
		std::string s = str.str();
		return s;
	}

	std::string PictureTextOut::writeJson()
	{
		std::string value;
		bool noPath = false;
		std::stringstream str;
		PicturePath picturePath = PicturePath::getObject();
		if (picturePath.isFound() == false) {
			value = "Empty";
			noPath = true;
		}
		if (picturePath.isValid() == false) {
			value = "Not found";
			noPath = true;
		}
		if (noPath == false) {
			value = "Valid";
		}
		str << "{" << std::endl;
		str << "\"Picture\" : {" << std::endl;
		str << writeJsonTag("Location", value);
		str << writeJsonTag("Path", picturePath.get());
		str << writeJsonTag("AutoView", ((picturePath.autoViewOn()) ? "On" : "Off"), true);
		str << "}" << std::endl;
		str << "}" << std::endl;
		std::string s = str.str();
		return s;
	}

	std::string PictureTextOut::writeHtml()
	{
		return std::string();
	}

	class WWWTextOut : public TextOut {
	public:
		std::string writePlain();
		std::string writeXML();
		std::string writeJson();
		std::string writeHtml();
	};


	bool ShowCommand::showWWW(const char* filename, const char* textOutType)
	{
		WWWTextOut wwwTextOut;
		if (wwwTextOut.parseTextOutType(textOutType) == false) {
			return false;
		}
		wwwTextOut.process();
		return true;
	}

	std::string WWWTextOut::writePlain() {
		AppConfig appConfig;
		std::stringstream str;
		str << std::endl;
		str << "WWW\n";
		str << "===\n";
		bool noPath = false;
		WWWImagePath wwwImagePath = WWWImagePath::getObject();
		if (wwwImagePath.isFound() == false) {
			str << "  path empty" << std::endl;
			noPath = true;
		}
		if (wwwImagePath.isValid() == false) {
			str << "  path not found" << std::endl;
			noPath = true;
		}
		if (noPath == false) {
			str << "  WWW Location" << std::endl;
			str << "        " << wwwImagePath.get() << std::endl;
		}
		str << "  Auto view: " << ((wwwImagePath.autoViewOn()) ? "On" : "Off") << std::endl;
		if (wwwImagePath.autoViewOn() && noPath) {
			str << "Note: This is an Error condition as WWW path location is not valid?" << std::endl;
		}
		str << std::endl;
		std::string s = str.str();
		return s;
	}

	std::string WWWTextOut::writeXML()
	{
		std::string value;
		bool noPath = false;
		std::stringstream str;
		WWWImagePath wwwImagePath = WWWImagePath::getObject();
		if (wwwImagePath.isFound() == false) {
			value = "Empty";
			noPath = true;
		}
		if (wwwImagePath.isValid() == false) {
			value = "Not found";
			noPath = true;
		}
		if (noPath == false) {
			value = "Valid";
		}
		str << "<wwwImagePath>" << std::endl;
		str << writeXMLTag("Location", value);
		str << writeXMLTag("Path", wwwImagePath.get());
		str << writeXMLTag("AutoView", ((wwwImagePath.autoViewOn()) ? "On" : "Off"));
		str << "</wwwImagePath>" << std::endl;
		std::string s = str.str();
		return s;
	}

	std::string WWWTextOut::writeJson()
	{
		std::string value;
		bool noPath = false;
		std::stringstream str;
		WWWImagePath wwwImagePath = WWWImagePath::getObject();
		if (wwwImagePath.isFound() == false) {
			value = "Empty";
			noPath = true;
		}
		if (wwwImagePath.isValid() == false) {
			value = "Not found";
			noPath = true;
		}
		if (noPath == false) {
			value = "Valid";
		}
		str << "{" << std::endl;
		str << "\"WWWImagePath\" : {" << std::endl;
		str << writeJsonTag("Location", value);
		str << writeJsonTag("Path", wwwImagePath.get());
		str << writeJsonTag("AutoView", ((wwwImagePath.autoViewOn()) ? "On" : "Off"), true);
		str << "}" << std::endl;
		str << "}" << std::endl;
		std::string s = str.str();
		return s;
	}

	std::string WWWTextOut::writeHtml()
	{
		return std::string();
	}


	bool ShowCommand::showBackup(const char* filename, const char* textOutType)
	{
		return false;
	}

	bool ShowCommand::showExiftool(const char* filename, const char* textOutType)
	{
		AppConfig appConfig;
		std::stringstream str;
		str << "    External Exif Tool" << std::endl;
		str << "        External Exif tool enabled:" << ((appConfig.isExternalExifToolEnabled()) ? "True" : "False") << std::endl;
		str << "        Exif map path:             " << appConfig.getExifMapPath() << std::endl;
		str << "        Exif map file:             " << appConfig.getExifMapFile() << std::endl;
		str << "        Exif Tool:                 " << appConfig.getExternalExifTool() << std::endl;
		str << "        Exif command line:         " << appConfig.getExternalCommandLine() << std::endl;

		std::string s = str.str();
		std::cout << s;
		return true;
	}

}; // namespace
	
	
