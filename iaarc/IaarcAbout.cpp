#include "IaarcAbout.h"

#include "Quiet.h"
#include "ConfigReader.h"
#include "IaarcAppOptions.h"
#include "ParseOptions.h"

using namespace CommandLineProcessing;

namespace simplearchive {

	bool IaarcAbout::doCommand() {
		IaarcAppOptions& appOptions = IaarcAppOptions::get();
		if (getParser().foundOption("out") == true) {
			OutputType outputType;
			std::string outType = getParser().optionValue("out");
			if (outputType.parse(getParser().optionValue("out").c_str()) == false) {
				ReturnCodeObject::setReturnString("Option for argument \"out\" for sub-command: %s is invalid: %s\n\n",
										getParser().getCurrentCommand().c_str(), getParser().optionValue("out").c_str());
				printf("%s", getParser().topicUsageDescription(getParser().getCurrentCommandId(), 80).c_str());
				return false;
			}
			else {
				appOptions.m_textOutputType = getParser().optionValue("out");
			}
		}
		if (getParser().foundOption("file") == true) {
			appOptions.m_outputFile = getParser().optionValue("file");
		}
		appOptions.setCommandMode(IaarcAppOptions::CommandMode::CM_About);
		return true;
	}
}