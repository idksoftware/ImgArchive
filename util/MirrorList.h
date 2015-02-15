/*
 * MirrorSet.h
 *
 *  Created on: Oct 20, 2014
 *      Author: wzw7yn
 */

#ifndef MIRRORSET_H_
#define MIRRORSET_H_
#include <string>
#include <vector>
#include <stdio.h>
#include "DirectoryVisitor.h"
#include "Mirror.h"
#include "SAUtils.h"
namespace simplearchive {

class MirrorFile {
	std::string m_path;

public:
	MirrorFile(const char *path) {
		m_path = path;

	}
	virtual ~MirrorFile() {
		//printf("deleting item %s\n", m_path.c_str());
	}
	void print() {
		printf("Item %s\n", m_path.c_str());
	}
	void doMirror();

	void doVerify();


};

class MirrorSet : public std::vector<MirrorFile *> {
	std::string m_path;
public:
	MirrorSet(const char *path) {
		m_path = path;
	}
	virtual ~MirrorSet() {
		//printf("deleting item %s\n", m_path.c_str());
	}
	void print() {

		printf("Set %s\n", m_path.c_str());
		for (std::vector<MirrorFile *>::iterator i = this->begin(); i != this->end(); i++) {
			MirrorFile *data = *i;
			data->print();
		}

	}
	void doMirror() {
		//printf("Set %s\n", m_path.c_str());
		Mirror &mirror = Mirror::get();
		mirror.mkDir(m_path.c_str());
		for (std::vector<MirrorFile *>::iterator i = this->begin(); i != this->end(); i++) {
			MirrorFile *data = *i;
			data->doMirror();
		}
	}

	void doVerify() {
		//printf("Set %s\n", m_path.c_str());
		verifyDir(m_path.c_str());
		for (std::vector<MirrorFile *>::iterator i = this->begin(); i != this->end(); i++) {
			MirrorFile *data = *i;
			data->doVerify();
		}
	}
	void verifyDir(const char *path) {
		Mirror &mirror = Mirror::get();
		if (mirror.DirExists(path) == false) {
			//printf("Folder not found %s\n", m_path.c_str());
		}
	}
};
class MirrorSets : public std::vector<MirrorSet *> {
public:
	void print() {

		for (std::vector<MirrorSet *>::iterator i = this->begin(); i != this->end(); i++) {
			MirrorSet *data = *i;
			data->print();
		}

	}
	void doMirror() {

		for (std::vector<MirrorSet *>::iterator i = this->begin(); i != this->end(); i++) {
			MirrorSet *data = *i;
			data->doMirror();
		}

	}
	void doVerify() {
		for (std::vector<MirrorSet *>::iterator i = this->begin(); i != this->end(); i++) {
			MirrorSet *data = *i;
			data->doVerify();
		}
	}
};

class MirrorList : public FolderVisitor {

	static int m_count;
	static MirrorSets *m_mirrorSets;

	MirrorSet *m_mirrorSet;
	static std::string m_rootFolder;
	int m_rootLength;
protected:
	virtual bool onStart(const char *path) {
		if (m_mirrorSets == 0) {
			m_mirrorSets = new MirrorSets;
		}
		std::string fullPath = path;
		std::string relPath = fullPath.substr(m_rootLength);
		m_mirrorSet = new MirrorSet(relPath.c_str());

		m_mirrorSets->insert(m_mirrorSets->end(), m_mirrorSet);
		//printf("==== Start ==== %d \n", m_count++);
		return true;
	};

	virtual bool onFile(const char *path) {

		std::string fullPath = path;
		std::string relPath = fullPath.substr(m_rootLength);
		//printf("File: %s\n", relPath.c_str());
		m_mirrorSet->insert(m_mirrorSet->end(), new MirrorFile(relPath.c_str()));
		return true;
	};
	virtual bool onDirectory(const char *path) {
		std::string fullPath = path;
		std::string relPath = fullPath.substr(m_rootLength);
		//printf("Dir:  %s\n", relPath.c_str());
		return true;

	};
	virtual bool onEnd() {
		//printf("===== End =====%d \n", --m_count);
		return true;
	};
	virtual FolderVisitor *make() {
		return new MirrorList;
	}
public:
	MirrorList();

	virtual ~MirrorList();
	static MirrorSets *getMirrorSets() {
		return m_mirrorSets;
	}

	static const std::string& getRootFolder() {
		return m_rootFolder;
	}

	static void setRootFolder(const char *path) {
		m_rootFolder = path;
	}
};

} /* namespace simplearchive */
#endif /* MIRRORSET_H_ */
