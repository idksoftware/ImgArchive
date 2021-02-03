#pragma once

#include "IaarcArgvParser.h"

using namespace CommandLineProcessing;

namespace simplearchive {

	class IaarcGet : public IaarcSubCommand {
	public:
		IaarcGet(IaarcArgvParser& argvParser) : IaarcSubCommand("get", argvParser) {};
	protected:
		bool doCommand() override;
	};

};