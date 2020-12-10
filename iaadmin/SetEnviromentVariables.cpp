#include "SetEnviromentVariables.h"
#include "AppConfig.h"
#include "SAUtils.h"
#include "AppPaths.h"

enum class FolderOptions
{
	FO_HOME_PATH,
	FO_MASTER_PATH,
	FO_MASTER_BACKUP_ONE,
	FO_MASTER_BACKUP_TWO,
	FO_DERIVATIVE_PATH,
	FO_DERIVATIVE_BACKUP_ONE,
	FO_DERIVATIVE_BACKUP_TWO,
	FO_PICTURE_PATH,
	FO_WWW_CAT,
	FO_WORKSPACE_PATH,
	FO_UNKNOWN
};

FolderOptions SetEnviromentVariables::processFolders(std::string& optionString)
{
	if (iequals(optionString, HOME_PATH_LABEL))
	{
		return FolderOptions::FO_HOME_PATH;
	}
	if (iequals(optionString, MASTER_PATH_LABEL))
	{
		return FolderOptions::FO_MASTER_PATH;
	}
	if (iequals(optionString, MASTER_BACKUP_ONE_LABEL))
	{
		return FolderOptions::FO_MASTER_BACKUP_ONE;
	}
	if (iequals(optionString, MASTER_BACKUP_TWO_LABEL))
	{
		return FolderOptions::FO_MASTER_BACKUP_TWO;
	}
	if (iequals(optionString, MASTER_PATH_LABEL))
	{
		return FolderOptions::FO_DERIVATIVE_PATH;
	}
	if (iequals(optionString, DERIVATIVE_BACKUP_ONE_LABEL))
	{
		return FolderOptions::FO_DERIVATIVE_BACKUP_ONE;
	}
	if (iequals(optionString, DERIVATIVE_BACKUP_TWO_LABEL))
	{
		return FolderOptions::FO_DERIVATIVE_BACKUP_TWO;
	}
	if (iequals(optionString, PICTURE_PATH_LABEL))
	{
		return FolderOptions::FO_PICTURE_PATH;
	}
	if (iequals(optionString, WWW_CAT_LABEL))
	{
		return FolderOptions::FO_WWW_CAT;
	}
	if (iequals(optionString, WORKSPACE_PATH_LABEL))
	{
		return FolderOptions::FO_WORKSPACE_PATH;
	}
	return FolderOptions::FO_UNKNOWN;
}

bool SetEnviromentVariables::parseFolders(const char* optionString)
{
	if (!processArgs(optionString))
	{
		return false;
	}
	m_optionBlock = FOLDERS_BLOCK;
	FolderOptions ret = processFolders(m_option);
	switch (ret)
	{
	case FolderOptions::FO_HOME_PATH: // Path to hook scripts
		m_option = IMGARCHIVE_HOME;
		return true;
	case FolderOptions::FO_MASTER_PATH: // Path to hook scripts
		m_option = IMGA_MASTER;
		return true;
	case FolderOptions::FO_MASTER_BACKUP_ONE: // Path to hook scripts
		m_option = IMGA_MASTER_BACKUP1;
		return true;
	case FolderOptions::FO_MASTER_BACKUP_TWO: // Path to hook scripts
		m_option = IMGA_MASTER_BACKUP2;
		return true;
	case FolderOptions::FO_DERIVATIVE_PATH: // Path to hook scripts
		m_option = IMGA_DERIVATIVE;
		return true;
	case FolderOptions::FO_DERIVATIVE_BACKUP_ONE: // Path to hook scripts
		m_option = IMGA_DERIVATIVE_BACKUP1;
		return true;
	case FolderOptions::FO_DERIVATIVE_BACKUP_TWO: // Path to hook scripts
		m_option = IMGA_DERIVATIVE_BACKUP2;
		return true;
	case FolderOptions::FO_PICTURE_PATH: // Path to hook scripts
		m_option = IMGA_PICTURE;
		return true;
	case FolderOptions::FO_WWW_CAT: // Path to hook scripts
		m_option = IMGA_WWWIMAGE;
		return true;
	case FolderOptions::FO_WORKSPACE_PATH: // Path to hook scripts
		m_option = IMGA_WORKSPACE;
		return true;
	default:
		return false;
	}
	return true;
}

bool SetEnviromentVariables::processArgs(const char* ov)
{
	std::string optionValueString = ov;
	size_t pos = optionValueString.find_first_of('=');
	if (pos == std::string::npos)
	{
		return false;
	}
	m_option = optionValueString.substr(0, pos);
	m_value = optionValueString.substr(pos + 1, optionValueString.length() - 1);

	return true;
}


bool SetEnviromentVariables::parseEnableOptions(const char* optionString, const char* value)
{
	m_value = value;
	if (iequals(optionString, MASTER_BACKUP_ONE_LABEL))
	{
		m_option = IMGA_MASTER_BACKUP1_ENABLE;
		return true;
	}
	if (iequals(optionString, MASTER_BACKUP_ONE_LABEL))
	{
		m_option = IMGA_MASTER_BACKUP2_ENABLE;
		return true;
	}
	if (iequals(optionString, DERIVATIVE_BACKUP_ONE_LABEL))
	{
		m_option = IMGA_DERIVATIVE_BACKUP1_ENABLE;
		return true;
	}
	if (iequals(optionString, DERIVATIVE_BACKUP_TWO_LABEL))
	{
		m_option = IMGA_DERIVATIVE_BACKUP2_ENABLE;
		return true;
	}
	if (iequals(optionString, WORKSPACE_LABEL))
	{
		m_option = IMGA_WORKSPACE_ENABLE;
		return true;
	}
	if (iequals(optionString, WORKSPACE_CHECKOUT_LABEL))
	{
		m_option = IMGA_WORKSPACE_CHKOUT;
		return true;
	}
	if (iequals(optionString, PICTURE_PATH_LABEL))
	{
		m_option = IMGA_PICTURE_ENABLE;
		return true;
	}
	if (iequals(optionString, WWW_CAT_LABEL))
	{
		m_option = IMGA_WWWIMAGE_ENABLE;
		return true;
	}
	return false;
}

bool SetEnviromentVariables::parseEnableOptions(const char* optionString)
{
	return parseEnableOptions(optionString, "enable");
}

bool SetEnviromentVariables::parseDisableOptions(const char* optionString)
{
	return parseEnableOptions(optionString, "disable");
}

bool SetEnviromentVariables::setVariable()
{
	bool user = false;
	if (m_userMode == UserMode::All)
	{
		user = true;
	}
	bool r = SAUtils::SetEnv(m_option, m_value, user);
	return r;
}

UserMode SetEnviromentVariables::userMode(const char* ua)
{
	if (iequals(ua, USER_LOCAL))
	{
		m_userMode = UserMode::Local;
		return UserMode::Local;
	}
	if (iequals(ua, USER_ALL))
	{
		m_userMode = UserMode::All;
		return UserMode::All;
	}
	return UserMode::Invalid;
}

UserMode SetEnviromentVariables::userMode()
{
	if (SAUtils::IsAdminMode() == false)
	{
		m_userMode = UserMode::Local;
		return UserMode::Local;
	}
	m_userMode = UserMode::All;
	return UserMode::All;
}
