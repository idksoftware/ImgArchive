#pragma once

#include <memory>
#include <string>
#include <cstdlib>
#include <cstdarg>
#include <sstream>
#include <iomanip>
#include <string>
#include "SIAArgvParser.h"

constexpr auto ALLUSERS_DEFAULT_CONFIG_PATH = "/IDK-Software/ImgArchive";
constexpr auto MYSELF_DEFAULT_CONFIG_PATH = "/.ImgArchive";

namespace CommandLineProcessing {


//	typedef std::unique_ptr<DupDataFile> DupDataFile_Ptr;

	class AppBase
	{
	protected:
		std::shared_ptr<SIAArgvParser> m_argvParser;
		bool m_configured;
		std::string  m_configPath;
		std::string  m_homePath;

		static int m_error;
		static std::string m_errorstring;

		AppBase(std::shared_ptr<SIAArgvParser> argvParser) :
					m_argvParser(argvParser),
					m_configured(false)
		{}

	public:
		~AppBase();
		
		int RunApp(int argc, char **argv);
		bool initalise(int argc, char **argv);
		
		/// @brief This is the main application run function. All application processing starts here.
		bool Run();
		/// @brief Shows the configuration of the archive. All application processing starts here.
		bool Show();
		
		bool isConfiguratedOk() {
			return m_configured;
		}

		const char *getHomePath() {
			return m_homePath.c_str();
		}

		static const char *getFullErrorString() {
			
			std::stringstream str;
			str << "E" << std::setw(4) << std::setfill('0')<< m_error << ": " << m_errorstring;
			m_errorstring = str.str();
			return m_errorstring.c_str();
		}

		static int getError() {
			return m_error;
		}

	protected:
		
		virtual bool initaliseConfig() = 0;
		virtual bool initaliseArgs(int argc, char **argv) = 0;
		virtual bool doRun() = 0;
		void setConfigPath(const char *configPath) {
			m_configPath = configPath;
		}

		static void setError(int err, const char *format, ...) {
			m_error = err;
			//m_errorstring = str;
			char message[512];
			va_list args;
			va_start(args, format);
#ifdef _WIN32
			vsprintf_s(message, format, args);
#else
			vsprintf(message, format, args);
#endif
			m_errorstring = message;
			va_end(args);
		}

		

		static const char *getErrorString() {
			return m_errorstring.c_str();
		}

	};

}