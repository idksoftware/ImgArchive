#pragma once

#include "AdminArgvParser.h"

using namespace CommandLineProcessing;

namespace simplearchive {

	class AdminAbout : public AdminSubCommand {
	public:
		AdminAbout(AdminArgvParser& argvParser) : AdminSubCommand("about", argvParser) {};
	protected:
		bool doCommand() override;
	};

};