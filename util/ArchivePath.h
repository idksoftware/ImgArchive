#pragma once

#include <string>

namespace simplearchive {

class RepositoryPath {

	std::string m_relpath;
	std::string m_imagePath;
	std::string m_imageName;
	std::string m_yearStr;
	std::string m_yyyymmddStr;
	std::string m_yearStrPath;				//< i.e. C:\ProgramData\IDK Software\ImageArchive1.0\shadow\2015
	std::string m_yyyymmddStrPath;			//< i.e. C:\ProgramData\IDK Software\ImageArchive1.0\shadow\2015\2015-11-26
	

	std::string m_pathToRepository;

	std::string m_metadataPath;
	std::string m_idxDBPath;
	std::string m_historyLogPath;
	std::string m_historyPath;
	std::string m_journalPath;
	std::string m_catalog;
	std::string m_dataPath;
	std::string m_DBPath;
	std::string m_ImageIndexPath;

	bool m_enabled;
public:
	bool settup();
	bool settupRelative(std::string &yyyymmddStr);
	bool isEnabled();
	void setEnabled(bool b);
	void setRepositoryPath(std::string &pathToRepository);

	void setImageName(const char *imageName);

	const std::string& getImageName() const;
	const std::string& getRelativePath() const;
	
	std::string getRepositoryPath();
	std::string getDataPath();
	std::string getCatalog() const;
	void setCatalog(std::string &catalog);
	std::string getHistory() const;
	void setHistory(std::string &history);
	std::string getLogHistory() const;
	std::string getCSVDatabasePath() const;

	std::string getJournalPath() const;
	void setJournalPath(std::string &journalPath);
	std::string getMetadataPath() const;
	void setMetadataPath(std::string &metadataPath);
	std::string getIdxDBPath() const;
	void setIdxDBPath(std::string &idxDBPath);
	std::string getImageIdxPath() const;
};

class ArchivePath {
	static std::string m_pathToHome;
	static std::string m_pathToWorkspace;
	static std::string m_mainMetadataPath;
	//	static std::string m_userDefinedSequenceNumberPath;
	static std::string m_mainDupsPath;
	static std::string m_mainHistory;
	//static ArchivePath *m_shadow;
	static std::string m_pathToShadow;

	static std::string m_workspaceYyyymmddPath;
	static std::string m_workspaceMetadataPath;

	static std::string m_backup1;
	static std::string m_backup2;
	static RepositoryPath m_shadow;
	static RepositoryPath m_backupPath[2];
public:

	//bool ArchivePath::settup() {
	static bool settupShadow();
	bool settupRelative(std::string &yyyymmddStr);

	static std::string getPathToHome();
	static void setPathToHome(std::string &pathToHome);
	static std::string getMainDupsPath();
	static void setMainDupsPath(std::string &mainDupsPath);
	static std::string getMainHistory();
	static void setMainHistory(std::string &mainHistory);
	static std::string getMainMetadataPath();
	static void setMainMetadataPath(std::string &mainMetadataPath);
	static std::string getPathToShadow();
	static void setPathToShadow(std::string &pathToShadow);
	static std::string getPathToWorkspace();
	static void setPathToWorkspace(std::string &pathToWorkspace);
	
	static std::string getWorkspaceYyyymmddPath();
	static std::string getWorkspaceMetadataPath();

	static std::string getShadowCatalog();
	static void setShadowCatalog(std::string &shadowCatalog);
	static std::string getShadowHistory();
	static void setShadowHistory(std::string &shadowHistory);
	static std::string getShadowJournalPath();
	static void setShadowJournalPath(std::string &shadowJournalPath);
	static std::string getShadowMetadataPath();
	static void setShadowMetadataPath(std::string &shadowMetadataPath);
	static std::string getIdxDBPath();
	static void setIdxDBPath(std::string &idxDBPath);
	static std::string getBackup1Path();
	static void setBackup1Path(std::string &pathToBackup1);
	static std::string getBackup2Path();
	static void setBackup2Path(std::string &pathToBackup2);
	static RepositoryPath& getShadow();
	static RepositoryPath& getBackup1();
	static RepositoryPath& getBackup2();
	static bool isShadowEnabled();
	static bool isBackup1Enabled();
	static bool isBackup2Enabled();

	static ArchivePath& getArchivePath();
};

}; // simplearchive
