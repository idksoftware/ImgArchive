#include "IaarcUncheckout.h"

#include "IaarcAbout.h"

#include "Quiet.h"
#include "ConfigReader.h"
#include "IaarcAppOptions.h"
#include "ParseOptions.h"

using namespace CommandLineProcessing;

namespace simplearchive {

	bool IaarcUncheckout::doCommand() {
		IaarcAppOptions& appOptions = IaarcAppOptions::get();
		bool gotImageAddress = false;
		if (getParser().foundOption("image") == true) {
			appOptions.m_imageAddress = getParser().optionValue("image");
			gotImageAddress = true;
		}

		if (getParser().foundOption("comment") == true) {
			appOptions.m_comment = getParser().optionValue("comment");
		}

		if (getParser().foundOption("force") == true) {
			appOptions.m_force = true;
		}

		appOptions.setCommandMode(IaarcAppOptions::CommandMode::CM_UnCheckout);
		return true;
	}
}