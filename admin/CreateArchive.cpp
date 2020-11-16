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
//#include "EnvFunc.h"
#include "CLogger.h"
#include "ErrorCode.h"
#include "AppPaths.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif
bool IsElevated();
namespace simplearchive {

	bool CreateArchive::createArchive(const char* archivePath, const char* workspacePath, const char* masterPath, const char* derivativePath, const char* cataloguePath, bool users) {

		
		std::string archivePathStr = archivePath;
		std::string workspacePathStr = workspacePath;
		//std::string reposPathStr = reposPath;
		std::string masterPathStr = masterPath;
		std::string derivativePathStr = derivativePath;
		std::string cataloguePathStr = cataloguePath;

		std::string masterPathOpt = masterPath;
		std::string derivativePathOpt = derivativePath;

		if (CreateArchive::createSystem(users, archivePathStr.c_str(), workspacePathStr.c_str(), masterPathStr.c_str(), derivativePathStr.c_str(), cataloguePathStr.c_str()) == false) {
			return false;
		}
		if (CreateArchive::createHomeEnvVar(CreateArchive::getArchivePath().c_str(), users) == false) {
			std::cout << "Failed creating enviroment variable IMGARCHIVE_HOME" << '\n';
			return false;
		}

		if (CreateArchive::makeFolders(CreateArchive::getArchivePath().c_str()) == false) {
			std::cout << "Failed creating folders" << '\n';
			return false;
		}

		if (CreateArchive::createHookFiles(CreateArchive::getArchivePath().c_str(), HOOKS_PATH) == false) {
			std::cout << "Failed creating hook files" << '\n';
			return false;
		}

		if (CreateArchive::createConfigFiles(CreateArchive::getArchivePath().c_str(), CONFIG_PATH, CreateArchive::getWorkspace().c_str(), CreateArchive::getMaster().c_str(), derivativePathStr.c_str(), cataloguePathStr.c_str()) == false) {
			std::cout << "Failed creating configuration files" << '\n';
			return false;
		}
		return true;
	}



// hook folder

	const char *viewPreview1doc[] = {
		"#import subprocess",
		"#print subprocess.check_output([magick convert DSC01277.JPG - resize 250x250 DSC01277_1.JPG])",
		"import os",
		"import sys",
		"",
		"os.system('magick convert ' + sys.argv[1] + ' -resize 250x250 ' + sys.argv[2])"
	};

	const char *viewPreview2doc[] = {
		"#import subprocess",
		"#print subprocess.check_output([magick convert DSC01277.JPG - resize 650x650 DSC01277_1.JPG])",
		"import os",
		"import sys",
		"",
		"os.system('magick convert ' + sys.argv[1] + ' -resize 650x650 ' + sys.argv[2])"
	};

	const char *viewPreview3doc[] = {
		"#import subprocess",
		"#print subprocess.check_output([magick convert DSC01277.JPG - resize 1200x1200 DSC01277_1.JPG])",
		"import os",
		"import sys",
		"",
		"os.system('magick convert ' + sys.argv[1] + ' -resize 1200x1200 ' + sys.argv[2])"
	};

	const char *viewThumbnaildoc[] = {
		"#import subprocess",
		"#print subprocess.check_output([magick convert DSC01277.JPG - resize 100x100 DSC01277_1.JPG])",
		"import os",
		"import sys",
		"",
		"os.system('magick convert ' + sys.argv[1] + ' -resize 100x100 ' + sys.argv[2])"
	};

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
std::string  CreateArchive::m_master;
std::string  CreateArchive::m_derivative;
std::string  CreateArchive::m_catalogue;

// Config folder files
const char *configdoc[] = {
	"# The main configuration file #"
};;

std::string configFileStr;

const char *extdoc[] = {
	"nef:Raw:nikon-raw:Nikon RAW",
	"nrw:Raw:nikon-raw:Nikon RAW",
	"dng:Raw:Adobe-raw:Adobe RAW",
	"raw:Raw:Panasonic-raw:Panasonic RAW",
	"raf:Raw:Fuji-raw:Fuji RAW",
	"orf:Raw:Olympus-raw:Olympus RAW",
	"srf:Raw:Sony-raw:Sony RAW",
	"sr2:Raw:Sony-raw:Sony RAW",
	"arw:Raw:Sony-raw:Sony RAW",
	"k25:Raw:Kodak-raw:Kodak RAW",
	"kdc:Raw:Kodak-raw:Kodak RAW",
	"dcr:Raw:Kodak-raw:Kodak RAW",
	"dcs:Raw:Kodak-raw:Kodak RAW",
	"drf:Raw:Kodak-raw:Kodak RAW",
	"mos:Raw:Leaf-raw:Leaf RAW",
	"pxn:Raw:Logitech -raw:Logitech RAW",
	"crw:Raw:Canon-raw:Canon RAW",
	"cr2:Raw:Canon-raw:Canon RAW",
	"mrw:Raw:Minolta-raw:Minolta RAW",
	"pef:Raw:Pentax-raw:Pentax RAW",
	"ptx:Raw:Pentax-raw:Pentax RAW",
	"mef:Raw:Mamiya-raw:Mamiya RAW",
	"3fr:Raw:Hasselblad-raw:Hasselblad RAW",
	"fff:Raw:Hasselblad-raw:Hasselblad RAW",
	"ari:Raw:Arriflex-raw:Arriflex RAW",
	"bay:Raw:Casio-raw:Casio RAW",
	"erf:Raw:Epsom -raw:Epsom RAW",
	"cap:Raw:PhaseOne-raw:Phase One RAW",
	"iiq:Raw:PhaseOne-raw:Phase One RAW",
	"eip:Raw:PhaseOne-raw:Phase One RAW",
	"rwl:Raw:Leica-raw:Leica RAW",
	"rwz:Raw:Rawzor-raw:Rawzor RAW",
	"srw:Raw:Samsung-raw:Samsung RAW",
	"x3f:Raw:Sigma-raw:Sigma RAW",
	"R3D:Raw:RED-raw:RED RAW",
	"jpg:Picture:jpg:JPG",
	"png:Picture:png:PNG",
	"bmp:Picture:bmp:bmp",
	"gif:Picture:gif:gif",
	"tif:Picture:tif:tif",
	"tiff:Picture:tiff:tiff"
};

bool CreateArchive::createHomeEnvVar(const char *root, bool users) {
	if (SAUtils::DirExists(root) == false) {
		return false;
	}
	
	return SAUtils::SetEnv(IMGARCHIVE_HOME, root, users);

}

std::string CreateArchive::makeConfigFile(const char *root, const char *workspace, const char *master, const char *derivative, const char *catalogue) {
	std::stringstream s;
	s << "# The main ImgArchive configuration file #\n";
	/*
	s << "[General]\n";
	s << "[Logging]\n";
	s << "[Network]\n";
	s << "[System Folders]\n";
	s << "HomePath=" << root << '\n';
	s << "WorkspacePath=" << workspace << '\n';
	s << "MasterPath=" << master << '\n';
	s << "DerivativePath=" << derivative << '\n';
	s << "MasterCataloguePath=" << catalogue << '\n';
	s << "[Master Archive] # This archive contains the master images.This section controls this archive\n";
	//s << "#BackupOne = \"d:/Backup Test One\" # BackupOne = <path to root backup one folder>\n";
	//s << "#BackupTwo = \"d:/Backup Test Two\" # BackupTwo = <path to root backup two folder>\n";
	//s << "#BackupOneEnabled = true # BackupOneEnabled = <true | false>\n";
	//s << "#BackupTwoEnabled = true # BackupOneEnabled = <true | false>\n";
	*/
	return s.str();
}

bool CreateArchive::createSystem(bool users, const char *archivePath, const char *workspace, const char *master, const char *derivative, const char *catalogue) {
	if (users == true) {
		// Admin version
		if (IsAdmin() == false) {
			return false;
		}
		if (createAdminSystem(archivePath, workspace, master, derivative, catalogue) == false) {
			return false;
		}
	}
	else {
		// User version
		if (createUserSystem(archivePath, workspace, master, derivative, catalogue) == false) {
			return false;
		}
	}
	return true;
}

bool CreateArchive::createAdminSystem(const char *archivePath, const char *workspace, const char *master, const char *derivative, const char *catalogue) {

	
	if (SAUtils::FileExists(archivePath) == true) {
		std::string configPath = archivePath;
		configPath += CONFIG_PATH;
		if (SAUtils::FileExists(configPath.c_str()) == true) {
			std::cout << "Config path folder found: \"" << m_archivePath << "\" so archive exists? exiting\n";
			return false;
		}
	}
		
	m_archivePath = archivePath;
	std::cout << "home path folder found: " << archivePath << '\n';
	
	if (SAUtils::FileExists(workspace) == true) {
		std::cout << "Config path folder found: \"" << workspace << "\" so archive exists? exiting\n";
		return false;
	}

	if (SAUtils::FileExists(master) == true) {
		std::cout << "Config path folder found: \"" << master << "\" so archive exists? exiting\n";
		return false;
	}

	if (SAUtils::FileExists(derivative) == true) {
		std::cout << "Config path folder found: \"" << derivative << "\" so archive exists? exiting\n";
		return false;
	}
	if (SAUtils::FileExists(catalogue) == true) {
		std::cout << "catalogue path folder found: \"" << derivative << "\" so archive exists? exiting\n";
		return false;
	}
	if (catalogue == nullptr || *catalogue == '\0') {

		std::string temp = SAUtils::GetPOSIXEnv("USERPROFILE");
		std::string path = temp;
		path += "/Pictures";
		if (SAUtils::FileExists(path.c_str()) == false) {
			return false;
		}
		path += "/IAPictures";
		if (SAUtils::FileExists(path.c_str()) == false) {
			if (SAUtils::mkDir(path.c_str()) == false) {
				return false;
			}
		}
		m_catalogue = path;
	}
	else {
		
		if (SAUtils::makePath(catalogue) == false) {
			return false;
		}
		m_catalogue = catalogue;
	}

	if (SAUtils::makePath(workspace) == false) {
		return false;
	}
	m_workspace = workspace;
	std::cout << "Created workspace path folder: " << m_workspace << '\n';

	if (SAUtils::makePath(master) == false) {
		return false;
	}
	m_master = master;
	std::cout << "Created master path folder: " << m_master << '\n';
	
	if (SAUtils::makePath(derivative) == false) {
		return false;
	}
	m_derivative = derivative;
	std::cout << "Created derivative path folder: " << derivative << '\n';
	
	return true;
}

bool CreateArchive::createUserSystem(const char* archivePath, const char* workspace, const char* master, const char* derivative, const char* catalogue) {

	if (SAUtils::FileExists(archivePath) == false) {
		if (SAUtils::makePath(archivePath) == false) {
			return false;
		}
	}
	m_archivePath = archivePath;


	if (SAUtils::FileExists(catalogue) == false) {
		if (SAUtils::makePath(catalogue) == false) {
			return false;
		}
	}
	m_catalogue = catalogue;
	

	if (SAUtils::FileExists(workspace) == false) {
		if (SAUtils::makePath(workspace) == false) {
			return false;
		}
	}
	m_workspace = workspace;

	if (SAUtils::FileExists(master) == false) {
		if (SAUtils::makePath(master) == false) {
			return false;
		}
	}
	m_master = master;

	if (SAUtils::FileExists(derivative) == false) {
		if (SAUtils::makePath(derivative) == false) {
			return false;
		}
	}
	m_derivative = derivative;
	return true;
}

bool CreateArchive::makeFolders(const char *root) {

	if (makeFolder(root, CONFIG_PATH) == false) {
		return false;
	}
	if (makeFolder(root, TEMP_PATH) == false) {
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
	if (makeFolder(root, BACKUPS_PATH) == false) {
		return false;
	}
	if (makeFolder(root, TOOLS_PATH) == false) {
		return false;
	}
	if (makeFolder(root, HOOKS_PATH) == false) {
		return false;
	}
	
	if (makeFolder(root, MASTER_WWW_CATALOGUE_PATH) == false) {
		return false;
	}
	if (makeFolder(root, TEMPLATE_PATH) == false) {
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
	if (checkFolder(root, TEMP_PATH) == false) {
		std::cout << "Failed to find " << TEMP_PATH << " folder : " << root << TEMP_PATH << '\n';
	}
	std::cout << "Found " << TEMP_PATH << " folder: " << root << TEMP_PATH << '\n';

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

	if (checkFolder(root, MASTER_CATALOGUE_PATH) == false) {
		std::cout << "Failed to find " << MASTER_CATALOGUE_PATH << " folder: " << root << '\\' << MASTER_CATALOGUE_PATH << '\n';
	}
	std::cout << "Found " << MASTER_CATALOGUE_PATH << " folder: " << root << MASTER_CATALOGUE_PATH << '\n';

	if (checkFolder(root, MASTER_WWW_CATALOGUE_PATH) == false) {
		std::cout << "Failed to find " << MASTER_WWW_CATALOGUE_PATH << " folder: " << root << '\\' << MASTER_WWW_CATALOGUE_PATH << '\n';
	}
	std::cout << "Found " << MASTER_WWW_CATALOGUE_PATH << " folder: " << root << MASTER_WWW_CATALOGUE_PATH << '\n';
	return true;
}

bool CreateArchive::createConfigFiles(const char *root, const char *folder, const char *workspace, const char *master, const char *derivative, const char *catalogue) {
	//createFile(const char *root, const char *folder, const char *filename, std::string &str) {
	std::string configFile = makeConfigFile(root, workspace, master, derivative, catalogue);
	
	if (createFile(root, folder, "config.dat", configFile) == false) {
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

	if (createFile(root, folder, "view-preview1.py", (const char **)viewPreview1doc, (sizeof(viewPreview1doc) / sizeof(char *))) == false) {
		return false;
	}

	if (createFile(root, folder, "view-preview2.py", (const char **)viewPreview2doc, (sizeof(viewPreview2doc) / sizeof(char *))) == false) {
		return false;
	}

	if (createFile(root, folder, "view-preview3.py", (const char **)viewPreview3doc, (sizeof(viewPreview3doc) / sizeof(char *))) == false) {
		return false;
	}

	if (createFile(root, folder, "view-thumbnail.py", (const char **)viewThumbnaildoc, (sizeof(viewThumbnaildoc) / sizeof(char *))) == false) {
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
#ifdef WIN32
	return IsElevated();
#else
	return false;
#endif
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

