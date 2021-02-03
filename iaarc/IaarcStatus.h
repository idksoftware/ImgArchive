#pragma once

#include "IaarcArgvParser.h"

using namespace CommandLineProcessing;

namespace simplearchive {

	class IaarcStatus : public IaarcSubCommand {
	public:
		IaarcStatus(IaarcArgvParser& argvParser) : IaarcSubCommand("status", argvParser) {};
	protected:
		bool doCommand() override;
	};

};
