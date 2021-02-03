#pragma once

#include "AdminArgvParser.h"

using namespace CommandLineProcessing;

namespace simplearchive {

	class AdminAllow : public AdminSubCommand {
	public:
		AdminAllow(AdminArgvParser& argvParser) : AdminSubCommand("allow", argvParser) {};
	protected:
		bool doCommand() override;
	};

};