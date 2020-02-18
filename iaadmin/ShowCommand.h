#pragma once
#include "ImageExtentions.h"
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
		bool processSettings(const char* str);
		bool processAllowed(const char* str);

		bool showAllowed(SelectionType type);
		bool showAllowedRaw();
		bool showAllowedPicture();
		bool showAllowedAll();
		Error m_error;
	public:
		ShowCommand();
		~ShowCommand() = default;
		
		bool parseOptions(const char* optionString);
		bool process(const char* configOption, const char* configValue);
		
		Error getError() { return m_error; };
	};

} // simplearchive
