#include "IaarcTemplate.h"

#include "Quiet.h"
#include "ConfigReader.h"
#include "IaarcAppOptions.h"
#include "IAParseOptions.h"

using namespace CommandLineProcessing;

namespace simplearchive {

	bool IaarcTemplate::doCommand() {
		IaarcAppOptions& appOptions = IaarcAppOptions::get();
		IAParseOptions iaParseOptions;
		bool res = false;
		std::string opt;
		appOptions.m_current = true;
		appOptions.m_master = true;
		if (getParser().foundOption("current") == true) {
			std::string opt = getParser().optionValue("current");
			res = iaParseOptions.parseCurrentOptions(opt.c_str());
			if (!res) {
				printf("Invalid argument for \"current\" \"%s\"\n\n", opt.c_str());
				printf("%s", getParser().topicUsageDescription(getParser().getCurrentCommandId(), 80).c_str());
				return false;
			}
			appOptions.m_current = iaParseOptions.isCurrent();
		}
		if (getParser().foundOption("archive") == true) {
			std::string opt = getParser().optionValue("archive");
			res = iaParseOptions.parseArchiveOptions(opt.c_str());
			if (!res) {
				printf("Invalid argument for \"archive\" \"%s\"\n\n", opt.c_str());
				printf("%s", getParser().topicUsageDescription(getParser().getCurrentCommandId(), 80).c_str());
				return false;
			}
			appOptions.m_master = iaParseOptions.isMaster();

		}
		if (getParser().foundOption("option") == true) {
			std::string opt = getParser().optionValue("option");
			//printf("option string %s\n", opt.c_str());
			appOptions.m_option = opt;
		}
		appOptions.setCommandMode(IaarcAppOptions::CommandMode::CM_Template);
		return true;
	}
}