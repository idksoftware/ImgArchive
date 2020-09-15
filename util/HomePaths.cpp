#include "HomePaths.h"
#include <string>
#include <vector>
#include "SAUtils.h"
#include "AppPaths.h"

bool HomePathsBase::m_first = true;
std::string HomePathsBase::m_allUsersHomeEnvironmentPath;
std::string HomePathsBase::m_myselfHomeEnvironmentPath;
std::string HomePathsBase::m_allusersHomeDefaultPath;
std::string HomePathsBase::m_myselfHomeDefaultPath;
std::string HomePathsBase::m_homeDrive;
std::string HomePathsBase::m_homePathEnv;

std::vector<HomePathsBase*> ImgArchiveHome::m_list;
std::string ImgArchiveHome::m_imgArchiveHome;

void ImgArchiveHome::initHomePaths() {
	
	HomePathsBase* path = &(MasterPath::getObject());
	m_list.push_back(path);
	path = &(MasterBackupOnePath::getObject());
	m_list.push_back(path);
	path = &(MasterBackupTwoPath::getObject());
	m_list.push_back(path);
	path = &(DerivativePath::getObject());
	m_list.push_back(path);
	path = &(DerivativeBackupOnePath::getObject());
	m_list.push_back(path);
	path = &(DerivativeBackupTwoPath::getObject());
	m_list.push_back(path);
	path = &(WorkspacePath::getObject());
	m_list.push_back(path);
	path = &(PicturePath::getObject());
	m_list.push_back(path);
	path = &(WWWImagePath::getObject());
	m_list.push_back(path);

}

const std::string& ImgArchiveHome::getImgArchiveHome() {
	return m_imgArchiveHome;
}

bool ImgArchiveHome::init() {

	bool res = true;
	for (auto i = m_list.begin(); i != m_list.end(); i++) {
		auto hp = *i;
		if (hp->init() == false) {
			m_error = HPError::initaliseError;
			res = false;
		}
	}

	return res;
}

bool ImgArchiveHome::setArchiveHome()
{
	// Set Windows Defaults (they can be overridden later)
	std::string allUsersHomeEnvironmentPath = SAUtils::GetEnv(IMGARCHIVE_HOME, true);
	std::string myselfHomeEnvironmentPath = SAUtils::GetEnv(IMGARCHIVE_HOME, false);
	// All Users
	std::string allusersHomeDefaultPath = SAUtils::GetPOSIXEnv("ProgramData");
	allusersHomeDefaultPath += DEFAULT_ALLUSER_HOME_PATH;
	std::string myselfHomeDefaultPath = SAUtils::GetPOSIXEnv("LOCALAPPDATA");
	myselfHomeDefaultPath += DEFAULT_LOCAL_HOME_PATH;

	// Looking the HKEY_LOCAL_MACHINE first
	if (allUsersHomeEnvironmentPath.empty() == false) {
		m_type = HomePathType::SystemEnv;	// System Environment set
		m_found = true;
		m_imgArchiveHome = allUsersHomeEnvironmentPath;
	}
	else if (myselfHomeEnvironmentPath.empty() == false) {
		m_type = HomePathType::LocalEnv;
		m_found = true;
		m_imgArchiveHome = myselfHomeEnvironmentPath;
	}
	else if (SAUtils::DirExists(allusersHomeDefaultPath.c_str()) == true) {
		m_imgArchiveHome = allusersHomeDefaultPath;
		m_type = HomePathType::AllUsers;
		m_found = true;

	}
	else if (SAUtils::DirExists(myselfHomeDefaultPath.c_str()) == true) {
		m_imgArchiveHome = myselfHomeDefaultPath;
		m_type = HomePathType::UserOnly;
		m_found = true;

	}
	else {
		m_error = HPError::NotFound;
		return false;
	}

	if (m_type == HomePathType::SystemEnv) {
		if (SAUtils::DirExists(m_imgArchiveHome.c_str()) == false) {
			m_error = HPError::CannotLocatePath;
			return false;
		}

	}
	else if (m_type == HomePathType::LocalEnv) {

		if (SAUtils::DirExists(m_imgArchiveHome.c_str()) == false) {
			m_error = HPError::CannotLocatePath;
			return false;
		}

	}
	m_valid = true;
	return true;
}

bool HomePathsBase::loadEnv() {
	
	HomePathsBase::m_allUsersHomeEnvironmentPath = SAUtils::GetEnv(IMGARCHIVE_HOME, true);
	HomePathsBase::m_myselfHomeEnvironmentPath = SAUtils::GetEnv(IMGARCHIVE_HOME, false);
	// All Users
	HomePathsBase::m_allusersHomeDefaultPath = SAUtils::GetPOSIXEnv("ProgramData");
	HomePathsBase::m_myselfHomeDefaultPath = SAUtils::GetPOSIXEnv("LOCALAPPDATA");

	HomePathsBase::m_homeDrive = SAUtils::GetPOSIXEnv("HOMEDRIVE");
	HomePathsBase::m_homePathEnv = SAUtils::GetPOSIXEnv("HOMEPATH");

	return true;
}

bool HomePathsBase::baseInit(std::string &path, const char *relativePath, const char *enviromentPath)
{

	path = ImgArchiveHome::getImgArchiveHome();
	path += relativePath;
	m_type = HomePathType::Default;
	// Set Windows Defaults (they can be overridden later)
	std::string allUsersHomeEnvironmentPath = SAUtils::GetEnv(enviromentPath, true);
	std::string myselfHomeEnvironmentPath = SAUtils::GetEnv(enviromentPath, false);
	// Looking the HKEY_LOCAL_MACHINE first
	if (allUsersHomeEnvironmentPath.empty() == false) {
		m_type = HomePathType::SystemEnv;	// System Environment set
		path = allUsersHomeEnvironmentPath;

	}
	else if (myselfHomeEnvironmentPath.empty() == false) {
		m_type = HomePathType::LocalEnv;
		path = myselfHomeEnvironmentPath;
	}
	m_found = true;
	if (SAUtils::DirExists(path.c_str()) == false) {
		m_error = HPError::CannotLocatePath;
		return false;

	}
	m_valid = true;
	return true;
}



/**
	Master Path
*/

bool MasterPath::init()
{
	return baseInit(m_path, MASTER_PATH, IMGA_MASTER);
}

std::string MasterPath::m_path;

const std::string& MasterPath::get()
{
	return m_path;
}

bool MasterPath::setPath(const char* p)
{
	m_path = p;
	return true;
}



/**
	Master backup 1 Path
*/

bool MasterBackupOnePath::init()
{
	return baseInit(m_path, DEFAULT_MASTER_BACKUP1_PATH, IMGA_MASTER_BACKUP1);
}

std::string MasterBackupOnePath::m_path;

const std::string& MasterBackupOnePath::get()
{
	return m_path;
}

bool MasterBackupOnePath::setPath(const char* p)
{
	m_path = p;
	return true;
}

/**
	Master backup 2 Path
*/

bool MasterBackupTwoPath::init()
{
	return baseInit(m_path, DEFAULT_MASTER_BACKUP2_PATH, IMGA_MASTER_BACKUP2);
}

std::string MasterBackupTwoPath::m_path;

const std::string& MasterBackupTwoPath::get()
{
	return m_path;
}

bool MasterBackupTwoPath::setPath(const char* p)
{
	m_path = p;
	return true;
}

/**
	Derivative Path
*/

bool DerivativePath::init()
{
	return baseInit(m_path, DERIVATIVE_PATH, IMGA_DERIVATIVE);
}

std::string DerivativePath::m_path;


const std::string& DerivativePath::get()
{
	return m_path;
}


bool DerivativePath::setPath(const char* p)
{
	m_path = p;
	return true;
}

/**
	DerivativeBackupOne Path
*/

bool DerivativeBackupOnePath::init()
{
	return baseInit(m_path, DEFAULT_DERIVATIVE_BACKUP1_PATH, IMGA_DERIVATIVE_BACKUP1);
}

std::string DerivativeBackupOnePath::m_path;

const std::string& DerivativeBackupOnePath::get()
{
	return m_path;
}

bool DerivativeBackupOnePath::setPath(const char* p)
{
	m_path = p;
	return true;
}

/**
	DerivativeBackupTwo Path
*/

bool DerivativeBackupTwoPath::init()
{
	return baseInit(m_path, DEFAULT_DERIVATIVE_BACKUP2_PATH, IMGA_DERIVATIVE_BACKUP2);
}

std::string DerivativeBackupTwoPath::m_path;

const std::string& DerivativeBackupTwoPath::get()
{
	return m_path;
}

bool DerivativeBackupTwoPath::setPath(const char* p)
{
	m_path = p;
	return true;
}

/**
	Workspace Path
*/

std::string WorkspacePath::m_path;


bool WorkspacePath::init()
{
	
	std::string tempHomeDrive = m_homeDrive;
	std::string tempHomePath = m_homePathEnv;
	

	// Set Windows Defaults (they can be overridden later)
	std::string allUsersHomeEnvironmentPath = SAUtils::GetEnv(IMGA_WORKSPACE, true);
	std::string myselfHomeEnvironmentPath = SAUtils::GetEnv(IMGA_WORKSPACE, false);
	
	m_path = tempHomeDrive + tempHomePath + DEFAULT_ALLUSER_WORKSPACE_PATH;
	m_type = HomePathType::Default;
	// Looking the HKEY_LOCAL_MACHINE first
	if (allUsersHomeEnvironmentPath.empty() == false) {
		m_type = HomePathType::SystemEnv;	// System Environment set
		m_path = allUsersHomeEnvironmentPath;
		m_found = true;
	}
	else if (myselfHomeEnvironmentPath.empty() == false) {
		m_type = HomePathType::LocalEnv;
		m_path = myselfHomeEnvironmentPath;
		m_found = true;
	}
	else if (SAUtils::DirExists(m_path.c_str()) == true) {
		m_error = HPError::CannotLocatePath;
		m_found = true;
	}
	else {
		m_error = HPError::NotFound;
		return false;
	}
	m_valid = true;
	return true;
}




const std::string& WorkspacePath::get()
{
	return m_path;
}



bool WorkspacePath::setPath(const char* p)
{
	m_path = p;
	return true;
}


/**
	Picture Path
*/

std::string PicturePath::m_path;

bool PicturePath::init()
{

	std::string tempHomeDrive = m_homeDrive;
	std::string tempHomePath = m_homePathEnv;

	// Set Windows Defaults (they can be overridden later)
	std::string allUsersHomeEnvironmentPath = SAUtils::GetEnv(IMGA_PICTURE, true);
	std::string myselfHomeEnvironmentPath = SAUtils::GetEnv(IMGA_PICTURE, false);

	std::string allusersHomeDefaultPath = tempHomeDrive + tempHomePath + DEFAULT_ALLUSER_PICTURE_PATH;
	std::string myselfHomeDefaultPath = tempHomeDrive + tempHomePath + DEFAULT_LOCAL_PICTURE_PATH;


	// Looking the HKEY_LOCAL_MACHINE first
	if (allUsersHomeEnvironmentPath.empty() == false) {
		m_type = HomePathType::SystemEnv;	// System Environment set
		m_path = allUsersHomeEnvironmentPath;
		m_found = true;
	}
	else if (myselfHomeEnvironmentPath.empty() == false) {
		m_type = HomePathType::LocalEnv;
		m_path = myselfHomeEnvironmentPath;
		m_found = true;
	}
	else if (SAUtils::DirExists(allusersHomeDefaultPath.c_str()) == true) {
		m_path = allusersHomeDefaultPath;
		m_type = HomePathType::AllUsers;
		m_found = true;
	}
	else if (SAUtils::DirExists(myselfHomeDefaultPath.c_str()) == true) {

		m_path = myselfHomeDefaultPath;
		m_type = HomePathType::UserOnly;
		m_found = true;
	}
	else {
		m_error = HPError::NotFound;
		return false;
	}

	if (m_type == HomePathType::SystemEnv) {
		if (SAUtils::DirExists(m_path.c_str()) == false) {
			m_error = HPError::CannotLocatePath;
			return false;
		}
	}
	else if (m_type == HomePathType::LocalEnv) {

		if (SAUtils::DirExists(m_path.c_str()) == false) {
			m_error = HPError::CannotLocatePath;
			return false;
		}

	}
	m_valid = true;
	return true;
}

const std::string& PicturePath::get()
{
	return m_path;
}


bool PicturePath::setPath(const char* p)
{
	m_path = p;
	return true;
}
/**
	WWWImage Path
*/

std::string WWWImagePath::m_path;

bool WWWImagePath::init()
{
	// Set Windows Defaults (they can be overridden later)
	std::string allUsersHomeEnvironmentPath = SAUtils::GetEnv(IMGA_WWWIMAGE, true);
	std::string myselfHomeEnvironmentPath = SAUtils::GetEnv(IMGA_WWWIMAGE, false);

	std::string allusersHomeDefaultPath = m_allusersHomeDefaultPath + DEFAULT_ALLUSER_WWWIMAGE_PATH;

#ifdef WIN32
	std::string myselfHomeDefaultPath = m_myselfHomeDefaultPath + DEFAULT_LOCAL_WWWIMAGE_PATH;
#else
	std::string myselfHomeDefaultPath = SAUtils::GetPOSIXEnv("HOME") + DEFAULT_LOCAL_WWWIMAGE_PATH;
#endif
	// Looking the HKEY_LOCAL_MACHINE first
	if (allUsersHomeEnvironmentPath.empty() == false) {
		m_type = HomePathType::SystemEnv;	// System Environment set
		m_path = allUsersHomeEnvironmentPath;
		m_found = true;
	}
	else if (myselfHomeEnvironmentPath.empty() == false) {
		m_type = HomePathType::LocalEnv;
		m_path = myselfHomeEnvironmentPath;
		m_found = true;
	}
	else if (SAUtils::DirExists(allusersHomeDefaultPath.c_str()) == true) {
		m_path = allusersHomeDefaultPath;
		m_type = HomePathType::AllUsers;
		m_found = true;
	}
	else if (SAUtils::DirExists(myselfHomeDefaultPath.c_str()) == true) {

		m_path = myselfHomeDefaultPath;
		m_type = HomePathType::UserOnly;
		m_found = true;
	}
	else {
		m_error = HPError::NotFound;
		return false;
	}

	if (m_type == HomePathType::SystemEnv) {
		if (SAUtils::DirExists(m_path.c_str()) == false) {
			m_error = HPError::CannotLocatePath;
			return false;
		}
	}
	else if (m_type == HomePathType::LocalEnv) {

		if (SAUtils::DirExists(m_path.c_str()) == false) {
			m_error = HPError::CannotLocatePath;
			return false;
		}

	}
	m_valid = true;
	return true;
}



const std::string& WWWImagePath::get()
{
	return m_path;
}
