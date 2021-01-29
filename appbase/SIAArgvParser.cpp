#include "SIAArgvParser.h"
#include "Quiet.h"

namespace CommandLineProcessing {
	
	ReturnCode SIAArgvParser::parseErrorNumber(ParserResults _error_code) const
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
	
	bool SIAArgvParser::handleParseResult(int argc, char** argv) {

		ArgvParser::ParserResults res = parse(argc, argv);

		std::string errStr;
		switch (res) {
		case ArgvParser::ParserResults::NoParserError:
			break;
		case ArgvParser::ParserResults::ParserUnknownOption:
		case ArgvParser::ParserResults::ParserMissingValue:
		case ArgvParser::ParserResults::ParserOptionAfterArgument:
		case ArgvParser::ParserResults::ParserMalformedMultipleShortOption:
		case ArgvParser::ParserResults::ParserRequiredOptionMissing:
		case ArgvParser::ParserResults::ParserHelpRequested:
			errStr = parseErrorDescription(res);
			ReturnCodeObject::setReturnString(parseErrorNumber(res), "%s", errStr.c_str());
			Quiet::printf("%s", topicUsageDescription(getCurrentCommandId(), 80).c_str());
			return false;
		case ArgvParser::ParserResults::GeneralHelpRequested:
			printf("%s", usageDescription(80).c_str()); // this may need generalHelp(80);
			return false;
		case ArgvParser::ParserResults::TopicHelpRequested:
			printf("%s", topicUsageDescription(getCurrentCommandId(), 80).c_str());
			return false;
		case ArgvParser::ParserResults::ParserCommandNotFound:
			ReturnCodeObject::setReturnString(ParserCommandNotFound, "Invalid command: %s", getCurrentCommand().c_str());
			Quiet::printf("%s", usageDescription(80).c_str());
			return false;
		default:
			ReturnCodeObject::setReturnString(UnKnownError, "Invalid parser operation: %s");
			return false;
		}
		return true;
	}

	bool SIAArgvParser::addSubCommand(std::shared_ptr<SubCommand > subCommand) {
		m_list.push_back(subCommand);
		return true;
	}

	bool SIAArgvParser::doCommand() {

		for (auto i = m_list.begin(); i != m_list.end(); i++) {
			auto cmd = *i;
			if (command(cmd->getName())) {
				return cmd->command();
			}
		}
		ReturnCodeObject::setReturnString(SubCommandRequired, "Sub-command required?\n\n");
		Quiet::printf("%s\n", usageDescription().c_str());
		return false;
	}

}