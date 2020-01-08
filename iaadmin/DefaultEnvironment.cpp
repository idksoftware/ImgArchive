#include "DefaultEnvironment.h"
#include <string>
#include "SAUtils.h"
#include "AppConfig.h"
#include "ConfigReader.h"
#include "EnvFunc.h"
#include "AppBase.h"


namespace simplearchive {

	bool DefaultEnvironment::init()
	{
		//AppConfig &config = AppConfig::get();
		AppConfigBase config;
		m_configured = false;
		bool found = false;
		std::string homePath;

#ifdef WIN32
		// Find if the IMGARCHIVE_HOME pathe is in the windows registery 

		// Looking the HKEY_LOCAL_MACHINE first
		if (GetEnv(homePath, true) == true) {
			//printf("Found IMGARCHIVE_HOME in system variables: %s", homePath.c_str());
			found = true;
		}
		// Looking the HKEY_CURRENT_USER
		else if (GetEnv(homePath, false) == true) {
			//printf("Found IMGARCHIVE_HOME in user variables: %s", homePath.c_str());
			found = true;
		}
		else {
			homePath = SAUtils::GetPOSIXEnv("ProgramData");
			if (homePath.empty() == true || homePath.length() == 0) {
				printf("ImgArchiveUnable to start? Cannot read user profile.");
				
				return false;
			}
			else {
				homePath += DEFAULT_DATA_CONFIG_PATH;
				if (SAUtils::DirExists(homePath.c_str()) == true) {
					//printf("Found IMGARCHIVE_HOME in user profile: %s", homePath.c_str());
					found = true;
				}
			}
			if (found == false) {
				homePath = SAUtils::GetPOSIXEnv("USERPROFILE");
				if (homePath.empty() == true || homePath.length() == 0) {
					printf("ImgArchive Unable to start? Cannot read all users profile.");
					
					return false;
				}
				homePath += DEFAULT_DATA_CONFIG_PATH;
				if (SAUtils::DirExists(homePath.c_str()) == true) {
					//printf("Found IMGARCHIVE_HOME in all users profile: %s", homePath.c_str());
					found = true;
				}
			}
		}
#else
		// Under Linux look for the HKEY_LOCAL_MACHINE enviroment variable
		homePath = SAUtils::GetPOSIXEnv("IMGARCHIVE_HOME");
		if (homePath.empty() == true || homePath.length() == 0) {
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

		if (SAUtils::DirExists(homePath.c_str()) == false) {
			//setError(12, "ImgArchive Unable to start? Archive not found at default location and the environment variable IAHOME not set.\n"
			//	"Use siaadmin -i to create an empty archive at the default location (see documentation).\n");
			return false;

		}

		// try to set a systems temp folder 
		std::string tempPath = SAUtils::GetPOSIXEnv("TEMP");
		if (tempPath.empty() == true || tempPath.length() == 0) {
			tempPath = SAUtils::GetPOSIXEnv("TMP");
		}

		std::string configfile = homePath + "/config/" + "config.dat";
		std::string configPath = homePath + "/config";
		if (SAUtils::FileExists(configfile.c_str()) == true) {
			//setConfigPath(configPath.c_str());
			AppConfigReader configReader;
			configReader.setNoLogging();
			if (configReader.read(configfile.c_str(), config) == false) {
				//setError(13, "Error found at line %d in the configuration file.\n", configReader.getCurrentLineNumber());
				return false;
			}
			//config.fileBasedValues(homePath.c_str(), tempPath.c_str());
			m_configured = true;
		}


		return true;
	}

};
