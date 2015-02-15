/*
 * MirrorSet.cpp
 *
 *  Created on: Oct 20, 2014
 *      Author: wzw7yn
 */

#include "MirrorList.h"

namespace simplearchive {

MirrorSets *MirrorList::m_mirrorSets = 0;

int MirrorList::m_count = 0;
std::string MirrorList::m_rootFolder;

void MirrorFile::doMirror() {
	Mirror &mirror = Mirror::get();
	//printf("Item %s\n", m_path.c_str());
	//std::string fullSourcePath = MirrorList::getRootFolder() + m_path;
	mirror.copy(m_path.c_str());
}

void MirrorFile::doVerify() {
	Mirror &mirror = Mirror::get();
	//printf("Verify %s\n", m_path.c_str());
	//std::string fullSourcePath = MirrorList::getRootFolder() + m_path;
	if (mirror.verify(m_path.c_str()) == false) {
		//printf("Verify failed %s\n", m_path.c_str());
	}
}

MirrorList::MirrorList() {
	m_mirrorSet = 0;
	m_rootLength = m_rootFolder.length();

}

MirrorList::~MirrorList() {

};

} /* namespace simplearchive */
