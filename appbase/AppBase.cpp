#include "stdio.h"
#include <iostream>
#include "AppBase.h"


namespace CommandLineProcessing {
	


	AppBase::~AppBase()
	{
	}

	bool AppBase::initalise(int argc, char **argv) {

		

		if (initaliseConfig() == false) {
			return false;
		}
		if (initaliseArgs(argc, argv) == false) {
			return false;
		}
		return true;
	};

	

	bool AppBase::Run()
	{
		bool ret = doRun();
		if (ret == false) {
			std::cout << getFullErrorString() << '\n';
		}
		return ret;
	}

	int AppBase::m_error;
	std::string AppBase::m_errorstring;

}