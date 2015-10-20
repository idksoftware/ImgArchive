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

namespace simplearchive {

// hook folder

const char *onfiledoc[] = {
#ifdef WIN32
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

bool CreateArchive::createHomeEnvVar(const char *root) {
	if (SAUtils::DirExists(root) == false) {
		return false;
	}
	return SetEnv(root,false);
}

std::string CreateArchive::makeConfigFile(const char *root, const char *workspace, const char *shadow) {
	std::stringstream s;
	s << "# The main configuration file #\n";
	s << "HomePath=" << root << '\n';
	s << "WorkspacePath=" << workspace << '\n';
	s << "ShadowPath=" << shadow << '\n';
	return s.str();
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
#ifdef WIN32
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
} /* namespace simplearchive */
