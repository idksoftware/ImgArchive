#pragma once

#include "AdminArgvParser.h"

using namespace CommandLineProcessing;

namespace simplearchive {

	class AdminValidate : public AdminSubCommand {
	public:
		AdminValidate(AdminArgvParser& argvParser) : AdminSubCommand("about", argvParser) {};
	protected:
		bool doCommand() override;
	};

};