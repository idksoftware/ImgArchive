#include "stdio.h"
#include "AppBase.h"

namespace CommandLineProcessing {
	


	AppBase::~AppBase()
	{
	}

	bool AppBase::initaliseArgs(int argc, char **argv) {
		return doInitalise(argc, argv);
	};

	

	bool AppBase::Run()
	{
		// Find if the archive exists
		/*
		AppOptions &appOptions = AppOptions::get();
		if (appOptions.isConfiguratedOk() == false) {
			if (appOptions.getCommandMode() == AppOptions::CM_Show) {
				Show();
				return false;
			}
			if (appOptions.getCommandMode() == AppOptions::CM_InitArchive) {

				if (CreateArchive(appOptions.getHomePath(), appOptions.getWorkspacePath(), appOptions.getShadowPath(), appOptions.getUsers()) == false) {
					return false;
				}
				printf("\n\Completed initalising the Archive\n");
				return true;
			}
			return false;
		}
		*/
		return true;
	}

	
}