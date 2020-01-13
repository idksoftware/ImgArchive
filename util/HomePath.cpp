#include "HomePath.h"
#include <string>
#include <vector>
#include "SAUtils.h"
#include "AppPaths.h"

std::string HomePath::m_homePath;
bool HomePath::m_found = false;	// string found
bool HomePath::m_valid = false;	// in file system
HPError HomePath::m_error = HPError::Unknown;
HomePathType HomePath::m_type = HomePathType::Unknown;

bool HomePath::init()
{

	// Set Windows Defaults (they can be overridden later)
	std::string allUsersHomeEnvironmentPath = SAUtils::GetEnv(IMGARCHIVE_HOME, true);
	std::string myselfHomeEnvironmentPath = SAUtils::GetEnv(IMGARCHIVE_HOME, false);
	// All Users
	std::string allusersHomeDefaultPath = SAUtils::GetPOSIXEnv("ProgramData");
	allusersHomeDefaultPath += ALLUSERS_DEFAULT_CONFIG_PATH;
	std::string myselfHomeDefaultPath = SAUtils::GetPOSIXEnv("LOCALAPPDATA");
	myselfHomeDefaultPath += MYSELF_DEFAULT_CONFIG_PATH;

	// Looking the HKEY_LOCAL_MACHINE first
	if (allUsersHomeEnvironmentPath.empty() == false) {
		m_type = HomePathType::SystemEnv;	// System Environment set
		m_homePath = allUsersHomeEnvironmentPath;
		m_found = true;
	}
	else if (myselfHomeEnvironmentPath.empty() == false) {
		m_type = HomePathType::LocalEnv;
		m_homePath = myselfHomeEnvironmentPath;
		m_found = true;
	}
	else if (SAUtils::DirExists(allusersHomeDefaultPath.c_str()) == true) {
		m_homePath = allusersHomeDefaultPath;
		m_type = HomePathType::AllUsers;
		m_found = true;
	}
	else if (SAUtils::DirExists(myselfHomeDefaultPath.c_str()) == true) {
		
		m_homePath = myselfHomeDefaultPath;
		m_type = HomePathType::UserOnly;
		m_found = true;
	}
	else {
		m_error = HPError::NotFound;
		return false;
	}
	
	if (m_type == HomePathType::SystemEnv) {
		if (SAUtils::DirExists(m_homePath.c_str()) == false) {
			m_error = HPError::CannotLocatePath;
			return false;
		}
	}
	else if (m_type == HomePathType::LocalEnv) {
		
		if (SAUtils::DirExists(m_homePath.c_str()) == false) {
			m_error = HPError::CannotLocatePath;
			return false;
		}
		
	}
	m_valid = true;
	return true;
}

std::string HomePath::get()
{
	return m_homePath;
}

HomePathType HomePath::type()
{
	return m_type;
}


