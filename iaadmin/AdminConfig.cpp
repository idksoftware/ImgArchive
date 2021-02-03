#include "AdminConfig.h"

#include "Quiet.h"
#include "ConfigReader.h"
#include "AdminAppOptions.h"
#include "ParseOptions.h"
#include "SetConfig.h"

using namespace CommandLineProcessing;

namespace simplearchive {

	bool AdminConfig::doCommand() {
		AppOptions& appOptions = AppOptions::get();
		if (getParser().foundOption("quiet") == true) {
			Quiet::setOn();
		}
		SetConfig setConfig;
		if (getParser().foundOption("general") == true) {
			std::string opt = getParser().optionValue("general");

			if (setConfig.parseGeneralOptions(opt.c_str()) == false) {
				ReturnCodeObject::setReturnString(NoArgumentForSubCommand, "Invalid argument for sub-command: %s general \"%s\"", getParser().getCurrentCommand().c_str(), opt.c_str());
				Quiet::printf("%s", getParser().topicUsageDescription(getParser().getCurrentCommandId(), 80).c_str());
				return false;
			}

		}
		else if (getParser().foundOption("logging") == true) {
			std::string opt = getParser().optionValue("logging");

			if (setConfig.parseLoggingOptions(opt.c_str()) == false) {
				ReturnCodeObject::setReturnString(NoArgumentForSubCommand, "Invalid argument for sub-command: %s logging \"%s\"", getParser().getCurrentCommand().c_str(), opt.c_str());
				Quiet::printf("%s", getParser().topicUsageDescription(getParser().getCurrentCommandId(), 80).c_str());
				return false;
			}

		}
		else if (getParser().foundOption("folders") == true) {
			std::string opt = getParser().optionValue("folders");

			if (setConfig.parseFolderOptions(opt.c_str()) == false) {
				ReturnCodeObject::setReturnString(NoArgumentForSubCommand, "Invalid argument for sub-command: %s folders \"%s\" %s", getParser().getCurrentCommand().c_str(), opt.c_str(), setConfig.errorString());
				Quiet::printf("%s", getParser().topicUsageDescription(getParser().getCurrentCommandId(), 80).c_str());
				return false;
			}

		}
		else if (getParser().foundOption("exiftool") == true) {
			std::string opt = getParser().optionValue("exiftool");

			if (setConfig.parseExifToolOptions(opt.c_str()) == false) {
				ReturnCodeObject::setReturnString(NoArgumentForSubCommand, "Invalid argument for sub-command: %s exiftool \"%s", getParser().getCurrentCommand().c_str(), opt.c_str());
				Quiet::printf("%s", getParser().topicUsageDescription(getParser().getCurrentCommandId(), 80).c_str());
				return false;
			}
		}
		else if (getParser().foundOption("master") == true) {
			std::string opt = getParser().optionValue("master");

			if (setConfig.parseMasterOptions(opt.c_str()) == false) {
				ReturnCodeObject::setReturnString(NoArgumentForSubCommand, "Invalid argument for sub-command: %s master \"%s\" %s", getParser().getCurrentCommand().c_str(), opt.c_str(), setConfig.errorString());
				Quiet::printf("%s", getParser().topicUsageDescription(getParser().getCurrentCommandId(), 80).c_str());
				return false;
			}

		}
		else if (getParser().foundOption("derivative") == true) {
			std::string opt = getParser().optionValue("derivative");

			if (setConfig.parseDerivativeOptions(opt.c_str()) == false) {
				ReturnCodeObject::setReturnString(NoArgumentForSubCommand, "Invalid argument for sub-command: %s derivative \"%s", getParser().getCurrentCommand().c_str(), opt.c_str());
				Quiet::printf("%s", getParser().topicUsageDescription(getParser().getCurrentCommandId(), 80).c_str());
				return false;
			}

		}
		else if (getParser().foundOption("backup") == true) {
			std::string opt = getParser().optionValue("backup");

			if (setConfig.parseBackupOptions(opt.c_str()) == false) {
				ReturnCodeObject::setReturnString(NoArgumentForSubCommand, "Invalid argument for sub-command: %s derivative \"%s\"\n\n", getParser().getCurrentCommand().c_str(), opt.c_str());
				Quiet::printf("%s", getParser().topicUsageDescription(getParser().getCurrentCommandId(), 80).c_str());
				return false;
			}

		}
		else if (getParser().foundOption("network") == true) {
			std::string opt = getParser().optionValue("network");

			if (setConfig.parseNetworkOptions(opt.c_str()) == false) {
				ReturnCodeObject::setReturnString(NoArgumentForSubCommand, "Invalid argument for sub-command: %s network \"%s\"\n\n", getParser().getCurrentCommand().c_str(), opt.c_str());
				Quiet::printf("%s", getParser().usageDescription(80).c_str());
				return false;
			}

		}
		else {
			ReturnCodeObject::setReturnString(NoArgumentForSubCommand, "No argument for sub-command: %s", getParser().getCurrentCommand().c_str());
			Quiet::printf("%s", getParser().topicUsageDescription(getParser().getCurrentCommandId(), 80).c_str());
			return false;
		}
		appOptions.setConfigOptionBlock(setConfig.getOptionBlock().c_str());
		appOptions.setConfigOption(setConfig.getOption().c_str());
		appOptions.setConfigValue(setConfig.getValue().c_str());
		appOptions.setCommandMode(AppOptions::CommandMode::CM_Config);
	
		return true;
	}
}