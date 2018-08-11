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

#include "CheckDisk.h"
#include "SAUtils.h"
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <stdio.h>
#include <cstdlib>
#include <sstream>
#include <time.h>
#include "CIDKCrc.h"
#include "md5.h"
#include "CLogger.h"
#include "ExifDateTime.h"
#include "CSVArgs.h"
#include "ErrorCode.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

namespace simplearchive {

	void ReportStatus::set(const char *str) {
		std::string s(str);

		switch (s[0]) {
		case 'U':
			if (s.compare("Unchanged") == 0) {
				m_status = Unchanged;
			}
			else {
				m_status = Unknown;
			}
			break;
		case 'C':
			if (s.compare("ContentChanged") == 0) {
				m_status = ContentChanged;
			}
			else {
				m_status = Unknown;
			}
			break;
		case 'N':
			if (s.compare("NameChanged") == 0) {
				m_status = NameChanged;
			}
			else {
				m_status = Unknown;
			}
			break;
		case 'M':
			if (s.compare("Missing") == 0) {
				m_status = Missing;
			}
			else {
				m_status = Unknown;
			}
			break;
		case 'A':
			if (s.compare("Added") == 0) {
				m_status = Added;
			}
			else {
				m_status = Unknown;
			}
			break;
		default:
			m_status = Unknown;
		}
	}

	
	const char *ReportStatus::toString() {
		switch (m_status) {
		case ReportStatus::Unchanged: return "Unchanged";
		case ReportStatus::ContentChanged: return "ContentChanged";
		case ReportStatus::NameChanged: return "NameChanged";
		case ReportStatus::Missing: return "Missing";
		case ReportStatus::Added : return "Added";
		default:
			return "Unknown";
		}
	}

std::string CheckDisk::m_archivePath;

/*
 * This class manages the check disk process.
 * NOTE this is the check disk difference file, Not the
 * ChkDsk File
 *
 * crc, md5, filename
 */
class CkdskFile;
class CkdskData {
	friend class CkdskFile;
public:
	typedef enum {
		Unknown = -1,
		ContentChanged,
		NameChanged,
		Missing,
		Added,
		Unchanged,
	} Status;
private:
	std::string m_name;
	unsigned int m_crc;
	std::string m_md5;
	Status m_status;
	time_t m_created;
	time_t m_modified;
	bool m_checkedOut;	
	unsigned int m_size;
public:
	CkdskData() {
		m_size = -1;
		m_created = 0;
		m_modified = 0;
		m_crc = 0;
		m_status = Missing;
		m_checkedOut = false;
	};

	CkdskData(const char *dataString) {
		m_status = Missing;
		m_checkedOut = false;
		std::string data = dataString;
		CSVArgs csvArgs(':');
		csvArgs.process(dataString);

		std::string sizeStr = csvArgs.at(0);
		m_size = (unsigned int)strtol(sizeStr.c_str(), NULL, 10);

		std::string crcStr = csvArgs.at(1);
		m_crc = std::strtoul(crcStr.c_str(), NULL, 10);

		m_md5 = csvArgs.at(2);

		std::string m_createdStr = csvArgs.at(3);
		m_created = strtol(crcStr.c_str(), NULL, 10);

		std::string m_modifiedStr = csvArgs.at(4);
		m_modified = strtol(m_modifiedStr.c_str(), NULL, 10);

		m_name = csvArgs.at(5);
		if (csvArgs.size() > 6) {
			m_checkedOut = (csvArgs.at(6).compare("True") == 0) ? true : false;
		}
	}

	CkdskData(const char *filepath, const char *name) {
		
		unsigned long size = 0;
		CLogger &logger = CLogger::getLogger();
		if (SAUtils::fileSize(filepath, &size) == false) {
			logger.log(LOG_OK, CLogger::Level::ERR, "Failed to read file %s", filepath);

		}
		unsigned int crc;
		
		std::string md5Str;

		logger.log(LOG_OK, CLogger::Level::INFO, "Image:          %s", filepath);
		std::string buf;
		SAUtils::getFileContents(filepath, buf);
		MD5 md5(buf);
		md5Str = md5.hexdigest();
		logger.log(LOG_OK, CLogger::Level::INFO, "MD5 of image:   %s is %s", filepath, md5Str.c_str());

		CIDKCrc Crc;
		logger.log(LOG_OK, CLogger::Level::INFO, "Size of image:  %s is %d", filepath, size);
		crc = Crc.crc((unsigned char *)buf.c_str(), size);
		logger.log(LOG_OK, CLogger::Level::INFO, "CRC from image: %s is %x", filepath, crc);

		time_t created = SAUtils::createTime(filepath);
		//logger.log(LOG_OK, CLogger::INFO, "Create time of image: %s", created.toLogString().c_str());
		time_t modified = SAUtils::modTime(filepath);
		// test old

		init(name, size, crc, md5Str.c_str(), created, modified);
	}

	CkdskData(const char *name, int size, unsigned int crc, const char *md5, time_t created, time_t modified) {
		init(name, size, crc, md5, created, modified);
	}

	void init(const char *name, int size, unsigned int crc, const char *md5, time_t created, time_t modified) {
		m_name = name;
		m_size = size;
		m_crc = crc;
		m_md5 = md5;
		m_status = Added;
		m_created = created;
		m_modified = modified;
		m_checkedOut = false;
	}


	CkdskData(const CkdskData &obj) {
		m_name = obj.m_name;
		m_size = obj.m_size;
		m_crc = obj.m_crc;
		m_md5 = obj.m_md5;
		m_status = obj.m_status;
		m_created = obj.m_created;
		m_modified = obj.m_modified;
		m_checkedOut = obj.m_checkedOut;
	}

	CkdskData &operator=(const CkdskData &obj) {
		m_name = obj.m_name;
		m_size = obj.m_size;
		m_crc = obj.m_crc;
		m_md5 = obj.m_md5;
		m_status = obj.m_status;
		m_created = obj.m_created;
		m_modified = obj.m_modified;
		m_checkedOut = obj.m_checkedOut;
		return *this;
	}

	std::string diffStatusline() {
		return m_name + ":" + statusString();
	}

	std::string toString() {
		std::stringstream s;
		s << m_size << ':' << m_crc << ':' << m_md5 << ':' << m_created << ':' << m_modified << ':' << m_name << ':' << ((m_checkedOut)?"True":"False");
		return s.str();
	}


	bool match(const char *name, unsigned int crc, const char *md5) {
		bool n = false;
		bool c = false;
		bool m = false;
		bool a = false;
		if (m_name.compare(name) == 0) {
			n =true;
			a = true;
		}
		if (m_crc == crc) {
			c = true;
			a = true;
		}
		if (m_md5.compare(md5) == 0) {
			m = true;
			a = true;
		}
		if (a == false) {
			//printf("%s: All false\n", name);
			return false;
		}
		// All the same
		if (n && c && m) {
			m_status = Unchanged;
			//printf("%s: Unchanged\n", name);
			return true;
		}
		// Name found crc or md5 changed
		if (n && !(c && m)) {
			m_status = ContentChanged;
			//printf("%s: Changed\n", name);
			return false;
		}
		// No match
		//printf("%s: No Match\n", name);
		return false;

	}

	bool add(const char *name, int crc, const char *md5);

	unsigned int getCrc() const {
		return m_crc;
	}

	void setCrc(unsigned int crc) {
		this->m_crc = crc;
	}

	const std::string& getMd5() const {
		return m_md5;
	}

	void setMd5(const std::string& md5) {
		this->m_md5 = md5;
	}

	const std::string& getName() const {
		return m_name;
	}

	void setName(const std::string& name) {
		this->m_name = name;
	}

	Status getStatus() const {
		return m_status;
	}

	bool getCheckedOut() const {
		return m_checkedOut;
	}

	void setCheckedOut(bool b) {
		m_checkedOut = b;
	}

	void setStatus(Status status) {
		m_status = status;
	}

	const char *statusString() {
		switch (m_status) {
		case Unknown:
			return "Unknown";
		case ContentChanged:
			return "ContentChanged";
		case NameChanged:
			return "NameChanged";
		case Missing:
			return "Missing";
		case Added:
			return "Added";
		case Unchanged:
			return "Unchanged";
		default:
			return "Unknown";
		}
	}

	int getSize() const {
		return m_size;
	}

	time_t getModified() const {
		return m_modified;
	}
};

/*
 * This class puts the Ckhdsk data into a vector to
 * create the chkdsk Data file
 */

class CkdskDataContainer : public std::vector<CkdskData> {};

class CkdskManifestFile {
	std::unique_ptr<CkdskDataContainer> m_data;
	void insert(int crc, std::string);
public:
	CkdskManifestFile() : m_data(std::make_unique<CkdskDataContainer>()) {};
	virtual ~CkdskManifestFile() {};

	bool read(const char *datafile);
	/// Write the manifest file
	bool write(const char *datafile);
	/// Write the status file
	bool writeStatus(const char *datafile);
	//bool add(const char *name, int crc, const char *md5);
	int find(int crc, const char *md5, const char *datafile);
	CkdskData *findFile(const char *filename);
	//std::shared_ptr<CkdskData> findFile(const char *filename);
	bool isFileFound(const char *filename);
	bool add(const char *filepath, const char *name);

	CkdskDataContainer& getData() {
		return *m_data;
	}

};

bool CkdskManifestFile::add(const char *filepath, const char *name) {
	
	if (!isFileFound(name)) {
		CLogger &logger = CLogger::getLogger();

		logger.log(LOG_OK, CLogger::Level::INFO, "Image: %s", name);
		unsigned long size;
		if (SAUtils::fileSize(filepath, &size) == false) {
			logger.log(LOG_OK, CLogger::Level::INFO, "Can not read image: %s", filepath);
			return false;
		}
		logger.log(LOG_OK, CLogger::Level::INFO, "Size of image: %s is %d", filepath, size);
		CIDKCrc Crc;
		logger.log(LOG_OK, CLogger::Level::INFO, "Image: %s", filepath);
		std::string buf;
		SAUtils::getFileContents(filepath, buf);
		MD5 md5(buf);
		std::string md5Str = md5.hexdigest();
		logger.log(LOG_OK, CLogger::Level::INFO, "MD5 of image: %s is %s", filepath, md5Str.c_str());
		unsigned int crc = Crc.crc((unsigned char *)buf.c_str(), size);
		logger.log(LOG_OK, CLogger::Level::INFO, "CRC from image: %s is %x", filepath, crc);
		
		time_t created = SAUtils::createTime(filepath);
		//logger.log(LOG_OK, CLogger::INFO, "Create time of image: %s", m_createTime.toLogString().c_str());
		time_t modified = SAUtils::modTime(filepath);
		
		// old m_data->insert(m_data->end(), *ckdskData);
		// new
		m_data->emplace(m_data->end(), name, size, crc, md5Str.c_str(), created, modified);
		return true;
	}
	return false;
}

bool CkdskManifestFile::read(const char *datafile) {
	std::string text;
	std::ifstream file(datafile);
	if (file.is_open() == false) {
		// empty?
		return false;
	}

	while (std::getline(file, text)) {
		m_data->emplace_back(std::move(CkdskData(text.c_str())));
	}
	file.close();

	return true;
}

bool CkdskManifestFile::write(const char *datafile) {
	std::ofstream file(datafile, std::ios::out);
	if (file.is_open() == false) {
		return false;
	}
	for (std::vector<CkdskData>::iterator i = m_data->begin(); i != m_data->end(); i++) {
		CkdskData &data = *i;
		file << data.toString() << '\n';

	}
	file.close();
	return true;
}

/*
bool CkdskManifestFile::writeStatus(const char *datafile) {
	std::ofstream file(datafile);
	if (file.is_open() == false) {
		return false;
	}

	for (std::map<unsigned long, CkdskData>::iterator ii = m_data->begin(); ii != m_data->end(); ++ii) {
		CkdskData &data = ii->second;
		file << data.statusString() + '\n';
		//printf("%s", name.c_str());
	}
	file.close();
	return true;
}
*/

int CkdskManifestFile::find(int crc, const char *md5, const char *datafile) {

	for (std::vector<CkdskData>::iterator i = m_data->begin(); i != m_data->end(); i++) {
		CkdskData &data = *i;
		if (data.match(datafile, crc, md5) == true) {
			return true;
		}

	}
	return false;
}

bool CkdskManifestFile::isFileFound(const char *filename) {
	int n = 0;
	for (std::vector<CkdskData>::iterator i = m_data->begin(); i != m_data->end(); i++, n++) {
		CkdskData &data = *i;
		if (data.getName().compare(filename) == 0) {
			return true;
		}
	}
	return false;
}


CkdskData *CkdskManifestFile::findFile(const char *filename) {
	int n = 0;
	for (std::vector<CkdskData>::iterator i = m_data->begin(); i != m_data->end(); i++, n++) {
		CkdskData &data = *i;
		if (data.getName().compare(filename) == 0) {
			return &data;
		}
	}
	return 0;
}


/*
std::shared_ptr<CkdskData> CkdskManifestFile::findFile(const char *filename) {
	int n = 0;
	for (std::vector<CkdskData>::iterator i = m_data->begin(); i != m_data->end(); i++, n++) {
		CkdskData &data = *i;
		
		if (data.getName().compare(filename) == 0) {
			//CkdskData *ptr = &data;
			std::shared_ptr<CkdskData> datap;
			datap.reset(&data);
			return datap;
		}
	}
	return nullptr;
}
*/



// make crc in the real version
//class CkdskDiffContainer : public std::map<unsigned long, CkdskData> {};
class FilenameDiffContainer : public std::map<std::string, std::shared_ptr<CkdskData>> {};
class CRCDiffContainer : public std::map<unsigned int, std::shared_ptr<CkdskData>> {};

/**
	This class puts the Ckhdsk data into a map to
	create the chkdsk difference file


*/
class CkdskDiffFile : public std::vector<std::string> {
	std::unique_ptr<FilenameDiffContainer> m_filedata;
	std::unique_ptr<CRCDiffContainer> m_crcdata;
	std::string m_orginalName;
public:
	CkdskDiffFile() : m_filedata(std::make_unique<FilenameDiffContainer>()),
						m_crcdata(std::make_unique<CRCDiffContainer>()) {};

	virtual ~CkdskDiffFile() = default;

	bool read(const char *datafile);
	bool write(const char *datafile);
	bool add(const char *filepath, const char *name);
	CkdskData::Status find(unsigned int crc, const char *md5, const char *datafile);
	CkdskData *findFile(const char *filename);
	const char *getOrginalName() {
		return m_orginalName.c_str();
	}
	FilenameDiffContainer &getFileNames() {
		return *m_filedata;
	}
		
};

bool CkdskDiffFile::read(const char *datafile) {
	std::string text;
	std::ifstream file(datafile);
	if (file.is_open() == false) {
		return false;
	}

	while (std::getline(file, text)) {
		std::shared_ptr<CkdskData> data = std::make_shared<CkdskData>(text.c_str());

		std::string name = data->getName();
		push_back(name);
		m_filedata->insert(std::make_pair(name, data));
		m_crcdata->insert(std::make_pair(data->getCrc(), data));
	}
	file.close();

	return true;
}

bool CkdskDiffFile::write(const char *datafile) {
	std::ofstream file(datafile);
	if (file.is_open() == false) {
		return false;
	}

	for (auto ii = m_filedata->begin(); ii != m_filedata->end(); ++ii) {
		auto data = ii->second;
		file << data->diffStatusline() + '\n';
		//printf("%s", name.c_str());
	}
	file.close();
	return true;
}

bool CkdskDiffFile::add(const char *filepath, const char *name) {
	CkdskData *ckdskData = 0;
	if ((ckdskData = findFile(name)) == 0) {
		CLogger &logger = CLogger::getLogger();
		
		logger.log(LOG_OK, CLogger::Level::INFO, "Image: %s", filepath);
		unsigned long size;
		if (SAUtils::fileSize(filepath, &size) == false) {
			// Log error
		}
		logger.log(LOG_OK, CLogger::Level::INFO, "Size of image: %s is %d", filepath, size);
		CIDKCrc Crc;
		logger.log(LOG_OK, CLogger::Level::INFO, "Image: %s", filepath);
		std::string buf;
		SAUtils::getFileContents(filepath, buf);
		MD5 md5(buf);
		std::string md5Str = md5.hexdigest();
		logger.log(LOG_OK, CLogger::Level::INFO, "MD5 of image: %s is %s", filepath, md5Str.c_str());
		unsigned int crc = Crc.crc((unsigned char *)buf.c_str(), size);
		logger.log(LOG_OK, CLogger::Level::INFO, "CRC from image: %s is %x", filepath, crc);
		
		time_t created = SAUtils::createTime(filepath);
		//logger.log(LOG_OK, CLogger::INFO, "Create time of image: %s", m_createTime.toLogString().c_str());
		time_t modified = SAUtils::modTime(filepath);
		std::shared_ptr<CkdskData> ckdskData = std::make_shared<CkdskData>(name, size, crc, md5Str.c_str(), created, modified);
		push_back(name);
		m_filedata->insert(std::make_pair(name, std::move(ckdskData)));
		m_crcdata->insert(std::make_pair(crc, std::move(ckdskData)));
		return true;
	}
	return false;
}


CkdskData *CkdskDiffFile::findFile(const char *filename) {
	for (auto ii = m_filedata->begin(); ii != m_filedata->end(); ++ii) {
		auto data = ii->second;
		if (data->getName().compare(filename) == 0) {
			return data.get();
		}
	}
	return nullptr;
}

CkdskData::Status CkdskDiffFile::find(unsigned int crc, const char *md5, const char *datafile) {
	std::map<std::string, std::shared_ptr<CkdskData> >::iterator fit;
	std::map<unsigned int, std::shared_ptr<CkdskData> >::iterator cit;

	if ((fit = m_filedata->find(datafile)) == m_filedata->end()) {
		if ((cit = m_crcdata->find(crc)) == m_crcdata->end()) {
			return CkdskData::Status::Missing;
		}
		else {
			std::shared_ptr<CkdskData> value = cit->second;
			m_orginalName = value->getName();
			return CkdskData::Status::NameChanged;
		}
	}
	std::shared_ptr<CkdskData> value = fit->second;
	if (value->match(datafile, crc, md5) == true) {
		return CkdskData::Status::Unchanged;
	}
	return value->getStatus();
}


void CheckDisk::setArchivePath(const char *archivePath) {
	m_archivePath = archivePath;
	
}


CheckDisk::~CheckDisk() {}

bool CheckDisk::makeCheckData(const char *targetdir) {

	std::string path = targetdir + std::string("/.sia");
	if (SAUtils::DirExists(path.c_str()) == false) {
		if (SAUtils::mkDir(path.c_str()) == false) {
			throw std::exception();
		}
	}
	path += std::string("/chkdsk");
	if (SAUtils::DirExists(path.c_str()) == false) {
		if (SAUtils::mkDir(path.c_str()) == false) {
			throw std::exception();
		}
	}
	return makeCheckData(targetdir, path.c_str());
	
}


bool CheckDisk::makeCheckData(const char *targetdir, const char *savefolder) {
	FileList_Ptr filelist = SAUtils::getFiles_(targetdir);

	// do files
	std::string fpath = savefolder + std::string("/fdata.csv");
	std::ofstream filedat(fpath.c_str());
	if (filedat.is_open() == false) {
		return false;
	}
	fpath = savefolder + std::string("/fdata.xml");
	std::ofstream filexml(fpath.c_str());
	if (filexml.is_open() == false) {
		return false;
	}
	filexml <<	"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
					<<	"<FileList>\n";

	std::string targetdirStr = targetdir;

	// do directories
	std::string dpath = savefolder + std::string("/ddata.csv");
	std::ofstream dir(dpath.c_str());
	if (dir.is_open() == false) {
		return false;
	}

	for (std::vector<std::string>::iterator i = filelist->begin(); i != filelist->end(); i++) {
		std::string name = *i;
		if (name.compare(".") == 0 || name.compare("..") == 0 ) {
			continue;
		}
		if (name.compare(".chk") == 0 ) {
			continue;
		}
		std::string filepath = targetdirStr + "/" + name;
		if (SAUtils::IsFile(filepath.c_str()) == false) {
			//printf("%s\n", name->c_str());
			dir << name +'\n';
			filexml <<	"<Folder>\n"
				<< writeTag("Name", name.c_str(), 1);
			filexml <<	"</Folder>\n";
		}
	}
	dir.close();

	for (std::vector<std::string >::iterator i = filelist->begin(); i != filelist->end(); i++) {
		
		std::string name = *i;
		std::string filepath = targetdirStr + "/" + name;
		char c = (name)[0];
		if (c == '.' ) {
			continue;
		}
		if (SAUtils::IsFile(filepath.c_str()) == true) {
			
			
			std::string f_name = name;
			CkdskData dskData(filepath.c_str(), f_name.c_str());
			std::string line = dskData.toString();

			filedat << line.c_str() << '\n';
			ExifDateTime modified;
			modified.setDateTime(dskData.getModified());
			std::string checkedOut = (dskData.getCheckedOut()) ? "True" : "";
			filexml <<	"<File>\n"
					<< writeTag("Name", name.c_str(), 1)
					<< writeTag("Size", dskData.getSize(), 1)
					<< writeTag("CRC", dskData.getCrc(), 1)
					<< writeTag("Md5", dskData.getMd5().c_str(), 1)
					<< writeTag("Modified", modified.toString(), 1)
					<< writeTag("CheckedOut", checkedOut.c_str(), 1);
			filexml <<	"</File>\n";


		}
	}
	filedat.close();
	filexml <<	"</FileList>\n";
	filexml.close();

	// test only 
	/*
	if (check(targetdir, savefolder) == false) {
		return false;
	}
	*/
	return true;
}

bool CheckDisk::makeXML(const char *targetdir) {

	// do files

	std::string fpath = targetdir + std::string("/fdata.csv");
	std::string targetdirStr = targetdir;
	std::string fpathxml = targetdirStr + std::string("/fdata.xml");
	CkdskManifestFile ckdskDataFile;
	if (ckdskDataFile.read(fpath.c_str()) == false) {
		return false;
	}
	std::ofstream filexml(fpathxml.c_str());

	filexml <<	"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
					<<	"<FileList>\n";


	CkdskDataContainer& list = ckdskDataFile.getData();
	for (std::vector<CkdskData>::iterator i = list.begin(); i != list.end(); i++) {
		CkdskData &data = *i;
		std::string filepath = targetdirStr + "/" + data.getName();
		std::string checkedOut = (data.getCheckedOut()) ? "True" : "";
		ExifDateTime modified;
		modified.setDateTime(data.getModified());
		filexml << "<FileItem>\n"
				<< writeTag("Filename", data.getName().c_str(), 1)
				<< writeTag("size", data.getSize(), 1)
				<< writeTag("CRC", data.getCrc(), 1)
				<< writeTag("Md5", data.getMd5(), 1)
				<< writeTag("ModifyTime", modified.toString(), 1)
				<< writeTag("CheckedOut", checkedOut.c_str(), 1);
		filexml << "</FileItem>\n";

	}

	filexml << "</FileList>\n";
	filexml.close();
	return true;
}

bool CheckDisk::update(const char *targetdir, const char *targetfile) {

	std::string targetStr = targetdir;
	std::string yearStr = targetStr.substr(0, 4);
	std::string path = m_archivePath + '/' + yearStr + '/';
	path += targetdir + std::string("/chdsk");
	std::string fpath = path + std::string("/fdata.csv");
	if (SAUtils::DirExists(path.c_str()) == false) {
		if (SAUtils::mkDir(path.c_str()) == false) {
			throw std::exception();
		}
	}
	std::string fullpath = m_archivePath;
	fullpath += '/';
	fullpath += yearStr;
	fullpath += '/';
	fullpath += targetStr;
	fullpath += "/images/";
	fullpath += targetfile;
	if (SAUtils::FileExists(path.c_str()) == false) {
		throw std::exception();
	}
	CkdskManifestFile ckdskDataFile;
	if (SAUtils::FileExists(fpath.c_str()) == true) {
		if (ckdskDataFile.read(fpath.c_str()) == false) {
			return false;
		}
	}
	if (ckdskDataFile.add(fullpath.c_str(), targetfile) == false) {
		return false;
	}
	if (ckdskDataFile.write(fpath.c_str()) == false) {
		return false;
	}
	if (makeXML(path.c_str()) == false) {
		return false;
	}

	return true;
}

//bool CheckDisk::check(const char *targetdir) {
//	return check(targetdir, targetdir);
//}

bool CheckDisk::checkMissing(CkdskDiffFile &ckdskDiffFile, FileList_Ptr &filelist, VisitingObject *visitingObject, const char *address) {
	bool found = false;
	// This is a very slow way? better ways must be found.
	for (std::vector<std::string>::iterator j = ckdskDiffFile.begin(); j != ckdskDiffFile.end(); j++) {
		std::string &chkname = *j;
		for (std::vector<std::string>::iterator i = filelist->begin(); i != filelist->end(); i++) {
			std::string name = *i;
			if (chkname.compare(name) == 0) {
				found = true;
				break;
			}
		}
		if (found == false) {
			std::string fullAddress = address; fullAddress += '/'; fullAddress += chkname;
			ReportStatus status = ReportStatus::Status::Missing;
			visitingObject->process(fullAddress.c_str(), status);
		}
		found = false;
	}
	return true;
}

bool CheckDisk::check(const char *targetdir, const char *checkFilePath, const char *address, VisitingObject& visitingObject) {
	// Read the target folder
	FileList_Ptr filelist = SAUtils::getFiles_(targetdir);
	std::string path = checkFilePath;
	// Check the folder the manifest file folder exists
	if (SAUtils::DirExists(path.c_str()) == false) {
		return false;
	}
	// Check the manifest file exists
	std::string fpath = path + std::string("/fdata.csv");
	if (SAUtils::IsFile(fpath.c_str()) == false) {
		return false;
	}
	CkdskDiffFile ckdskDiffFile;
	// read the manifest file
	ckdskDiffFile.read(fpath.c_str());

	bool errors = false;

	if (checkMissing(ckdskDiffFile, filelist, &visitingObject, address) == false) {
		return false;
	}
	// Iterate round the files in the target folder
	CLogger &logger = CLogger::getLogger();
	std::string targetdirStr = targetdir;
	for (std::vector<std::string>::iterator i = filelist->begin(); i != filelist->end(); i++) {
		std::string name = *i;
		std::string filepath = targetdirStr + "/" + name;
		std::string fullAddress = address; fullAddress += '/'; fullAddress += name;

		if (SAUtils::IsFile(filepath.c_str()) == true) {
			unsigned long size;
			if (SAUtils::fileSize(filepath.c_str(), &size) == false) {
				// Log error
			}
			CIDKCrc Crc;	
			logger.log(LOG_OK, CLogger::Level::INFO, "Image: %s", filepath.c_str());
			
			
			std::string buf;
			SAUtils::getFileContents(filepath.c_str(), buf);
			MD5 md5(buf);
			std::string md5Str = md5.hexdigest();
			unsigned int crc = Crc.crc((unsigned char *)buf.c_str(), size);
			CkdskData::Status fileStatus;
			ReportStatus status;
			std::string orginal;
			fileStatus = ckdskDiffFile.find(crc, md5Str.c_str(), name.c_str());
			switch (fileStatus) 
			{	
			case CkdskData::Status::Unchanged:
				status = ReportStatus::Status::Unchanged;
				ckdskDiffFile.add(filepath.c_str(), name.c_str());
				break;
			case CkdskData::Status::ContentChanged:
				status = ReportStatus::Status::ContentChanged;
				ckdskDiffFile.add(filepath.c_str(), name.c_str());
				break;
			case CkdskData::Status::NameChanged:
				status = ReportStatus::Status::NameChanged;
				orginal = ckdskDiffFile.getOrginalName();
				break;
			case CkdskData::Status::Added:
				status = ReportStatus::Status::Added;
				break;
			default:
				status = ReportStatus::Status::Unknown;
			}
			
			if (orginal.length() == 0) {
				visitingObject.process(fullAddress.c_str(), status);
			}
			else {
				visitingObject.process(fullAddress.c_str(), status, orginal.c_str());
			}
			
		}
	}
	std::string epath = path + std::string("/error.csv");
	ckdskDiffFile.write(epath.c_str());
	return !errors;

}



std::string CheckDisk::writeTag(const char *tag, const std::string& value, int tab) {
	std::ostringstream xml;
	for (int i = 0; i < tab; i++) {
		xml << '\t';
	}
	if (!value.empty()) {
		xml << "<" << tag << ">" << value << "</" << tag << ">\n";
	} else {
		xml << "<" << tag << "/>\n";
	}
	return xml.str();
}

std::string CheckDisk::writeTag(const char *tag, const unsigned int value, int tab) {
	std::ostringstream xml;
	for (int i = 0; i < tab; i++) {
		xml << '\t';
	}
	xml << "<" << tag << ">" << value << "</" << tag << ">\n";
	return xml.str();
}

bool CheckDisk::showCheckedOut(const char *filePath, SimpleImageList &imageList) {
	CkdskManifestFile ckdskManifestFile;
	// read the manifest file
	std::string fpath = filePath + std::string("/fdata.csv");
	if (ckdskManifestFile.read(fpath.c_str()) == false) {
		return false;
	}
	CkdskDataContainer& datacontainer = ckdskManifestFile.getData();
	for (auto i = datacontainer.begin(); i != datacontainer.end(); i++) {
		CkdskData &data = *i;
		if (data.getCheckedOut() == true) {
			imageList.push_back(data.getName());
		}
		
	}
	return true;
}

bool CheckDisk::ischeckedOut(const char *filePath, const char *image) {
	CkdskManifestFile ckdskManifestFile;
	// read the manifest file
	if (ckdskManifestFile.read(filePath) == false) {
		ErrorCode::setErrorCode(SIA_ERROR::IMAGE_NOT_FOUND);
		return false;
	}
	CkdskData* data = ckdskManifestFile.findFile(image);
	if (data == nullptr) {
		ErrorCode::setErrorCode(SIA_ERROR::IMAGE_NOT_FOUND);
		return false;
	}
	if (data->getCheckedOut() == true) {
		return true; // already checked out
	}
	return false;
}


unsigned int CheckDisk::getCrc(const char *filePath, const char *image) {
	CkdskManifestFile ckdskManifestFile;
	// read the manifest file
	if (ckdskManifestFile.read(filePath) == false) {
		ErrorCode::setErrorCode(SIA_ERROR::READ_ERROR);
		return 0;
	}
	CkdskData* data = ckdskManifestFile.findFile(image);
	if (data == nullptr) {
		ErrorCode::setErrorCode(SIA_ERROR::IMAGE_NOT_FOUND);
		return 0;
	}
	unsigned int crc = data->getCrc();	
	return crc;
}

bool CheckDisk::checkout(const char *filePath, const char *image) {
	CkdskManifestFile ckdskManifestFile;
	// read the manifest file
	if (ckdskManifestFile.read(filePath) == false) {
		ErrorCode::setErrorCode(SIA_ERROR::READ_ERROR);
		return false;
	}
	CkdskData* data = ckdskManifestFile.findFile(image);
	if (data == nullptr) {
		ErrorCode::setErrorCode(SIA_ERROR::IMAGE_NOT_FOUND);
		return false;
	}
	if (data->getCheckedOut() == true) {
		ErrorCode::setErrorCode(SIA_ERROR::ALREADY_CHECKED_OUT);
		return false; // already checked out
	}
	data->setCheckedOut(true);
	ckdskManifestFile.write(filePath);
	std::string path = filePath;
	int pos = path.find_last_of('/');
	path = path.substr(0, pos);
	if (makeXML(path.c_str()) == false) {
		ErrorCode::setErrorCode(SIA_ERROR::XML_WRITE_ERROR);
		return false;
	}
	return true;
}

bool CheckDisk::checkin(const char *filePath, const char *image) {
	CkdskManifestFile ckdskManifestFile;
	// read the manifest file
	ckdskManifestFile.read(filePath);
	CkdskData* data = ckdskManifestFile.findFile(image);
	if (data == nullptr) {
		ErrorCode::setErrorCode(SIA_ERROR::IMAGE_NOT_FOUND);
		return false;
	}
	if (data->getCheckedOut() == false) {
		return false; // already checked in
	}
	data->setCheckedOut(false);
	ckdskManifestFile.write(filePath);
	std::string path = filePath;
	int pos = path.find_last_of('/');
	path = path.substr(0, pos);
	if (makeXML(path.c_str()) == false) {
		return false;
	}
	return true;
}

} /* namespace simplearchive */
