#include "DefaultEnvironment.h"
#include <string>
#include "SAUtils.h"
#include "AppConfig.h"
#include "ConfigReader.h"
//#include "EnvFunc.h"
//#include "AppBase.h"
#include "AppPaths.h"
#include "HomePaths.h"
#ifdef _WIN32
#include <Windows.h>



bool IsAdminMode() {
	bool fRet = false;
	HANDLE hToken = NULL;
	if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
		TOKEN_ELEVATION Elevation;
		DWORD cbSize = sizeof(TOKEN_ELEVATION);
		if (GetTokenInformation(hToken, TokenElevation, &Elevation, sizeof(Elevation), &cbSize)) {
			fRet = Elevation.TokenIsElevated;
		}
	}
	if (hToken) {
		CloseHandle(hToken);
	}
	return fRet;
}
#endif
namespace simplearchive {

	bool DefaultEnvironment::m_configured = false;
	bool DefaultEnvironment::m_isInAdminMode = false;
	bool DefaultEnvironment::m_isHomePathSet = false;
	bool DefaultEnvironment::m_isHomePathValid = false;

	

	bool DefaultEnvironment::init()
	{
		//AppConfig &config = AppConfig::get();
		AppConfigBase config;
		m_configured = false;
		

		// try to set a systems temp folder 
		std::string tempPath = SAUtils::GetPOSIXEnv("TEMP");
		if (tempPath.empty() == true || tempPath.length() == 0) {
			tempPath = SAUtils::GetPOSIXEnv("TMP");
		}
		std::string homePath = ImgArchiveHome::getImgArchiveHome();
		std::string configfile = homePath + "/config/" + "config.dat";
		std::string configPath = homePath + "/config";
		if (SAUtils::FileExists(configfile.c_str()) == true) {
			m_configured = true;
		}
		else {
			m_configured = false;
		}
		return true;
	}

	

	bool NewInstallDefaultLocations::setAllUserDefaultLocations() {
		ImgArchiveHome::getImgArchiveHome().c_str();
		return true;
	}

	bool NewInstallDefaultLocations::setLocalDefaultLocations() {
		ImgArchiveHome::getImgArchiveHome().c_str();
		return true;
	}

	

	bool DefaultEnvironment::setAllUserDefaultLocations() {
		return locations(ImgArchiveHome::getImgArchiveHome().c_str());
	}

	bool DefaultEnvironment::setLocalDefaultLocations() {
		return locations(ImgArchiveHome::getImgArchiveHome().c_str());
	}

	bool DefaultEnvironment::locations(const char *home) {
		
		std::string homePath = home;
		// Repository Archive Path
		//m_historyPath = HomePath::get() + HISTORY_PATH;
		return false;
	}

	bool NewInstallDefaultLocations::m_configured = false;
	bool NewInstallDefaultLocations::m_isInAdminMode = false;
	bool NewInstallDefaultLocations::m_isHomePathSet = false;
	bool NewInstallDefaultLocations::m_isHomePathValid = false;
	std::string NewInstallDefaultLocations::m_imgArchiveHome;
	NewInstallDefaultLocations::HomePathType NewInstallDefaultLocations::m_type = NewInstallDefaultLocations::HomePathType::Unknown;
	bool NewInstallDefaultLocations::m_found = false;

	bool NewInstallDefaultLocations::init() {

		// Set Windows Defaults (they can be overridden later)
		std::string allUsersHomeEnvironmentPath = SAUtils::GetEnv(IMGARCHIVE_HOME, true);
		std::string myselfHomeEnvironmentPath = SAUtils::GetEnv(IMGARCHIVE_HOME, false);
		// All Users
		std::string allusersHomeDefaultPath = SAUtils::GetPOSIXEnv("ProgramData");
		allusersHomeDefaultPath += DEFAULT_ALLUSER_HOME_PATH;
		std::string myselfHomeDefaultPath = SAUtils::GetPOSIXEnv("LOCALAPPDATA");
		myselfHomeDefaultPath += DEFAULT_LOCAL_HOME_PATH;
		// Looking the HKEY_LOCAL_MACHINE first
		if (allUsersHomeEnvironmentPath.empty() == false) {
			if (SAUtils::IsAdminMode() == true) {
				// Found the allUsersHomeEnvironmentPath but if not in admin mode then cannot be used
				m_type = HomePathType::SystemEnv;	// System Environment set
				m_found = true;
				m_imgArchiveHome = allUsersHomeEnvironmentPath;
			}
			else {
				if (myselfHomeEnvironmentPath.empty() == false) {
					m_type = HomePathType::LocalEnv;
					m_found = true;
					m_imgArchiveHome = myselfHomeEnvironmentPath;
				}
			}
		}
		else if (myselfHomeEnvironmentPath.empty() == false) {
			m_type = HomePathType::LocalEnv;
			m_found = true;
			m_imgArchiveHome = myselfHomeEnvironmentPath;
		}
		if (m_found == false) {
			// Not found as a Home Environment Path, So find the defaults
			if (SAUtils::IsAdminMode() == true) {
				if (SAUtils::DirExists(allusersHomeDefaultPath.c_str()) == true) {
					m_imgArchiveHome = allusersHomeDefaultPath;
					m_type = HomePathType::AllUsers;
					m_found = true;

				}
				else if (SAUtils::DirExists(myselfHomeDefaultPath.c_str()) == true) {
					m_imgArchiveHome = myselfHomeDefaultPath;
					m_type = HomePathType::UserOnly;
					m_found = true;
				}
				else {
					return false;
				}
			}
			else {
				if (SAUtils::DirExists(myselfHomeDefaultPath.c_str()) == true) {
					m_imgArchiveHome = myselfHomeDefaultPath;
					m_type = HomePathType::UserOnly;
					m_found = true;
				}
				else {
					return false;
				}
			}
		}
		return true;
	}

	bool NewInstallDefaultLocations::setDefaultLocations() {
		if (DefaultEnvironment::isInAdminMode()) {
			setAllUserDefaultLocations();
		}
		else {
			setLocalDefaultLocations();
		}
		return true;
	}

	bool DefaultEnvironment::setDefaultLocations() {
		if (DefaultEnvironment::isInAdminMode()) {
			DefaultEnvironment::setAllUserDefaultLocations();
		}
		else {
			DefaultEnvironment::setLocalDefaultLocations();
		}
		return true;
	}

std::string NewInstallOptions::getDerivativePath() const {
	return m_derivativePath;
}

void NewInstallOptions::setDerivativePath(std::string mDerivativePath) {
	m_derivativePath = mDerivativePath;
}

std::string NewInstallOptions::getHomePath() const {
	return m_homePath;
}

void NewInstallOptions::setHomePath(std::string mHomePath) {
	m_homePath = mHomePath;
}

std::string NewInstallOptions::getMasterPath() const {
	return m_masterPath;
}

void NewInstallOptions::setMasterPath(std::string mMasterPath) {
	m_masterPath = mMasterPath;
}

std::string NewInstallOptions::getPicturePath() const {
	return m_picturePath;
}

void NewInstallOptions::setPicturePath(std::string mPicturePath) {
	m_picturePath = mPicturePath;
}

std::string NewInstallOptions::getWorkspacePath() const {
	return m_workspacePath;
}

void NewInstallOptions::setWorkspacePath(std::string mWorkspacePath) {
	m_workspacePath = mWorkspacePath;
}

void NewInstallOptions::setWwwPath(std::string mWwwPath) {
	m_wwwPath = mWwwPath;
}

void NewInstallDefaultLocations::setDerivativePath(std::string mDerivativePath) {
	m_newInstallOptions.setDerivativePath(mDerivativePath);
}

void NewInstallDefaultLocations::setHomePath(std::string mHomePath) {
	m_newInstallOptions.setHomePath(mHomePath);
}

void NewInstallDefaultLocations::setMasterPath(std::string mMasterPath) {
	m_newInstallOptions.setMasterPath(mMasterPath);
}

void NewInstallDefaultLocations::setPicturePath(std::string mPicturePath) {
	m_newInstallOptions.setPicturePath(mPicturePath);
}

void NewInstallDefaultLocations::setWorkspacePath(std::string mWorkspacePath) {
	m_newInstallOptions.setWorkspacePath(mWorkspacePath);
}

void NewInstallDefaultLocations::setWwwPath(std::string mWwwPath) {
	m_newInstallOptions.setWwwPath(mWwwPath);
}

}
;
