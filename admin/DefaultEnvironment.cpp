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
	NewInstallOptions NewInstallDefaultLocations::m_newInstallOptions;
	

	bool DefaultEnvironment::init()
	{
		//AppConfig &config = AppConfig::get();
		AppConfigBase config;
		m_configured = false;
		ImgArchiveHome::getObject();
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
	/*
	constexpr auto DEFAULT_ALLUSER_HOME_PATH = "/IDK-Software/ImgArchive";
constexpr auto DEFAULT_ALLUSER_WWWIMAGE_PATH = "/IDK-Software/ImgArchive/IAWWWPictures";
constexpr auto DEFAULT_ALLUSER_PICTURE_PATH = "/ImgArchive/Pictures";
constexpr auto DEFAULT_ALLUSER_WORKSPACE_PATH = "/ImgArchive/Workspace";
constexpr auto DEFAULT_ALLUSER_MASTER_PATH = "/IDK-Software/ImgArchive/master";
constexpr auto DEFAULT_ALLUSER_DERIVATIVE_PATH = "/IDK-Software/ImgArchive/derivative";
	*/
	bool NewInstallDefaultLocations::setAllUserDefaultLocations() {

		std::string imgArchiveHome = m_imgArchiveHome;
		if (m_type == HomePathType::LocalEnv || m_type == HomePathType::SystemEnv) {
			m_newInstallOptions.setHomePath(imgArchiveHome);
			m_newInstallOptions.setDerivativePath(imgArchiveHome + DEFAULT_LOCAL_DERIVATIVE_PATH);
			m_newInstallOptions.setMasterPath(imgArchiveHome + DEFAULT_ALLUSER_MASTER_PATH);
		}
		else {
			imgArchiveHome += DEFAULT_ALLUSER_HOME_PATH;;
			m_newInstallOptions.setHomePath(imgArchiveHome);
			m_newInstallOptions.setDerivativePath(imgArchiveHome + DEFAULT_ALLUSER_DERIVATIVE_PATH);
			m_newInstallOptions.setMasterPath(imgArchiveHome + DEFAULT_ALLUSER_MASTER_PATH);
		}


		std::string homeEnv = SAUtils::GetPOSIXEnv("HOMEDRIVE");
		std::string homePathEnv = SAUtils::GetPOSIXEnv("HOMEPATH");
		homeEnv += homePathEnv;
		//m_imgArchiveHome DEFAULT_LOCAL_HOME_PATH


		m_newInstallOptions.setPicturePath(homeEnv + DEFAULT_LOCAL_PICTURE_PATH);
		m_newInstallOptions.setWorkspacePath(homeEnv + DEFAULT_LOCAL_WORKSPACE_PATH);
		m_newInstallOptions.setWwwPath(homeEnv + DEFAULT_LOCAL_WWWIMAGE_PATH);

		return true;
	}

	bool NewInstallDefaultLocations::isConfigured() {

		std::string imgArchiveHome = m_imgArchiveHome;
		if (m_type != HomePathType::LocalEnv && m_type != HomePathType::SystemEnv) {
			imgArchiveHome += DEFAULT_LOCAL_HOME_PATH;
		}

		if (SAUtils::DirExists(imgArchiveHome.c_str())) {
			std::string configFilePath = imgArchiveHome + "/config/" + "config.dat";
			if (SAUtils::FileExists(configFilePath.c_str())) {
				return true;
			}
		}
		return false;
	}


	bool NewInstallDefaultLocations::setLocalDefaultLocations() {

		std::string imgArchiveHome = m_imgArchiveHome;
		if (m_type == HomePathType::LocalEnv || m_type == HomePathType::SystemEnv) {
			m_newInstallOptions.setHomePath(imgArchiveHome);
			m_newInstallOptions.setDerivativePath(imgArchiveHome + DEFAULT_LOCAL_DERIVATIVE_PATH);
			m_newInstallOptions.setMasterPath(imgArchiveHome + DEFAULT_LOCAL_MASTER_PATH);
		}
		else {
			imgArchiveHome += DEFAULT_LOCAL_HOME_PATH;
			m_newInstallOptions.setHomePath(imgArchiveHome);
			m_newInstallOptions.setDerivativePath(imgArchiveHome + DEFAULT_LOCAL_DERIVATIVE_PATH);
			m_newInstallOptions.setMasterPath(imgArchiveHome + DEFAULT_LOCAL_MASTER_PATH);
		}

		
		std::string homeEnv = SAUtils::GetPOSIXEnv("HOMEDRIVE");
		std::string homePathEnv = SAUtils::GetPOSIXEnv("HOMEPATH");
		homeEnv += homePathEnv;
		//m_imgArchiveHome DEFAULT_LOCAL_HOME_PATH
		
		
		m_newInstallOptions.setPicturePath(homeEnv + DEFAULT_LOCAL_PICTURE_PATH);
		m_newInstallOptions.setWorkspacePath(homeEnv + DEFAULT_LOCAL_WORKSPACE_PATH);
		m_newInstallOptions.setWwwPath(homeEnv + DEFAULT_LOCAL_WWWIMAGE_PATH);
			
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
#ifdef _WIN32
		std::string allUsersHomeEnvironmentPath = SAUtils::GetEnv(IMGARCHIVE_HOME, true);
		std::string myselfHomeEnvironmentPath = SAUtils::GetEnv(IMGARCHIVE_HOME, false);
#else
		std::string allUsersHomeEnvironmentPath = SAUtils::GetEnv(IMGARCHIVE_HOME);
		std::string myselfHomeEnvironmentPath = SAUtils::GetEnv(IMGARCHIVE_HOME);
#endif
#ifdef _WIN32
		// All Users
		std::string allusersHomeDefaultPath = SAUtils::GetPOSIXEnv("ProgramData");
		// Local
		std::string myselfHomeDefaultPath = SAUtils::GetPOSIXEnv("LOCALAPPDATA");
#else
		// All Users
		std::string allusersHomeDefaultPath = SAUtils::GetPOSIXEnv(LINUX_VAR_OPT);
		// Local
		std::string myselfHomeDefaultPath = SAUtils::GetPOSIXEnv("HOME");
		//myselfHomeDefaultPath += DEFAULT_LOCAL_HOME_PATH;
#endif
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
		switch (m_type) {
		case HomePathType::LocalEnv:	// Local Environment set
			break;
		case HomePathType::SystemEnv:	// System Environment set
			break;
		case HomePathType::UserOnly:	// user only archive
			
			break;
		case HomePathType::AllUsers:	// all users archive
			break;
		case HomePathType::Default:	// Default value
		case HomePathType::Unknown:
		default:
			return false;
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

std::string NewInstallOptions::getWwwPath() const {
	return m_wwwPath;
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

};
