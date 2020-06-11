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

#ifndef MIRRORMANAGER_H_
#define MIRRORMANAGER_H_

#include <string>

namespace simplearchive {
//*

class MirrorList;
class MirrorManager {

	static std::string m_rootFolder;
	static std::string m_configFile;

	static std::string m_masterRepositoryPath;
	static std::string m_derivativeRepositoryPath;
	static std::string m_primaryMirrorPath;
	static std::string m_primaryIndexPath;

public:
	MirrorManager();
	virtual ~MirrorManager();

	void setPrimaryIndexPath(const char* path);

	static bool initalise(const char *rootFolder, const char *configFile);
	/// This creates a mirror of the target root folder and all sub folders.
	/// using the mirrors defined in the configuration file passed in the
	/// initialise function
	static bool mirror();
	/// This synchronises the Mirror with the source (i.e. any missing files in
	/// the Mirror is copied from the source
	static bool SyncMirror();
	/// This checks the mirrors are in sync with the source directory.
	/// The source data is read and the mirror is checked that the file
	/// or directory exists. (only direct mirrors are used)
	static bool isMirrorInSync();
	/// This checks the source is in-sync with the mirror directory.
	/// The source data is read and the mirror is checked that the file
	/// or directory exists. (only direct mirrors are used)
	static bool isSourceInSync();
	/// Verifies that the mirrors are correct.
	static bool verifyMirror();
	/// Verifies that the Source data is correct.
	static bool verifySource();
};

} /* namespace simplearchive */
#endif /* MIRRORMANAGER_H_ */
