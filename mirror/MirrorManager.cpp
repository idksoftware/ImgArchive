/*
 * MirrorManager.cpp
 *
 *  Created on: Nov 13, 2014
 *      Author: wzw7yn
 */


#include "MirrorManager.h"
#include "Mirror.h"
#include "StagingManager.h"
#include "MirrorList.h"
#include "DirectoryVisitor.h"

namespace simplearchive {




std::string MirrorManager::m_rootFolder;
std::string MirrorManager::m_configFile;


MirrorManager::MirrorManager() {
	// TODO Auto-generated constructor stub

}

MirrorManager::~MirrorManager() {
	// TODO Auto-generated destructor stub
}

bool MirrorManager::initalise(const char *rootFolder, const char *configFile) {
	m_rootFolder = rootFolder;
	m_configFile = configFile;
	m_configFile += "/mirror.conf";

	MirrorList::setRootFolder(rootFolder);

	try {
		if (Mirror::initalise(MirrorList::getRootFolder().c_str(), m_configFile.c_str()) == false) {
			return false;
		}
	} catch (MirrorException &e) {
		printf("Error: %s\n", e.what());
	}
	return true;
}



bool MirrorManager::mirror() {
	MirrorList &mirrorList = MirrorList::get();
	mirrorList.setFunction(MirrorList::DoMirror);
	DirectoryVisitor directoryVisitor(&mirrorList, false);
	return directoryVisitor.process(MirrorList::getRootFolder().c_str());
}

bool MirrorManager::SyncMirror() {
	MirrorList &mirrorList = MirrorList::get();
	mirrorList.setFunction(MirrorList::DoSyncMirror);
	DirectoryVisitor directoryVisitor(&mirrorList, false);
	return directoryVisitor.process(MirrorList::getRootFolder().c_str());
}

bool MirrorManager::isMirrorInSync() {
	MirrorList &mirrorList = MirrorList::get();
	mirrorList.setFunction(MirrorList::DoIsMirrorInSync);
	DirectoryVisitor directoryVisitor(&mirrorList, false);
	return directoryVisitor.process(MirrorList::getRootFolder().c_str());
}

bool MirrorManager::isSourceInSync() {
	MirrorList &mirrorList = MirrorList::get();
	mirrorList.setFunction(MirrorList::DoIsSourceInSync);
	DirectoryVisitor directoryVisitor(&mirrorList, false);
	return directoryVisitor.process(MirrorList::getRootFolder().c_str());
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
