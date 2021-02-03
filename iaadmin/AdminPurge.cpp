#include "AdminPurge.h"

#include "Quiet.h"
#include "ConfigReader.h"
#include "AdminAppOptions.h"
#include "ParseOptions.h"

using namespace CommandLineProcessing;

namespace simplearchive {

	bool AdminPurge::doCommand() {
		AppOptions& appOptions = AppOptions::get();
		Quiet::printf("Purge command\n");
		appOptions.setCommandMode(AppOptions::CommandMode::CM_Purge);
		return true;
	}
}