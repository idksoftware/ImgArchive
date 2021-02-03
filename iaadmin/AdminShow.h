
#pragma once

#include "AdminArgvParser.h"

using namespace CommandLineProcessing;

namespace simplearchive {

	class AdminShow : public AdminSubCommand {
	public:
		AdminShow(AdminArgvParser& argvParser) : AdminSubCommand("about", argvParser) {};
	protected:
		bool doCommand() override;
	};

};