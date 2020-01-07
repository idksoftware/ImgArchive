#include "stdio.h"
#include <iostream>
#include "AppBase.h"


namespace CommandLineProcessing {
	


	AppBase::~AppBase()
	{
	}

	bool AppBase::initalise(int argc, char **argv) {
		if (initaliseArgs(argc, argv) == false) {
			return false;
		}
		if (initaliseConfig() == false) {
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

	int AppBase::RunApp(int argc, char **argv) {
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
			return code;
		}
		else {
			return 0;
		}
	}


	int AppBase::m_error;
	std::string AppBase::m_errorstring;

}