#include "IaarcCheckin.h"

#include "Quiet.h"
#include "ConfigReader.h"
#include "IaarcAppOptions.h"
#include "ParseOptions.h"
#include "argvparser.h"

using namespace CommandLineProcessing;

namespace simplearchive {

	bool IaarcCheckin::doCommand() {
		IaarcAppOptions& appOptions = IaarcAppOptions::get();
		if (getParser().foundOption("scope") == true) {
			IaarcAppOptions::m_imageAddress = getParser().optionValue("scope");
		}
		if (getParser().foundOption("comment") == true) {
			IaarcAppOptions::m_comment = getParser().optionValue("comment");
		}


		if (getParser().foundOption("force") == true) {
			IaarcAppOptions::m_force = true;
		}

		//const auto& args = ArgvParser::getDefaultArgumentsContainer();
		//for (auto i = args.begin(); i != args.end(); i++) {
		//	appOptions.setDefaultArguments(*i);
		//}

		appOptions.setCommandMode(IaarcAppOptions::CommandMode::CM_Checkin);
		return true;
	}
}


