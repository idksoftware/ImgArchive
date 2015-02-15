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
#include <list>
#include <stdio.h>
#include "DirectoryVisitor.h"
#include "Mirror.h"
#include "SAUtils.h"
#include "MirrorList.h"
#include "CheckDisk.h"

namespace simplearchive {
/**
 * @brief This class is the base class that both file and directory nodes are derived from.
 *
 */
class MirrorNode {
public:
	typedef enum {
		FileNode,
		DirNode,
	} NodeType;
protected:
	static NodeType m_FileType;
	static NodeType m_DirType;

public:
	MirrorNode() {};
	virtual ~MirrorNode() {};
	/// Prints debud infomation to standard out
	virtual void print() = 0;
	/// Mirrors the node item.
	virtual void doMirror() = 0;
	/// Sync the source data with the mirror (i.e. copy any missing data to mirror).
	virtual bool doSyncMirror() = 0;
	/// Sync the mirror data with the source (i.e. copy any missing data to source).
	virtual bool doSyncSource() = 0;
	/// Checks the source data is present in the mirror
	virtual void doMirrorInSync() = 0;
	/// Checks the mirror data is present in the source
	virtual void doSourceInSync() = 0;
	/// Checks the mirror data is consistent with the source data
	virtual void doVerify() = 0;
	/// Returns the node type (FileNode or DirNode).
	virtual NodeType isNode() = 0;
};

class MirrorFile : public MirrorNode {
	std::string m_path;

public:
	MirrorFile(const char *path) {
		m_path = path;
		//printf("Mirror File %s\n", m_path.c_str());
	}
	virtual ~MirrorFile() {
		//printf("deleting item %s\n", m_path.c_str());
	}
	void print() {
		printf("Item %s\n", m_path.c_str());
	}
	void doMirror();
	bool doSyncMirror();
	bool doSyncSource();
	void doMirrorInSync();
	void doSourceInSync();
	void doVerify();

	NodeType isNode() { return m_FileType; };
};

class MirrorDir : public MirrorNode {
	std::string m_path;

public:
	MirrorDir(const char *path) {
		m_path = path;
		//printf("MirrorDir %s\n", m_path.c_str());
	}
	virtual ~MirrorDir() {
		//printf("deleting item %s\n", m_path.c_str());
	}
	void print() {
		printf("Item %s\n", m_path.c_str());
	}
	void doMirror();
	bool doSyncMirror();
	bool doSyncSource();
	void doMirrorInSync();
	void doSourceInSync();
	void doVerify();

	NodeType isNode() { return m_DirType; };
};

class MirrorSet : public std::vector<MirrorNode *> {

	std::string m_path;
public:
	MirrorSet(const char *path) {
		m_path = path;
	}
	virtual ~MirrorSet() {
		//printf("deleting item %s\n", m_path.c_str());
		for (std::vector<MirrorNode *>::iterator i = this->begin(); i != this->end(); i++) {
			MirrorNode *data = *i;
			delete data;
		}
		this->clear();
	}
	void print() {

		printf("Set %s\n", m_path.c_str());
		for (std::vector<MirrorNode *>::iterator i = this->begin(); i != this->end(); i++) {
			MirrorFile *data = (MirrorFile *)*i;
			data->print();
		}

	}
	void doMirror() {
		//printf("Set doMirror %s\n", m_path.c_str());
		Mirror &mirror = Mirror::get();
		mirror.mkDir(m_path.c_str());
		for (std::vector<MirrorNode *>::iterator i = this->begin(); i != this->end(); i++) {
			MirrorNode *data = (MirrorNode*)*i;
			data->doMirror();
		}
		mirror.makeCheckfile(m_path.c_str());
	}
	void doSyncMirror() {
		//printf("Set doMirror %s\n", m_path.c_str());
		Mirror &mirror = Mirror::get();
		mirror.syncDir(m_path.c_str());
		for (std::vector<MirrorNode *>::iterator i = this->begin(); i != this->end(); i++) {
			MirrorNode *data = (MirrorNode*)*i;
			data->doSyncMirror();
		}
		mirror.makeCheckfile(m_path.c_str());
	}

	void doMirrorInSync() {
		verifyDir(m_path.c_str());
		Mirror &mirror = Mirror::get();
		for (std::vector<MirrorNode *>::iterator i = this->begin(); i != this->end(); i++) {
			MirrorNode *data = (MirrorNode*)*i;
			data->doMirrorInSync();
		}
	}

	void doSourceInSync() {
		verifyDir(m_path.c_str());
		Mirror &mirror = Mirror::get();
		for (std::vector<MirrorNode *>::iterator i = this->begin(); i != this->end(); i++) {
			MirrorNode *data = (MirrorNode*)*i;
			data->doSourceInSync();
		}
	}

	void doSourceVerify() {
		//printf("Set doVerify %s\n", m_path.c_str());
		verifyDir(m_path.c_str());
		Mirror &mirror = Mirror::get();
		if (mirror.mapFileVerify(m_path.c_str()) == true) {
			printf("Verify failed at: %s\n", m_path.c_str());
		}
		/*
		for (std::vector<MirrorNode *>::iterator i = this->begin(); i != this->end(); i++) {
			MirrorFile *data = (MirrorFile *)*i;
			data->doVerify();
		}
		*/
	}
	void doMirrorVerify() {
		//printf("Set doVerify %s\n", m_path.c_str());
		verifyDir(m_path.c_str());
		Mirror &mirror = Mirror::get();
		if (mirror.mapFileVerify(m_path.c_str()) == true) {
			printf("Verify failed at: %s\n", m_path.c_str());
		}
		/*
		for (std::vector<MirrorNode *>::iterator i = this->begin(); i != this->end(); i++) {
			MirrorFile *data = (MirrorFile *)*i;
			data->doVerify();
		}
		*/
	}
	void verifyDir(const char *path) {
		Mirror &mirror = Mirror::get();
		if (mirror.DirExists(path) == false) {
			printf("Folder not found %s\n", m_path.c_str());
		}
	}

	const char *getPath() const {
		return m_path.c_str();
	}
};


/*
 *
 */
class MirrorList : public FolderVisitor {
public:
	typedef enum {
		DoMirror,
		DoMirrorVerify,
		DoSourceVerify,
		DoIsMirrorInSync,
		DoIsSourceInSync,
		DoSyncMirror,
		DoSyncSource,
		Unknown,
	} Function;

private:
	static MirrorList *m_this;
	static int m_count;
	MirrorSet *m_currentDir;
	static std::string m_rootFolder;
	static std::string m_mirrorFolder;
	std::list<MirrorSet *> m_setPath;
	int m_rootLength;
	Function m_function;

	MirrorList() {
		m_currentDir = 0;
		m_function = Unknown;
		m_rootLength = m_rootFolder.length();
		MirrorSet *m_mirrorSet = new MirrorSet("/");
		m_setPath.insert(m_setPath.end(), m_mirrorSet);
	}

protected:

	bool onStart(const char *path) {

		std::string fullPath = path;
		std::string relPath = fullPath.substr(m_rootLength);
		m_currentDir = new MirrorSet(relPath.c_str());
		m_setPath.insert(m_setPath.end(), m_currentDir);
		/*
		printf("==== Start ==== %s %d \n",relPath.c_str(), m_count++);
		for (std::list<MirrorSet *>::iterator i = m_setPath.begin(); i != m_setPath.end(); i++) {
					MirrorSet *data = *i;
					printf("%s\n", data->getPath());
				}
		printf("====\n");
		*/
		return true;
	};

	virtual bool onFile(const char *path) {

		std::string fullPath = path;
		std::string relPath = fullPath.substr(m_rootLength+1);
		//printf("onFile: %s  ++++ ", relPath.c_str());
		m_currentDir->insert(m_currentDir->end(), new MirrorFile(relPath.c_str()));
		return true;
	};
	virtual bool onDirectory(const char *path) {
		std::string fullPath = path;
		std::string relPath = fullPath.substr(m_rootLength+1);
		Mirror &mirror = Mirror::get();
		if (m_function == DoMirror) {
			if (mirror.mkDir(relPath.c_str()) == false) {
				return false;
			}
			return true;
		} else if (m_function == DoSyncMirror) {
			if (mirror.syncDir(relPath.c_str()) == false) {
				return false;
			}
			return true;
		}
		return false;
	};
	bool onEnd() {
/*
		printf("=== onEnd === %d \n", --m_count);
		for (std::list<MirrorSet *>::iterator i = m_setPath.begin(); i != m_setPath.end(); i++) {
			MirrorSet *data = *i;
			printf("%s\n", data->getPath());
		}


		MirrorSet *data = m_setPath.back();
		printf("==== %s\n", data->getPath());
		data->getPath();
*/
		MirrorSet *data = m_setPath.back();
		//data->print();
		printf("Current dir %s\n", data->getPath());
		if (m_function == DoMirror) {
			data->doMirror();
		} else if (m_function == DoMirrorVerify) {
			data->doMirrorVerify();
		} else if (m_function == DoSourceVerify) {
			data->doSourceVerify();
		} else if (m_function == DoIsMirrorInSync) {
			data->doMirrorInSync();
		} else if (m_function == DoIsSourceInSync) {
			data->doSourceInSync();
		} else if (m_function == DoSyncMirror) {
			data->doSyncMirror();
		}
		MirrorSet *current = m_setPath.back();
		delete current;
		m_setPath.pop_back();
		m_currentDir = m_setPath.back();
		return true;
	};
	virtual FolderVisitor *make() {
		if (m_this == 0) {
			m_this = new MirrorList;
		}
		return m_this;
	}



public:

	static MirrorList &get() {
		if (m_this == 0) {
			m_this = new MirrorList;
		}
		return *m_this;
	}

	virtual ~MirrorList();
	//static MirrorSets *getMirrorSets() {
	//	return m_mirrorSets;
	//}

	static const std::string& getRootFolder() {
		return m_rootFolder;
	}

	static const std::string& getMirrorFolder() {
		return m_rootFolder;
	}

	static void setRootFolder(const char *path) {
		m_rootFolder = path;
	}

	static void setMirrorFolder(const char *path) {
		m_mirrorFolder = path;
	}

	Function getFunction() const {
		return m_function;
	}

	void setFunction(Function function) {
		m_function = function;
	}
};

} /* namespace simplearchive */
#endif /* MIRRORSET_H_ */
