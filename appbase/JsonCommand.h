#pragma once
#include <string>
#include <vector>
#include <memory>
class JsonCommand {
public:
	class Argument {
	public:
		std::string m_arg;
		std::string m_value;
		Argument(std::string arg, std::string value)
			: m_arg{ arg }, m_value{ value }
		{};
	};
private:
	enum class ExprType {
		Array,
		Block,
		Unknown
	};
	std::shared_ptr<std::vector<std::string>> m_tokens;
	std::string m_command;
	std::string m_str;
	std::string m_eLeft;
	std::string m_eRight;

	bool trim(char left, char right, std::string& str);
	bool trimBlock(std::string& str);
	void trimString(std::string& str);
	bool expression(std::string& str);
	ExprType expressionType(std::string& str);
	bool parseBlock(std::string& str);
	bool parseArray(std::string& str);
	bool splitArray(std::string& str);

	std::vector<Argument> m_argumentList;
public:
	JsonCommand(std::string& inputString);
	virtual ~JsonCommand() = default;

	bool parse();
	std::string getCommand() { return m_command; };
	std::size_t numberOfArgs() {
		return m_argumentList.size();
	}
	std::string getArgument(int i) { return m_argumentList[i].m_arg; };
	std::string getValue(int i) { return m_argumentList[i].m_value; };

	void print() {
		printf("Commant: %s\n", getCommand().c_str());
		printf("Arguments\n");
		for (int i = 0; i < numberOfArgs(); i++) {
			printf("Argument %d: %s Value: %s\n", i, getArgument(i).c_str(), getValue(i).c_str());
		}
	}
};

