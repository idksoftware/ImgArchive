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

#include <stdio.h>
#include <memory>
#include "SAUtils.h"
#include "ManagedMirror.h"
#include "Mirror.h"
#include "CSVDBFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

namespace simplearchive {

ManagedMirror::ManagedMirror() {
	// TODO Auto-generated constructor stub

}

ManagedMirror::~ManagedMirror() {
	// TODO Auto-generated destructor stub
}

std::string getDate(const char *path) {
	std::string fpath = path;
	int sepIdx = fpath.find_first_of('/');
	return fpath.substr(0, sepIdx);
}
std::string getYear(const char *path) {
	std::string fpath = path;
	return fpath.substr(0, 4);
}


bool ManagedMirror::initalise(const char *rootFolder, const char *configFile) {
	m_rootFolder = rootFolder;
	m_configFile = configFile;
	if (SAUtils::DirExists(rootFolder) == false) {
		return false;
	}
	if (SAUtils::FileExists(configFile) == false) {
		return false;
	}
	try {
		bool ret = Mirror::initalise(rootFolder, configFile);

	} catch (MirrorException &e) {
		printf("Error: %s\n", e.what());
	}
	return true;
}

bool ManagedMirror::process() {
	int volume = 1;

	std::string indexPath = m_rootFolder;
	indexPath += "/.sia/index";
	CSVDBFile csvDBFile;
	csvDBFile.setPath(indexPath.c_str());
	int max = csvDBFile.getMaxIndex();
	printf("max: %d\n", max);
	int size = 0;
	int current = 0;
	std::string volumePath; //= distPath;


	//std::auto_ptr<CSVJournalFile> pJournalFile (new CSVJournalFile);

	for (;current < max; current++) {
		std::unique_ptr<ImageInfo> ii = csvDBFile.getItemAt(current);
		//printf("%d\n", ii->getIdx());
		size += ii->getSize();
		//CDate created = CDate::timeNow();
		//CDate modified = CDate::timeNow();

		//pJournalFile->add(current, ii->getImagePath().c_str(), 2000+current, 3000+current,
		//		"4567890123456", created, modified);

		std::string datePath = getDate(ii->getImagePath().c_str());
		std::string year = getYear(datePath.c_str());

		std::string sourceFile = year;
		sourceFile += '/';
		sourceFile += ii->getImagePath();
		printf("name %s\n", sourceFile.c_str());

		Mirror &mirror = Mirror::get();
		mirror.copy(sourceFile.c_str());
		//if (SAUtils::copy(sourceFile.c_str(), distFile.c_str()) == false) {
		//	return false;
		//}


	}

	/*
	cout << csvDBFile.findPath(24) << endl;

	for (int i = 0; i < 2000; i++) {
		CDate date = CDate::timeNow();
		csvDBFile.insert(i, "ggggg", 1000+i, 2000+i, "2adc231321231daef", "eddfeac341caddf", 0, date);
	}
	*/
	/*
	std::vector<std::string *> *filelist = SAUtils::GetFiles(m_archivePath.c_str());

	for (std::vector<std::string *>::iterator i = filelist->begin(); i != filelist->end(); i++) {
		std::string *name = *i;
		if ((*name)[0] == '.') {
			continue;
		}
		printf("name %s\n", name->c_str());
		std::string path = m_archivePath;
		path += '/';
		path += name->c_str();
		std::vector<std::string *> *filelist = SAUtils::GetFiles(path.c_str());
		for (std::vector<std::string *>::iterator i = filelist->begin(); i != filelist->end(); i++) {
			std::string *name = *i;
			if ((*name)[0] == '.') {
				continue;
			}
			printf("name %s\n", name->c_str());
		}
	}
	*/
	return true;
}

} /* namespace simplearchive */
