#pragma once
#include "SIAArgvParser.h"

using namespace CommandLineProcessing;

namespace simplearchive {

	class SIAArcArgvParser : public SIAArgvParser
	{
		bool m_error;
	public:

		SIAArcArgvParser()
		{
		}

		virtual ~SIAArcArgvParser()
		{
		}

		bool initalise(int argc, char **argv) {
			return doInitalise(argc, argv);
		}

	protected:
		virtual bool doInitalise(int argc, char **argv);
		std::string usageDescriptionHeader(unsigned int _width) const;
	};

} /* namespace */