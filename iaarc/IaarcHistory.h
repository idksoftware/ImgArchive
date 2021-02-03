
#pragma once

#include "IaarcArgvParser.h"

using namespace CommandLineProcessing;

namespace simplearchive {

	class IaarcHistory : public IaarcSubCommand {
	public:
		IaarcHistory(IaarcArgvParser& argvParser) : IaarcSubCommand("history", argvParser) {};
	protected:
		bool doCommand() override;
	};

};