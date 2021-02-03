#include "AdminSetenv.h"

#include "Quiet.h"
#include "ConfigReader.h"
#include "AdminAppOptions.h"
#include "SAUtils.h"
#include "SetEnviromentVariables.h"

using namespace CommandLineProcessing;

namespace simplearchive {

	bool AdminSetenv::doCommand() {
		AppOptions& appOptions = AppOptions::get();
		appOptions.setCommandMode(AppOptions::CommandMode::CM_Setenv);
		SetEnviromentVariables setEnviromentVariables;
		if (getParser().foundOption("users") == true) {

			std::string users = getParser().optionValue("users");
			switch (setEnviromentVariables.userMode(users.c_str())) {
			case UserMode::All:
				if (SAUtils::IsAdminMode() == false) {
					// Not in admin mode so cannot be initalised in admin mode so return with error
					ReturnCodeObject::setReturnString("Invalid operation? Not in admin mode so cannot be initalised in admin mode");
					Quiet::printf("%s", getParser().usageDescription(80).c_str());
					return false;
				}
				break;
			case UserMode::Invalid:
				ReturnCodeObject::setReturnString("User mode Invalid? Not a user mode");
				Quiet::printf("%s", getParser().topicUsageDescription(getParser().getCurrentCommandId(), 80).c_str());
				return false;
			}
		}
		else {
			setEnviromentVariables.userMode();
		}

		if (getParser().foundOption("folders") == true) {
			std::string opt = getParser().optionValue("folders");

			if (setEnviromentVariables.parseFolders(opt.c_str()) == false) {
				//ReturnCodeObject::setReturnString("Invalid argument for sub-command: %s folders \"%s\" %s\n\n", getCurrentCommand().c_str(), opt.c_str(), setConfig.errorString());
				ReturnCodeObject::setReturnString("Invalid argument for sub-command: %s folders \"%s\"", getParser().getCurrentCommand().c_str(), opt.c_str());
				Quiet::printf("%s", getParser().topicUsageDescription(getParser().getCurrentCommandId(), 80).c_str());
				return false;
			}
			if (setEnviromentVariables.setVariable() == false) {
				ReturnCodeObject::setReturnString("Unable to set enviroment variable\n");
				Quiet::printf("%s", getParser().topicUsageDescription(getParser().getCurrentCommandId(), 80).c_str());
				return false;
			}
			Quiet::printf("Set enviroment variable\n");

		}
		else if (getParser().foundOption("enable") == true) {
			std::string opt = getParser().optionValue("enable");

			if (setEnviromentVariables.parseEnableOptions(opt.c_str()) == false) {
				ReturnCodeObject::setReturnString("Invalid argument for sub-command: %s enable \"%s\"\n\n", getParser().getCurrentCommand().c_str(), opt.c_str());
				Quiet::printf("%s", getParser().topicUsageDescription(getParser().getCurrentCommandId(), 80).c_str());
				return false;
			}
			else {
				if (setEnviromentVariables.setVariable() == false) {
					ReturnCodeObject::setReturnString("Unable to set enviroment variable\n");
					Quiet::printf("%s", getParser().topicUsageDescription(getParser().getCurrentCommandId(), 80).c_str());
					return false;
				}
				Quiet::printf("Set enviroment variable\n");
			}
		}
		else if (getParser().foundOption("disable") == true) {
			std::string opt = getParser().optionValue("disabled");

			if (setEnviromentVariables.parseEnableOptions(opt.c_str()) == false) {
				ReturnCodeObject::setReturnString(InvalidArgument, "Invalid argument for sub-command: %s disabled \"%s\"\n\n", getParser().getCurrentCommand().c_str(), opt.c_str());
				Quiet::printf("%s", getParser().topicUsageDescription(getParser().getCurrentCommandId(), 80).c_str());
				return false;
			}
			else {
				if (setEnviromentVariables.setVariable() == false) {
					ReturnCodeObject::setReturnString("Unable to set enviroment variable\n");
					Quiet::printf("%s", getParser().topicUsageDescription(getParser().getCurrentCommandId(), 80).c_str());
					return false;
				}
				Quiet::printf("Set enviroment variable\n");
				
				return true;
			}
		}
		else {
			ReturnCodeObject::setReturnString("No argument for sub-command: %s\n", getParser().getCurrentCommand().c_str());
			Quiet::printf("%s", getParser().topicUsageDescription(getParser().getCurrentCommandId(), 80).c_str());
			return false;
		}
		//appOptions.setConfigOption(parseEnableOptions.getOption().c_str());
		//appOptions.setConfigValue(parseEnableOptions.getValue().c_str());

		
		
		return true;
	}
}