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

	bool DefaultEnvironment::setNewInstallAllUserDefaultLocations() {
		return locations(ImgArchiveHome::getImgArchiveHome().c_str());
	}

	bool DefaultEnvironment::setNewInstallLocalDefaultLocations() {
		return locations(ImgArchiveHome::getImgArchiveHome().c_str());
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

	bool DefaultEnvironment::setNewInstallDefaultLocations() {
		if (DefaultEnvironment::isInAdminMode()) {
			DefaultEnvironment::setNewInstallAllUserDefaultLocations();
		}
		else {
			DefaultEnvironment::setNewInstallLocalDefaultLocations();
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
};
