#pragma once
class IaarcProp
{
};
#pragma once

#include "IaarcArgvParser.h"

using namespace CommandLineProcessing;

namespace simplearchive {

	class IaarcProp : public IaarcSubCommand {
	public:
		IaarcProp(IaarcArgvParser& argvParser) : IaarcSubCommand("prop", argvParser) {};
	protected:
		bool doCommand() override;
	};

};
