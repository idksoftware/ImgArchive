#pragma once
#include "SIAArgvParser.h"

using namespace CommandLineProcessing;

namespace simplearchive {

	class IaarcArgvParser;

	class IaarcSubCommand : public SubCommand {
		IaarcArgvParser& m_argvParser;
	protected:
		const IaarcArgvParser& getParser() { return m_argvParser; };
	public:
		IaarcSubCommand(const char* name, IaarcArgvParser& argvParser)
			: SubCommand(name),
			m_argvParser(argvParser) {};
	};


	class IaarcArgvParser : public SIAArgvParser
	{
		bool m_error;
	public:

		IaarcArgvParser() : m_error(false) {};

		virtual ~IaarcArgvParser()
		{
		}

		bool initalise(int argc, char **argv) {
			defineOptions();
			return doInitalise(argc, argv);
		}

	protected:
		virtual bool doInitalise(int argc, char **argv);
		virtual void defineOptions();
		std::string usageDescriptionHeader(unsigned int _width) const;

		/** Returns a string with the usage descriptions for all options. The
		 * description string is formated to fit into a terminal of width _width.*/
		virtual std::string commandUsage(unsigned int _width = 80) const;

		virtual std::string generalHelp(unsigned int _width) const;

	};

} /* namespace */