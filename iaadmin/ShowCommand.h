#pragma once

namespace simplearchive {

	class ShowCommand
	{
	public:
		enum class Error {
			Ok,
			ParseError,
			CommandError,
			Unknown
		};
	private:
		bool showSettup();
		Error m_error;
	public:
		ShowCommand();
		~ShowCommand() = default;
		bool parseOptions(const char* optionString);
		Error getError() { return m_error; };
	};

} // simplearchive
