#include "AdminShow.h"

#include "Quiet.h"
#include "ConfigReader.h"
#include "AdminAppOptions.h"
#include "ParseOptions.h"
#include "SetConfig.h"

using namespace CommandLineProcessing;

namespace simplearchive {

	bool AdminShow::doCommand() {
		AppOptions& appOptions = AppOptions::get();
		bool argFound = false;
		std::string subOption;
		appOptions.setCommandMode(AppOptions::CommandMode::CM_Show);
		/*
		 general logging network folders master derivative backup exiftool
		*/
		SetSettings setSettings;
		if (getParser().foundOption("setting") == true) {
			std::string opt = getParser().optionValue("setting");
			if (setSettings.parseSettingsOptions(getParser().optionValue("setting").c_str()) == true) {
				subOption = "setting";
				argFound = true;
			}
			if (getParser().foundOption("out") == true) {
				OutputType outputType;
				std::string outType = getParser().optionValue("out");
				if (outputType.parse(getParser().optionValue("out").c_str()) == false) {
					ReturnCodeObject::setReturnString(InvalidArgument, "Option for argument \"out\" for sub-command: %s is invalid: %s\n\n", getParser().getCurrentCommand().c_str(), getParser().optionValue("out").c_str());
					Quiet::printf("%s", getParser().topicUsageDescription(getParser().getCurrentCommandId(), 80).c_str());
					return false;
				}
				else {
					appOptions.m_textOutputType = getParser().optionValue("out");
				}
			}
			if (getParser().foundOption("file") == true) {
				appOptions.m_outputFile = getParser().optionValue("file");
			}
		}
		else if (getParser().foundOption("allowed") == true) {
			if (setSettings.parseAllowedOptions(getParser().optionValue("allowed").c_str()) == true) {
				subOption = "allowed";
				argFound = true;
			}
			if (getParser().foundOption("out") == true) {
				OutputType outputType;
				std::string outType = getParser().optionValue("out");
				if (outputType.parse(getParser().optionValue("out").c_str()) == false) {
					ReturnCodeObject::setReturnString(InvalidArgument, "Option for argument \"out\" for sub-command: %s is invalid: %s\n\n", getParser().getCurrentCommand().c_str(), getParser().optionValue("out").c_str());
					Quiet::printf("%s", getParser().topicUsageDescription(getParser().getCurrentCommandId(), 80).c_str());
					return false;
				}
				else {
					appOptions.m_textOutputType = getParser().optionValue("out");
				}
			}
			if (getParser().foundOption("file") == true) {
				appOptions.m_outputFile = getParser().optionValue("file");
			}
		}
		else if (getParser().foundOption("env") == true) {
			if (setSettings.parseEnvOptions(getParser().optionValue("env").c_str()) == true) {
				subOption = "env";
				argFound = true;
			}
			if (getParser().foundOption("out") == true) {
				OutputType outputType;
				std::string outType = getParser().optionValue("out");
				if (outputType.parse(getParser().optionValue("out").c_str()) == false) {
					ReturnCodeObject::setReturnString(InvalidArgument, "Option for argument \"out\" for sub-command: %s is invalid: %s\n\n", getParser().getCurrentCommand().c_str(), getParser().optionValue("out").c_str());
					Quiet::printf("%s", getParser().topicUsageDescription(getParser().getCurrentCommandId(), 80).c_str());
					return false;
				}
				else {
					appOptions.m_textOutputType = getParser().optionValue("out");
				}
			}
			if (getParser().foundOption("file") == true) {
				appOptions.m_outputFile = getParser().optionValue("file");
			}
		}


		if (argFound == false) {
			ReturnCodeObject::setReturnString(NoArgumentForSubCommand, "No argument for sub-command: %s\n\n", getParser().getCurrentCommand().c_str());
			Quiet::printf("%s", getParser().topicUsageDescription(getParser().getCurrentCommandId(), 80).c_str());
			return false;
		}

		if (argFound == true) {
			appOptions.setConfigOption(subOption.c_str());
			appOptions.setConfigValue(setSettings.getValue().c_str());
			
		}
		return true;
	}
}