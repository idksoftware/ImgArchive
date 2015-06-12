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
#include "SAUtils.h"
#include "CreateArchive.h"

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
};

const char *mirrordoc[] = {
	"#name=mode. path, type",
	"disk1=direct,/backup/home/tmp1,mirror"
};


const char *viewdoc[] = {
	"# name ,mode, path, type",
	"default=dynamic,/backup/home/tmp1,link,readonly,include,/home/wzw7yn/sia/piclist.dat",
	"#link=dynamic,/backup/home/test/link,link,readonly",
	"#copy=dynamic,/backup/home/test/copy,copy,readonly"
};

const char *extdoc[] = {
	"nef:Raw:Nikon-raw:Nikon RAW",
	"jpg:Picture:jpg:JPG",
	"bmp:Picture:bmp:BMP"
};


CreateArchive::CreateArchive() {
	// TODO Auto-generated constructor stub

}

CreateArchive::~CreateArchive() {
	// TODO Auto-generated destructor stub
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
	if (makeFolder(root, SHADOW_PATH) == false) {
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



bool CreateArchive::createConfigFiles(const char *root, const char *folder) {
	if (createFile(root, folder, "config.dat", (const char **)configdoc, (sizeof(configdoc)/sizeof(char *))) == false) {
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

	std::ofstream extFile(fullPath.c_str());
	extFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	if (!extFile.is_open()) {
		return false;
	}
	for (unsigned int i = 0; i < size; i++) {
		extFile << array[i] << '\n';
	}
	extFile.close();
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
