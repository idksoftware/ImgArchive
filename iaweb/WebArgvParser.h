#pragma once
#include "SIAArgvParser.h"



using namespace CommandLineProcessing;

namespace simplearchive {

	class WebArgvParser : public SIAArgvParser
	{
	public:

		WebArgvParser()
		{
		}

		virtual ~WebArgvParser()
		{
		}

		bool initalise(int argc, char **argv) {
			return doInitalise(argc, argv);
		}

	protected:
		virtual bool doInitalise(int argc, char **argv);
		std::string usageDescriptionHeader(unsigned int _width) const;

		virtual std::string commandUsage(unsigned int _width = 80) const;
		virtual std::string generalHelp(unsigned int _width) const;
	};

} /* namespace CommandLineProcessing */