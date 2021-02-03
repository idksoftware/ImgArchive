#include "AdminAllow.h"

#include "Quiet.h"
#include "ConfigReader.h"
#include "AdminAppOptions.h"
#include "ParseOptions.h"
#include "SetImageExtentionFile.h"

using namespace CommandLineProcessing;

namespace simplearchive {

	bool AdminAllow::doCommand() {
		AppOptions& appOptions = AppOptions::get();
		std::string opt;
		std::string value;
		if (getParser().foundOption("add") == true) {
			opt = "add";
			value = getParser().optionValue("add");
			if (!SetImageExtentionFile::validateOptions(value.c_str())) {
				ReturnCodeObject::setReturnString(InvalidArgument, "Invalid argument for sub-command: %s \"%s=%s\"", getParser().getCurrentCommand().c_str(), opt.c_str(), value.c_str());
				Quiet::printf("%s", getParser().usageDescription(80).c_str());
				return false;
			}
		}
		else if (getParser().foundOption("edit") == true) {
			opt = "edit";
			value = getParser().optionValue("edit");
			if (!SetImageExtentionFile::validateOptions(value.c_str())) {
				ReturnCodeObject::setReturnString(InvalidArgument, "Invalid argument for sub-command: %s \"%s=%s\"", getParser().getCurrentCommand().c_str(), opt.c_str(), value.c_str());
				Quiet::printf("%s", getParser().usageDescription(80).c_str());
				return false;
			}
		}
		else if (getParser().foundOption("delete") == true) {
			opt = "delete";
			value = getParser().optionValue("delete");
		}
		else {
			ReturnCodeObject::setReturnString(InvalidArgument, "Invalid argument for sub-command: %s \"%s\"", getParser().getCurrentCommand().c_str(), opt.c_str());
			Quiet::printf("%s", getParser().usageDescription(80).c_str());
			return false;
		}

		appOptions.setConfigOption(opt.c_str());
		appOptions.setConfigValue(value.c_str());
		appOptions.setCommandMode(AppOptions::CommandMode::CM_Allow);
		
		return true;	
	}
}