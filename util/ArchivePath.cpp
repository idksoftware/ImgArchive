/*
 * ArchivePath.cpp
 *
 *  Created on: 23 Nov 2015
 *      Author: FergusonI
 */

#include "SAUtils.h"
#include "ArchivePath.h"

namespace simplearchive {

static ArchivePath m_this;

ArchivePath& ArchivePath::getArchivePath() {
	return m_this;
}


bool ArchivePath::settupShadow() {
	return m_shadow.settup();
}

bool ArchivePath::settupRelative(std::string &yyyymmddStr) {

	std::string yearStr = yyyymmddStr.substr(0, 4);
	
	std::string workspaceYearPath = m_pathToWorkspace + '/' + yearStr;
	if (SAUtils::DirExists(workspaceYearPath.c_str()) == false) {
		SAUtils::mkDir(workspaceYearPath.c_str());
	}
	m_workspaceYyyymmddPath = workspaceYearPath + '/' + yyyymmddStr;

	if (SAUtils::DirExists(m_workspaceYyyymmddPath.c_str()) == false) {
		SAUtils::mkDir(m_workspaceYyyymmddPath.c_str());
	}
	m_workspaceMetadataPath = m_workspaceYyyymmddPath + "/.sia";
	if (SAUtils::DirExists(m_workspaceMetadataPath.c_str()) == false) {
		SAUtils::mkDir(m_workspaceMetadataPath.c_str());
	}
	return true;
}


	bool RepositoryPath::settup() {
		
		if (SAUtils::DirExists(m_pathToRepository.c_str()) == false) {
			return false;
		}

		std::string dataFolder = m_pathToRepository;
		dataFolder += "/system";
		if (SAUtils::DirExists(dataFolder.c_str()) == false) {
			SAUtils::mkDir(dataFolder.c_str());

		}
		if (m_metadataPath.empty() == true) {
			m_metadataPath = dataFolder + "/metadata";
			m_idxDBPath = dataFolder + "/imageid";
			m_journalPath = dataFolder + "/journal";
			m_historyLogPath = dataFolder + "/history";
			m_DBPath = dataFolder + "/db";
			m_ImageIndexPath = dataFolder + "/imageindex";
			if (SAUtils::DirExists(m_metadataPath.c_str()) == false) {
				SAUtils::mkDir(m_metadataPath.c_str());
			}
			if (SAUtils::DirExists(m_ImageIndexPath.c_str()) == false) {
				SAUtils::mkDir(m_ImageIndexPath.c_str());
			}
			if (SAUtils::DirExists(m_idxDBPath.c_str()) == false) {
				SAUtils::mkDir(m_idxDBPath.c_str());
			}

			if (SAUtils::DirExists(m_historyLogPath.c_str()) == false) {
				SAUtils::mkDir(m_historyLogPath.c_str());
			}
		}
		return true;
	}

	

	bool RepositoryPath::settupRelative(std::string &yyyymmddStr) {

		m_yearStr = yyyymmddStr.substr(0, 4);
		m_yyyymmddStr = yyyymmddStr;

		m_relpath = m_yearStr + '/' + yyyymmddStr;

		
		m_yearStrPath = m_pathToRepository + '/' + m_yearStr;
		if (SAUtils::DirExists(m_yearStrPath.c_str()) == false) {
			SAUtils::mkDir(m_yearStrPath.c_str());

		}

		m_yyyymmddStrPath = m_yearStrPath + '/' + m_yyyymmddStr;
		if (SAUtils::DirExists(m_yyyymmddStrPath.c_str()) == false) {
			SAUtils::mkDir(m_yyyymmddStrPath.c_str());

		}
		m_dataPath = m_yyyymmddStrPath + "/data";
		m_metadataPath = m_yyyymmddStrPath + "/metadata";
		m_historyPath = m_yyyymmddStrPath + "/history";
		

		if (SAUtils::DirExists(m_dataPath.c_str()) == false) {
			SAUtils::mkDir(m_dataPath.c_str());

		}
		if (SAUtils::DirExists(m_metadataPath.c_str()) == false) {
			SAUtils::mkDir(m_metadataPath.c_str());

		}
		if (SAUtils::DirExists(m_DBPath.c_str()) == false) {
			SAUtils::mkDir(m_DBPath.c_str());

		}
		if (SAUtils::DirExists(m_historyPath.c_str()) == false) {
			SAUtils::mkDir(m_historyPath.c_str());

		}
	
		
		//IntegrityManager &integrityManager = IntegrityManager::get();
		//integrityManager.addDayFolder(m_yyyymmddStr.c_str());
		return true;
	}





	bool RepositoryPath::isEnabled() {
		return m_enabled;
	}

	void RepositoryPath::setImageName(const char *imageName) {
		m_imageName = imageName;
	}

	const std::string& RepositoryPath::getImageName() const {
		return m_imageName;
	}

	const std::string& RepositoryPath::getRelativePath() const {
		return m_relpath;
	}

	std::string RepositoryPath::getDataPath() {
		return m_dataPath;
	}

std::string RepositoryPath::getCatalog() const {
	return m_catalog;
}

void RepositoryPath::setCatalog(std::string& catalog) {
	m_catalog = catalog;
}

std::string RepositoryPath::getHistory() const {
	return m_historyPath;
}

std::string RepositoryPath::getLogHistory() const {
	return m_historyLogPath;
}

std::string RepositoryPath::getCSVDatabasePath() const {
	return m_DBPath;
}

void RepositoryPath::setHistory(std::string& history) {
	m_historyPath = history;
}

std::string RepositoryPath::getJournalPath() const {
	return m_journalPath;
}

void RepositoryPath::setJournalPath(std::string& journalPath) {
	m_journalPath = journalPath;
}

std::string RepositoryPath::getMetadataPath() const {
	return m_metadataPath;
}

void RepositoryPath::setMetadataPath(std::string& metadataPath) {
	m_metadataPath = metadataPath;
}

std::string RepositoryPath::getIdxDBPath() const {
	return m_idxDBPath;
}

void RepositoryPath::setIdxDBPath(std::string& idxDBPath) {
	m_idxDBPath = idxDBPath;
}

std::string RepositoryPath::getImageIdxPath() const {
	return m_ImageIndexPath;
}

	void RepositoryPath::setEnabled(bool b) {
		m_enabled = b;
	}
	void RepositoryPath::setRepositoryPath(std::string &pathToBackup) {
		m_pathToRepository = pathToBackup;
		m_enabled = true;
	}
	std::string  RepositoryPath::getRepositoryPath() {
		return m_pathToRepository;
		
	}

std::string ArchivePath::m_pathToHome;
std::string ArchivePath::m_pathToWorkspace;
std::string ArchivePath::m_mainMetadataPath;
//	static std::string m_userDefinedSequenceNumberPath;
std::string ArchivePath::m_mainDupsPath;
std::string ArchivePath::m_mainHistory;
//static ArchivePath *m_shadow;
std::string ArchivePath::m_pathToShadow;

std::string ArchivePath::m_workspaceYyyymmddPath;
std::string ArchivePath::m_workspaceMetadataPath;

RepositoryPath ArchivePath::m_shadow;
RepositoryPath ArchivePath::m_backupPath[2];

std::string ArchivePath::m_backup1;
std::string ArchivePath::m_backup2;



std::string ArchivePath::getBackup1Path() {
	return m_backupPath[0].getRepositoryPath();
}
void ArchivePath::setBackup1Path(std::string &pathToBackup) {
	m_backupPath[0].setRepositoryPath(pathToBackup);
	m_backupPath[0].setEnabled(true);
}
std::string ArchivePath::getBackup2Path() {
	return m_backupPath[1].getRepositoryPath();
}
void ArchivePath::setBackup2Path(std::string &pathToBackup) {
	m_backupPath[1].setRepositoryPath(pathToBackup);
	m_backupPath[1].setEnabled(true);
}

bool ArchivePath::isBackup1Enabled() {
	return m_backupPath[0].isEnabled();
}
bool ArchivePath::isBackup2Enabled() {
	return m_backupPath[1].isEnabled();
}

bool ArchivePath::isShadowEnabled() {
	return m_shadow.isEnabled();
}


RepositoryPath& ArchivePath::getBackup1() {
	return m_backupPath[0];
}

RepositoryPath& ArchivePath::getBackup2() {
	return m_backupPath[1];
}

RepositoryPath& ArchivePath::getShadow() {
	return m_shadow;
}

std::string ArchivePath::getPathToHome() {
	return m_pathToHome;
}

void ArchivePath::setPathToHome(std::string &pathToHome) {
	m_pathToHome = pathToHome;
}

std::string ArchivePath::getMainDupsPath() {
	return m_mainDupsPath;
}

void ArchivePath::setMainDupsPath(std::string &mainDupsPath) {
	m_mainDupsPath = mainDupsPath;
}

std::string ArchivePath::getMainHistory() {
	return m_mainHistory;
}

void ArchivePath::setMainHistory(std::string &mainHistory) {
	m_mainHistory = mainHistory;
}

std::string ArchivePath::getMainMetadataPath() {
	return m_mainMetadataPath;
}

void ArchivePath::setMainMetadataPath(std::string &mainMetadataPath) {
	m_mainMetadataPath = mainMetadataPath;
}

std::string ArchivePath::getPathToShadow() {
	return m_shadow.getRepositoryPath();
}

void ArchivePath::setPathToShadow(std::string &pathToShadow) {
	m_shadow.setRepositoryPath(pathToShadow);
	m_shadow.setEnabled(true);
}
	

std::string ArchivePath::getPathToWorkspace() {
	return m_pathToWorkspace;
}

void ArchivePath::setPathToWorkspace(std::string &pathToWorkspace) {
	m_pathToWorkspace = pathToWorkspace;
}

std::string ArchivePath::getWorkspaceYyyymmddPath() {
	return m_workspaceYyyymmddPath;
}

std::string ArchivePath::getWorkspaceMetadataPath() {
	return m_workspaceMetadataPath;
}

/*
static ArchivePath* ArchivePath::getShadow() const {
	return m_shadow;
}

void ArchivePath::setShadow(static ArchivePath* shadow) {
	m_shadow = shadow;
}
*/

std::string ArchivePath::getShadowCatalog() {
	return m_shadow.getCatalog();
}

void ArchivePath::setShadowCatalog(std::string &shadowCatalog) {
	m_shadow.setCatalog(shadowCatalog);
}

std::string ArchivePath::getShadowHistory() {
	return m_shadow.getHistory();
}

void ArchivePath::setShadowHistory(std::string &shadowHistory) {
	m_shadow.setHistory(shadowHistory);
}

std::string ArchivePath::getShadowJournalPath() {
	return m_shadow.getJournalPath();
}

void ArchivePath::setShadowJournalPath(std::string &shadowJournalPath) {
	m_shadow.setJournalPath(shadowJournalPath);
}

std::string ArchivePath::getShadowMetadataPath() {
	return m_shadow.getMetadataPath();
}

void ArchivePath::setShadowMetadataPath(std::string &shadowMetadataPath) {
	m_shadow.setMetadataPath(shadowMetadataPath);
}

std::string ArchivePath::getIdxDBPath() {
	return m_shadow.getIdxDBPath();
}

void ArchivePath::setIdxDBPath(std::string &idxDBPath) {
	m_shadow.setIdxDBPath(idxDBPath);
}

} // simplearchive
