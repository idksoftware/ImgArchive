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
		std::string usageDescriptionHeader(unsigned int _width) const;
		

		/** Returns a string with the usage descriptions for all options. The
		 * description string is formated to fit into a terminal of width _width.*/
		virtual std::string commandUsage(unsigned int _width = 80) const;

		virtual std::string usageDescription(unsigned int _width = 80) const;

		virtual std::string generalHelp(unsigned int _width) const;

		virtual std::string topicUsageDescription(unsigned int topic, unsigned int _width = 80) const;
	};

} /* namespace CommandLineProcessing */