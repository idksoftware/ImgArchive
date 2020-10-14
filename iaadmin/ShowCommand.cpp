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
		return std::string();
	}
	std::string ShowAllowedTextOut::writeXML() {
		AppConfig appConfig;
		std::stringstream str;

		ImageExtentions& imageExtentions = ImageExtentions::get();
		std::vector<std::shared_ptr<ExtentionItem>> list;
		str << XML_HEATER << '\n';
		str << "<AllowedTypes>" << '\n';
		imageExtentions.getList(list, m_type);
		for (auto ii = list.begin(); ii != list.end(); ++ii) {
			std::shared_ptr<ExtentionItem> data = *ii;
			str << "\t" << "<Item>" << '\n';
			str << "\t\t" << writeXMLTag("Ext", data->getExt());
			str << "\t\t" << writeXMLTag("ImageType", data->getType().toString());
			str << "\t\t" << writeXMLTag("Mime", data->getMimeType());
			str << "\t\t" << writeXMLTag("Description", data->getDesciption());
			str << "\t" << "</Item>" << '\n';
		}
		str << "</AllowedTypes>" << '\n';
		std::string s = str.str();
		return s;
	}
	std::string ShowAllowedTextOut::writeJson() {
		AppConfig appConfig;
		std::stringstream str;

		ImageExtentions& imageExtentions = ImageExtentions::get();
		std::vector<std::shared_ptr<ExtentionItem>> list;
		str << "{" << '\n';
		str << "\tAllowedTypes : [" << '\n';
		imageExtentions.getList(list, m_type);
		for (auto ii = list.begin(); ii != list.end(); ++ii) {
			std::shared_ptr<ExtentionItem> data = *ii;
			str << "\t\t" << "Item : {" << '\n';
			str << "\t\t\t" << writeJsonTag("Ext", data->getExt());
			str << "\t\t\t" << writeJsonTag("ImageType", data->getType().toString());
			str << "\t\t\t" << writeJsonTag("Mime", data->getMimeType());
			str << "\t\t\t" << writeJsonTag("Description", data->getDesciption());
			str << "\t\t" << "}" << '\n';
		}
		str << "\t]" << '\n';
		str << "}" << '\n';
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
			str << data->toString() << '\n';
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

		str << "    General" << '\n';
		str << "        Log level:                 " << appConfig.getLogLevel() << '\n';
		str << "        Console level:             " << appConfig.getConsoleLevel() << '\n';
		str << "        SQL database:              " << ((appConfig.isSQL()) ? "True" : "False") << '\n';
		str << "        Silent On:                 " << ((appConfig.isSilent()) ? "True" : "False") << '\n';
		str << "        Quiet On:                  " << ((appConfig.isQuiet()) ? "True" : "False") << '\n';

		std::string s = str.str();
		return s;
	}
	std::string GeneralTextOut::writeXML() {
		AppConfig appConfig;
		std::stringstream str;

		str << XML_HEATER << '\n';
		str << "<GeneralSettings>" << '\n';
		str << writeXMLTag("Loglevel", appConfig.getLogLevel());
		str << writeXMLTag("Consolelevel", appConfig.getConsoleLevel());
		str << writeXMLTag("SQLDatabase", appConfig.isSQL());
		str << writeXMLTag("SilentOn", appConfig.isSilent());
		str << writeXMLTag("QuietOn", appConfig.isQuiet());
		str << "</GeneralSettings>" << '\n';
		std::string s = str.str();
		return s;
	}
	std::string GeneralTextOut::writeJson() {
		AppConfig appConfig;
		std::stringstream str;

		str << "    General" << '\n';
		str << "        Log level:                 " << appConfig.getLogLevel() << '\n';
		str << "        Console level:             " << appConfig.getConsoleLevel() << '\n';
		str << "        SQL database:              " << ((appConfig.isSQL()) ? "True" : "False") << '\n';
		str << "        Silent On:                 " << ((appConfig.isSilent()) ? "True" : "False") << '\n';
		str << "        Quiet On:                  " << ((appConfig.isQuiet()) ? "True" : "False") << '\n';

		std::string s = str.str();
		return s;
	}
	std::string GeneralTextOut::writeHtml() {
		AppConfig appConfig;
		std::stringstream str;
		/*
		str << "    General" << '\n';
		str << "        Log level:                 " << appConfig.getLogLevel();
		str << "        Console level:             " << appConfig.getConsoleLevel();
		str << "        SQL database:              " << appConfig.isSQL();
		str << "        Silent On:                 " << appConfig.isSilent();
		str << "        Quiet On:                  " << appConfig.isQuiet();
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

		str << "Application paths" << '\n';
		str << "  Configuration path:        " << appConfig.getConfigPath() << '\n';
		str << "  System path:               " << appConfig.getSystemPath() << '\n';
		str << "  Log path:                  " << appConfig.getLogPath() << '\n';
		str << "  Tools path:                " << appConfig.getToolsPath() << '\n';
		str << "  Hook path:                 " << appConfig.getHookPath() << '\n';
		str << "  History path:              " << appConfig.getHistoryPath() << '\n';     
		str << "  SQL Database path:         " << appConfig.getDatabasePath() << '\n';
		str << "  Templates path:            " << appConfig.getTemplatePath() << '\n';
		str << "  Duplicates path:           " << ImgArchiveHome::getImgArchiveHome() << "/dups" << '\n';
		str << std::endl;
		std::string s = str.str();
		return s;
	}

	std::string FoldersTextOut::writeXML()
	{
		return std::string();
	}

	std::string FoldersTextOut::writeJson()
	{
		return std::string();
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
			str << "Note: This is an Error condition as Master Archive path is mandatory?" << '\n';
			std::string s = str.str();
			return s;
		}
		if (masterPath.isValid() == false) {
			str << "Master Archive path not found" << std::endl;
			str << "Note: This is an Error condition as Master Archive path is mandatory?" << '\n';
			std::string s = str.str();
			return s;
		}
		str << "  Master Archive Location" << std::endl;
		str << "        " << masterPath.get() << '\n';
		str << std::endl;
		str << "  Master Archive Backups\n";
			str << "        Backup One Enabled:        " << ((appConfig.isMasterBackup1Enabled()) ? "True" : "False") << '\n';
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
		str << "        Backup Two Enabled:        " << ((appConfig.isMasterBackup2Enabled()) ? "True" : "False") << '\n';
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
		return std::string();
	}

	std::string MasterTextOut::writeJson()
	{
		return std::string();
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
			str << "Note: This is an Error condition as Derivative Archive path is mandatory?" << '\n';
			std::string s = str.str();
			return s;
		}
		if (derivativePath.isValid() == false) {
			str << "Derivative path not found" << std::endl;
			str << "Note: This is an Error condition as Derivative Archive path is mandatory?" << '\n';
			std::string s = str.str();
			return s;
		}

		str << "  Derivative Archive Location" << std::endl;
		str << "        " << derivativePath.get() << '\n';
		str << std::endl;

		/// Derivative Archive Backups
		
		str << "  Derivative Archive Backups\n";

		/// Backup One
		
		str << "        Backup One Enabled:        " << ((appConfig.isDerivativeBackup1Enabled()) ? "True" : "False") << '\n';
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
			str << "Note: This is an Error condition as Derivative backup 1 Path location is not valid?" << '\n';
		}

		str << "        Backup Two Enabled:        " << ((appConfig.isDerivativeBackup2Enabled()) ? "True" : "False") << '\n';
		if (appConfig.isDerivativeBackup2Enabled() && noPath) {
			str << "Note: This is an Error condition as Derivative backup 1 Path location is not valid?" << '\n';
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
			str << "Note: This is an Error condition as Derivative backup 2 Path location is not valid?" << '\n';
		}
		str << std::endl;
		std::string s = str.str();
		return s;
	}

	std::string DerivativeTextOut::writeXML()
	{
		return std::string();
	}

	std::string DerivativeTextOut::writeJson()
	{
		return std::string();
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
			str << "        " << workspacePath.get() << '\n';
		}			
		str << "      Auto view: " << ((workspacePath.autoViewOn()) ? "On" : "Off") << '\n';
		str << "  Auto checkout: " << ((workspacePath.autoCheckout()) ? "On" : "Off") << '\n';
		if ((workspacePath.autoViewOn() || workspacePath.autoCheckout()) && noPath) {
			str << "Note: This is an Error condition as Workspace path location is not valid?" << '\n';
		}
		str << std::endl;
		std::string s = str.str();
		return s;
	}

	std::string WorkspaceTextOut::writeXML()
	{
		return std::string();
	}

	std::string WorkspaceTextOut::writeJson()
	{
		return std::string();
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
			str << "        " << picturePath.get() << '\n';
		}
		str << "  Auto view: " << ((picturePath.autoViewOn()) ? "On" : "Off") << '\n';
		if (picturePath.autoViewOn() && noPath) {
			str << "Note: This is an Error condition as Picture path location is not valid?" << '\n';
		}
		str << std::endl;
		std::string s = str.str();
		return s;
	}

	std::string PictureTextOut::writeXML()
	{
		return std::string();
	}

	std::string PictureTextOut::writeJson()
	{
		return std::string();
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
			str << "        " << wwwImagePath.get() << '\n';
		}
		str << "  Auto view: " << ((wwwImagePath.autoViewOn()) ? "On" : "Off") << '\n';
		if (wwwImagePath.autoViewOn() && noPath) {
			str << "Note: This is an Error condition as WWW path location is not valid?" << '\n';
		}
		str << std::endl;
		std::string s = str.str();
		return s;
	}

	std::string WWWTextOut::writeXML()
	{
		return std::string();
	}

	std::string WWWTextOut::writeJson()
	{
		return std::string();
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
	
	
