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

#include "Environment.h"
#include "SetEnv.h"
#include "AppConfig.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

namespace simplearchive {




	Environment::Environment() {
		// TODO Auto-generated constructor stub

	}

	void Environment::setEnvironment() {
		SetEnv setEnv;
		CAppConfig &config = CAppConfig::get();
		
		setEnv.insert(setEnv.end(), EnvItem(HOOK_SCRIPTS_PATH_LABEL, config.getHookPath()));
		setEnv.insert(setEnv.end(), EnvItem(TOOLS_PATH_LABEL, config.getToolsPath()));
		setEnv.insert(setEnv.end(), EnvItem(TEMP_PATH_LABEL, config.getTempPath()));
		setEnv.insert(setEnv.end(), EnvItem(SOURCE_PATH_LABEL, config.getSourcePath()));
		setEnv.insert(setEnv.end(), EnvItem(ARCHIVE_PATH_LABEL, config.getWorkspacePath()));
		setEnv.insert(setEnv.end(), EnvItem(CONFIG_PATH_LABEL, config.getConfigPath()));
		setEnv.insert(setEnv.end(), EnvItem(LOG_PATH_LABEL, config.getLogPath()));
		setEnv.insert(setEnv.end(), EnvItem(HOME_PATH_LABEL, config.getHomePath()));
		setEnv.insert(setEnv.end(), EnvItem(INDEX_PATH_LABEL, config.getIndexPath()));
		setEnv.insert(setEnv.end(), EnvItem(HISTORY_PATH_LABEL, config.getHistoryPath()));
		setEnv.insert(setEnv.end(), EnvItem(EXTERNAL_COMMAND_LINE_LABEL, config.getExternalCommandLine()));
		setEnv.insert(setEnv.end(), EnvItem(EXIF_MAP_PATH_LABEL, config.getExifMapPath()));
		setEnv.insert(setEnv.end(), EnvItem(METADATA_TEMPLATE_PATH_LABEL, config.getMetadataTemplatePath()));

		setEnv.process();

	}




	Environment::~Environment() {
		// TODO Auto-generated destructor stub
	}

} /* namespace simplearchive */
