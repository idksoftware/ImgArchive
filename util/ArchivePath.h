#pragma once

#include <string>
#include "AppPaths.h"

namespace simplearchive {

class PrimaryIndexPath {
	std::string m_ImageIndexPath;
	std::string m_idxDBPath;
	std::string m_historyPath;
	std::string m_metadataPath;
	std::string m_pathToRepository;
	std::string m_CSVDatabasePath;
	std::string m_CheckoutStatusPath;
public:
	void setRepositoryPath(std::string &pathToRepository);
	bool settup();

	const std::string& getIdxDbPath() const {
		return m_idxDBPath;
	}

	const std::string& getImageIndexPath() const {
		return m_ImageIndexPath;
	}

	const std::string& getHistoryPath() const {
		return m_historyPath;
	}

	const std::string& getMetadataPath() const {
		return m_metadataPath;
	}

	const std::string& getPathToRepository() const {
		return m_pathToRepository;
	}

	const std::string& getCSVDatabasePath() const {
		return m_CSVDatabasePath;
	}

	const std::string& getCheckoutStatusPath() const {
		return m_CheckoutStatusPath;
	}
};
/**
	class RepositoryPath contains the paths to a Date based repository
*/
class RepositoryPath {

	std::string m_relpath;
	std::string m_imagePath;
	std::string m_imageName;
	std::string m_yearStr;
	std::string m_yyyymmddStr;
	std::string m_yearStrPath;				//< i.e. C:\ProgramData\IDK Software\ImageArchive1.0\Master\2015
	std::string m_yyyymmddStrPath;			//< i.e. C:\ProgramData\IDK Software\ImageArchive1.0\Master\2015\2015-11-26
	std::string m_pathToRepository;
	std::string m_metadataPath;
	std::string m_idxDBPath;
	std::string m_historySystemPath;
	std::string m_historyImagePath;
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
	const std::string& getYYYYMMDD() const;
	const std::string& getYYYYMMDDPath() const;
	const std::string& getRelativePath() const;
	
	std::string getRepositoryPath() const;
	std::string getDataPath();
	std::string getCatalog() const;
	void setCatalog(std::string &catalog);
	std::string getImageHistory() const;
	void setImageHistory(std::string &history);
	std::string getSystemHistory() const;
	std::string getCSVDatabasePath() const;

	std::string getJournalPath() const;
	void setJournalPath(std::string &journalPath);
	std::string getMetadataPath() const;
	void setMetadataPath(std::string &metadataPath);
	std::string getIdxDBPath() const;
	void setIdxDBPath(std::string &idxDBPath);
	std::string getImageIdxPath() const;
};
/**
class ArchivePath contains the set of Repository Paths and the global paths for the Archive.
*/
class ArchivePath {

	static std::string m_pathToHome;
	static std::string m_pathToWorkspace;
	static std::string m_mainMetadataPath;
	static std::string m_mainDupsPath;
	static std::string m_mainHistory;
	static std::string m_pathToMaster;
	static std::string m_pathToDerivitive;
	static std::string m_workspaceYyyymmddPath;
	static std::string m_workspaceMetadataPath;
	//static std::string m_backup1;
	//static std::string m_backup2;
	static RepositoryPath m_master;
	static RepositoryPath m_derivative;
	static RepositoryPath m_masterBackupPath[2];
	static RepositoryPath m_derivativeBackupPath[2];
	static PrimaryIndexPath m_primaryIndex;

public:

	
	bool settupRelative(std::string &yyyymmddStr);

	static std::string getPathToHome();
	static void setPathToHome(std::string &pathToHome);
	static std::string getMainDupsPath();
	static void setMainDupsPath(std::string &mainDupsPath);
	static std::string getMainHistory();
	static void setMainHistory(std::string &mainHistory);
	static std::string getMainMetadataPath();
	static void setMainMetadataPath(std::string &mainMetadataPath);
	static std::string getMasterPath();
	static void setMasterPath(std::string &pathToMaster);
	static std::string getDerivativePath();
	static void setDerivativePath(std::string &path);
	static std::string getPathToWorkspace();
	static void setPathToWorkspace(std::string &pathToWorkspace);
	
	static std::string getWorkspaceYyyymmddPath();
	static std::string getWorkspaceMetadataPath();

	static std::string getMasterCatalog();
	static void setMasterCatalog(std::string &MasterCatalog);
	static std::string getIndexHistory();
	//static void setMasterHistory(std::string &MasterHistory);
	static std::string getMasterJournalPath();
	static void setMasterJournalPath(std::string &MasterJournalPath);
	static std::string getMasterMetadataPath();
	static void setMasterMetadataPath(std::string &MasterMetadataPath);
	static std::string getIdxDBPath();
	static void setIdxDBPath(std::string &idxDBPath);

	static std::string getMasterBackup1Path();
	static void setMasterBackup1Path(std::string &pathToBackup1);
	static std::string getMasterBackup2Path();
	static void setMasterBackup2Path(std::string &pathToBackup2);

	static std::string getDerivativeBackup1Path();
	static void setDerivativeBackup1Path(std::string &pathToBackup1);
	static std::string getDerivativeBackup2Path();
	static void setDerivativeBackup2Path(std::string &pathToBackup2);

	static void setMasterBackup1Enabled(bool b);
	static void setMasterBackup2Enabled(bool b);

	static void setDerivativeBackup1Enabled(bool b);
	static void setDerivativeBackup2Enabled(bool b);

	static RepositoryPath& getMaster();
	static RepositoryPath& getDerivative();

	static RepositoryPath& getMasterBackup1();
	static RepositoryPath& getMasterBackup2();

	static RepositoryPath& getDerivativeBackup1();
	static RepositoryPath& getDerivativeBackup2();

	static PrimaryIndexPath& getPrimaryIndex();
	static bool isMasterEnabled();
	static bool isDerivativeEnabled();

	static bool isMasterBackup1Enabled();
	static bool isMasterBackup2Enabled();

	static bool isDerivativeBackup1Enabled();
	static bool isDerivativeBackup2Enabled();

	static ArchivePath& getArchivePath();
};

}; // simplearchive
