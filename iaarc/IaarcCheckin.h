#pragma once

#include "IaarcArgvParser.h"

using namespace CommandLineProcessing;

namespace simplearchive {

	class IaarcCheckin : public IaarcSubCommand {
	public:
		IaarcCheckin(IaarcArgvParser& argvParser) : IaarcSubCommand("checkin", argvParser) {};
	protected:
		bool doCommand() override;
	};

};