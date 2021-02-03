
#pragma once

#include "AdminArgvParser.h"

using namespace CommandLineProcessing;

namespace simplearchive {

	class AdminSetenv : public AdminSubCommand {
	public:
		AdminSetenv(AdminArgvParser& argvParser) : AdminSubCommand("setenv", argvParser) {};
	protected:
		bool doCommand() override;
	};

};