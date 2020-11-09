#include "SetEnviromentVariables.h"
#include "AppConfig.h"
#include "SAUtils.h"

enum class FolderOptions {
	
	WWW_CAT,
	BACKUP_ONE,
	BACKUP_TWO,
	SYSTEM_PATH,
	MASTER_PATH,
	MASTER_BACKPUP_1_PATH,
	PICTURE_PATH,
	DERIVATIVE_PATH,
	HOME_PATH,
	BACKUP_ONE_ENABLED,
	BACKUP_TWO_ENABLED,
	CATALOG_PATH,
	WORKSPACE_PATH,
	UNKNOWN
};

enum class EnabledOptions {
	UNKNOWN
};

FolderOptions SetEnviromentVariables::processFolders(std::string& optionString)
{
	
	if (iequals(optionString, SYSTEM_PATH_LABEL)) {
		return FolderOptions::SYSTEM_PATH;
	}
	else if (iequals(optionString, WORKSPACE_PATH_LABEL)) {
		return FolderOptions::WORKSPACE_PATH;
	}
	else if (iequals(optionString, MASTER_PATH_LABEL)) {
		return FolderOptions::MASTER_PATH;
	}
	else if (iequals(optionString, PICTURE_PATH_LABEL)) {
		return FolderOptions::PICTURE_PATH;
	}
	else if (iequals(optionString, DERIVATIVE_PATH_LABEL)) {
		return FolderOptions::DERIVATIVE_PATH;
	}
	
	return FolderOptions::UNKNOWN;
}

EnabledOptions SetEnviromentVariables::processEnableOptions(std::string& optionString)
{
    return EnabledOptions();
}

EnabledOptions SetEnviromentVariables::processDisableOptions(std::string& optionString)
{
    return EnabledOptions();
}

bool SetEnviromentVariables::parseFolders(const char* optionString)
{
	if (!processArgs(optionString)) {
		return false;
	}
	m_optionBlock = FOLDERS_BLOCK;
	FolderOptions ret = processFolders(m_option);
	switch (ret) {
	case FolderOptions::HOOK_SCRIPTS_PATH: // Path to hook scripts
		m_option = HOOK_SCRIPTS_PATH_LABEL;
		return true;
	default:
		return false;
	}
	return true;
}

bool SetEnviromentVariables::parseEnableOptions(const char* optionString)
{
    return false;
}

bool SetEnviromentVariables::parseDisableOptions(const char* optionString)
{
    return false;
}
