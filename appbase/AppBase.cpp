#include "stdio.h"
#include <iostream>
#include "AppBase.h"
#include "ReturnCode.h"

namespace CommandLineProcessing {
	


	AppBase::~AppBase()
	{
	}

	bool AppBase::initalise(int argc, char **argv) {
		if (initaliseConfig() == false) {
			return false;
		}
		if (initaliseArgs(argc, argv) == false) {
			std::string  retStr = ReturnCodeObject::getReturnString();
			
			if (!retStr.empty()) {
				m_returnString = retStr;
				m_returnCode = ReturnCodeObject::getReturnCode();
			}
			return false;
		}
		return true;
	};

	

	bool AppBase::Run()
	{
		bool ret = doRun();
		if (ret == false) {
			//std::cout << getFullErrorString() << '\n';
		}
		return ret;
	}

	ExitCode AppBase::RunApp(int argc, char **argv) {
		bool error = false;
		if (initalise(argc, argv) == false) {

			error = true;
		}
		else {
			if (Run() == false) {
				error = true;
			}
		}
		if (error) {
			int code = CommandLineProcessing::AppBase::getError();
			std::cout << CommandLineProcessing::AppBase::getFullErrorString();
			
		}
		else {
			std::cout << "0000:Success";
		}
		return m_exitCode;
	}
	ReturnCode AppBase::m_returnCode;
	std::string AppBase::m_appName;
	std::string AppBase::m_returnString;
	ExitCode AppBase::m_exitCode;
}