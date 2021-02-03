#include "AdminValidate.h"

#include "Quiet.h"
#include "ConfigReader.h"
#include "AdminAppOptions.h"
#include "ParseOptions.h"
#include "SAUtils.h"

using namespace CommandLineProcessing;

namespace simplearchive {

	bool AdminValidate::doCommand() {
		AppOptions& appOptions = AppOptions::get();
		appOptions.setCommandMode(AppOptions::CommandMode::CM_Validate);
		

		/*
		"value=[master] | [derivative] | [workspace] | [all] | [main]");

	defineOption("validate-backup", "validate backup. Note: both backups will be validate by default if avalible.", ArgvParser::OptionRequiresValue);
	defineCommandSyntax("validate-backup", "--validate-backup=<Value>\n"
		"value=[1] | [2] | [both]");
		*/

		if (getParser().foundOption("scope") == true) {

			std::string opt = getParser().optionValue("scope");
			//					Quiet::printf(opt.c_str()); Quiet::printf("\n");
			if (opt.compare("master") == 0) {
				appOptions.m_verifyOperation = AppOptions::VerifyOperation::Master;
			}
			else if (opt.compare("derivative") == 0) {
				appOptions.m_verifyOperation = AppOptions::VerifyOperation::Derivative;
			}
			else if (opt.compare("workspace") == 0) {
				appOptions.m_verifyOperation = AppOptions::VerifyOperation::Workspace;
			}
			else if (opt.compare("all") == 0) {
				appOptions.m_verifyOperation = AppOptions::VerifyOperation::All;
			}
			else if (opt.compare("main") == 0) {
				appOptions.m_verifyOperation = AppOptions::VerifyOperation::Main;
			}
			else {
				ReturnCodeObject::setReturnString(NoArgumentForSubCommand, "Invalid argument for sub-command: %s \"%s\"", getParser().getCurrentCommand().c_str(), opt.c_str());
				Quiet::printf("%s", getParser().usageDescription(80).c_str());
				return false;
			}
		}
		if (getParser().foundOption("validate-main") == true) {

			std::string opt = getParser().optionValue("validate-main");
			SAUtils::isTrueFalse(opt);
			appOptions.m_validateMain = (opt.compare("True") == 0);
		}
		if (getParser().foundOption("validate-backup") == true) {
			std::string opt = getParser().optionValue("validate-backup");
			if (opt.compare("1") == 0) {
				appOptions.m_verifyBackups = AppOptions::VerifyBackups::Backup_1;
			}
			else if (opt.compare("2") == 0) {
				appOptions.m_verifyBackups = AppOptions::VerifyBackups::Backup_2;
			}
			else if (opt.compare("both") == 0) {
				appOptions.m_verifyBackups = AppOptions::VerifyBackups::Both;
			}
			else {
				ReturnCodeObject::setReturnString(NoArgumentForSubCommand, "Invalid argument for sub-command: %s \"%s\"", getParser().getCurrentCommand().c_str(), opt.c_str());
				Quiet::printf("%s", getParser().usageDescription(80).c_str());
				return false;
			}
		}
		if (getParser().foundOption("repair") == true) {
			std::string opt = getParser().optionValue("repair");
			SAUtils::isTrueFalse(opt);
			appOptions.m_repair = (opt.compare("True") == 0);
		}
		return true;
	}
}