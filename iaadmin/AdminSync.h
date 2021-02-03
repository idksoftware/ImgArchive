

#pragma once

#include "AdminArgvParser.h"

using namespace CommandLineProcessing;

namespace simplearchive {

	class AdminSync : public AdminSubCommand {
	public:
		AdminSync(AdminArgvParser& argvParser) : AdminSubCommand("about", argvParser) {};
	protected:
		bool doCommand() override;
	};

};