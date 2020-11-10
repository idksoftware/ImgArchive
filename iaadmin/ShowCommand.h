#pragma once
#include "ImageExtentions.h"
#include "TextOut.h"

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
		
		bool showGeneral(const char* filename, const char* textOutType);
		bool showLogging(const char* filename, const char* textOutType);
		bool showNetwork(const char* filename, const char* textOutType);
		bool showFolders(const char* filename, const char* textOutType);
		bool showMaster(const char* filename, const char* textOutType);
		bool showDerivative(const char* filename, const char* textOutType);
		bool showWorkspace(const char* filename, const char* textOutType);
		bool showPicture(const char* filename, const char* textOutType);
		bool showWWW(const char* filename, const char* textOutType);
		bool showBackup(const char* filename, const char* textOutType);
		bool showExiftool(const char* filename, const char* textOutType);

		bool processSettings(const char* str);
		bool processAllowed(const char* str);
		bool processEnv(const char* str);

		bool showAllowed(SelectionType type);
		bool showAllowedRaw();
		bool showAllowedPicture();
		bool showAllowedAll();
		Error m_error;
		std::string m_outputFile;
		std::string m_textOutputType;
	public:
		ShowCommand();
		~ShowCommand() = default;
		void setOutputFile(const char* s);
		void setTextOutputType(const char* s);
		bool parseOptions(const char* optionString);
		bool process(const char* configOption, const char* configValue);
		
		Error getError() { return m_error; };
	};

} // simplearchive
