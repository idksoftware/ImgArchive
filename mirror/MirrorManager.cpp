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

#include "MirrorManager.h"
#include "Mirror.h"
#include "StagingManager.h"
#include "MirrorList.h"
#include "DirectoryVisitor.h"

#include "PrimaryMirror.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

namespace simplearchive {

std::string MirrorManager::m_rootFolder;
std::string MirrorManager::m_configFile;

std::string MirrorManager::m_masterRepositoryPath;
std::string MirrorManager::m_derivativeRepositoryPath;
std::string MirrorManager::m_primaryMirrorPath;
std::string MirrorManager::m_primaryIndexPath;


MirrorManager::MirrorManager() {
	// TODO Auto-generated constructor stub

}

MirrorManager::~MirrorManager() {
	// TODO Auto-generated destructor stub
}

void MirrorManager::setPrimaryIndexPath(const char* path)
{
	m_primaryIndexPath = path;
}


bool MirrorManager::initalise(const char *rootFolder, const char *configFile) {
	m_rootFolder = rootFolder;
	m_configFile = configFile;
	m_configFile += "/mirror.dat";

	MirrorList::setRootFolder(rootFolder);

	try {
		if (Mirror::initalise(MirrorList::getRootFolder().c_str(), m_configFile.c_str()) == false) {
			return false;
		}
	} catch (MirrorException &e) {
		printf("Error: %s\n", e.what());
		return false;
	}
	return true;
}



bool MirrorManager::mirror() {
	PrimaryMirror& primaryMirror = PrimaryMirror::get();
	primaryMirror.setPrimaryIndexPath(m_primaryIndexPath.c_str());
	primaryMirror.process();
	/*
	MirrorList &mirrorList = MirrorList::get();
	mirrorList.setFunction(MirrorList::DoMirror);
	
	std::shared_ptr<MirrorList> mirrorList_ptr = 0; // std::make_shared<MirrorList>(mirrorList);
	DirectoryVisitor directoryVisitor(mirrorList_ptr, false);
	return directoryVisitor.process(MirrorList::getRootFolder().c_str());
	*/
	return true;
}

bool MirrorManager::SyncMirror() {
	MirrorList &mirrorList = MirrorList::get();
	mirrorList.setFunction(MirrorList::DoSyncMirror);
	/*
	std::shared_ptr<MirrorList> mirrorList_ptr = 0; // std::make_shared<MirrorList>(mirrorList);
	DirectoryVisitor directoryVisitor(mirrorList_ptr, false);
	return directoryVisitor.process(MirrorList::getRootFolder().c_str());
	*/
	return true;
}

bool MirrorManager::isMirrorInSync() {
	MirrorList &mirrorList = MirrorList::get();
	mirrorList.setFunction(MirrorList::DoIsMirrorInSync);
	/*
	std::shared_ptr<MirrorList> mirrorList_ptr = 0; // std::make_shared<MirrorList>(mirrorList);
	DirectoryVisitor directoryVisitor(mirrorList_ptr, false);
	return directoryVisitor.process(MirrorList::getRootFolder().c_str());
	*/
	return true;
}

bool MirrorManager::isSourceInSync() {
	MirrorList &mirrorList = MirrorList::get();
	mirrorList.setFunction(MirrorList::DoIsSourceInSync);
	/*
	std::shared_ptr<MirrorList> mirrorList_ptr = std::make_shared<MirrorList>(&mirrorList);
	DirectoryVisitor directoryVisitor(mirrorList_ptr, false);
	return directoryVisitor.process(MirrorList::getRootFolder().c_str());
	*/
	return true;
}

bool MirrorManager::verifyMirror() {
	/*
	MirrorList &mirrorList = MirrorList::get();
	mirrorList.setFunction(MirrorList::DoMirrorVerify);
	DirectoryVisitor directoryVisitor(&mirrorList, false);
	return directoryVisitor.process(MirrorList::getRootFolder().c_str());
	*/
	Mirror &mirror = Mirror::get();
	return mirror.verifyMirrors();
}

bool MirrorManager::verifySource() {
	/*
	MirrorList &mirrorList = MirrorList::get();
	mirrorList.setFunction(MirrorList::DoSourceVerify);
	DirectoryVisitor directoryVisitor(&mirrorList);
	return directoryVisitor.process(MirrorList::getRootFolder().c_str());
	*/
	Mirror &mirror = Mirror::get();
	return mirror.verifySource();
}

} // namespace simplearchive
