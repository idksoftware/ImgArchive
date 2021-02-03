#include "AdminSync.h"

#include "Quiet.h"
#include "ConfigReader.h"
#include "AdminAppOptions.h"
#include "SyncCommand.h"

using namespace CommandLineProcessing;

namespace simplearchive {

	bool AdminSync::doCommand() {
		AppOptions& appOptions = AppOptions::get();
		SyncCommand syncCommand;
		bool ok = false;
		std::string opt;
		if (getParser().foundOption("master-with") == true) {
			opt = getParser().optionValue("master-with");
			appOptions.setConfigOption("master");
			if (syncCommand.setBackup(opt.c_str()) == true) {
				ok = true;
			}

		}
		else if (getParser().foundOption("derivative-with") == true) {
			opt = getParser().optionValue("derivative-with");
			appOptions.setConfigOption("derivative");
			if (syncCommand.setBackup(opt.c_str()) == true) {
				ok = true;
			}

		}
		else if (getParser().foundOption("both-with") == true) {
			opt = getParser().optionValue("both-with");
			appOptions.setConfigOption("both");
			if (syncCommand.setBackup(opt.c_str()) == true) {
				ok = true;
			}

		}
		else if (getParser().foundOption("workspace") == true) {
			opt = getParser().optionValue("workspace");
			if (syncCommand.setBackup(opt.c_str()) == true) {
				ok = true;
			}

		}
		else if (getParser().foundOption("catalogue") == true) {
			opt = getParser().optionValue("catalogue");
			if (syncCommand.setBackup(opt.c_str()) == true) {
				ok = true;
			}

		}
		else if (getParser().foundOption("www") == true) {
			opt = getParser().optionValue("www");
			if (syncCommand.setBackup(opt.c_str()) == true) {
				ok = true;
			}

		}
		else {
			ReturnCodeObject::setReturnString(NoArgumentForSubCommand, "Invalid argument for sub-command: %s folders \"%s\" %s\n\n", getParser().getCurrentCommand().c_str(), appOptions.getConfigOption(), opt.c_str());
			Quiet::printf("%s", getParser().topicUsageDescription(getParser().getCurrentCommandId(), 80).c_str());
			return false;
		}
		if (ok) {
			appOptions.setConfigValue(opt.c_str());
		}
		else {
			ReturnCodeObject::setReturnString(NoArgumentForSubCommand, "Invalid argument for sub-command: %s folders \"%s\"\n\n", getParser().getCurrentCommand().c_str(), opt.c_str());
			Quiet::printf("%s", getParser().topicUsageDescription(getParser().getCurrentCommandId(), 80).c_str());
			return false;
		}
		appOptions.setCommandMode(AppOptions::CommandMode::CM_Sync);
		
		return true;
	}
}