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
#include "CIDKCrc.h"
#include "md5.h"

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
		m_size = -1;
		m_created = 0;
		m_modified = 0;
		m_crc = 0;
		m_status = Missing;
	};

	CkdskData(const char *dataString) {
		m_status = Missing;
		std::string data = dataString;
		// s << m_size << ':' << m_crc << ':' << m_md5 << ':' << m_created << ':' << m_modified << ':' << m_name;
		int delim1 = data.find_first_of(":");
		int delim2 = data.find_first_of(":", delim1+1);
		int delim3 = data.find_first_of(":", delim2+1);
		int delim4 = data.find_first_of(":", delim3+1);
		int delim5 = data.find_first_of(":", delim4+1);

		std::string sizeStr = data.substr(0,delim1);
		m_size = strtol(sizeStr.c_str(),NULL,16);

		std::string crcStr = data.substr(delim1+1, delim2-(delim1+1));
		m_crc = strtol(crcStr.c_str(),NULL,16);

		m_md5 = data.substr(delim2+1, delim3-(delim2+1));

		std::string m_createdStr = data.substr(delim3+1, delim4-(delim3+1));
		m_created = strtol(crcStr.c_str(),NULL,16);

		std::string m_modifiedStr = data.substr(delim4+1, delim5-(delim4+1));
		m_modified = strtol(m_modifiedStr.c_str(),NULL,16);

		m_name = data.substr(delim5+1, data.length());
	}



	CkdskData(const char *name, int size, int crc, const char *md5, time_t created, time_t modified) {
		m_name = name;
		m_size = size;
		m_crc = crc;
		m_md5 = md5;
		m_status = Added;
		m_created = created;
		m_modified = modified;
	}


	CkdskData(const CkdskData &obj) {
			m_name = obj.m_name;
			m_size = obj.m_size;
			m_crc = obj.m_crc;
			m_md5 = obj.m_md5;
			m_status = obj.m_status;
			m_created = obj.m_created;
			m_modified = obj.m_modified;
		}

	CkdskData &operator=(const CkdskData &obj) {
		m_name = obj.m_name;
		m_size = obj.m_size;
		m_crc = obj.m_crc;
		m_md5 = obj.m_md5;
		m_status = obj.m_status;
		m_created = obj.m_created;
		m_modified = obj.m_modified;
		return *this;
	}

	std::string diffStatusline() {
		return m_name + ":" + statusString();
	}

	std::string toString() {
		std::stringstream s;
		s << m_size << ':' << m_crc << ':' << m_md5 << ':' << m_created << ':' << m_modified << ':' << m_name;
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

class CkdskManifestFile {
	CkdskDataContainer *m_data;
	void insert(int crc, std::string);
public:
	CkdskManifestFile() {
		m_data = new CkdskDataContainer;
	};
	virtual ~CkdskManifestFile() {
		delete m_data;
	};

	bool read(const char *datafile);
	/// Write the manifest file
	bool write(const char *datafile);
	/// Write the status file
	bool writeStatus(const char *datafile);
	//bool add(const char *name, int crc, const char *md5);
	int find(int crc, const char *md5, const char *datafile);
	CkdskData *findFile(const char *filename);
	bool add(const char *name);

	CkdskDataContainer*& getData() {
		return m_data;
	}

};

bool CkdskManifestFile::add(const char *name) {
	CkdskData *ckdskData = 0;
	if ((ckdskData = findFile(name)) == 0) {
		// new
		// test old

		long int size = 12345;
		long int crc;
		time_t created;
		time_t modified;
		const char *md5 = "4f794af4633152a8798c1cf654f67";
		time((time_t *)&crc);
		time(&created);
		time(&modified);
		ckdskData = new CkdskData( name, size, crc, md5, created, modified);
		m_data->insert(m_data->end(), *ckdskData);
		return true;
	}
	return false;
}

bool CkdskManifestFile::read(const char *datafile) {
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

CkdskData *CkdskManifestFile::findFile(const char *filename) {
	int n = 0;
	for (std::vector<CkdskData>::iterator i = m_data->begin(); i != m_data->end(); i++, n++) {
		CkdskData &data = *i;
		if (data.getName().compare(filename) == true) {
			return &data;
		}
	}
	return 0;
}


/*
 * This class puts the Ckhdsk data into a map to
 * create the chkdsk difference file
 */

// make crc in the real version
//class CkdskDiffContainer : public std::map<unsigned long, CkdskData> {};
class CkdskDiffContainer : public std::map<std::string, CkdskData> {};
class CkdskDiffFile {
	CkdskDiffContainer *m_data;

	void insert(int crc, std::string);
public:
	CkdskDiffFile() {
		m_data = new CkdskDiffContainer;
	};
	virtual ~CkdskDiffFile() {
		delete m_data;
	};

	bool read(const char *datafile);
	bool write(const char *datafile);
	bool add(const char *name);
	int find(int crc, const char *md5, const char *datafile);
	CkdskData *findFile(const char *filename);
};

bool CkdskDiffFile::read(const char *datafile) {
	char text[256];
	std::ifstream file(datafile);
	if (file.is_open() == false) {
		return false;
	}

	while (file.getline(text, 100)) {
		CkdskData data = CkdskData(text);

		std::string name = data.getName();
		m_data->insert(std::make_pair(name, data));
		//m_data->insert(std::make_pair(data.getCrc(), data));
	}
	file.close();

	return true;
}

bool CkdskDiffFile::write(const char *datafile) {
	std::ofstream file(datafile);
	if (file.is_open() == false) {
		return false;
	}

	for (std::map<std::string, CkdskData>::iterator ii = m_data->begin(); ii != m_data->end(); ++ii) {
		CkdskData &data = ii->second;
		file << data.diffStatusline() + '\n';
		//printf("%s", name.c_str());
	}
	file.close();
	return true;
}

bool CkdskDiffFile::add(const char *name) {
	CkdskData *ckdskData = 0;
	if ((ckdskData = findFile(name)) == 0) {
		// new
		// test old
		long int size = 12346;
		long int crc;
		time_t created;
		time_t modified;
		const char *md5 = "4f794af4633152a8798c1cf654f67";
		time((time_t *)&crc);
		time(&created);
		time(&modified);
		ckdskData = new CkdskData( name, size, crc, md5, created, modified);
		m_data->insert(std::make_pair(name, *ckdskData));
		return true;
	}
	return false;
}


CkdskData *CkdskDiffFile::findFile(const char *filename) {
	for (std::map<std::string, CkdskData>::iterator ii = m_data->begin(); ii != m_data->end(); ++ii) {
		CkdskData &data = ii->second;
		if (data.getName().compare(filename) == 0) {
			return &data;
		}
	}
	return 0;
}

int CkdskDiffFile::find(int crc, const char *md5, const char *datafile) {
	std::map<std::string, CkdskData>::iterator it;

	if ((it = m_data->find(datafile)) == m_data->end()) {
		return false;
	}
	CkdskData value = it->second;
	if (value.match(datafile, crc, md5) == true) {
		// temp for testing
		value.setStatus(CkdskData::Unchanged);
		return true;
	}
	return false;
}

/*
CheckDisk::CheckDisk(const char *archivePath) {
	m_archivePath = archivePath;
}
*/

CheckDisk::~CheckDisk() {}

bool CheckDisk::makeCheckData(const char *targetdir) {

	std::string path = targetdir + std::string("/.chdsk");
	if (SAUtils::DirExists(path.c_str()) == false) {
		if (SAUtils::mkDir(path.c_str()) == false) {
			throw std::exception();
		}
	}
	return makeCheckData(targetdir, path.c_str());
}


bool CheckDisk::makeCheckData(const char *targetdir, const char *savefolder) {
	std::vector<std::string *> *filelist = SAUtils::getFiles(targetdir);

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

	for (std::vector<std::string *>::iterator i = filelist->begin(); i != filelist->end(); i++) {
		std::string *name = *i;
		if (name->compare(".") == 0 || name->compare("..") == 0 ) {
			continue;
		}
		if (name->compare(".chk") == 0 ) {
			continue;
		}
		std::string filepath = targetdirStr + "/" + *name;
		if (SAUtils::IsFile(filepath.c_str()) == false) {
			//printf("%s\n", name->c_str());
			dir << *name +'\n';
			filexml <<	"<Folder>\n"
				<< writeTag("Name", name->c_str(), 1);
			filexml <<	"</Folder>\n";
		}
	}
	dir.close();

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
			time_t crc;
			time_t created;
			time_t modified;
			const char *md5 = "4f794af4633152a8798c1cf654f67";
			time((time_t *)&crc);
			time(&created);
			time(&modified);
			std::string f_name = *name;
			CkdskData dskData( f_name.c_str(), size, crc, md5, created, modified);
			std::string line = dskData.toString();

			filedat << line.c_str() << '\n';

			filexml <<	"<File>\n"
					<< writeTag("Name", name->c_str(), 1)
					<< writeTag("Size", size, 1)
					<< writeTag("CRC", crc, 1)
					<< writeTag("Md5", md5, 1);
			filexml <<	"</File>\n";


		}
	}
	filedat.close();
	filexml <<	"</FileList>\n";
	filexml.close();

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
	/*
	std::string targetStr = targetdir;
	std::string yearStr = targetStr.substr(0,4);
	std::string path = m_archivePath + '/' + yearStr + '/';
	path += targetdir + std::string("/.chdsk");
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
	*/
	return true;
}

bool CheckDisk::check(const char *targetdir) {
	return check(targetdir, targetdir);
}

bool CheckDisk::check(const char *targetdir, const char *savedir) {
	// Read the target folder
	std::vector<std::string *> *filelist = SAUtils::getFiles(targetdir);
	std::string path = savedir;
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
	int tmpcrc = 23;

	// Iterate round the files in the target folder
	std::string targetdirStr = targetdir;
	for (std::vector<std::string *>::iterator i = filelist->begin(); i != filelist->end(); i++) {
		std::string *name = *i;
		std::string filepath = targetdirStr + "/" + *name;
		if (SAUtils::IsFile(filepath.c_str()) == true) {
			const char *md5 = "9c794af4633152a8798c1cf654f67";
			if (ckdskDiffFile.find(tmpcrc++, md5, name->c_str()) == false) {
				errors = true;
				ckdskDiffFile.add(name->c_str());
			}
		}
	}
	std::string epath = path + std::string("/error");
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

std::string CheckDisk::writeTag(const char *tag, const int value, int tab) {
	std::ostringstream xml;
	for (int i = 0; i < tab; i++) {
		xml << '\t';
	}
	xml << "<" << tag << ">" << value << "</" << tag << ">\n";
	return xml.str();
}
} /* namespace simplearchive */
