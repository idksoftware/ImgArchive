
#pragma once

#include "AdminArgvParser.h"

using namespace CommandLineProcessing;

namespace simplearchive {

	class AdminConfig : public AdminSubCommand {
	public:
		AdminConfig(AdminArgvParser& argvParser) : AdminSubCommand("about", argvParser) {};
	protected:
		bool doCommand() override;
	};

};