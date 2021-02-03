
#pragma once

#include "IaarcArgvParser.h"

using namespace CommandLineProcessing;

namespace simplearchive {

	class IaarcExport : public IaarcSubCommand {
	public:
		IaarcExport(IaarcArgvParser& argvParser) : IaarcSubCommand("export", argvParser) {};
	protected:
		bool doCommand() override;
	};

};
