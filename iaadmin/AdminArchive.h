
#pragma once

#include "AdminArgvParser.h"

using namespace CommandLineProcessing;

namespace simplearchive {

	class AdminArchive : public AdminSubCommand {
	public:
		AdminArchive(AdminArgvParser& argvParser) : AdminSubCommand("archive", argvParser) {};
	protected:
		bool doCommand() override;
	};

};
