#pragma once

#include "AdminArgvParser.h"

using namespace CommandLineProcessing;

namespace simplearchive {

	class AdminInit : public AdminSubCommand {
	public:
		AdminInit(AdminArgvParser& argvParser) : AdminSubCommand("init", argvParser) {};
	protected:
		bool doCommand() override;
	};

};
