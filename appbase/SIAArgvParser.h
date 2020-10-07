#pragma once
#include "argvparser.h"

namespace CommandLineProcessing {

	class SIAArgvParser : public ArgvParser
	{
	public:

		SIAArgvParser() = default;
		virtual ~SIAArgvParser() = default;
	
		virtual bool doInitalise(int argc, char **argv) = 0;
		virtual std::string usageDescriptionHeader(unsigned int _width) const = 0;
	};

} /* namespace CommandLineProcessing */