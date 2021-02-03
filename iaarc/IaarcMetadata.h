#pragma once

#include "IaarcArgvParser.h"

using namespace CommandLineProcessing;

namespace simplearchive {

	class IaarcMetadata : public IaarcSubCommand {
	public:
		IaarcMetadata(IaarcArgvParser& argvParser) : IaarcSubCommand("metadata", argvParser) {};
	protected:
		bool doCommand() override;
	};

};

