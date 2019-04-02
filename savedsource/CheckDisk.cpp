/*
 * CheckDisk.cpp
 *
 *  Created on: May 19, 2014
 *      Author: wzw7yn
 */

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

namespace simplearchive {
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
		Changed,
		Missing,
		Added,
		Unchanged,
	} Status;
private:
	std::string m_name;
	int m_crc;
	std::string m_md5;
	Status m_status;
	time_t m_created;
	time_t m_modified;
	int m_size;
public:
	CkdskData(){
		m_created = 0;
		m_modified = 0;
		m_crc = 0;
		m_status = Missing;
	};

	CkdskData(const char *dataString) {
		m_status = Missing;
		std::string data = dataString;
		int delim1 = data.find_first_of(":");
		int delim2 = data.find_first_of(":", delim1+1);
		int delim3 = data.find_first_of(":", delim2+1);
		int delim4 = data.find_first_of(":", delim3+1);
		std::string crcStr = data.substr(0,delim1);
		m_crc = strtol(crcStr.c_str(),NULL,16);
		m_md5 = data.substr(delim1+1, delim2-(delim1+1));
		std::string m_createdStr = data.substr(delim2+1, delim3-(delim2+1));
		m_created = strtol(crcStr.c_str(),NULL,16);
		std::string m_modifiedStr = data.substr(delim3+1, delim4-(delim3+1));
		m_modified = strtol(m_modifiedStr.c_str(),NULL,16);
		m_name = data.substr(delim4+1, data.length());
	}



	CkdskData(const char *name, int crc, const char *md5, time_t created, time_t modified) {
		m_name = name;
		m_crc = crc;
		m_md5 = md5;
		m_status = Missing;
		m_created = created;
		m_modified = modified;
	}


	CkdskData(const CkdskData &obj) {
			m_name = obj.m_name;
			m_crc = obj.m_crc;
			m_md5 = obj.m_md5;
			m_status = obj.m_status;
			m_created = obj.m_created;
			m_modified = obj.m_modified;
		}
	
	CkdskData &operator=(const CkdskData &obj) {
		m_name = obj.m_name;
		m_crc = obj.m_crc;
		m_md5 = obj.m_md5;
		m_status = obj.m_status;
		m_created = obj.m_created;
		m_modified = obj.m_modified;
		return *this;
	}

	std::string line() {
		return (m_name + ":" + statusString());
	}

	std::string toString() {
		std::stringstream s;
		s << m_crc << ':' << m_md5 << ':' << m_created << ':' << m_modified << ':' << m_name;
		return s.str();
	}


	bool match(const char *name, int crc, const char *md5) {
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
			m_status = Changed;
			//printf("%s: Changed\n", name);
			return true;
		}
		// No match
		//printf("%s: No Match\n", name);
		return false;

	}

	bool add(const char *name, int crc, const char *md5);

	int getCrc() const {
		return m_crc;
	}

	void setCrc(int crc) {
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

	void setStatus(Status status) {
		m_status = status;
	}

	const char *statusString() {
		switch (m_status) {
		case Unknown:
			return "Unknown";
		case Changed:
			return "Changed";
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

class CkdskDataFile {
	CkdskDataContainer *m_data;
	void insert(int crc, std::string);
public:
	CkdskDataFile() {
		m_data = new CkdskDataContainer;
	};
	virtual ~CkdskDataFile() {
		delete m_data;
	};

	bool read(const char *datafile);
	bool write(const char *datafile);
	//bool add(const char *name, int crc, const char *md5);
	int find(int crc, const char *md5, const char *datafile);
	CkdskData *findFile(const char *filename);
	bool add(const char *name);

	CkdskDataContainer*& getData() {
		return m_data;
	}

};

bool CkdskDataFile::add(const char *name) {
	CkdskData *ckdskData = 0;
	if ((ckdskData = findFile(name)) == 0) {
		// new
		// test old
		long int crc = 9999;
		long int created = 55555;
		long int modified = 77777;
		const char *md5 = "4f794af4633152a8798c1cf654f67";
		//time(&crc);
		//time(&created);
		//time(&modified);
		ckdskData = new CkdskData( name, crc, md5, created, modified);
		m_data->insert(m_data->end(), *ckdskData);
		return true;
	}
	return false;
}

bool CkdskDataFile::read(const char *datafile) {
	char text[256];
	std::ifstream file(datafile);
	if (file.is_open() == false) {
		// empty?
		return true;
	}

	while (file.getline(text, 100)) {
		m_data->push_back(*(new CkdskData(text)));
	}
	file.close();

	return true;
}

bool CkdskDataFile::write(const char *datafile) {
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

int CkdskDataFile::find(int crc, const char *md5, const char *datafile) {

	for (std::vector<CkdskData>::iterator i = m_data->begin(); i != m_data->end(); i++) {
		CkdskData &data = *i;
		if (data.match(datafile, crc, md5) == true) {
			return true;
		}

	}
	return false;
}

CkdskData *CkdskDataFile::findFile(const char *filename) {
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
 * This class puts the Ckhdsk data into a map to
 * create the chkdsk diffence file
 */
class CkdskContainer : public std::map<unsigned long, CkdskData> {};

class CkdskFile {
	CkdskContainer *m_data;

	void insert(int crc, std::string);
public:
	CkdskFile() {
		m_data = new CkdskContainer;
	};
	virtual ~CkdskFile() {
		delete m_data;
	};

	bool read(const char *datafile);
	bool write(const char *datafile);
	bool add(const char *name);
	int find(int crc, const char *md5, const char *datafile);
	CkdskData *findFile(const char *filename);
};

bool CkdskFile::read(const char *datafile) {
	char text[256];
	std::ifstream file(datafile);
	if (file.is_open() == false) {
		return false;
	}

	while (file.getline(text, 100)) {
		CkdskData data = CkdskData(text);
		m_data->insert(std::make_pair(data.getCrc(), data));
	}
	file.close();

	return true;
}

bool CkdskFile::write(const char *datafile) {
	std::ofstream file(datafile);
	if (file.is_open() == false) {
		return false;
	}

	for (std::map<unsigned long, CkdskData>::iterator ii = m_data->begin(); ii != m_data->end(); ++ii) {
		CkdskData &data = ii->second;
		file << data.line() + '\n';
		//printf("%s", name.c_str());
	}
	file.close();
	return true;
}

bool CkdskFile::add(const char *name) {
	CkdskData *ckdskData = 0;
	if ((ckdskData = findFile(name)) == 0) {
		// new
		// test old
		long int crc = 99999;
		long int created = 8888;
		long int modified = 7777;
		const char *md5 = "4f794af4633152a8798c1cf654f67";
		//time(&crc);
		//time(&created);
		//time(&modified);
		ckdskData = new CkdskData( name, crc, md5, created, modified);
		m_data->insert(std::make_pair(ckdskData->getCrc(), *ckdskData));
		return true;
	}
	return false;
}


CkdskData *CkdskFile::findFile(const char *filename) {
	for (std::map<unsigned long, CkdskData>::iterator ii = m_data->begin(); ii != m_data->end(); ++ii) {
		CkdskData &data = ii->second;
		if (data.getName().compare(filename) == 0) {
			return &data;
		}
	}
	return 0;
}

int CkdskFile::find(int crc, const char *md5, const char *datafile) {

	/*
	for (std::vector<CkdskData>::iterator i = m_data->begin(); i != m_data->end(); i++) {
		CkdskData &data = *i;
		if (data.match(datafile, crc, md5) == true) {
			return true;
		}

	}
	*/
	return false;
}


CheckDisk::CheckDisk(const char *archivePath) {
	m_archivePath = archivePath;
}

CheckDisk::~CheckDisk() {}

bool CheckDisk::makeCheckData(const char *targetdir) {
	std::vector<std::string *> *filelist = SAUtils::getFiles(targetdir);

	std::string path = targetdir + std::string("/.sia");
	if (SAUtils::DirExists(path.c_str()) == false) {
		if (SAUtils::mkDir(path.c_str()) == false) {
			throw std::exception();
		}
		if (SAUtils::setHidden(path.c_str()) == false) {
			throw std::exception();
		}
	}
	path = targetdir + std::string("/.sia/chdsk");
	if (SAUtils::DirExists(path.c_str()) == false) {
		if (SAUtils::mkDir(path.c_str()) == false) {
			throw std::exception();
		}
	}
	// do files
	std::string fpath = path + std::string("/fdata.csv");
	std::ofstream filedat(fpath.c_str());
	if (filedat.is_open() == false) {
		return false;
	}
	fpath = path + std::string("/fdata.xml");
	std::ofstream filexml(fpath.c_str());
	if (filexml.is_open() == false) {
		return false;
	}
	filexml <<	"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
					<<	"<FileList>\n";
	int tmpcrc = 23;
	std::string targetdirStr = targetdir;
	for (std::vector<std::string *>::iterator i = filelist->begin(); i != filelist->end(); i++) {
		std::string *name = *i;
		std::string filepath = targetdirStr + "/" + *name;
		char c = (*name)[0];
		if (c == '.' ) {
			continue;
		}
		if (SAUtils::IsFile(filepath.c_str()) == true) {
			//printf("%s\n", name->c_str());
			unsigned long size = 0;
			if (SAUtils::fileSize(filepath.c_str(), &size) == false) {
				// Log error
			}
			char c_size[9];
			sprintf(c_size, "%.8x", size);
			unsigned int crc = tmpcrc++;
			char c_crc[9];
			sprintf(c_crc, "%.8x", crc);
			const char *md5 = "9c794af4633152a8798c1cf654f67";
			std::string line = std::string(c_size) + ":" + std::string(c_crc) + ":" + std::string(md5) + ":" + *name;
			filedat << line +'\n';

			filexml <<	"<FileItem>\n"
					<< writeTag("Filename", name->c_str(), 1)
					<< writeTag("size", c_size, 1)
					<< writeTag("CRC", c_crc, 1)
					<< writeTag("Md5", md5, 1);
			filexml <<	"</FileItem>\n";


		}
	}
	filedat.close();
	filexml <<	"</FileList>\n";
	filexml.close();
	// do directories
	std::string dpath = path + std::string("/ddata");
	std::ofstream dir(dpath.c_str());
	if (dir.is_open() == false) {
		return false;
	}

	for (std::vector<std::string *>::iterator i = filelist->begin(); i != filelist->end(); i++) {
		std::string *name = *i;
		std::string filepath = targetdirStr + "/" + *name;
		if (SAUtils::IsFile(filepath.c_str()) == false) {
			//printf("%s\n", name->c_str());
			dir << *name +'\n';
		}
	}
	dir.close();
	return true;
}

bool CheckDisk::makeXML(const char *targetdir) {

	// do files

	std::string fpath = targetdir + std::string("/fdata.csv");
	std::string targetdirStr = targetdir;
	std::string fpathxml = targetdirStr + std::string("/fdata.xml");
	CkdskDataFile ckdskDataFile;
	if (ckdskDataFile.read(fpath.c_str()) == false) {
		return false;
	}
	std::ofstream filexml(fpathxml.c_str());

	filexml <<	"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
					<<	"<FileList>\n";


	CkdskDataContainer*& list = ckdskDataFile.getData();
	for (std::vector<CkdskData>::iterator i = list->begin(); i != list->end(); i++) {
	
		CkdskData &data = *i;
		std::string filepath = targetdirStr + "/" + data.getName();
		filexml <<	"<FileItem>\n"
				<< writeTag("Filename", data.getName().c_str(), 1)
				<< writeTag("size", data.getSize(), 1)
				<< writeTag("CRC", data.getCrc(), 1)
				<< writeTag("Md5", data.getMd5(), 1)
				<< writeTag("ModifyTime", data.getModified(), 1);
		filexml <<	"</FileItem>\n";
		
	}

	filexml <<	"</FileList>\n";
	filexml.close();
	return true;
}

bool CheckDisk::update(const char *targetdir, const char *targetfile) {


	std::string targetStr = targetdir;
	std::string yearStr = targetStr.substr(0,4);
	std::string path = m_archivePath + '/' + yearStr + '/';
	path += targetdir + std::string("/.sia/chdsk");
	std::string fpath = path + std::string("/fdata.csv");
	if (SAUtils::DirExists(path.c_str()) == false) {
		if (SAUtils::mkDir(path.c_str()) == false) {
			throw std::exception();
		}
	}
	CkdskDataFile ckdskDataFile;
	if (ckdskDataFile.read(fpath.c_str()) == false) {
		return false;
	}
	if (ckdskDataFile.add(targetfile) == false) {
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

bool CheckDisk::check(const char *targetdir) {
	std::vector<std::string *> *filelist = SAUtils::getFiles(targetdir);
	std::string path = targetdir + std::string("/.sia/chdsk");
	if (SAUtils::DirExists(path.c_str()) == false) {
		return false;
	}
	std::string fpath = path + std::string("/fdata");
	if (SAUtils::IsFile(fpath.c_str()) == false) {
		return false;
	}
	CkdskDataFile ckdskDataFile;
	ckdskDataFile.read(fpath.c_str());


	bool errors = false;
	int tmpcrc = 23;
	std::string targetdirStr = targetdir;
	for (std::vector<std::string *>::iterator i = filelist->begin(); i != filelist->end(); i++) {
		std::string *name = *i;
		std::string filepath = targetdirStr + "/" + *name;
		if (SAUtils::IsFile(filepath.c_str()) == true) {
			const char *md5 = "9c794af4633152a8798c1cf654f67";
			if (ckdskDataFile.find(tmpcrc++, md5, name->c_str()) == false) {
				errors = true;
			}
		}
	}
	std::string epath = path + std::string("/error");
	ckdskDataFile.write(epath.c_str());
	return errors;

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

std::string CheckDisk::writeTag(const char *tag, const int value, int tab) {
	std::ostringstream xml;
	for (int i = 0; i < tab; i++) {
		xml << '\t';
	}
	xml << "<" << tag << ">" << value << "</" << tag << ">\n";
	return xml.str();
}
} /* namespace simplearchive */
