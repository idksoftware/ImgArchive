#pragma once
#include "argvparser.h"
#include "ReturnCode.h"

namespace CommandLineProcessing {

	

	class SIAArgvParser : public ArgvParser
	{
		

	public:

		SIAArgvParser() = default;
		virtual ~SIAArgvParser() = default;
	
		virtual bool doInitalise(int argc, char **argv) = 0;
		virtual std::string usageDescriptionHeader(unsigned int _width) const = 0;
		

		ReturnCode parseErrorNumber(ParserResults _error_code) const
		{


			switch (_error_code)
			{
			case ParserResults::ParserHelpRequested:
			case ParserResults::GeneralHelpRequested:
			case ParserResults::TopicHelpRequested:
			case ParserResults::NoParserError:
				return NoParserError;
			case ParserResults::ParserUnknownOption:
				return ParserUnknownOption;
			case ParserResults::ParserMissingValue:
				return ParserMissingValue;
			case ParserResults::ParserOptionAfterArgument:
				return ParserOptionAfterArgument;
			case ParserResults::ParserMalformedMultipleShortOption:
				return ParserMalformedMultipleShortOption;
			case ParserResults::ParserRequiredOptionMissing:
				return ParserRequiredOptionMissing;
			case ParserResults::ParserCommandNotFound:
				return ParserUnknownOption;
			}
			return UnKnownError;
		}

	};

} /* namespace CommandLineProcessing */