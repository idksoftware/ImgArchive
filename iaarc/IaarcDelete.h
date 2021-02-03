#pragma once
class IaarcDelete
{
};
#include "IaarcArgvParser.h"

using namespace CommandLineProcessing;

namespace simplearchive {

	class IaarcDelete : public IaarcSubCommand {
	public:
		IaarcDelete(IaarcArgvParser& argvParser) : IaarcSubCommand("delete", argvParser) {};
	protected:
		bool doCommand() override;
	};

};
