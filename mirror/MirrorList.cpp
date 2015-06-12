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

#include "MirrorList.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

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
