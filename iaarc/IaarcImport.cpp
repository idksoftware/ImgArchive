#include "IaarcImport.h"

#include "AppConfig.h"
#include "SAUtils.h"
#include "Quiet.h"
#include "ConfigReader.h"
#include "IaarcAppOptions.h"
#include "ParseOptions.h"
#include "ConfigReader.h"
#include "AppOptions.h"
#include "Environment.h"

using namespace CommandLineProcessing;

namespace simplearchive {

	bool IaarcImport::doCommand() {

		SIAARCConfig config;
		IaarcAppOptions& appOptions = IaarcAppOptions::get();

		bool isSourePathSet = false;
		// Source of images
		if (getParser().foundOption("source-path") == true) {
			std::string opt = getParser().optionValue("source-path");
			config.setSourcePath(opt.c_str());
			isSourePathSet = true;
		}
		if (isSourePathSet == false) {
			config.setSourcePath(SAUtils::getCurrentDirectory().c_str());
		}
		if (getParser().foundOption("lightroom") == true) {
			std::string opt = getParser().optionValue("lightroom");
			config.setLightroom();
		}



		if (getParser().foundOption("peek") == true) {
			appOptions.m_peekOnly = true;
		}
		if (getParser().foundOption("force-date") == true) {
			std::string opt = getParser().optionValue("force-date");
			if (opt.compare("FileDate") == 0) {
				appOptions.m_useFileDate = true;
			}

			if (opt.compare("TodaysDate") == 0) {
				appOptions.m_useDateToday = true;
			}
			else {
				appOptions.m_archiveDate = ExifDate(opt.c_str());
			}
			appOptions.m_forceDate = true;

		}
		if (getParser().foundOption("default-date") == true) {
			std::string opt = getParser().optionValue("default-date");
			config.setSourcePath(opt.c_str());
			if (opt.compare("FileDate") == 0) {
				appOptions.m_useFileDate = true;
			}

			if (opt.compare("TodaysDate") == 0) {
				appOptions.m_useDateToday = true;
			}
			else {
				appOptions.m_archiveDate = ExifDate(opt.c_str());
			}
			appOptions.m_useDate = true;
		}

		appOptions.setCommandMode(IaarcAppOptions::CommandMode::CM_Import);

		Environment::setEnvironment();
		
		return true;
	}
}
/*

*/