
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "CheckFile.h"
#include "pathcontroller.h"

namespace simplearchive {

class CheckFileItem : public std::vector<std::string> {
	std::string m_path;
public:
	CheckFileItem(const char *path);
	bool check(const char *filename);
	bool uncheck(const char *filename);
	bool read();
	bool write();
};

CheckFileItem::CheckFileItem(const char *path) {
	m_path = path;
}

bool CheckFileItem::check(const char *filename) {
	push_back(filename);
	return true;
}
bool CheckFileItem::uncheck(const char *filename) {
	/*
	std::vector<std::string>::iterator i = find(begin(), end(), filename);
	if (i != end()) {

	}
	*/
	erase(std::remove(begin(), end(), filename), end());
	
	return true;
}

bool CheckFileItem::read() {
	char text[261];
	std::ifstream file(m_path.c_str());
	if (file.is_open() == false) {
		// empty?
		return false;
	}

	while (file.getline(text, 261)) {
		push_back(text);
	}
	file.close();

	return true;
}

bool CheckFileItem::write() {
	std::ofstream file(m_path.c_str(), std::ios::out);
	if (file.is_open() == false) {
		return false;
	}
	for (std::vector<std::string>::iterator i = begin(); i != end(); i++) {
		std::string &data = *i;
		file << data << '\n';

	}
	file.close();
	return true;
}

/*
std::string CheckWorkspaceFile::m_workspacePath;

bool CheckWorkspaceFile::initalise(const char *workspacePath) {
	m_workspacePath = workspacePath;
	return true;
}

std::string CheckMasterFile::m_masterPath;

bool CheckMasterFile::initalise(const char *masterPath) {
	m_masterPath = masterPath;
	return true;
}


std::string CheckWorkspaceFile::getFullAddress(const char *address) {
	std::string addr = address;
	PathController pathcontroller(m_workspacePath.c_str());
	pathcontroller.splitShort(address);
	pathcontroller.makePath();
	std::string fullPath = pathcontroller.getFullPath();
	fullPath += "/.sia/ckfile.dat";
	return fullPath;
}

std::string CheckMasterFile::getFullAddress(const char *address) {
	std::string addr = address;
	PathController pathcontroller(m_masterPath.c_str());
	pathcontroller.splitShort(address);
	pathcontroller.makePath();
	std::string fullPath = pathcontroller.getFullPath();
	fullPath += "/.sia/ckfile.dat";
	return fullPath;
}

bool CheckWorkspaceFile::CheckOut(const char *address) {
	std::string fullPath = getFullAddress(address);
	CheckFileItem checkFileItem(fullPath.c_str());
	if (checkFileItem.read() == false) {
		ErrorCode::setErrorCode(IMGA_ERROR::READ_ERROR);
		return true;
	}
	std::string addr = address;
	PathController pathcontroller;
	pathcontroller.splitShort(address);
	checkFileItem.check(pathcontroller.getImage().c_str());
	if (checkFileItem.write() == false) {
		ErrorCode::setErrorCode(IMGA_ERROR::WRITE_ERROR);
		return true;
	}
	return true;
}

bool CheckMasterFile::CheckOut(const char *address) {
	std::string fullPath = getFullAddress(address);
	CheckFileItem checkFileItem(fullPath.c_str());
	if (checkFileItem.read() == false) {
		ErrorCode::setErrorCode(IMGA_ERROR::READ_ERROR);
		return true;
	}
	std::string addr = address;
	PathController pathcontroller;
	pathcontroller.splitShort(address);
	checkFileItem.check(pathcontroller.getImage().c_str());
	if (checkFileItem.write() == false) {
		ErrorCode::setErrorCode(IMGA_ERROR::WRITE_ERROR);
		return true;
	}
	return true;
}
bool CheckWorkspaceFile::CheckIn(const char *address) {
	std::string fullPath = getFullAddress(address);
	CheckFileItem checkFileItem(fullPath.c_str());
	if (checkFileItem.read() == false) {
		ErrorCode::setErrorCode(IMGA_ERROR::READ_ERROR);
		return true;
	}
	std::string addr = address;
	std::string filename = addr.substr(14, addr.length() - 14);
	checkFileItem.uncheck(filename.c_str());
	if (checkFileItem.write() == false) {
		ErrorCode::setErrorCode(IMGA_ERROR::WRITE_ERROR);
		return true;
	}
	return true;
}

bool CheckMasterFile::CheckIn(const char *address) {
	std::string fullPath = getFullAddress(address);
	CheckFileItem checkFileItem(fullPath.c_str());
	if (checkFileItem.read() == false) {
		ErrorCode::setErrorCode(IMGA_ERROR::READ_ERROR);
		return true;
	}
	std::string addr = address;
	std::string filename = addr.substr(14, addr.length() - 14);
	checkFileItem.uncheck(filename.c_str());
	if (checkFileItem.write() == false) {
		ErrorCode::setErrorCode(IMGA_ERROR::WRITE_ERROR);
		return true;
	}
	return true;
}
*/

std::string CDCheckInOutManager::m_masterPath;
std::string CDCheckInOutManager::m_workspacePath;
std::string CDCheckInOutManager::m_derivativePath;

std::string CDCheckInOutManager::getFullAddress(DB db, const char *address) {
	std::string addr = address;
	std::string fullPath;
	switch (db) {
	case DB::Workspace:
	{
		PathController pathcontroller(m_workspacePath.c_str());
		pathcontroller.splitShort(address);
		pathcontroller.makePath();
		fullPath = pathcontroller.getFullPath();
		break;
	}
	case DB::Master:
	{
		PathController pathcontroller(m_masterPath.c_str());
		pathcontroller.splitShort(address);
		pathcontroller.makePath();
		fullPath = pathcontroller.getFullPath();
		break;
	}
	case DB::Derivative:
	{
		PathController pathcontroller(m_derivativePath.c_str());
		pathcontroller.splitShort(address);
		pathcontroller.makePath();
		fullPath = pathcontroller.getFullPath();
		break;
	}
	}
	fullPath += "/.sia/ckfile.dat";
	return fullPath;
}


bool CDCheckInOutManager::CheckOut(DB db, const char * address)
{
	std::string fullPath = getFullAddress(db, address);
	CheckFileItem checkFileItem(fullPath.c_str());
	if (checkFileItem.read() == false) {
		ErrorCode::setErrorCode(IMGA_ERROR::READ_ERROR);
		return false;
	}
	std::string addr = address;
	PathController pathcontroller;
	pathcontroller.splitShort(address);
	checkFileItem.check(pathcontroller.getImage().c_str());
	if (checkFileItem.write() == false) {
		ErrorCode::setErrorCode(IMGA_ERROR::WRITE_ERROR);
		return false;
	}
	return true;
}


bool CDCheckInOutManager::CheckIn(DB db, const char * address)
{
	std::string fullPath = getFullAddress(db, address);
	CheckFileItem checkFileItem(fullPath.c_str());
	if (checkFileItem.read() == false) {
		ErrorCode::setErrorCode(IMGA_ERROR::READ_ERROR);
		return false;
	}
	std::string addr = address;
	std::string filename = addr.substr(14, addr.length() - 14);
	checkFileItem.uncheck(filename.c_str());
	if (checkFileItem.write() == false) {
		ErrorCode::setErrorCode(IMGA_ERROR::WRITE_ERROR);
		return false;
	}
	return true;
}

CDCheckInOutManager & CDCheckInOutManager::get()
{
	static CDCheckInOutManager _this;

	return _this;
}

} /* namespace simplearchive */