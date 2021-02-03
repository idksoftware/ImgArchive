#include "IaarcProp.h"

#include "Quiet.h"
#include "ConfigReader.h"
#include "IaarcAppOptions.h"
#include "ParseOptions.h"
#include "ParseProperties.h"

using namespace CommandLineProcessing;

namespace simplearchive {

	bool IaarcProp::doCommand() {
		IaarcAppOptions& appOptions = IaarcAppOptions::get();
		if (getParser().foundOption("scope") == true) {
			appOptions.m_imageAddress = getParser().optionValue("scope");
		}
		if (getParser().foundOption("set") == true) {
			std::string opt = getParser().optionValue("set");
			ParseProperties parseProperties;
			if (!parseProperties.parse(opt.c_str())) {
				printf("Invalid argument for \"archive\" \"%s\"\n\n", opt.c_str());
				printf("%s", getParser().topicUsageDescription(getParser().getCurrentCommandId(), 80).c_str());
				return false;
			}
			appOptions.m_option = parseProperties.getOption();
			appOptions.m_value = parseProperties.getValue();
		}

		appOptions.setCommandMode(IaarcAppOptions::CommandMode::CM_Prop);

		return true;
	}
}