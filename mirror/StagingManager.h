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

#ifndef STAGINGMANAGER_H_
#define STAGINGMANAGER_H_
#include <string>
namespace simplearchive {

class StagingManager {
	std::string m_name;
	std::string m_stagingFolder;
	std::string m_remoteMirrorPath;
	std::string m_root;
	time_t m_time;
	std::string unqueNumger();
	bool txtFile(std::string &number, const char *path);
	bool verifyTxtFile(std::string &number, const char *path);
	/// reads and returns the path of the file. Empty on error
	std::string readTxtFile(const char *txtFilePath);

public:
	StagingManager(const char *root, const char *name, const char *stagingFolder);
	/// Puts the data into the staging area.
	bool put(const char *path);
	/// Forward file to mirror
	bool putMirror();
	virtual ~StagingManager();
	const char *getRemoteMirrorPath() const;
	void setRemoteMirrorPath(const char *remoteMirrorPath);
};

} /* namespace simplearchive */
#endif /* STAGINGMANAGER_H_ */
