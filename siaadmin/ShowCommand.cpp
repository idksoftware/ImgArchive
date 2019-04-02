#include "ShowCommand.h"
#include "AppConfig.h"
#include "CLogger.h"
#include "siaglobal.h"
#include "SAUtils.h"

namespace simplearchive {

	ShowCommand::ShowCommand()
	{
	}


	ShowCommand::~ShowCommand()
	{
	}

	bool ShowCommand::process(const char *str) {
		std::string arg = str;
		if (arg.compare("settup") == 0) {
			showSettup();
		}
		return true;
	}

	bool ShowCommand::showSettup()
	{
		AdminConfig config;
		config.printAll();
		return false;
	}

} // simplearchive
