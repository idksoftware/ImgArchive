/* **************************************************
**
**    III                DDD  KKK
**    III                DDD  KKK
**                       DDD  KKK
**    III   DDDDDDDDDDD  DDD  KKK            KKK
**    III  DDD           DDD  KKK            KKK
**    III  DDD           DDD  KKK           KKK
**    III  DDD           DDD  KKK        KKKKKK
**    III  DDD           DDD  KKK   KKKKKKKKK
**    III  DDD           DDD  KKK        KKKKKK
**    III  DDD           DDD  KKK           KKK
**    III  DDD           DDD  KKK            KKK
**    III   DDDDDDDDDDDDDDDD  KKK            KKK
**
**
**     SSS         FF
**    S           F   T
**     SSS   OO   FF  TTT W   W  AAA  R RR   EEE
**        S O  O  F   T   W W W  AAAA RR  R EEEEE
**    S   S O  O  F   T   W W W A   A R     E
**     SSS   OO  FFF   TT  W W   AAAA R      EEE
**
**    Copyright: (c) 2015 IDK Software Ltd
**
****************************************************
**
**	Filename	: CRegString.cpp
**	Author		: I.Ferguson
**	Version		: 1.000
**	Date		: 26-05-2015
**
** #$$@@$$# */

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
