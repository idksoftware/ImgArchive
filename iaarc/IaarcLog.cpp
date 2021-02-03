#include "IaarcLog.h"

#include "Quiet.h"
#include "ConfigReader.h"
#include "IaarcAppOptions.h"
#include "ParseOptions.h"

using namespace CommandLineProcessing;

namespace simplearchive {

	bool IaarcLog::doCommand() {
		IaarcAppOptions& appOptions = IaarcAppOptions::get();
		bool gotImageAddress = false;
		if (getParser().foundOption("image") == true) {
			appOptions.m_imageAddress = getParser().optionValue("image");
			gotImageAddress = true;
		}

		if (getParser().foundOption("scope") == true) {
			appOptions.m_imageAddress = getParser().optionValue("scope");
		}

		if (getParser().foundOption("file") == true) {
			std::string opt = getParser().optionValue("file");
			appOptions.m_filePath = opt;
		}

		if (getParser().foundOption("format-type") == true) {
			std::string opt = getParser().optionValue("format-type");
			if ((appOptions.m_formatType = ResultsPresentation::parse(opt.c_str())) == ResultsPresentation::FormatType::unknown) {
				printf("Invalid argument for \"FormatType\" \"%s\"\n\n", opt.c_str());
				printf("%s", getParser().topicUsageDescription(getParser().getCurrentCommandId(), 80).c_str());
				return false;
			}
			appOptions.m_textOutputType = opt.c_str();
		}

		if (gotImageAddress == false) {
			// must be system history
		}

		appOptions.setCommandMode(IaarcAppOptions::CommandMode::CM_Log);
		return true;
	}
}
