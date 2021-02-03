#pragma once
class IaarcMode
{
};
#pragma once

#include "IaarcArgvParser.h"

using namespace CommandLineProcessing;

namespace simplearchive {

	class IaarcMode : public IaarcSubCommand {
	public:
		IaarcMode(IaarcArgvParser& argvParser) : IaarcSubCommand("mode", argvParser) {};
	protected:
		bool doCommand() override;
	};

};


