#pragma once
#include <memory>
#include "argvparser.h"
#include "ReturnCode.h"

namespace CommandLineProcessing {

	
	class SubCommand {
		std::string m_name;
		
	protected:
		virtual bool doCommand() = 0;
		
	public:
		SubCommand(const char *name)
			: m_name(name) {};
		bool command() {
			return doCommand();
		};

		const char* getName() {
			return m_name.c_str();
		};
	};
	

	class SIAArgvParser : public ArgvParser
	{
		std::vector<std::shared_ptr<SubCommand>> m_list;

	public:

		SIAArgvParser() = default;
		virtual ~SIAArgvParser() = default;
	
		bool initalise(int argc, char** argv) {
			defineOptions();
			return doInitalise(argc, argv);
		}

		virtual void defineOptions() = 0;
		virtual bool doInitalise(int argc, char **argv) = 0;
		virtual std::string usageDescriptionHeader(unsigned int _width) const = 0;
		
		ReturnCode parseErrorNumber(ParserResults _error_code) const;
		bool handleParseResult(int argc, char** argv);

		bool addSubCommand(std::shared_ptr<SubCommand > subCommand);
		bool doCommand();
	};

} /* namespace CommandLineProcessing */