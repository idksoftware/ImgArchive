/*
* Enviroment.h
*
*  Created on: Dec 9, 2014
*      Author: wzw7yn
*/

#ifndef ENVIRONMENT_H_
#define ENVIRONMENT_H_
#include <string>

namespace simplearchive {

#define HOOK_SCRIPTS_PATH       "HookScripsPath"
#define TOOLS_PATH           	"ToolsPath"
#define TEMP_PATH           	"TempPath"
#define SOURCE_PATH         	"SourcePath"
#define ARCHIVE_PATH         	"ArchivePath"
	/**
	*
	* @brief The Environment object is used to pass the application environment
	*        into the shell that the Hook scripts will possibly use.
	*
	*/
	class Environment {
	
	public:
		Environment();
		virtual ~Environment();
		static void setEnvironment();
	};

} /* namespace simplearchive */
#endif /* ENVIRONMENT_H_ */
