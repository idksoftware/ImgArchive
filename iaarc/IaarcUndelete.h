#pragma once
class IaarcUndelete
{
};

#include "IaarcArgvParser.h"

using namespace CommandLineProcessing;

namespace simplearchive {

	class IaarcUndelete : public IaarcSubCommand {
	public:
		IaarcUndelete(IaarcArgvParser& argvParser) : IaarcSubCommand("undelete", argvParser) {};
	protected:
		bool doCommand() override;
	};

};