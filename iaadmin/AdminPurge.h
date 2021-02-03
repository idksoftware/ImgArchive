#pragma once

#include "AdminArgvParser.h"

using namespace CommandLineProcessing;

namespace simplearchive {

	class AdminPurge : public AdminSubCommand {
	public:
		AdminPurge(AdminArgvParser& argvParser) : AdminSubCommand("purge", argvParser) {};
	protected:
		bool doCommand() override;
	};

};