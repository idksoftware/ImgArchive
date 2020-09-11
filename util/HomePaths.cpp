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
	path = &(DerivativePath::getObject());
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

bool ImgArchiveHome::setAllUserDefaultHome() {
	bool res = true;
	for (auto i = m_list.begin(); i != m_list.end(); i++) {
		auto hp = *i;
		if (hp->setAllUserDefaultHome() == false) {
			m_error = HPError::initaliseError;
			res = false;
		}
	}
	return res;
}

bool ImgArchiveHome::setLocalUserDefaultHome() {
	bool res = true;
	for (auto i = m_list.begin(); i != m_list.end(); i++) {
		auto hp = *i;
		if (hp->setLocalUserDefaultHome() == false) {
			m_error = HPError::initaliseError;
			res = false;
		}
	}
	if (res == false) {
		return false;
	}
	return true;
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

/*
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

std::string HomePath::m_path;

std::string HomePath::get()
{
	return m_path;
}



bool HomePath::setLocalUserDefaultHome() {
#ifdef WIN32
	std::string myselfHomeDefaultPath = m_myselfHomeDefaultPath;
#else
	std::string myselfHomeDefaultPath = SAUtils::GetPOSIXEnv("HOME");
#endif
	myselfHomeDefaultPath += DEFAULT_LOCAL_HOME_PATH;
	m_path = myselfHomeDefaultPath;


	m_found = true;
	if (SAUtils::DirExists(m_path.c_str()) == false) {
		return false;
	}
	m_valid = true;
	return true;
}

bool HomePath::setAllUserDefaultHome() {
	std::string allusersHomeDefaultPath = m_allusersHomeDefaultPath;
	allusersHomeDefaultPath += DEFAULT_ALLUSER_HOME_PATH;
	m_path = allusersHomeDefaultPath;
	m_found = true;
	if (SAUtils::DirExists(m_path.c_str()) == false) {
		
		return false;
	}
	m_valid = true;
	return true;
}

bool HomePath::setEnvironmentPath() {
	return true;
}

bool HomePath::setPath(const char* p)
{
	m_path = p;
	return true;
}
*/
/**

*/

bool MasterPath::init()
{

	std::string m_path = ImgArchiveHome::getImgArchiveHome();
	m_path += MASTER_PATH;
	// Set Windows Defaults (they can be overridden later)
	std::string allUsersHomeEnvironmentPath = SAUtils::GetEnv(IMGA_MASTER, true);
	std::string myselfHomeEnvironmentPath = SAUtils::GetEnv(IMGA_MASTER, false);
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

std::string MasterPath::m_path;

const std::string& MasterPath::get()
{
	
	return m_path;
}


bool MasterPath::setLocalUserDefaultHome() {

#ifdef WIN32
	std::string myselfHomeDefaultPath = m_myselfHomeDefaultPath;
#else
	std::string myselfHomeDefaultPath = SAUtils::GetPOSIXEnv("HOME");
#endif
	myselfHomeDefaultPath += DEFAULT_LOCAL_MASTER_PATH;
	m_path = myselfHomeDefaultPath;
	m_found = true;
	if (SAUtils::DirExists(m_path.c_str()) == false) {
		return false;
	}
	m_valid = true;
	return true;
}

bool MasterPath::setAllUserDefaultHome() {
	std::string allusersHomeDefaultPath = m_allusersHomeDefaultPath;
	allusersHomeDefaultPath += DEFAULT_ALLUSER_MASTER_PATH;
	m_path = allusersHomeDefaultPath;
	m_found = true;
	if (SAUtils::DirExists(m_path.c_str()) == false) {

		return false;
	}
	m_valid = true;
	return true;
}

bool MasterPath::setEnvironmentPath() {
	return true;
}

bool MasterPath::setPath(const char* p)
{
	m_path = p;
	return true;
}

std::string DerivativePath::m_path;


bool DerivativePath::init()
{
	std::string m_path = ImgArchiveHome::getImgArchiveHome();
	m_path += DERIVATIVE_PATH;
	// Set Windows Defaults (they can be overridden later)
	std::string allUsersHomeEnvironmentPath = SAUtils::GetEnv(IMGA_DERIVATIVE, true);
	std::string myselfHomeEnvironmentPath = SAUtils::GetEnv(IMGA_DERIVATIVE, false);
	
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

const std::string& DerivativePath::get()
{
	
	return m_path;
}


bool DerivativePath::setLocalUserDefaultHome() {
#ifdef WIN32
	std::string myselfHomeDefaultPath = m_myselfHomeDefaultPath;
#else
	std::string myselfHomeDefaultPath = SAUtils::GetPOSIXEnv("HOME");
#endif
	myselfHomeDefaultPath += DEFAULT_LOCAL_DERIVATIVE_PATH;
	m_path = myselfHomeDefaultPath;
	m_found = true;
	if (SAUtils::DirExists(m_path.c_str()) == false) {
		return false;
	}
	m_valid = true;
	return true;
}

bool DerivativePath::setAllUserDefaultHome() {
	std::string allusersHomeDefaultPath = m_allusersHomeDefaultPath;
	allusersHomeDefaultPath += DEFAULT_ALLUSER_DERIVATIVE_PATH;
	m_path = allusersHomeDefaultPath;
	m_found = true;
	if (SAUtils::DirExists(m_path.c_str()) == false) {

		return false;
	}
	m_valid = true;
	return true;
}

bool DerivativePath::setEnvironmentPath() {
	return true;
}

bool DerivativePath::setPath(const char* p)
{
	m_path = p;
	return true;
}
/**
	Workspace Path
*/

std::string WorkspacePath::m_path;


bool WorkspacePath::setLocalUserDefaultHome() {

	std::string tempHomeDrive = m_homeDrive;
	std::string tempHomePath = m_homePathEnv;
	// Set Windows Defaults (they can be overridden later)
	std::string myselfHomeEnvironmentPath = SAUtils::GetEnv(IMGA_WORKSPACE, false);
	std::string myselfHomeDefaultPath = tempHomeDrive + tempHomePath + DEFAULT_LOCAL_WORKSPACE_PATH;
	if (myselfHomeEnvironmentPath.empty()) {
		m_path = myselfHomeDefaultPath;
	}
	else {
		m_path = myselfHomeEnvironmentPath;
	}
	m_found = true;
	if (SAUtils::DirExists(m_path.c_str()) == false) {
		return false;
	}
	m_valid = true;
	return true;
}

bool WorkspacePath::setAllUserDefaultHome() {

	std::string tempHomeDrive = m_homeDrive;
	std::string tempHomePath = m_homePathEnv;
	// Set Windows Defaults (they can be overridden later)
	std::string allUsersHomeEnvironmentPath = SAUtils::GetEnv(IMGA_WORKSPACE, true);
	std::string allusersHomeDefaultPath = tempHomeDrive + tempHomePath + DEFAULT_ALLUSER_WORKSPACE_PATH;
	if (allUsersHomeEnvironmentPath.empty()) {
		m_path = allusersHomeDefaultPath;
	}
	else {
		m_path = allUsersHomeEnvironmentPath;
	}
	m_found = true;
	if (SAUtils::DirExists(m_path.c_str()) == false) {

		return false;
	}
	m_valid = true;
	return true;
}

bool WorkspacePath::init()
{
	
	std::string tempHomeDrive = m_homeDrive;
	std::string tempHomePath = m_homePathEnv;
	

	// Set Windows Defaults (they can be overridden later)
	std::string allUsersHomeEnvironmentPath = SAUtils::GetEnv(IMGA_WORKSPACE, true);
	std::string myselfHomeEnvironmentPath = SAUtils::GetEnv(IMGA_WORKSPACE, false);
	
	std::string allusersHomeDefaultPath = tempHomeDrive + tempHomePath + DEFAULT_ALLUSER_WORKSPACE_PATH;
	std::string myselfHomeDefaultPath = tempHomeDrive + tempHomePath + DEFAULT_LOCAL_WORKSPACE_PATH;
	

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

bool WorkspacePath::setEnvironmentPath() {
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


bool PicturePath::setLocalUserDefaultHome() {

	std::string tempHomeDrive = m_homeDrive;
	std::string tempHomePath = m_homePathEnv;
	// Set Windows Defaults (they can be overridden later)
	
	std::string myselfHomeEnvironmentPath = SAUtils::GetEnv(IMGA_WORKSPACE, false);

	std::string myselfHomeDefaultPath = tempHomeDrive + tempHomePath + DEFAULT_LOCAL_PICTURE_PATH;

	if (myselfHomeEnvironmentPath.empty()) {
		m_path = myselfHomeDefaultPath;
	}
	else {
		m_path = myselfHomeEnvironmentPath;
	}
	m_found = true;
	if (SAUtils::DirExists(m_path.c_str()) == false) {
		return false;
	}
	m_valid = true;
	return true;
}

bool PicturePath::setAllUserDefaultHome() {

	std::string tempHomeDrive = m_homeDrive;
	std::string tempHomePath = m_homePathEnv;
	// Set Windows Defaults (they can be overridden later)
	std::string allUsersHomeEnvironmentPath = SAUtils::GetEnv(IMGA_WORKSPACE, true);
	std::string allusersHomeDefaultPath = tempHomeDrive + tempHomePath + DEFAULT_ALLUSER_PICTURE_PATH;
	if (allUsersHomeEnvironmentPath.empty()) {
		m_path = allusersHomeDefaultPath;
	}
	else {
		m_path = allUsersHomeEnvironmentPath;
	}
	m_found = true;
	if (SAUtils::DirExists(m_path.c_str()) == false) {

		return false;
	}
	m_valid = true;
	return true;
}

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

bool PicturePath::setEnvironmentPath() {
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

bool WWWImagePath::setLocalUserDefaultHome() {
	std::string myselfHomeEnvironmentPath = m_myselfHomeEnvironmentPath;

#ifdef WIN32
	std::string myselfHomeDefaultPath = m_myselfHomeDefaultPath + DEFAULT_LOCAL_WWWIMAGE_PATH;;
#else
	std::string myselfHomeDefaultPath = SAUtils::GetPOSIXEnv("HOME") + DEFAULT_LOCAL_WWWIMAGE_PATH;;
#endif
	// Set Windows Defaults (they can be overridden later)
	if (myselfHomeEnvironmentPath.empty()) {
		m_path = myselfHomeDefaultPath;
	}
	else {
		m_path = myselfHomeEnvironmentPath;
	}
	m_found = true;
	if (SAUtils::DirExists(m_path.c_str()) == false) {
		return false;
	}
	m_valid = true;
	return true;
}

bool WWWImagePath::setAllUserDefaultHome() {

	std::string allUsersHomeEnvironmentPath = SAUtils::GetEnv(IMGA_WWWIMAGE, true);
	std::string allusersHomeDefaultPath = m_allusersHomeDefaultPath + DEFAULT_ALLUSER_WWWIMAGE_PATH;
#ifdef WIN32
	std::string myselfHomeDefaultPath = m_myselfHomeDefaultPath + DEFAULT_LOCAL_WWWIMAGE_PATH;
#else
	std::string myselfHomeDefaultPath = SAUtils::GetPOSIXEnv("HOME") + DEFAULT_LOCAL_WWWIMAGE_PATH;
#endif
	if (allUsersHomeEnvironmentPath.empty()) {
		m_path = allusersHomeDefaultPath;
	}
	else {
		m_path = allUsersHomeEnvironmentPath;
	}
	m_found = true;
	if (SAUtils::DirExists(m_path.c_str()) == false) {
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

bool WWWImagePath::setEnvironmentPath() {
	return true;
}


const std::string& WWWImagePath::get()
{
	return m_path;
}
