#pragma once

#include "IaarcArgvParser.h"

using namespace CommandLineProcessing;

namespace simplearchive {

	class IaarcTemplate : public IaarcSubCommand {
	public:
		IaarcTemplate(IaarcArgvParser& argvParser) : IaarcSubCommand("template", argvParser) {};
	protected:
		bool doCommand() override;
	};

};