#include "DefaultEnvironment.h"
#include <string>
#include "SAUtils.h"
#include "AppConfig.h"
#include "ConfigReader.h"
#include "EnvFunc.h"
#include "AppBase.h"

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

	bool DefaultEnvironment::init()
	{
		//AppConfig &config = AppConfig::get();
		AppConfigBase config;
		m_configured = false;
		bool found = false;
		
		m_isInAdminMode = IsAdminMode();
#ifdef WIN32
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
				printf("ImgArchiveUnable to start? Cannot read user profile.");
				
				return false;
			}
			else {
				m_homePath += DEFAULT_DATA_CONFIG_PATH;
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

};
