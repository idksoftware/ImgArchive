#pragma once
#include "SIAArgvParser.h"



using namespace CommandLineProcessing;

namespace simplearchive {

	class AdminArgvParser : public SIAArgvParser
	{
	public:

		AdminArgvParser()
		{
		}

		virtual ~AdminArgvParser()
		{
		}

		bool initalise(int argc, char **argv) {
			return doInitalise(argc, argv);
		}

	protected:
		virtual bool doInitalise(int argc, char **argv);
	};

} /* namespace CommandLineProcessing */