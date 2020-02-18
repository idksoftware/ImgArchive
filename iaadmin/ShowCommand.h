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
		
		bool showGeneral();
		bool showLogging();
		bool showNetwork();
		bool showFolders();
		bool showMaster();
		bool showDerivative();
		bool showBackup();
		bool showExiftool();
	
		Error m_error;
	public:
		ShowCommand();
		~ShowCommand() = default;
		
		bool parseOptions(const char* optionString);
		bool process(const char* str);
		Error getError() { return m_error; };
	};

} // simplearchive
