#include "IaarcShow.h"

#include "Quiet.h"
#include "ConfigReader.h"
#include "IaarcAppOptions.h"
#include "ParseOptions.h"

using namespace CommandLineProcessing;

namespace simplearchive {

	bool IaarcShow::doCommand() {
		IaarcAppOptions& appOptions = IaarcAppOptions::get();
		appOptions.setCommandMode(IaarcAppOptions::CommandMode::CM_Show);

		return true;
	}
}