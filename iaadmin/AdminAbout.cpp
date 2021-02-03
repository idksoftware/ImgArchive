#include "AdminAbout.h"
#include "Quiet.h"
#include "ConfigReader.h"
#include "AdminAppOptions.h"
#include "ParseOptions.h"

using namespace CommandLineProcessing;

namespace simplearchive {

	bool AdminAbout::doCommand() {
		AppOptions& appOptions = AppOptions::get();
		if (getParser().foundOption("out") == true) {
			OutputType outputType;
			std::string outType = getParser().optionValue("out");
			if (outputType.parse(getParser().optionValue("out").c_str()) == false) {
				Quiet::printf("Option for argument \"out\" for sub-command: %s is invalid: %s\n\n", getParser().getCurrentCommand().c_str(), getParser().optionValue("out").c_str());
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
		appOptions.setCommandMode(AppOptions::CommandMode::CM_About);
		return true;
	}
}