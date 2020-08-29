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
std::string HomePathsBase::m_homePath;


std::vector<HomePathsBase&> InitialiseHomePaths::m_list;

void InitialiseHomePaths::initHomePaths() {
	/*
	HomePathsBase& path = HomePath::getObject();
	m_list.push_back(path);
	path = MasterPath::getObject();
	m_list.push_back(path);
	path = DerivativePath::getObject();
	m_list.push_back(path);
	path = WorkspacePath::getObject();
	m_list.push_back(path);
	path = PicturePath::getObject();
	m_list.push_back(path);
	path = WWWImagePath::getObject();
	m_list.push_back(path);
	*/
}


bool HomePathsBase::loadEnv() {
	HomePathsBase::m_allUsersHomeEnvironmentPath = SAUtils::GetEnv(IMGARCHIVE_HOME, true);
	HomePathsBase::m_myselfHomeEnvironmentPath = SAUtils::GetEnv(IMGARCHIVE_HOME, false);
	// All Users
	HomePathsBase::m_allusersHomeDefaultPath = SAUtils::GetPOSIXEnv("ProgramData");
	HomePathsBase::m_myselfHomeDefaultPath = SAUtils::GetPOSIXEnv("LOCALAPPDATA");

	HomePathsBase::m_homeDrive = SAUtils::GetPOSIXEnv("HOMEDRIVE");
	HomePathsBase::m_homePath = SAUtils::GetPOSIXEnv("HOMEPATH");
	return true;
}

std::string HomePath::m_homePath;

bool HomePath::init()
{

	// Set Windows Defaults (they can be overridden later)
	std::string allUsersHomeEnvironmentPath = m_allUsersHomeEnvironmentPath;
	std::string myselfHomeEnvironmentPath = m_myselfHomeEnvironmentPath;
	// All Users
	std::string allusersHomeDefaultPath = m_allusersHomeDefaultPath;
	allusersHomeDefaultPath += DEFAULT_ALLUSER_HOME_PATH;
	std::string myselfHomeDefaultPath = m_myselfHomeDefaultPath;
	myselfHomeDefaultPath += DEFAULT_LOCAL_HOME_PATH;

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



bool HomePath::setLocalUserDefaultHome() {
#ifdef WIN32
	std::string myselfHomeDefaultPath = m_myselfHomeDefaultPath;
#else
	std::string myselfHomeDefaultPath = SAUtils::GetPOSIXEnv("HOME");
#endif
	myselfHomeDefaultPath += DEFAULT_LOCAL_HOME_PATH;
	m_homePath = myselfHomeDefaultPath;


	m_found = true;
	if (SAUtils::DirExists(m_homePath.c_str()) == false) {
		return false;
	}
	m_valid = true;
	return true;
}

bool HomePath::setAllUserDefaultHome() {
	std::string allusersHomeDefaultPath = m_allusersHomeDefaultPath;
	allusersHomeDefaultPath += DEFAULT_ALLUSER_HOME_PATH;
	m_homePath = allusersHomeDefaultPath;
	m_found = true;
	if (SAUtils::DirExists(m_homePath.c_str()) == false) {
		
		return false;
	}
	m_valid = true;
	return true;
}

bool HomePath::setPath(const char* p)
{
	m_homePath = p;
	return true;
}
/**

*/

std::string MasterPath::m_homePath;

bool MasterPath::init()
{

	// Set Windows Defaults (they can be overridden later)
	std::string allUsersHomeEnvironmentPath = m_allUsersHomeEnvironmentPath;
	std::string myselfHomeEnvironmentPath = m_myselfHomeEnvironmentPath;
	// All Users
	std::string allusersHomeDefaultPath = m_allusersHomeDefaultPath;
	allusersHomeDefaultPath += DEFAULT_ALLUSER_MASTER_PATH;
	std::string myselfHomeDefaultPath = m_myselfHomeDefaultPath;
	myselfHomeDefaultPath += DEFAULT_LOCAL_MASTER_PATH;

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

std::string MasterPath::get()
{
	if (m_found == false) {
		init();
	}
	return m_homePath;
}


bool MasterPath::setLocalUserDefaultHome() {

#ifdef WIN32
	std::string myselfHomeDefaultPath = m_myselfHomeDefaultPath;
#else
	std::string myselfHomeDefaultPath = SAUtils::GetPOSIXEnv("HOME");
#endif
	myselfHomeDefaultPath += DEFAULT_LOCAL_MASTER_PATH;
	m_homePath = myselfHomeDefaultPath;
	m_found = true;
	if (SAUtils::DirExists(m_homePath.c_str()) == false) {
		return false;
	}
	m_valid = true;
	return true;
}

bool MasterPath::setAllUserDefaultHome() {
	std::string allusersHomeDefaultPath = m_allusersHomeDefaultPath;
	allusersHomeDefaultPath += DEFAULT_ALLUSER_MASTER_PATH;
	m_homePath = allusersHomeDefaultPath;
	m_found = true;
	if (SAUtils::DirExists(m_homePath.c_str()) == false) {

		return false;
	}
	m_valid = true;
	return true;
}

bool MasterPath::setPath(const char* p)
{
	m_homePath = p;
	return true;
}

std::string DerivativePath::m_homePath;


bool DerivativePath::init()
{

	// Set Windows Defaults (they can be overridden later)
	std::string allUsersHomeEnvironmentPath = m_allUsersHomeEnvironmentPath;
	std::string myselfHomeEnvironmentPath = m_myselfHomeEnvironmentPath;
	// All Users
	std::string allusersHomeDefaultPath = m_allusersHomeDefaultPath;
	allusersHomeDefaultPath += DEFAULT_ALLUSER_DERIVATIVE_PATH;
#ifdef WIN32
	std::string myselfHomeDefaultPath = m_myselfHomeDefaultPath;
#else
	std::string myselfHomeDefaultPath = SAUtils::GetPOSIXEnv("HOME");
#endif

	myselfHomeDefaultPath += DEFAULT_LOCAL_DERIVATIVE_PATH;

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

std::string DerivativePath::get()
{
	
	return m_homePath;
}


bool DerivativePath::setLocalUserDefaultHome() {
#ifdef WIN32
	std::string myselfHomeDefaultPath = m_myselfHomeDefaultPath;
#else
	std::string myselfHomeDefaultPath = SAUtils::GetPOSIXEnv("HOME");
#endif
	myselfHomeDefaultPath += DEFAULT_LOCAL_DERIVATIVE_PATH;
	m_homePath = myselfHomeDefaultPath;
	m_found = true;
	if (SAUtils::DirExists(m_homePath.c_str()) == false) {
		return false;
	}
	m_valid = true;
	return true;
}

bool DerivativePath::setAllUserDefaultHome() {
	std::string allusersHomeDefaultPath = m_allusersHomeDefaultPath;
	allusersHomeDefaultPath += DEFAULT_ALLUSER_DERIVATIVE_PATH;
	m_homePath = allusersHomeDefaultPath;
	m_found = true;
	if (SAUtils::DirExists(m_homePath.c_str()) == false) {

		return false;
	}
	m_valid = true;
	return true;
}

bool DerivativePath::setPath(const char* p)
{
	m_homePath = p;
	return true;
}
/**
	Workspace Path
*/

std::string WorkspacePath::m_homePath;


bool WorkspacePath::setLocalUserDefaultHome() {

	std::string tempHomeDrive = SAUtils::GetPOSIXEnv("HOMEDRIVE");
	std::string tempHomePath = SAUtils::GetPOSIXEnv("HOMEPATH");
	// Set Windows Defaults (they can be overridden later)
	std::string myselfHomeEnvironmentPath = SAUtils::GetEnv(IMGA_WORKSPACE, false);
	std::string myselfHomeDefaultPath = tempHomeDrive + tempHomePath + DEFAULT_LOCAL_WORKSPACE_PATH;
	if (myselfHomeEnvironmentPath.empty()) {
		m_homePath = myselfHomeDefaultPath;
	}
	else {
		m_homePath = myselfHomeEnvironmentPath;
	}
	m_found = true;
	if (SAUtils::DirExists(m_homePath.c_str()) == false) {
		return false;
	}
	m_valid = true;
	return true;
}

bool WorkspacePath::setAllUserDefaultHome() {

	std::string tempHomeDrive = SAUtils::GetPOSIXEnv("HOMEDRIVE");
	std::string tempHomePath = SAUtils::GetPOSIXEnv("HOMEPATH");
	// Set Windows Defaults (they can be overridden later)
	std::string allUsersHomeEnvironmentPath = SAUtils::GetEnv(IMGA_WORKSPACE, true);
	std::string allusersHomeDefaultPath = tempHomeDrive + tempHomePath + DEFAULT_ALLUSER_WORKSPACE_PATH;
	if (allUsersHomeEnvironmentPath.empty()) {
		m_homePath = allusersHomeDefaultPath;
	}
	else {
		m_homePath = allUsersHomeEnvironmentPath;
	}
	m_found = true;
	if (SAUtils::DirExists(m_homePath.c_str()) == false) {

		return false;
	}
	m_valid = true;
	return true;
}

bool WorkspacePath::init()
{
	
	std::string tempHomeDrive = SAUtils::GetPOSIXEnv("HOMEDRIVE");
	std::string tempHomePath = SAUtils::GetPOSIXEnv("HOMEPATH");
	

	// Set Windows Defaults (they can be overridden later)
	std::string allUsersHomeEnvironmentPath = SAUtils::GetEnv(IMGA_WORKSPACE, true);
	std::string myselfHomeEnvironmentPath = SAUtils::GetEnv(IMGA_WORKSPACE, false);
	
	std::string allusersHomeDefaultPath = tempHomeDrive + tempHomePath + DEFAULT_ALLUSER_WORKSPACE_PATH;
	std::string myselfHomeDefaultPath = tempHomeDrive + tempHomePath + DEFAULT_LOCAL_WORKSPACE_PATH;
	

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

std::string WorkspacePath::get()
{
	return m_homePath;
}



bool WorkspacePath::setPath(const char* p)
{
	m_homePath = p;
	return true;
}


/**
	Picture Path
*/

std::string PicturePath::m_homePath;


bool PicturePath::setLocalUserDefaultHome() {

	std::string tempHomeDrive = SAUtils::GetPOSIXEnv("HOMEDRIVE");
	std::string tempHomePath = SAUtils::GetPOSIXEnv("HOMEPATH");
	// Set Windows Defaults (they can be overridden later)
	std::string myselfHomeEnvironmentPath = SAUtils::GetEnv(IMGA_PICTURE, false);
	std::string myselfHomeDefaultPath = tempHomeDrive + tempHomePath + DEFAULT_LOCAL_PICTURE_PATH;

	if (myselfHomeEnvironmentPath.empty()) {
		m_homePath = myselfHomeDefaultPath;
	}
	else {
		m_homePath = myselfHomeEnvironmentPath;
	}
	m_found = true;
	if (SAUtils::DirExists(m_homePath.c_str()) == false) {
		return false;
	}
	m_valid = true;
	return true;
}

bool PicturePath::setAllUserDefaultHome() {

	std::string tempHomeDrive = SAUtils::GetPOSIXEnv("HOMEDRIVE");
	std::string tempHomePath = SAUtils::GetPOSIXEnv("HOMEPATH");
	// Set Windows Defaults (they can be overridden later)
	std::string allUsersHomeEnvironmentPath = SAUtils::GetEnv(IMGA_PICTURE, true);
	std::string allusersHomeDefaultPath = tempHomeDrive + tempHomePath + DEFAULT_ALLUSER_PICTURE_PATH;
	if (allUsersHomeEnvironmentPath.empty()) {
		m_homePath = allusersHomeDefaultPath;
	}
	else {
		m_homePath = allUsersHomeEnvironmentPath;
	}
	m_found = true;
	if (SAUtils::DirExists(m_homePath.c_str()) == false) {

		return false;
	}
	m_valid = true;
	return true;
}

bool PicturePath::init()
{

	std::string tempHomeDrive = SAUtils::GetPOSIXEnv("HOMEDRIVE");
	std::string tempHomePath = SAUtils::GetPOSIXEnv("HOMEPATH");


	// Set Windows Defaults (they can be overridden later)
	std::string allUsersHomeEnvironmentPath = SAUtils::GetEnv(IMGA_PICTURE, true);
	std::string myselfHomeEnvironmentPath = SAUtils::GetEnv(IMGA_PICTURE, false);

	std::string allusersHomeDefaultPath = tempHomeDrive + tempHomePath + DEFAULT_ALLUSER_PICTURE_PATH;
	std::string myselfHomeDefaultPath = tempHomeDrive + tempHomePath + DEFAULT_LOCAL_PICTURE_PATH;


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

std::string PicturePath::get()
{
	return m_homePath;
}


bool PicturePath::setPath(const char* p)
{
	m_homePath = p;
	return true;
}
/**
	WWWImage Path
*/

std::string WWWImagePath::m_homePath;

bool WWWImagePath::setLocalUserDefaultHome() {
	std::string myselfHomeEnvironmentPath = SAUtils::GetEnv(IMGA_WWWIMAGE, false);

#ifdef WIN32
	std::string myselfHomeDefaultPath = SAUtils::GetPOSIXEnv("LOCALAPPDATA") + DEFAULT_LOCAL_WWWIMAGE_PATH;;
#else
	std::string myselfHomeDefaultPath = SAUtils::GetPOSIXEnv("HOME") + DEFAULT_LOCAL_WWWIMAGE_PATH;;
#endif
	// Set Windows Defaults (they can be overridden later)
	if (myselfHomeEnvironmentPath.empty()) {
		m_homePath = myselfHomeDefaultPath;
	}
	else {
		m_homePath = myselfHomeEnvironmentPath;
	}
	m_found = true;
	if (SAUtils::DirExists(m_homePath.c_str()) == false) {
		return false;
	}
	m_valid = true;
	return true;
}

bool WWWImagePath::setAllUserDefaultHome() {

	std::string allUsersHomeEnvironmentPath = SAUtils::GetEnv(IMGA_WWWIMAGE, true);
	std::string allusersHomeDefaultPath = SAUtils::GetPOSIXEnv("LOCALAPPDATA") + DEFAULT_ALLUSER_WWWIMAGE_PATH;
#ifdef WIN32
	std::string myselfHomeDefaultPath = SAUtils::GetPOSIXEnv("LOCALAPPDATA") + DEFAULT_LOCAL_WWWIMAGE_PATH;
#else
	std::string myselfHomeDefaultPath = SAUtils::GetPOSIXEnv("HOME") + DEFAULT_LOCAL_WWWIMAGE_PATH;
#endif
	if (allUsersHomeEnvironmentPath.empty()) {
		m_homePath = allusersHomeDefaultPath;
	}
	else {
		m_homePath = allUsersHomeEnvironmentPath;
	}
	m_found = true;
	if (SAUtils::DirExists(m_homePath.c_str()) == false) {
		return false;
	}
	m_valid = true;
	return true;
}

bool WWWImagePath::init()
{

	

	// Set Windows Defaults (they can be overridden later)
	std::string allUsersHomeEnvironmentPath = SAUtils::GetEnv(IMGA_WWWIMAGE, true);
	std::string myselfHomeEnvironmentPath = SAUtils::GetEnv(IMGA_WWWIMAGE, false);

	std::string allusersHomeDefaultPath = SAUtils::GetPOSIXEnv("LOCALAPPDATA") + DEFAULT_ALLUSER_WWWIMAGE_PATH;

#ifdef WIN32
	std::string myselfHomeDefaultPath = SAUtils::GetPOSIXEnv("LOCALAPPDATA") + DEFAULT_LOCAL_WWWIMAGE_PATH;
#else
	std::string myselfHomeDefaultPath = SAUtils::GetPOSIXEnv("HOME") + DEFAULT_LOCAL_WWWIMAGE_PATH;
#endif
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

std::string WWWImagePath::get()
{
	return m_homePath;
}
