#include "UpdateConfig.h"
#include <string>
#include "SAUtils.h"
#include "AppConfig.h"
#include "ConfigReader.h"
#include "EnvFunc.h"
#include "AppBase.h"

namespace simplearchive {

	bool UpdateConfig::read()
	{
		SIAARCConfig imgaConfig;
		/*
		const std::string key = "IMGARCHIVE_HOME";
		std::string temp = SAUtils::GetPOSIXEnv(key);
		std::string homePath = temp;
		*/
		bool found = false;
		std::string homePath;
		// Looking the HKEY_LOCAL_MACHINE first
		if (GetEnv(homePath, true) == true) {
			printf("Found IMGARCHIVE_HOME in system variables: %s", homePath.c_str());
			found = true;
		}
		else if (GetEnv(homePath, false) == true) {
			//printf("Found IMGARCHIVE_HOME in user variables: %s", homePath.c_str());
			found = true;
		}
		else {
			homePath = SAUtils::GetPOSIXEnv("USERPROFILE");
			if (homePath.empty() == true || homePath.length() == 0) {
				printf("ImgArchive Unable to start? Cannot read user profile.");
				return false;
			}
			else {
				homePath += DEFAULT_DATA_CONFIG_PATH;
				if (SAUtils::DirExists(homePath.c_str()) == true) {
					printf("Found IMGARCHIVE_HOME in user profile: %s", homePath.c_str());
					found = true;
				}
			}
			if (found == false) {
				homePath = SAUtils::GetPOSIXEnv("ProgramData");
				if (homePath.empty() == true || homePath.length() == 0) {
					printf("ImgArchive Unable to start? Cannot read all users profile.");
					return false;
				}
				homePath += DEFAULT_DATA_CONFIG_PATH;
				if (SAUtils::DirExists(homePath.c_str()) == true) {
					printf("Found IMGARCHIVE_HOME in all users profile: %s", homePath.c_str());
					found = true;
				}
			}
		}
		if (found == false) {
			printf("ImgArchive Unable to start? No archive found in the default location or"
				" the environment variable IMGARCHIVE_HOME not set.\nUse siaadmin to initalise an archive.\n");

			return false;
		}
		std::string configfile = homePath + "/config/" + "config.dat";
		if (SAUtils::FileExists(configfile.c_str()) == false) {

			printf("ImgArchive Unable to start? No config.dat file found in the default location or"
				" the environment variable IMGARCHIVE_HOME not set.\nUse siaadmin to initalise an archive.\n");
			return false;
		}
		AppConfigReader configReader;
		configReader.setNoLogging();
		configReader.read(configfile.c_str(), imgaConfig);
		imgaConfig.printAll();
		//ConfigWriter configWriter;
		//if (configWriter.write("C:\\Temp\\config.txt", imgaConfig) == false) {
		//	return false;
		//}
		return true;
	}
};