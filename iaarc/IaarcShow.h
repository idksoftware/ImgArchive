
#pragma once

#include "IaarcArgvParser.h"

using namespace CommandLineProcessing;

namespace simplearchive {

	class IaarcShow : public IaarcSubCommand {
	public:
		IaarcShow(IaarcArgvParser& argvParser) : IaarcSubCommand("show", argvParser) {};
	protected:
		bool doCommand() override;
	};

};