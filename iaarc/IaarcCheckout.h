#pragma once
#include "IaarcArgvParser.h"

using namespace CommandLineProcessing;

namespace simplearchive {

	class IaarcCheckout : public IaarcSubCommand {
	public:
		IaarcCheckout(IaarcArgvParser& argvParser) : IaarcSubCommand("checkout", argvParser) {};
	protected:
		bool doCommand() override;
	};

};