#pragma once
#include "SIAArgvParser.h"



using namespace CommandLineProcessing;

namespace simplearchive {

	class AdminArgvParser;

	class AdminSubCommand : public SubCommand {
		
		AdminArgvParser& m_argvParser;
	protected:
		
		const AdminArgvParser& getParser() { return m_argvParser; };
	public:
		AdminSubCommand(const char* name, AdminArgvParser& argvParser)
			: SubCommand(name),
			m_argvParser(argvParser) {};
	};



	class AdminArgvParser : public SIAArgvParser
	{
	public:

		AdminArgvParser() = default;
		virtual ~AdminArgvParser() = default;

		

	protected:
		virtual bool doInitalise(int argc, char **argv);

		virtual void defineOptions();
		std::string usageDescriptionHeader(unsigned int _width) const;
		
		/** Returns a string with the usage descriptions for all options. The
		 * description string is formated to fit into a terminal of width _width.*/
		virtual std::string commandUsage(unsigned int _width = 80) const;

		virtual std::string generalHelp(unsigned int _width) const;
	};

} /* namespace CommandLineProcessing */