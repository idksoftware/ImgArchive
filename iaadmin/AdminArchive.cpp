#include "AdminArchive.h"

#include "Quiet.h"
#include "ConfigReader.h"
#include "AdminAppOptions.h"
#include "ParseOptions.h"

using namespace CommandLineProcessing;

namespace simplearchive {

	bool AdminArchive::doCommand() {
		AppOptions& appOptions = AppOptions::get();
		appOptions.setCommandMode(AppOptions::CommandMode::CM_Backup);
		if (getParser().foundOption("media-path") == true) {

			appOptions.m_mediaPath = getParser().optionValue("media-path");
			//					Quiet::printf(opt.c_str()); Quiet::printf("\n");
							//config.setBackupDestinationPath(opt.c_str());
							//appOptions.m_toDate;
		}
		if (getParser().foundOption("media-size") == true) {

			appOptions.m_mediaSize = getParser().optionValue("media-size");
			//					Quiet::printf(opt.c_str()); Quiet::printf("\n");
							//config.setBackupMediaSize(opt.c_str());

		}
		if (getParser().foundOption("from-date") == true) {

			appOptions.m_fromDate = getParser().optionValue("from-date");
			//					Quiet::printf(opt.c_str()); Quiet::printf("\n");
							//config.setFromDate(opt.c_str());

		}
		if (getParser().foundOption("to-date") == true) {

			appOptions.m_toDate = getParser().optionValue("to-date");
			//					Quiet::printf(opt.c_str()); Quiet::printf("\n");
							//config.setToDate(opt.c_str());

		}
		
		return true;
	}
}