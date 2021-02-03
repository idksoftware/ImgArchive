#pragma once

#include "IaarcArgvParser.h"

using namespace CommandLineProcessing;

namespace simplearchive {

	class IaarcUncheckout : public IaarcSubCommand {
	public:
		IaarcUncheckout(IaarcArgvParser& argvParser) : IaarcSubCommand("uncheckout", argvParser) {};
	protected:
		bool doCommand() override;
	};

};