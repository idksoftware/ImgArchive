#pragma once

#include "IaarcArgvParser.h"

using namespace CommandLineProcessing;

namespace simplearchive {

	class IaarcImport : public IaarcSubCommand {
	public:
		IaarcImport(IaarcArgvParser& argvParser) : IaarcSubCommand("import", argvParser) {};
	protected:
		bool doCommand() override;
	};

};