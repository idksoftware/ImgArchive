#include "DefaultEnvironment.h"
#include <string>
#include "SAUtils.h"
#include "AppConfig.h"
#include "ConfigReader.h"
#include "EnvFunc.h"
#include "AppBase.h"
#include "AppPaths.h"
#include "HomePath.h"
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

	std::string DefaultEnvironment::m_homePath;
	std::string DefaultEnvironment::m_picturesPath;
	std::string DefaultEnvironment::m_archivePath;
	std::string DefaultEnvironment::m_workspacePath;
	std::string DefaultEnvironment::m_masterPath;
	std::string DefaultEnvironment::m_derivativePath;
	std::string DefaultEnvironment::m_cataloguePath;
	std::string DefaultEnvironment::m_wwwCataloguePath;
	std::string DefaultEnvironment::m_historyPath;

	bool DefaultEnvironment::init()
	{
		//AppConfig &config = AppConfig::get();
		AppConfigBase config;
		m_configured = false;
		

		DefaultEnvironment::m_isInAdminMode = SAUtils::IsAdminMode();
		HomePath::init();
		bool isFound = HomePath::isFound();
		if (isFound) {
			m_homePath = HomePath::get();
		}
		else {
			m_configured = false;
			return true;
		}
#ifdef XXXXXXXXXXXXXXX
#ifdef WIN32
		bool found = false;
		// Find if the IMGARCHIVE_HOME pathe is in the windows registery 

		// Looking the HKEY_LOCAL_MACHINE first
		if (GetEnv(m_homePath, true) == true) {
			//printf("Found IMGARCHIVE_HOME in system variables: %s", homePath.c_str());
			found = true;
		}
		// Looking the HKEY_CURRENT_USER
		else if (GetEnv(m_homePath, false) == true) {
			//printf("Found IMGARCHIVE_HOME in user variables: %s", homePath.c_str());
			found = true;
		}
		else {
			m_homePath = SAUtils::GetPOSIXEnv("ProgramData");
			if (m_homePath.empty() == true || m_homePath.length() == 0) {
				printf("ImgArchive Unable to start? Cannot read Program Data.");
				return false;
			}
			else {
				m_homePath += ALLUSERS_DEFAULT_CONFIG_PATH;
				if (SAUtils::DirExists(m_homePath.c_str()) == true) {
					//printf("Found IMGARCHIVE_HOME in user profile: %s", homePath.c_str());
					found = true;
				}
			}
			if (found == false) {
				m_homePath = SAUtils::GetPOSIXEnv("USERPROFILE");
				if (m_homePath.empty() == true || m_homePath.length() == 0) {
					printf("ImgArchive Unable to start? Cannot read all users profile.");
					
					return false;
				}
				m_homePath += DEFAULT_DATA_CONFIG_PATH;
				if (SAUtils::DirExists(m_homePath.c_str()) == true) {
					//printf("Found IMGARCHIVE_HOME in all users profile: %s", homePath.c_str());
					found = true;
				}
			}
		}
#else
		// Under Linux look for the HKEY_LOCAL_MACHINE enviroment variable
		m_homePath = SAUtils::GetPOSIXEnv("IMGARCHIVE_HOME");
		if (m_homePath.empty() == true || homePath.length() == 0) {
			printf("ImgArchive Unable to start? Cannot read user profile.");
			setError(12, "ImgArchive Unable to start? Cannot read user profile.");
			return false;
		}
		found = true;
#endif
		if (found) {
			// Initalise without the config file i.e. set defaults.
			//if (config.init(homePath.c_str()) == false) {
				
			//	return false;
			//}
		}
		else {
			//if (config.init() == false) {
			//	//setError(12, "Cannot find home path? exiting?");
			//	return false;
			//}
		}

		if (SAUtils::DirExists(m_homePath.c_str()) == false) {
			//setError(12, "ImgArchive Unable to start? Archive not found at default location and the environment variable IAHOME not set.\n"
			//	"Use siaadmin -i to create an empty archive at the default location (see documentation).\n");
			m_isHomePathSet = true;
			m_configured = false;
			return true;

		}
#endif
		// try to set a systems temp folder 
		std::string tempPath = SAUtils::GetPOSIXEnv("TEMP");
		if (tempPath.empty() == true || tempPath.length() == 0) {
			tempPath = SAUtils::GetPOSIXEnv("TMP");
		}

		std::string configfile = m_homePath + "/config/" + "config.dat";
		std::string configPath = m_homePath + "/config";
		if (SAUtils::FileExists(configfile.c_str()) == true) {
			m_configured = true;
		}
		m_configured = false;

		return true;
	}
	bool DefaultEnvironment::setAllUserDefaultLocations() {
		HomePath::setAllUserDefaultHome();
		m_homePath = HomePath::get();

		MasterPath::setAllUserDefaultHome();
		m_masterPath = MasterPath::get();
		DerivativePath::setAllUserDefaultHome();
		m_derivativePath = DerivativePath::get();

		WorkspacePath::setAllUserDefaultHome();
		m_workspacePath = WorkspacePath::get();

		PicturePath::setAllUserDefaultHome();
		m_cataloguePath = PicturePath::get();
		WWWImagePath::setAllUserDefaultHome();
		m_wwwCataloguePath = WWWImagePath::get();

		return locations(m_homePath.c_str());
	}

	bool DefaultEnvironment::setLocalDefaultLocations() {

		HomePath::setLocalUserDefaultHome();
		m_homePath = HomePath::get();

		MasterPath::setLocalUserDefaultHome();
		m_masterPath = MasterPath::get();
		DerivativePath::setLocalUserDefaultHome();
		m_derivativePath = DerivativePath::get();

		WorkspacePath::setLocalUserDefaultHome();
		m_workspacePath = WorkspacePath::get();

		PicturePath::setLocalUserDefaultHome();
		m_cataloguePath = PicturePath::get();
		WWWImagePath::setLocalUserDefaultHome();
		m_wwwCataloguePath = WWWImagePath::get();
		
		return locations(m_homePath.c_str());
	}

	bool DefaultEnvironment::locations(const char *home) {
		
		std::string homePath = home;
		// Repository Archive Path
		m_historyPath = homePath + HISTORY_PATH;
		return false;
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
};
