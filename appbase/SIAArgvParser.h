#pragma once
#include "argvparser.h"

namespace CommandLineProcessing {

	class SIAArgvParser : public ArgvParser
	{
	public:

		SIAArgvParser()
		{
		}

		virtual ~SIAArgvParser()
		{
		}
		virtual bool doInitalise(int argc, char **argv) = 0;
	};

} /* namespace CommandLineProcessing */