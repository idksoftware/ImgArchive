#include "IaarcExport.h"

#include "Quiet.h"
#include "ConfigReader.h"
#include "IaarcAppOptions.h"
#include "ParseOptions.h"

using namespace CommandLineProcessing;

namespace simplearchive {

	bool IaarcExport::doCommand() {
		IaarcAppOptions& appOptions = IaarcAppOptions::get();
		if (getParser().foundOption("scope") == true) {
			appOptions.m_imageAddress = getParser().optionValue("scope");
		}

		if (getParser().foundOption("comment") == true) {
			appOptions.m_comment = getParser().optionValue("comment");
		}

		if (getParser().foundOption("force") == true) {
			appOptions.m_force = true;
		}

		if (getParser().foundOption("version") == true) {
			std::string opt = getParser().optionValue("version");
			appOptions.m_version = opt.c_str();
		}

		appOptions.setCommandMode(IaarcAppOptions::CommandMode::CM_Export);
		return true;
	}
}