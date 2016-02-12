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
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
//#include <dirent.h>
#include <cstdlib>
#include <stdio.h>
#include <vector>
#include <string.h>
#include <sstream>
//#include <unistd.h>
#ifdef WINDOWS
	#include <sysstat.h>
#endif
#include <sys/types.h>
#include <sys/stat.h>
#include <cstdio>
#include "CIDKFileFind.h"
#include "DirectoryVisitor.h"
#include "SAUtils.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

namespace simplearchive {




/**
 * This class is the directory node that is used to
 * create a folder path that the DirectoryVisitor uses
 * to navigate the directory 
 */
class DirNode {
	std::string m_dirpath;

	DirNode *m_dirNode;
	FolderVisitor *m_folderVisitor;
	bool m_deleteFolderVisitor;
public:
	DirNode(DirNode *dirNode, const char *dirpath, bool deleteFolderVisitor) {
		m_dirNode = dirNode;
		m_dirpath = dirpath;
		m_deleteFolderVisitor = deleteFolderVisitor;
		m_folderVisitor = 0;
	}
	DirNode(DirNode *dirNode, const char *dirpath, FolderVisitor *folderVisitor, bool deleteFolderVisitor) {
		m_dirNode = dirNode;
		m_dirpath = dirpath;
		m_deleteFolderVisitor = deleteFolderVisitor;
		m_folderVisitor = folderVisitor;
		m_folderVisitor->onStart(dirpath);
	}

	~DirNode() {
		if (m_folderVisitor != 0) {
			m_folderVisitor->onEnd();
			if (m_deleteFolderVisitor) {
				delete m_folderVisitor;
			}
			m_folderVisitor = 0;
		}
	}

	bool process();
};



DirectoryVisitor::DirectoryVisitor(FolderVisitor *folderVisitor, bool val) {

	m_dirNode = NULL;
	m_folderVisitor = folderVisitor;
	m_deleteFolderVisitor = val;

}

DirectoryVisitor::~DirectoryVisitor() {

}


bool DirectoryVisitor::process(const char *rf) {
	std::string rootFolder = rf;
	if (m_folderVisitor != 0) {
		m_dirNode = new DirNode(NULL, rootFolder.c_str(), m_folderVisitor->make(), m_deleteFolderVisitor);
	} else {
		m_dirNode = new DirNode(NULL, rootFolder.c_str(), m_deleteFolderVisitor);
	}
	m_dirNode->process();
	delete m_dirNode;
	return true;
}

bool DirNode::process() {
#ifdef _WIN32
	std::string path = m_dirpath;
	path = path + "/*.*";
	CIDKFileFind fileFind(path);
	fileFind.Open();
	if (fileFind.GotFile() == false) {
		return true;
	}
	do {
		std::string ent = fileFind.GetFileName();
		if (!ent.compare(".") || !ent.compare("..")) {
			continue;
		}
		std::string filename = m_dirpath + '/' + ent;
		if (SAUtils::IsFile(filename.c_str()) == true) {
			if (m_folderVisitor) {
				m_folderVisitor->onFile(filename.c_str());
			}
		} else {
			if (m_folderVisitor) {
				m_folderVisitor->onDirectory(filename.c_str());
			}
			if (m_folderVisitor != 0) {
				m_dirNode = new DirNode(NULL, filename.c_str(), m_folderVisitor->make(), m_deleteFolderVisitor);
			}
			else {
				m_dirNode = new DirNode(NULL, filename.c_str(), m_deleteFolderVisitor);
			}
			m_dirNode->process();
			delete m_dirNode;
			m_dirNode = 0;
		}
	} while (fileFind.GetNext());
#else
	DIR *dir;
	struct dirent *ent;

	if ((dir = opendir(m_dirpath.c_str())) == NULL) {
		return false;
	}
	while ((ent = readdir(dir)) != NULL) {
		if (!strcmp(ent->d_name,".") || !strcmp(ent->d_name,"..")) {
			continue;
		}

		std::string path = m_dirpath + '/' + ent->d_name;

		if (opendir(path.c_str()) == NULL) {
			if (m_folderVisitor) {
				if (m_folderVisitor->onFile(path.c_str()) == false) {
					return false;
				}
			}
		} else {

			if (m_folderVisitor) {
				if (m_folderVisitor->onDirectory(path.c_str()) == false) {
					return false;
				}
			}
			if (m_folderVisitor != 0) {
				m_dirNode = new DirNode(NULL, path.c_str(), m_folderVisitor->make());
			} else {
				m_dirNode = new DirNode(NULL, path.c_str());
			}
			m_dirNode->process();
			delete m_dirNode;
			m_dirNode = 0;
		}

	}
#endif
	return true;
}



} /* namespace simplearchive */
