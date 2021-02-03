#pragma once

#include "IaarcArgvParser.h"

using namespace CommandLineProcessing;

namespace simplearchive {

	class IaarcAbout : public IaarcSubCommand {
	public:
		IaarcAbout(IaarcArgvParser& argvParser) : IaarcSubCommand("about", argvParser) {};
	protected:
		bool doCommand() override;
	};

};