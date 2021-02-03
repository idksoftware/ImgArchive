#pragma once

#include "IaarcArgvParser.h"

using namespace CommandLineProcessing;

namespace simplearchive {

	class IaarcLog : public IaarcSubCommand {
	public:
		IaarcLog(IaarcArgvParser& argvParser) : IaarcSubCommand("log", argvParser) {};
	protected:
		bool doCommand() override;
	};

};