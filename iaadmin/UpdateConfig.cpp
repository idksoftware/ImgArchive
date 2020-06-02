#include "UpdateConfig.h"
#include <string>
#include <iostream>
#include <fstream>
#include "SAUtils.h"
#include "AppConfig.h"
#include "ConfigReader.h"
#include "EnvFunc.h"
#include "AppBase.h"
#include "HomePath.h"

namespace simplearchive {

	bool UpdateConfig::read(AppConfigBase&imgaConfig)
	{
		
		/*
		const std::string key = "IMGARCHIVE_HOME";
		std::string temp = SAUtils::GetPOSIXEnv(key);
		std::string homePath = temp;
		*/
		
		
		bool res = HomePath::init();
		HomePathType homePathType = HomePath::type();
		bool found = HomePath::isFound();

		switch (homePathType) {
		case HomePathType::LocalEnv:	// Local Environment set
			if (res == false) {
				//printf("Found IMGARCHIVE_HOME as system profile: %s but archive not found at loacation", HomePath::get().c_str());
				return false;
			}
			//printf("Found IMGARCHIVE_HOME as system profile: %s. Archive found at that loacation", HomePath::get().c_str());
			break;
		case HomePathType::SystemEnv:	// System Environment set
			if (res == false) {
				//printf("Found IMGARCHIVE_HOME as system profile: %s. Archive found at that loacation", HomePath::get().c_str());
				return false;
			}
			break;
		case HomePathType::UserOnly:	// user only archive
			if (res == false) {
				//printf("Archive not found at default loacation");
				return false;
			}
			//printf("Archive found at default user loacation: %s.", HomePath::get().c_str());
			break;
		case HomePathType::AllUsers:	// all users archive
			if (res == false) {
				//printf("Archive not found at default loacation");
				return false;
			}
			//printf("Archive found at default system loacation: %s.", HomePath::get().c_str());
			break;
		case HomePathType::Unknown:
		default:
			//printf("Unknown error");
			return false;
		}
		std::string homePath = HomePath::get();
		if (found == false) {
			//printf("ImgArchive Unable to start? No archive found in the default location or"
			//	" the environment variable IMGARCHIVE_HOME not set.\nUse siaadmin to initalise an archive.\n");

			return false;
		}
		m_configfile = homePath + "/config/" + "config.dat";
		if (SAUtils::FileExists(m_configfile.c_str()) == false) {

			//printf("ImgArchive Unable to start? No config.dat file found in the default location or"
			//	" the environment variable IMGARCHIVE_HOME not set.\nUse siaadmin to initalise an archive.\n");
			return false;
		}
		AppConfigReader configReader;
		configReader.setNoLogging();
		configReader.read(m_configfile.c_str(), imgaConfig);
		
		
		return true;
	}
	bool UpdateConfig::update(const char* configOptionBlock, const char* configOption, const char* configValue)
	{
		SIAARCConfig imgaConfig;
		if (read(imgaConfig) == false) {
			return false;
		}
		AppConfigBaseWriter configWriter(imgaConfig);
		configWriter.update(configOptionBlock, configOption, configValue);
		imgaConfig.printAll();
		std::ofstream configFile;
		configFile.open(m_configfile.c_str());
		//configFile.open("C:\\temp\\t.dat");
		if (configFile.bad()) {
			return false;
		}
		configFile << imgaConfig;
		std::cout << imgaConfig;
		configFile.close();
		
		return true;
	}
};