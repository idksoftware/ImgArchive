#include "AdminInit.h"

#include "Quiet.h"
#include "ConfigReader.h"
#include "AdminAppOptions.h"
#include "ParseOptions.h"
#include "SAUtils.h"
#include "DefaultEnvironment.h"

using namespace CommandLineProcessing;

namespace simplearchive {

	bool AdminInit::doCommand() {
		AppOptions& appOptions = AppOptions::get();
		// This command will initalise the configuration.
			// so the the configuration need not to be initalised.
		bool isForced = false;

		if (getParser().foundOption("force") == true) {
			isForced = true;
		}
		appOptions.setCommandMode(AppOptions::CommandMode::CM_InitArchive);
		appOptions.m_users = SAUtils::IsAdminMode();
		if (NewInstallDefaultLocations::init() == false) {
			ReturnCodeObject::setReturnString("A root folder for the instalation cannot be established.");
			return false;
		}

		if (NewInstallDefaultLocations::isConfigured() == true) {
			if (isForced == false) {
				ReturnCodeObject::setReturnString("\nInit failed? ImgArchive is configured. Use fource to override current configuration.");
				return false;
			}
		}

		appOptions.m_configured = false;
		std::string opt;
		if (getParser().foundOption("users") == true) {
			std::string users = getParser().optionValue("users");
			if (users.compare("local") == 0) {
				appOptions.setAllUsers(false);
				NewInstallDefaultLocations::setLocalDefaultLocations();
			}
			else if (users.compare("all") == 0) {
				if (SAUtils::IsAdminMode() == false) {
					// Not in admin mode so cannot be initalised in admin mode so return with error
					ReturnCodeObject::setReturnString("Invalid operation? Not in admin mode so cannot be initalised in admin mode.");
					Quiet::printf("%s", getParser().usageDescription(80).c_str());
					return false;
				}
				appOptions.setAllUsers(true);
				NewInstallDefaultLocations::setAllUserDefaultLocations();
			}
			else {
				// Invalid option
				ReturnCodeObject::setReturnString("%s", getParser().topicUsageDescription(getParser().getCurrentCommandId(), 80).c_str());
				return false;
			}
		}
		else {
			// users not set so deside which to install myself or all
			if (SAUtils::IsAdminMode() == false) {
				// Not in admin mode so install in myself mode
				appOptions.setAllUsers(false);
				NewInstallDefaultLocations::setLocalDefaultLocations();
			}
			else {
				// In admin mode so install in all users mode
				appOptions.setAllUsers(true);
				NewInstallDefaultLocations::setAllUserDefaultLocations();
			}
		}
		BoolOption setHomeEnv = BoolOption::Invalid;
		if (getParser().foundOption("set-home") == true) {
			std::string value = getParser().optionValue("set-home");
			if ((setHomeEnv = SAUtils::isTrueFalse(value)) == BoolOption::Invalid) {
				Quiet::printf("%s", getParser().topicUsageDescription(getParser().getCurrentCommandId(), 80).c_str());
				return false;
			}
			bool homeEnv = (setHomeEnv == BoolOption::True) ? true : false;
			appOptions.setHomeEnv(homeEnv);
		}
		NewInstallOptions& newInstallOptions = NewInstallDefaultLocations::getOptions();

		AppOptions::m_homePath = newInstallOptions.getHomePath();
		AppOptions::m_workspacePath = newInstallOptions.getWorkspacePath();
		AppOptions::m_masterPath = newInstallOptions.getMasterPath();
		AppOptions::m_configPath = newInstallOptions.getHomePath();
		AppOptions::m_derivativePath = newInstallOptions.getDerivativePath();
		AppOptions::m_wwwImagePath = newInstallOptions.getWwwPath();
		AppOptions::m_catalogPath = newInstallOptions.getPicturePath();

		if (getParser().foundOption("archive-path") == true) {
			opt = getParser().optionValue("archive-path");
			appOptions.setHomePath(opt.c_str());
		}

		if (getParser().foundOption("workspace-path") == true) {
			opt = getParser().optionValue("workspace-path");
			appOptions.setWorkspacePath(opt.c_str());
		}

		if (getParser().foundOption("master-path") == true) {
			opt = getParser().optionValue("master-path");
			appOptions.setMasterPath(opt.c_str());
		}

		if (getParser().foundOption("derivative-path") == true) {
			opt = getParser().optionValue("derivative-path");
			appOptions.setDerivativePath(opt.c_str());
		}

		//if (foundOption("picture-path") == true) {
		//	opt = optionValue("picture-path");
		//	appOptions.setCataloguePath(opt.c_str());
		//}

		if (getParser().foundOption("picture-path") == true) {
			opt = getParser().optionValue("picture-path");
			appOptions.setPicturePath(opt.c_str());
		}

		if (getParser().foundOption("www-image-path") == true) {
			opt = getParser().optionValue("www-image-path");
			appOptions.setWWWImagePath(opt.c_str());
		}
		return true;
	}
}