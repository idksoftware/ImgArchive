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
#include <string>
#include "MakeMedia.h"
#include "SAUtils.h"
#include "CSVDBFile.h"
#include "DirectoryVisitor.h"
#include "JournalFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

#define MEGABYTE (1474560)

namespace simplearchive {

class BackupVisitor : public FolderVisitor {
	//static BackupVisitor *m_this;
	std::string m_path;
public:
	virtual std::shared_ptr<FolderVisitor> make() {
		//if (m_this == 0) {
		return (std::make_shared<BackupVisitor>());
		//}
		//return m_this;
	}

	virtual bool onStart(const char *path)
	{
		printf("Start: %s\n", path);
		m_path = path;
		return true;
	};

	virtual bool onFile(const char *path)
	{
		printf("File: %s\n", path);
		if (SAUtils::delFile(path) == false) {
			return false;
		}
		return true;
	};

	virtual bool onDirectory(const char *path)
	{
		printf("Dir: %s\n", path);
		return true;
	};

	virtual bool onEnd()
	{
		printf("onEnd: %s\n", m_path.c_str());
		if (SAUtils::delDir(m_path.c_str()) == false) {
			return false;
		}
		return true;
	};
};
//BackupVisitor *BackupVisitor::m_this = 0;


bool MakeMedia::initalise(const char *archivePath, const char *distPath, unsigned long sizeOfMedia) {
	m_archivePath = archivePath;
	m_sizeOfMedia = sizeOfMedia;
	m_distPath = distPath;
	m_all = true;
	return true;
	
}
bool MakeMedia::initalise(const char *archivePath, const char *distPath, unsigned long sizeOfMedia, ExifDateTime *startDate, ExifDateTime *endDate) {
	m_fromBegining = true;
	m_toEnd = true;
	m_all = true;
	m_archivePath = archivePath;
	m_sizeOfMedia = sizeOfMedia;
	m_distPath = distPath;
	if (startDate != nullptr) {
		m_fromBegining = false;
	}

	if (endDate != nullptr) {
		m_toEnd = false;
	}
	if (m_fromBegining || m_toEnd) {
		m_all = false;
	}
	m_startDate = *startDate;
	m_endDate = *endDate;
	
	return true;
}
MakeMedia::~MakeMedia() {
	// TODO Auto-generated destructor stub
}
std::string getDate(const char *path) {
	std::string fpath = path;
	size_t sepIdx = fpath.find_first_of('/');
	size_t endIdx = fpath.find_last_of('/');
	return fpath.substr(sepIdx + 1, fpath.length() - (endIdx -1));
}
static std::string getYear(const char *path) {
	std::string fpath = path;
	return fpath.substr(0, 4);
}
bool MakeMedia::clearBackup() {
	std::string distPath = m_distPath;
	distPath += "/media";
	std::shared_ptr<BackupVisitor> backupVisitor_ptr = std::make_shared<BackupVisitor>();
	DirectoryVisitor directoryVisitor(backupVisitor_ptr);
	directoryVisitor.process(distPath.c_str());
	return true;
}
bool MakeMedia::process() {
	int volume = 1;
	std::string sourcePath = m_archivePath;
	std::string distPath = m_distPath;
	if (SAUtils::DirExists(sourcePath.c_str()) == false) {
		return false;
	}
	if (SAUtils::DirExists(distPath.c_str()) == false) {
		return false;
	}
	distPath += "/media";
	if (SAUtils::DirExists(distPath.c_str()) == false) {
		if (SAUtils::mkDir(distPath.c_str()) == false) {
			throw std::exception();
		}
	}

	printf("path %s\n", sourcePath.c_str());
	std::string indexPath = m_archivePath;
	indexPath += IMAGEID_FOLDER;
	CSVDBFile csvDBFile;
	csvDBFile.setPath(indexPath.c_str());
	int max = csvDBFile.getMaxIndex();
	printf("max: %d\n", max);
	int size = 0;
	int current = 0;
	std::string volumePath = distPath;
	volumePath += "/volume_";
	volumePath += SAUtils::to_string(volume);
	if (SAUtils::mkDir(volumePath.c_str()) == false) {
		return false;
	}

	std::shared_ptr<CSVJournalFile> pJournalFile = std::make_shared<CSVJournalFile>();

	for (;current < max; current++) {
		std::unique_ptr<ImageInfo> ii = csvDBFile.getItemAt(current);
		//printf("%d\n", ii->getIdx());
		size += ii->getSize();

		ExifDateTime created;
		created.now();
		ExifDateTime modified;
		modified.now();
		int mediasize = m_sizeOfMedia * MEGABYTE;
		pJournalFile->add(current, ii->getImagePath().c_str(), 2000+current, 3000+current,
				"4567890123456", created, modified);
		if (size > (m_sizeOfMedia * MEGABYTE)) {
			// write journal file
			std::string journalFile = volumePath;
			journalFile += "/journal.csv";
			pJournalFile->writeCSV(journalFile.c_str());
			pJournalFile->clearUp();
			volumePath = distPath;
			volumePath += "/volume_";
			volume++;
			volumePath += SAUtils::to_string(volume);
			if (SAUtils::mkDir(volumePath.c_str()) == false) {
				return false;
			}
			size = 0;
		}
		else
		{
			std::string datePath = getDate(ii->getImagePath().c_str());
			std::string year = getYear(datePath.c_str());
			std::string sourceFile = sourcePath;
			sourceFile += '/';
			sourceFile += year;
			sourceFile += '/';
			sourceFile += ii->getImagePath();

			std::string fullDatePath = volumePath;
			fullDatePath += '/';
			fullDatePath += datePath;
			if (SAUtils::DirExists(fullDatePath.c_str()) == false) {
				if (SAUtils::mkDir(fullDatePath.c_str()) == false) {
					throw std::exception();
				}
			}
			std::string distFile = volumePath;
			distFile += '/';
			distFile += ii->getImagePath();
			if (SAUtils::makePath(volumePath.c_str(), fullDatePath.c_str()) == false) {
				return false;
			}
			if (SAUtils::copy(sourceFile.c_str(), distFile.c_str()) == false) {
				return false;
			}
		}


	}
	std::string csvJournalFile = volumePath;
	csvJournalFile += "/journal.csv";
	std::string xmlJournalFile = volumePath;
	xmlJournalFile += "/journal.xml";
	pJournalFile->writeCSV(csvJournalFile.c_str());
	pJournalFile->writeXML(xmlJournalFile.c_str());
	pJournalFile->clearUp();
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
