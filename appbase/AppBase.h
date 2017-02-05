#pragma once

#include <memory>
#include <string>
#include "SIAArgvParser.h"

namespace CommandLineProcessing {


//	typedef std::unique_ptr<DupDataFile> DupDataFile_Ptr;

	class AppBase
	{
	protected:
		std::unique_ptr<SIAArgvParser> m_argvParser;
		bool m_configured;
		std::string  m_configPath;

		AppBase(SIAArgvParser *argvParser) {
			m_argvParser.reset(argvParser);
			m_configured = false;
		}
	public:
		~AppBase();
		virtual bool initaliseHomePath() = 0;
		bool initaliseConfig();
		bool initaliseArgs(int argc, char **argv);
		
		/// @brief This is the main application run function. All application processing starts here.
		bool Run();
		/// @brief Shows the configuration of the archive. All application processing starts here.
		bool Show();
		
		bool isConfiguratedOk() {
			return m_configured;
		}

	protected:
		virtual bool doInitalise(int argc, char **argv) = 0;
		virtual bool doRun() = 0;
		void setConfigPath(const char *configPath) {
			m_configPath = configPath;
		}
	};

}