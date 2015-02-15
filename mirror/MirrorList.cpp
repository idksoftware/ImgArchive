/*
 * MirrorSet.cpp
 *
 *  Created on: Oct 20, 2014
 *      Author: wzw7yn
 */

#include "MirrorList.h"


namespace simplearchive {

MirrorNode::NodeType MirrorNode::m_FileType = MirrorNode::FileNode;
MirrorNode::NodeType MirrorNode::m_DirType = MirrorNode::DirNode;
//MirrorSets *MirrorList::m_mirrorSets = 0;

int MirrorList::m_count = 0;
std::string MirrorList::m_rootFolder;
MirrorList *MirrorList::m_this = 0;;

void MirrorFile::doMirror() {
	Mirror &mirror = Mirror::get();
	//printf("Item %s\n", m_path.c_str());
	//std::string fullSourcePath = MirrorList::getRootFolder() + m_path;
	mirror.copy(m_path.c_str());
}

bool MirrorFile::doSyncMirror() {
	Mirror &mirror = Mirror::get();
	mirror.syncFile(m_path.c_str());
	return true;
}

bool MirrorFile::doSyncSource() {
	Mirror &mirror = Mirror::get();
	//printf("Item %s\n", m_path.c_str());
	//std::string fullSourcePath = MirrorList::getRootFolder() + m_path;
	mirror.copy(m_path.c_str());
	return true;
}

void MirrorFile::doMirrorInSync() {
	Mirror &mirror = Mirror::get();
	//printf("In-Sync %s\n", m_path.c_str());
	if (mirror.verifyMirrorName(m_path.c_str()) == false) {
		printf("In-Sync failed %s\n", m_path.c_str());
	}
}

void MirrorFile::doSourceInSync() {
	Mirror &mirror = Mirror::get();
	//printf("In-Sync %s\n", m_path.c_str());
	if (mirror.verifySourceName(m_path.c_str()) == false) {
		printf("In-Sync failed %s\n", m_path.c_str());
	}
}

void MirrorFile::doVerify() {
	Mirror &mirror = Mirror::get();
	printf("Verify %s\n", m_path.c_str());
	//std::string fullSourcePath = MirrorList::getRootFolder() + m_path;
	if (mirror.verifyContents(m_path.c_str()) == false) {
		printf("Verify failed %s\n", m_path.c_str());
	}
}

void MirrorDir::doMirror() {
	//Mirror &mirror = Mirror::get();
	//printf("Mirroring Item %s\n", m_path.c_str());
	std::string fullSourcePath = MirrorList::getRootFolder() + m_path;

}

bool MirrorDir::doSyncMirror() {
	Mirror &mirror = Mirror::get();
	mirror.syncDir(m_path.c_str());
	return true;
}

bool MirrorDir::doSyncSource() {
	Mirror &mirror = Mirror::get();
	//printf("Item %s\n", m_path.c_str());
	//std::string fullSourcePath = MirrorList::getRootFolder() + m_path;
	mirror.copy(m_path.c_str());
	return true;
}

void MirrorDir::doMirrorInSync() {
	Mirror &mirror = Mirror::get();
	//printf("Verify %s\n", m_path.c_str());
	if (mirror.FileExists(m_path.c_str()) == false) {
		printf("Verify failed %s\n", m_path.c_str());
	}
}

void MirrorDir::doSourceInSync() {
	Mirror &mirror = Mirror::get();
	//printf("Verify %s\n", m_path.c_str());
	if (mirror.FileExists(m_path.c_str()) == false) {
		printf("Verify failed %s\n", m_path.c_str());
	}
}

void MirrorDir::doVerify() {
	//Mirror &mirror = Mirror::get();
	printf("Verify %s\n", m_path.c_str());
	//std::string fullSourcePath = MirrorList::getRootFolder() + m_path;
	//if (mirror.verify(m_path.c_str()) == false) {
	//	printf("Verify failed %s\n", m_path.c_str());
	//}
}



/*
MirrorList::MirrorList() {
	//m_mirrorSet = 0;
	m_rootLength = m_rootFolder.length();

}
*/
MirrorList::~MirrorList() {

};

} /* namespace simplearchive */
