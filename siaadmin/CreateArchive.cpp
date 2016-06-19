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

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "SAUtils.h"
#include "CreateArchive.h"
#include "EnvFunc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif
bool IsElevated();
namespace simplearchive {

// hook folder

const char *onfiledoc[] = {
#ifdef _WIN32
#else
	"#!/bin/ksh",
#endif
	"",
	"echo \"On file\"",
	"echo $hookScripts",
	"echo $ToolsPath",
	"echo $HookScripsPath",
	"echo $SourcePath",
	"echo $ArchivePath",

};

std::string  CreateArchive::m_archivePath;
std::string  CreateArchive::m_workspace;
std::string  CreateArchive::m_shadow;

// Config folder files
const char *configdoc[] = {
	"# The main configuration file #"
};;

std::string configFileStr;

const char *mirrordoc[] = {
	"#name=mode. path, type",
	"disk1=direct,c:/temp/siamirror,mirror"
};


const char *viewdoc[] = {
	"# name ,mode, path, type",
	"#default=dynamic,/backup/home/tmp1,link,readonly,include,/home/wzw7yn/sia/piclist.dat",
	"#link=dynamic,/backup/home/test/link,link,readonly",
	"#copy=dynamic,/backup/home/test/copy,copy,readonly"
};

const char *extdoc[] = {
	"nef:Raw:nikon-raw:Nikon RAW",
	"nrw:Raw:nikon-raw:Nikon RAW",
	"dng:Raw:nikon-raw:Adobe RAW",
	"raw:Raw:nikon-raw:Panasonic RAW",
	"raf:Raw:nikon-raw:Fuji RAW",
	"orf:Raw:nikon-raw:Olympus RAW",
	"srf:Raw:nikon-raw:Sony RAW",
	"sr2:Raw:nikon-raw:Sony RAW",
	"arw:Raw:nikon-raw:Sony RAW",
	"k25:Raw:nikon-raw:Kodak RAW",
	"kdc:Raw:nikon-raw:Kodak RAW",
	"dcr:Raw:nikon-raw:Kodak RAW",
	"dcs:Raw:nikon-raw:Kodak RAW",
	"drf:Raw:nikon-raw:Kodak RAW",
	"mos:Raw:nikon-raw:Leaf RAW",
	"pxn:Raw:nikon-raw:Logitech RAW",
	"crw:Raw:nikon-raw:Canon RAW",
	"cr2:Raw:nikon-raw:Canon RAW",
	"mrw:Raw:nikon-raw:Minolta RAW",
	"pef:Raw:nikon-raw:Pentax RAW",
	"ptx:Raw:nikon-raw:Pentax RAW",
	"mef:Raw:nikon-raw:Mamiya RAW",
	"3fr:Raw:nikon-raw:Hasselblad RAW",
	"fff:Raw:nikon-raw:Hasselblad RAW",
	"ari:Raw:nikon-raw:Arriflex RAW",
	"bay:Raw:nikon-raw:Casio RAW",
	"erf:Raw:nikon-raw:Epsom RAW",
	"cap:Raw:nikon-raw:Phase One RAW",
	"iiq:Raw:nikon-raw:Phase One RAW",
	"eip:Raw:nikon-raw:Phase One RAW",
	"rwl:Raw:nikon-raw:Leica RAW",
	"rwz:Raw:nikon-raw:Rawzor RAW",
	"srw:Raw:nikon-raw:Samsung RAW",
	"x3f:Raw:nikon-raw:Sigma RAW",
	"R3D:Raw:nikon-raw:RED RAW",
	"jpg:Picture:jpg:JPG",
	"png:Picture:png:PNG",
	"bmp:Picture:bmp:bmp",
	"gif:Picture:gif:gif",
	"tif:Picture:tif:tif",
	"tiff:Picture:tiff:tiff"
};


CreateArchive::CreateArchive() {
	// TODO Auto-generated constructor stub

}

CreateArchive::~CreateArchive() {
	// TODO Auto-generated destructor stub
}

bool CreateArchive::createHomeEnvVar(const char *root, bool users) {
	if (SAUtils::DirExists(root) == false) {
		return false;
	}
	
	return SetEnv(root, users);
}

std::string CreateArchive::makeConfigFile(const char *root, const char *workspace, const char *shadow) {
	std::stringstream s;
	s << "# The main configuration file #\n";
	s << "HomePath=" << root << '\n';
	s << "WorkspacePath=" << workspace << '\n';
	s << "ShadowPath=" << shadow << '\n';
	return s.str();
}

bool CreateArchive::createSystem(bool users, const char *archivePath, const char *workspace, const char *shadow) {
	if (users == true) {
		// Admin version
		if (IsAdmin() == false) {
			return false;
		}
		if (createAdminSystem(archivePath, workspace, shadow) == false) {
			return false;
		}
	}
	else {
		// User version
		if (createUserSystem(archivePath, workspace, shadow) == false) {
			return false;
		}
	}
	return true;
}

bool CreateArchive::createAdminSystem(const char *archivePath, const char *workspace, const char *shadow) {

	if (archivePath == nullptr || *archivePath == '\0') {
		std::string progPath = SAUtils::GetEnvironment("ProgramData");
		std::string siaPath = "/IDK Software/ImageArchive1.0";
		std::string path = progPath;
		path += siaPath;
		if (SAUtils::FileExists(path.c_str()) == true) {
			return false;
		}
		if (SAUtils::makePath(progPath.c_str(), siaPath.c_str()) == false) {
			return false;
		}
		
		m_archivePath = path;
		std::cout << "Created home path folder: " << m_archivePath << '\n';
	} else {
		if (SAUtils::FileExists(archivePath) == true) {
			return false;
		}
		if (SAUtils::makePath(archivePath) == false) {
			return false;
		}
		m_archivePath = archivePath;
		std::cout << "Created home path folder: " << m_archivePath << '\n';
	}

	if (workspace == nullptr || *workspace == '\0') {
	
		std::string temp = SAUtils::GetEnvironment("USERPROFILE");
		std::string path = temp;
		path += "/Documents";
		if (SAUtils::FileExists(path.c_str()) == false) {
			return false;
		}
		path += "/SIA Workspace";
		if (SAUtils::FileExists(path.c_str()) == false) {
			if (SAUtils::mkDir(path.c_str()) == false) {
				return false;
			}
		}
		m_workspace = path;
		std::cout << "Created workspace path folder: " << m_workspace << '\n';
	}
	else {
		if (SAUtils::FileExists(workspace) == true) {
			return false;
		}
		if (SAUtils::makePath(workspace) == false) {
			return false;
		}
		m_workspace = workspace;
		std::cout << "Created workspace path folder: " << m_workspace << '\n';
	}
	

	if (shadow == nullptr || *shadow == '\0') {
		std::string progPath = SAUtils::GetEnvironment("ProgramData");
		std::string siaPath = "/IDK Software/ImageArchive1.0/shadow";
		
		std::string path = progPath;
		path += siaPath;
		if (SAUtils::FileExists(path.c_str()) == true) {
			return false;
		}
		if (SAUtils::makePath(progPath.c_str(), siaPath.c_str()) == false) {
			return false;
		}
		m_shadow = path;
		std::cout << "Created shadow path folder: " << m_shadow << '\n';
	}
	else {
		if (SAUtils::FileExists(shadow) == true) {
			return false;
		}
		if (SAUtils::makePath(shadow) == false) {
			return false;
		}
		m_shadow = shadow;
		std::cout << "Created shadow path folder: " << m_shadow << '\n';
	}
	
	return true;
}

bool CreateArchive::createUserSystem(const char *archivePath, const char *workspace, const char *shadow) {

	if (archivePath == nullptr || *archivePath == '\0') {
		std::string progPath = SAUtils::GetEnvironment("USERPROFILE");
		std::string siaPath = "/IDK Software/ImageArchive1.0";
		std::string path = progPath;
		path += siaPath;
		if (SAUtils::FileExists(progPath.c_str()) == false) {
			return false;
		}
		if (SAUtils::makePath(progPath.c_str(), siaPath.c_str()) == false) {
			return false;
		}
		m_archivePath = path;

	}
	else {
		if (SAUtils::FileExists(archivePath) == false) {
			return false;
		}
		if (SAUtils::makePath(archivePath) == false) {
			return false;
		}
		m_archivePath = archivePath;
	}

	if (workspace == nullptr || *workspace == '\0') {

		std::string temp = SAUtils::GetEnvironment("USERPROFILE");
		std::string path = temp;
		path += "/Documents";
		if (SAUtils::FileExists(path.c_str()) == false) {
			return false;
		}
		path += "/SIA Workspace";
		if (SAUtils::FileExists(path.c_str()) == false) {
			if (SAUtils::mkDir(path.c_str()) == false) {
				return false;
			}
		}
		m_workspace = path;
	}
	else {
		if (SAUtils::FileExists(workspace) == false) {
			return false;
		}
		if (SAUtils::makePath(workspace) == false) {
			return false;
		}
		m_workspace = workspace;
	}


	if (shadow == nullptr || *shadow == '\0') {
		std::string progPath = SAUtils::GetEnvironment("USERPROFILE");
		std::string siaPath = "/IDK Software/ImageArchive1.0/shadow";

		std::string path = progPath;
		path += siaPath;
		if (SAUtils::FileExists(progPath.c_str()) == false) {
			return false;
		}
		if (SAUtils::makePath(progPath.c_str(), siaPath.c_str()) == false) {
			return false;
		}
		m_shadow = path;
	}
	else {
		if (SAUtils::FileExists(shadow) == false) {
			return false;
		}
		if (SAUtils::makePath(shadow) == false) {
			return false;
		}
		m_shadow = shadow;
	}
	
	return true;
}

bool CreateArchive::makeFolders(const char *root) {

	if (makeFolder(root, CONFIG_PATH) == false) {
		return false;
	}
	if (makeFolder(root, TMP_PATH) == false) {
		return false;
	}
	if (makeFolder(root, DUPS_PATH) == false) {
		return false;
	}
	if (makeFolder(root, SQLITEDB_PATH) == false) {
		return false;
	}
	if (makeFolder(root, LOG_PATH) == false) {
		return false;
	}
	if (makeFolder(root, HISTORY_PATH) == false) {
		return false;
	}
	if (makeFolder(root, BACKUP_PATH) == false) {
		return false;
	}
	if (makeFolder(root, TOOLS_PATH) == false) {
		return false;
	}
	if (makeFolder(root, HOOKS_PATH) == false) {
		return false;
	}
	return true;
}

bool CreateArchive::checkFolders(const char *root) {
	std::cout << "System Folders\n";
	std::cout << "==============\n";
	//if (checkFolder(root, CONFIG_PATH) == false) {
	//	std::cout << "Failed you need administrator privileges" << '\n';
	//	return false;
	//}
	if (checkFolder(root, TMP_PATH) == false) {
		std::cout << "Failed to find " << TMP_PATH << " folder : " << root << TMP_PATH << '\n';
	}
	std::cout << "Found " << TMP_PATH << " folder: " << root << TMP_PATH << '\n';

	if (checkFolder(root, DUPS_PATH) == false) {
		std::cout << "Failed to find " << DUPS_PATH << " folder: " << root << DUPS_PATH << '\n';
	}
	std::cout << "Found " << DUPS_PATH << " folder: " << root << DUPS_PATH << '\n';

	if (checkFolder(root, SQLITEDB_PATH) == false) {
		std::cout << "Failed to find " << SQLITEDB_PATH << " folder: " << root << SQLITEDB_PATH << '\n';
	}
	std::cout << "Found " << SQLITEDB_PATH << " folder: " << root << SQLITEDB_PATH << '\n';

	if (checkFolder(root, LOG_PATH) == false) {
		std::cout << "Failed to find " << LOG_PATH << " folder: " << root << LOG_PATH << '\n';
	}
	std::cout << "Found " << LOG_PATH << "  folder: " << root << LOG_PATH << '\n';

	if (checkFolder(root, HISTORY_PATH) == false) {
		std::cout << "Failed to find " << HISTORY_PATH << " folder: " << root << HISTORY_PATH << '\n';
	}
	std::cout << "Found " << HISTORY_PATH << " folder: " << root << HISTORY_PATH << '\n';

	if (checkFolder(root, BACKUP_PATH) == false) {
		std::cout << "Failed to find " << BACKUP_PATH << " folder: " << root << BACKUP_PATH << '\n';
	}
	std::cout << "Found " << BACKUP_PATH << " folder: " << root << BACKUP_PATH << '\n';

	if (checkFolder(root, TOOLS_PATH) == false) {
		std::cout << "Failed to find " << TOOLS_PATH << " folder: " << root << TOOLS_PATH << '\n';
	}
	std::cout << "Found  " << TOOLS_PATH << " folder: " << root << TOOLS_PATH << '\n';

	if (checkFolder(root, HOOKS_PATH) == false) {
		std::cout << "Failed to find " << HOOKS_PATH << " folder: " << root << '\\' << HOOKS_PATH << '\n';
	}
	std::cout << "Found " << HOOKS_PATH << " folder: " << root << HOOKS_PATH << '\n';

	return true;
}

bool CreateArchive::createConfigFiles(const char *root, const char *folder, const char *workspace, const char *shadow) {
	//createFile(const char *root, const char *folder, const char *filename, std::string &str) {
	std::string configFile = makeConfigFile(root, workspace, shadow);
	
	if (createFile(root, folder, "config.dat", configFile) == false) {
		return false;
	}

	if (createFile(root, folder, "mirror.dat", (const char **)mirrordoc, (sizeof(mirrordoc)/sizeof(char *))) == false) {
		return false;
	}
	if (createFile(root, folder, "view.dat", (const char **)viewdoc, (sizeof(viewdoc)/sizeof(char *))) == false) {
		return false;
	}
	if (createFile(root, folder, "ext.dat", (const char **)extdoc, (sizeof(extdoc)/sizeof(char *))) == false) {
		return false;
	}
	return true;
}

bool CreateArchive::createHookFiles(const char *root, const char *folder) {
#ifdef _WIN32
	if (createFile(root, folder, "on-file-test.bat", (const char **)onfiledoc, (sizeof(onfiledoc)/sizeof(char *))) == false) {
#else
	if (createFile(root, folder, "on-file-test.sh", (const char **)onfiledoc, (sizeof(onfiledoc)/sizeof(char *))) == false) {
#endif
		return false;
	}

	return true;
}

bool CreateArchive::createFile(const char *root, const char *folder, const char *filename,
																		const char *array[],unsigned int size) {
	std::string fullPath = root;
	fullPath += folder;
	fullPath += '/';
	fullPath += filename;

	if (SAUtils::FileExists(fullPath.c_str()) == false) {
		std::ofstream extFile(fullPath.c_str());
		extFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		if (!extFile.is_open()) {
			return false;
		}
		for (unsigned int i = 0; i < size; i++) {
			extFile << array[i] << '\n';
		}
		extFile.close();
	}
	return true;
}

bool CreateArchive::createFile(const char *root, const char *folder, const char *filename, std::string &str) {
	std::string fullPath = root;
	fullPath += folder;
	fullPath += '/';
	fullPath += filename;

	if (SAUtils::FileExists(fullPath.c_str()) == false) {
		std::ofstream extFile(fullPath.c_str());
		extFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		if (!extFile.is_open()) {
			return false;
		}

		extFile << str << '\n';

		extFile.close();
	}
	return true;
}

bool CreateArchive::makeFolder(const char *root, const char *folder) {
	std::string folderPath = root;
	folderPath += folder;
	if (SAUtils::DirExists(folderPath.c_str()) == false) {
		if (SAUtils::mkDir(folderPath.c_str()) == false) {
			return false;
		}
	}
	return true;
}

bool CreateArchive::checkFolder(const char *root, const char *folder) {
	std::string folderPath = root;
	folderPath += folder;
	if (SAUtils::DirExists(folderPath.c_str()) == false) {
		return false;
	}
	return true;
}

bool CreateArchive::IsAdmin() {
	return IsElevated();
}

} /* namespace simplearchive */
#if _WIN32
#include <Windows.h>

bool IsElevated() {
	bool fRet = false;
	HANDLE hToken = NULL;
	if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
		TOKEN_ELEVATION Elevation;
		DWORD cbSize = sizeof(TOKEN_ELEVATION);
		if (GetTokenInformation(hToken, TokenElevation, &Elevation, sizeof(Elevation), &cbSize)) {
			fRet = Elevation.TokenIsElevated;
		}
	}
	if (hToken) {
		CloseHandle(hToken);
	}
	return fRet;
}

#endif