#include "IaarcMode.h"

#include "Quiet.h"
#include "ConfigReader.h"
#include "IaarcAppOptions.h"
#include "ParseOptions.h"

using namespace CommandLineProcessing;

namespace simplearchive {

	bool IaarcMode::doCommand() {
		IaarcAppOptions& appOptions = IaarcAppOptions::get();
		if (getParser().foundOption("remote-server") == true) {
			//config.setServerModeON();
		}
		appOptions.setCommandMode(IaarcAppOptions::CommandMode::CM_Mode);
		return true;
	}
}