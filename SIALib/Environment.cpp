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

namespace simplearchive {




	Environment::Environment() {
		// TODO Auto-generated constructor stub

	}

	void Environment::setEnvironment() {
		SetEnv setEnv;
		CAppConfig &config = CAppConfig::get();
		
		setEnv.insert(setEnv.end(), EnvItem(HOOK_SCRIPTS_PATH, config.getHookPath()));
		setEnv.insert(setEnv.end(), EnvItem(TOOLS_PATH, config.getToolsPath()));
		setEnv.insert(setEnv.end(), EnvItem(TEMP_PATH, config.getTempPath()));
		setEnv.insert(setEnv.end(), EnvItem(SOURCE_PATH, config.getSourcePath()));
		setEnv.insert(setEnv.end(), EnvItem(ARCHIVE_PATH, config.getArchivePath()));
		setEnv.insert(setEnv.end(), EnvItem(CONFIG_PATH, config.getConfigPath()));
		setEnv.insert(setEnv.end(), EnvItem(LOG_PATH, config.getLogPath()));
		setEnv.insert(setEnv.end(), EnvItem(HOME_PATH, config.getHomePath()));
		setEnv.insert(setEnv.end(), EnvItem(INDEX_PATH, config.getIndexPath()));
		setEnv.insert(setEnv.end(), EnvItem(HISTORY_PATH, config.getHistoryPath()));
		setEnv.insert(setEnv.end(), EnvItem(EXTERNAL_COMMAND_LINE, config.getExternalCommandLine()));
		setEnv.insert(setEnv.end(), EnvItem(EXIF_MAP_PATH, config.getExifMapPath()));
		setEnv.insert(setEnv.end(), EnvItem(METADATA_TEMPLATE_PATH, config.getMetadataTemplatePath()));

		setEnv.process();

	}




	Environment::~Environment() {
		// TODO Auto-generated destructor stub
	}

} /* namespace simplearchive */
